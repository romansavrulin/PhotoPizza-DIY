/**
 * File: Timer.h
 * Description:
 * SoftTimer library is a lightweight but effective event based timeshare solution for Arduino.
 *
 * Author: Roman Savrulin <romeo.deepmind@gmail.com>, Balazs Kelemen <prampec+arduino@gmail.com>
 * Copyright: 2014 Roman Savrulin
 * Copying permission statement:
    This file is part of SoftTimer.

    SoftTimer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef SOFT_TIMER_TIMER_H
#define SOFT_TIMER_TIMER_H

#include "SoftTimer.h"
#include "Task.h"
#include <Arduino.h>

/**
 * Run a callback after a specified delay. The task will stop when finished. Also chains tasks.
 */
class Timer : public Task
{
  public:
    typedef enum {
      STATE_STOPPED,
      STATE_RUNNING
    }state;

    /**
     * Setup a delayed task.
     *  @param delayMs - The callback will be launched after this amount of milliseconds was passed.
     *    A value zero (0) may also have sense, when you only want to chain tasks.
     *    Do not add values greater then 4,294,967, which is about 71 minutes!
     *  @param nextTimer - If the followedBy was specified, than it will be started when this was finished.
     *   Starting the followedBy can be denied by returning FALSE in the callback.
     */
    Timer(unsigned long delayMs, Timer* nextTimer = NULL);

    /**
      * Register the task, and start waiting for the delayMs to pass.
      * If you need to prevent the task to start, you need to remove it from the
      * Timer Manager (with the SoftTimer.remove() function).
      */
    void start();

    /**
      * Unregister the task, and remove it from the
      * Timer Manager (can also be done with the SoftTimer.remove() function).
      */
    void stop(bool launchTask = false);

    /**
      * Gets the state of the timer
      */
    bool isRunning() { return _state == STATE_RUNNING; };


    virtual ~Timer() = 0;
    virtual bool operator()() = 0;

    state getState(){ return _state; };

  private:
    /** The timer should be started after this one was finished. */
    Timer* _nextTimer;
    static void step(Task* me);

    state _state;
};

inline Timer::~Timer() { }

#endif

