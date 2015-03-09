/*
 * paramRun.cpp
 *
 *  Created on: 09 марта 2015 г.
 *      Author: romeo
 */

#include "paramRun.h"
#include <AccelStepper.h>
#include <limits.h>

#include "presetManager.h"

namespace PhotoPizza{

AccelStepper stepper(AccelStepper::DRIVER, MOTOR_STP_PIN, MOTOR_DIR_PIN);

void paramRun::stopPreset(){
  if(!_run)
      return;
  _val = 0;
  Serial.println(F("stopping"));
  stepper.stop();
  libUpdateLCD(); //todo: refactor???
  //lcd.setCursor(0, 1);
  //lcd.print(F("Program stopping "));
  return;
}

void paramRun::loop(){
  if(!stepper.run() && _run){
    Serial.println((String) _run);
    _run = false;
    _val = 0;
    //presetManager::get()->discard();
    Serial.println(F("Finished"));
    Serial.println((String) _run);
    libUpdateLCD(); //todo: refactor???
    //finishPreset();
  }
}

void paramRun::runPreset() {
  Serial.println((String)F("Run: ") + _run);
  if(_run)
    return;

  _val = 1;
  _run = true;
  //lcd.setCursor(0, 1);
  //lcd.print(F("Program started "));
  presetManager *presetMgr = presetManager::get();

  Serial.println(F("Run"));
  long steps = presetMgr->getPreset()->_steps * presetMgr->getPreset()->_dir; //TODO: refactor getters (via local vars)
  Serial.println((String)F("Accel") + presetMgr->getPreset()->_acc);
  Serial.println((String)F("Steps") + steps);
  Serial.println((String)F("Speed") + presetMgr->getPreset()->_speed);
  stepper.setCurrentPosition(0L);
  if(presetMgr->getPreset()->_acc == 0){
    stepper.setAcceleration(10000000); //no acc.
  }else
    stepper.setAcceleration(presetMgr->getPreset()->_acc);
  if(steps != 0){
    stepper.moveTo(steps);
  }else
    stepper.moveTo(LONG_MAX * presetMgr->getPreset()->_dir);
  stepper.setMaxSpeed(presetMgr->getPreset()->_speed);
  libUpdateLCD(); //todo: refactor???
}

/*void paramRun::finishPreset(){
  //lcd.setCursor(0, 1);
  //lcd.print(F("Program finished"));

  //delay(1000);
  //presetMgr->stop();
  //show_curr_program();
}*/
};