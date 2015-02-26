/*
 * keyboard.cpp
 *
 *  Created on: 26 февр. 2015 г.
 *      Author: romeo
 */

#include "keyboard.h"

#include "defines.h";
#include "Arduino.h"
#include "IRReciever.h"

namespace PhotoPizza{

///////// LCD Buttons (not used yet)
static int read_LCD_buttons() { // read the buttons
  int adc_key_in;
  adc_key_in = analogRead(0); // read the value from the sensor

  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  // We make this the 1st option for speed reasons since it will be the most likely result

  if (adc_key_in > 1000)
    return btnNONE;

  // For V1.1 us this threshold
  if (adc_key_in < 50)
    return btnRIGHT;
  if (adc_key_in < 250)
    return btnUP;
  if (adc_key_in < 450)
    return btnDOWN;
  if (adc_key_in < 650)
    return btnLEFT;
  if (adc_key_in < 850)
    return btnSELECT;

  // For V1.0 comment the other threshold and use the one below:
  /*
   if (adc_key_in < 50)   return btnRIGHT;
   if (adc_key_in < 195)  return btnUP;
   if (adc_key_in < 380)  return btnDOWN;
   if (adc_key_in < 555)  return btnLEFT;
   if (adc_key_in < 790)  return btnSELECT;
   */

  return btnNONE; // when all others fail, return this.
}

kbKeys kbGetKey(){

  int key = IrGetKey();

  switch(key){
  case BTN_POWER: return kbPwr;
  case BTN_FUNC:  return kbMenu;

  case BTN_CH_U: return kbBksp;
  case BTN_EQ:   return kbClear;
  case BTN_CH_D: return kbTest;

  case BTN_PLAY: return kbOk;

  case BTN_VOL_U: return kbUp;
  case BTN_VOL_D: return kbDown;
  case BTN_RW:    return kbLeft;
  case BTN_FW:    return kbRight;

  case BTN_0: return kbBtn0;
  case BTN_1: return kbBtn1;
  case BTN_2: return kbBtn2;
  case BTN_3: return kbBtn3;
  case BTN_4: return kbBtn4;
  case BTN_5: return kbBtn5;
  case BTN_6: return kbBtn6;
  case BTN_7: return kbBtn7;
  case BTN_8: return kbBtn8;
  case BTN_9: return kbBtn9;

  default: return kbNoKey;
  }
}

int kbGetNumericKey(int key){
  if(key >= kbBtn0 && key <= kbBtn9)
    return key - kbBtn0;
  else return -1;
}

}
