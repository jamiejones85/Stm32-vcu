/*
 * This file is part of the tumanako_vc project.
 *
 * Copyright (C) 2010 Johannes Huebner <contact@johanneshuebner.com>
 * Copyright (C) 2010 Edward Cheeseman <cheesemanedward@gmail.com>
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "my_fp.h"
#include "my_math.h"
#include "test_list.h"
#include "throttle.h"

using namespace std;

void Param::Change(Param::PARAM_NUM p)
{
   // Dummy function - we ignore parameter changes in these tests
}

// Include a dummy error list to allow the tests to link. It is unused.
const char* errorListString = "";

static void TestSetup()
{
   //percentage of deadzone
   Throttle::throtdead = 5;
   Throttle::potmin[0] = 100;
   Throttle::potmax[0] = 4000;
   Throttle::throtmax = 100;
   Throttle::throttleRamp = 10;
   Param::SetInt(Param::dir, 1);
   Param::SetFloat(Param::idcmin, -100);
}

// TEMPERATURE DERATING
static void TestThrottleTemperateOverMaxThrottleTo0() {
   float temp = 60;
   float tempMax = 50;
   float finalSpnt = 100;
   ASSERT(Throttle::TemperatureDerate(temp, tempMax, finalSpnt) && finalSpnt == 0);
}

static void TestThrottleTemperateInDerateZoneThrottleTo50Percent() {
   float temp = 61;
   float tempMax = 60;
   float finalSpnt = 100;
   ASSERT(Throttle::TemperatureDerate(temp, tempMax, finalSpnt) && finalSpnt == 50);
}

static void TestThrottleUnderTemperateNoDeRate() {
   float temp = 30;
   float tempMax = 60;
   float finalSpnt = 100;
   ASSERT(Throttle::TemperatureDerate(temp, tempMax, finalSpnt) == false && finalSpnt == 100);
}

static void TestThrottleTemperateInDerateZoneThrottleButThrottleUnderLimit() {
   float temp = 61;
   float tempMax = 60;
   float finalSpnt = 49;
   ASSERT(Throttle::TemperatureDerate(temp, tempMax, finalSpnt) && finalSpnt == 49);
}

// CALC THROTTLE
static void TestCalcThrottleIs0WhenThrottleAndBrakePressed() {


   ASSERT(Throttle::CalcThrottle(3000, 0, true) == 0);
}

static void TestCalcThrottleIs0WhenNoThrottleAndBrakePressed() {

   ASSERT(Throttle::CalcThrottle(0, 0, true) == 0);
}

static void TestCalcThrottleIs0WhenInDeadZone() {
   //deadzone is first 5% of travel between 100 and 4000
   ASSERT(Throttle::CalcThrottle(295, 0, false) == 0);
}

static void TestCalcThrottleIsAbove0WhenJustOutOfDeadZone() {
   //deadzone is first 5% of travel between 100 and 4000
   ASSERT(Throttle::CalcThrottle(496, 0, false) > 0);
}

static void TestCalcThrottleIs100WhenMax() {
   //deadzone is first 5% of travel between 100 and 4000
   float throtVal = Throttle::CalcThrottle(4000, 0, false);
   ASSERT(throtVal ==  100);
}

static void TestCalcThrottleIs100WhenOverMax() {
   //deadzone is first 5% of travel between 100 and 4000
   float throtVal = Throttle::CalcThrottle(4001, 0, false);
   ASSERT(throtVal ==  100);
}

static void TestIdcLimitCommandWhenUnderIDCMin() {
   float throttleSpnt = 100;

   //run through a good chunk of loops to populate the filtered idc
   for (int i = 0; i < 50; i++) {
      Throttle::IdcLimitCommand(throttleSpnt, -99);
      throttleSpnt = 100;
   }
   Throttle::IdcLimitCommand(throttleSpnt, -99);

   ASSERT(throttleSpnt == 100);
}

static void TestIdcLimitCommandWhenOverIDCMin() {
   float throttleSpnt = 100;

   for (int i = 0; i < 50; i++) {
      Throttle::IdcLimitCommand(throttleSpnt, -120);
      throttleSpnt = 100;
   }

   Throttle::IdcLimitCommand(throttleSpnt, -120);

   ASSERT(throttleSpnt < 82);
}

static void TestIdcLimitCommandWhenOverIDCMinIncreasedWhenCurrentReduces() {
   float throttleSpnt = 100;

   for (int i = 0; i < 50; i++) {
      Throttle::IdcLimitCommand(throttleSpnt, -120);
      throttleSpnt = 100;
   }

   Throttle::IdcLimitCommand(throttleSpnt, -120);

   ASSERT(throttleSpnt < 82);

   for (int i = 0; i < 50; i++) {
      Throttle::IdcLimitCommand(throttleSpnt, -90);
      throttleSpnt = 100;
   }

   Throttle::IdcLimitCommand(throttleSpnt, -90);

   ASSERT(throttleSpnt == 100);
}

static void sweep() {
   float throttleSpnt = 100;

   for (int i = 0; i > -150; i--) {
      Throttle::IdcLimitCommand(throttleSpnt, i);
      cout << "IDC: " << i << " throttleSpnt: " << throttleSpnt << endl;
      throttleSpnt = 100;
   }
   for (int i = -150; i < 0; i++) {
      Throttle::IdcLimitCommand(throttleSpnt, i);
      cout << "IDC: " << i << " throttleSpnt: " << throttleSpnt << endl;
      throttleSpnt = 100;
   }
}



void ThrottleTest::RunTest()
{
   TestSetup();
   TestThrottleTemperateOverMaxThrottleTo0();
   TestThrottleTemperateInDerateZoneThrottleTo50Percent();
   TestThrottleUnderTemperateNoDeRate();
   TestThrottleTemperateInDerateZoneThrottleButThrottleUnderLimit();
   TestCalcThrottleIs0WhenThrottleAndBrakePressed();
   TestCalcThrottleIs0WhenNoThrottleAndBrakePressed();
   TestCalcThrottleIs0WhenInDeadZone();
   TestCalcThrottleIsAbove0WhenJustOutOfDeadZone();
   TestCalcThrottleIs100WhenMax();
   TestCalcThrottleIs100WhenOverMax();
   TestIdcLimitCommandWhenUnderIDCMin();
   TestIdcLimitCommandWhenOverIDCMin();
   TestIdcLimitCommandWhenOverIDCMinIncreasedWhenCurrentReduces();
   sweep();

}
