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

boolean paramRun::operator()(){
  if(_iterCount == 0 || _val == 0){
    _run = false;
    _val = 0;
    presetManager::get()->update();
    Serial.println(F("Finished"));
  }else
    startMotor(NULL);
  return true;
}

void paramRun::stopPreset(){
  if(!_run)
      return;
  _val = 0;
  _delayTime = 0;
  Serial.println(F("stopping"));
  stepper.stop();
  presetManager::get()->update();
  return;
}



void paramRun::up(){
  paramSpeed *sp = &presetManager::get()->getPreset()->_speed;
  sp->up();
  stepper.setMaxSpeed((long)*sp);
}
void paramRun::down(){
  paramSpeed *sp = &presetManager::get()->getPreset()->_speed;
  sp->down();
  stepper.setMaxSpeed((long)*sp);
}

void paramRun::loop(){
  presetManager *pMgr = presetManager::get();
  if(!stepper.run() && _run){
    if(!Timer::isRunning()){
      setPeriodMs(pMgr->getPreset()->_pause);
      start();
    }
    if(!_relay.isRunning() && !_relayCycle && _val){
      _relay.startDelayed();
      _relayCycle = true;
    }
    if(!_val){
      Timer::stop(true);
      _relay.stop(true);
    }
  }
}

bool paramRun::startMotor(Task *t){
  presetManager *presetMgr = presetManager::get();

  if(_iterCount >= 0)
    --_iterCount;

  _relayCycle = false;
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

  return false;
}

void paramRun::edit() {
  if(_run)
    return;
  Serial.println(F("Run"));
  _val = 1;
  _run = true;
  _iterCount = presetManager::get()->getPreset()->_iter;
  startMotor(NULL);

  presetManager::get()->update();
}

};