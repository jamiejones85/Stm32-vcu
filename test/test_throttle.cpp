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
#include <unistd.h>

using namespace std;

void Param::Change(Param::PARAM_NUM p)
{
   // Dummy function - we ignore parameter changes in these tests
}

// Include a dummy error list to allow the tests to link. It is unused.
const char* errorListString = "";
const int ROWS = 463;
const int COLS = 2;
const int POT_COL = 0;
const int SPEED_COL = 1;

const int testData[463][2] = {
        {428, 0},
        {427, 0},
        {428, 0},
        {427, 0},
        {428, 0},
        {429, 0},
        {429, 0},
        {428, 0},
        {428, 0},
        {428, 0},
        {428, 0},
        {427, 0},
        {427, 0},
        {428, 0},
        {426, 0},
        {428, 0},
        {428, 0},
        {427, 0},
        {428, 0},
        {427, 0},
        {426, 0},
        {427, 0},
        {428, 0},
        {427, 0},
        {426, 0},
        {427, 0},
        {427, 0},
        {425, 9},
        {426, 0},
        {426, 0},
        {426, 0},
        {474, 11},
        {555, 11},
        {787, 78},
        {888, 272},
        {874, 503},
        {736, 665},
        {638, 641},
        {428, 678},
        {426, 674},
        {427, 594},
        {426, 528},
        {471, 554},
        {532, 586},
        {690, 597},
        {694, 650},
        {767, 756},
        {847, 801},
        {831, 873},
        {429, 825},
        {571, 739},
        {767, 708},
        {756, 744},
        {756, 737},
        {758, 747},
        {428, 713},
        {427, 658},
        {427, 551},
        {427, 446},
        {426, 338},
        {427, 240},
        {426, 109},
        {439, 45},
        {534, 22},
        {689, 6},
        {683, 20},
        {687, 30},
        {638, 51},
        {645, 21},
        {671, 14},
        {669, 28},
        {711, 48},
        {777, 88},
        {705, 192},
        {623, 166},
        {727, 169},
        {617, 179},
        {428, 128},
        {427, 15},
        {428, 107},
        {548, 22},
        {756, 40},
        {767, 82},
        {852, 234},
        {808, 345},
        {793, 517},
        {427, 548},
        {428, 380},
        {427, 183},
        {428, 68},
        {427, 33},
        {427, 84},
        {427, 22},
        {427, 0},
        {439, 35},
        {575, 107},
        {694, 207},
        {757, 344},
        {714, 514},
        {430, 600},
        {426, 625},
        {427, 545},
        {428, 565},
        {429, 537},
        {428, 527},
        {427, 493},
        {428, 344},
        {427, 188},
        {427, 87},
        {427, 137},
        {427, 157},
        {427, 167},
        {426, 206},
        {428, 206},
        {427, 150},
        {426, 170},
        {426, 169},
        {689, 203},
        {982, 467},
        {721, 733},
        {428, 792},
        {427, 625},
        {428, 531},
        {1543, 802},
        {1603, 1324},
        {930, 1652},
        {746, 1704},
        {427, 1663},
        {427, 1617},
        {427, 1564},
        {428, 1530},
        {426, 1496},
        {427, 1388},
        {427, 1341},
        {426, 1351},
        {426, 1361},
        {426, 1356},
        {428, 1342},
        {426, 1356},
        {633, 1379},
        {711, 1405},
        {777, 1466},
        {772, 1539},
        {771, 1599},
        {757, 1662},
        {430, 1657},
        {425, 1659},
        {426, 1609},
        {428, 1604},
        {426, 1562},
        {429, 1537},
        {427, 1509},
        {426, 1483},
        {426, 1463},
        {426, 1428},
        {425, 1406},
        {427, 1385},
        {428, 1371},
        {427, 1359},
        {427, 1334},
        {426, 1270},
        {427, 1136},
        {425, 1013},
        {426, 813},
        {426, 399},
        {425, 101},
        {426, -10},
        {426, 0},
        {426, 0},
        {426, 0},
        {426, 0},
        {426, 0},
        {426, 0},
        {705, 70},
        {887, 287},
        {828, 546},
        {834, 742},
        {868, 851},
        {956, 1033},
        {968, 1163},
        {427, 1215},
        {427, 1115},
        {427, 1039},
        {426, 866},
        {426, 685},
        {425, 636},
        {426, 553},
        {433, 425},
        {700, 437},
        {909, 554},
        {940, 741},
        {948, 898},
        {953, 1030},
        {870, 1137},
        {835, 1154},
        {911, 1249},
        {925, 1348},
        {866, 1309},
        {946, 1494},
        {962, 1537},
        {947, 1582},
        {912, 1634},
        {940, 1677},
        {1004, 1793},
        {1028, 1942},
        {860, 2019},
        {884, 2010},
        {895, 2019},
        {912, 2027},
        {911, 2036},
        {901, 2047},
        {813, 2013},
        {817, 1962},
        {426, 1862},
        {687, 1727},
        {784, 1665},
        {827, 1623},
        {675, 1564},
        {426, 1464},
        {427, 1276},
        {426, 1158},
        {427, 1013},
        {424, 882},
        {427, 723},
        {425, 507},
        {427, 280},
        {426, 122},
        {426, 40},
        {426, 8},
        {425, 9},
        {425, 0},
        {426, 0},
        {425, 0},
        {426, 0},
        {426, 0},
        {426, 0},
        {426, 0},
        {425, 0},
        {426, 0},
        {425, 0},
        {425, 0},
        {425, 0},
        {425, 0},
        {425, 0},
        {425, 0},
        {426, 0},
        {426, 0},
        {534, 2},
        {856, 63},
        {961, 293},
        {855, 499},
        {856, 655},
        {855, 786},
        {861, 915},
        {862, 1011},
        {868, 1103},
        {891, 1173},
        {935, 1224},
        {918, 1333},
        {898, 1390},
        {902, 1410},
        {905, 1382},
        {907, 1393},
        {959, 1426},
        {990, 1467},
        {1017, 1570},
        {1010, 1605},
        {950, 1657},
        {919, 1603},
        {1047, 1655},
        {1021, 1744},
        {970, 1799},
        {968, 1837},
        {970, 1870},
        {968, 1927},
        {943, 1977},
        {842, 2040},
        {753, 1964},
        {428, 1873},
        {428, 1795},
        {735, 1743},
        {780, 1712},
        {785, 1689},
        {791, 1690},
        {811, 1699},
        {813, 1709},
        {826, 1741},
        {826, 1754},
        {853, 1768},
        {852, 1763},
        {857, 1762},
        {852, 1762},
        {428, 1736},
        {427, 1614},
        {715, 1558},
        {661, 1515},
        {525, 1436},
        {748, 1395},
        {743, 1383},
        {814, 1369},
        {891, 1452},
        {831, 1444},
        {429, 1402},
        {427, 1312},
        {426, 1231},
        {703, 1149},
        {928, 1139},
        {928, 1245},
        {929, 1308},
        {931, 1349},
        {930, 1394},
        {929, 1441},
        {933, 1478},
        {932, 1495},
        {939, 1502},
        {940, 1525},
        {934, 1514},
        {899, 1511},
        {902, 1507},
        {909, 1450},
        {905, 1434},
        {931, 1456},
        {921, 1461},
        {923, 1458},
        {922, 1448},
        {919, 1425},
        {907, 1431},
        {894, 1430},
        {427, 1300},
        {428, 1186},
        {428, 1077},
        {528, 989},
        {809, 977},
        {874, 997},
        {920, 1102},
        {932, 1153},
        {955, 1278},
        {982, 1422},
        {979, 1519},
        {990, 1616},
        {999, 1750},
        {982, 1863},
        {867, 1909},
        {428, 1879},
        {781, 1802},
        {884, 1775},
        {887, 1793},
        {911, 1810},
        {913, 1855},
        {909, 1884},
        {911, 1925},
        {904, 1961},
        {900, 2004},
        {859, 2023},
        {852, 2060},
        {432, 2042},
        {427, 1964},
        {428, 1888},
        {427, 1813},
        {731, 1778},
        {781, 1777},
        {873, 1780},
        {896, 1843},
        {891, 1895},
        {888, 1924},
        {869, 1945},
        {867, 1951},
        {847, 1956},
        {789, 1938},
        {817, 1898},
        {884, 1880},
        {925, 1876},
        {942, 1898},
        {946, 1930},
        {944, 1955},
        {938, 1988},
        {889, 2009},
        {851, 1990},
        {883, 1969},
        {909, 1971},
        {928, 1989},
        {919, 2024},
        {897, 2059},
        {836, 2023},
        {844, 2007},
        {843, 1962},
        {891, 1942},
        {905, 1885},
        {966, 1919},
        {981, 1956},
        {944, 1966},
        {801, 1899},
        {428, 1741},
        {850, 1601},
        {820, 1513},
        {428, 1380},
        {426, 1170},
        {428, 894},
        {563, 768},
        {832, 829},
        {822, 942},
        {735, 972},
        {739, 1015},
        {797, 1052},
        {787, 1113},
        {774, 1123},
        {745, 1185},
        {744, 1216},
        {745, 1253},
        {741, 1283},
        {744, 1295},
        {740, 1307},
        {427, 1284},
        {427, 1257},
        {426, 1249},
        {428, 1231},
        {426, 1199},
        {427, 1141},
        {429, 1025},
        {427, 1008},
        {427, 964},
        {426, 843},
        {432, 789},
        {545, 741},
        {595, 715},
        {426, 675},
        {426, 593},
        {426, 396},
        {426, 60},
        {706, -39},
        {1338, -133},
        {844, -268},
        {703, -266},
        {643, -266},
        {802, -186},
        {906, -198},
        {428, -169},
        {426, -140},
        {426, -90},
        {426, 0},
        {428, 0},
        {428, 0},
        {428, 0},
        {428, 0},
        {429, 0},
        {428, 0}
    };

static void TestSetup()
{
   //percentage of deadzone
   Throttle::throtdead = 5;
   Throttle::potmin[0] = 100;
   Throttle::potmax[0] = 4000;
   Throttle::throtmax = 100;
   Throttle::throttleRamp = 10;
   Throttle::speedLimit = 5000;
   Throttle::regenmax = -25;
   Throttle::ThrotRpmFilt = 100;
   Throttle::regenRpm = 2000;
   Throttle::regenendRpm = 200;
   Param::SetInt(Param::dir, 1);
   Param::SetFloat(Param::idcmin, -100);
   Param::SetInt(Param::udcmin, 381);
   Param::SetInt(Param::speed, 2000);

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

// static void sweep() {
//    float throttleSpnt = 100;

//    for (int i = 0; i > -150; i--) {
//       Throttle::IdcLimitCommand(throttleSpnt, i);
//       cout << "IDC: " << i << " throttleSpnt: " << throttleSpnt << endl;
//       throttleSpnt = 100;
//    }
//    for (int i = -150; i < 0; i++) {
//       Throttle::IdcLimitCommand(throttleSpnt, i);
//       cout << "IDC: " << i << " throttleSpnt: " << throttleSpnt << endl;
//       throttleSpnt = 100;
//    }
// }

static void testThrottleIsNotModifiedWhenUDCAboveMin() {

   float throttleSpnt = 0;
   bool throttleModified = false;

   for(int i = 0; i< 40; i++) {
      throttleSpnt = i;
      Throttle::UdcLimitCommand(throttleSpnt, i);
      //Throttle should equal input, not above or below.
      if (throttleSpnt != i) {
         throttleModified = true;
      }
   }
   ASSERT(throttleModified == false);   

}

static void testThrottleRampThroughOffPedalRegenRegion() {
   float throttleSpnt = 0;
   int speed = 2000;
   for(int i = 200; i< 3000; i=i+10) {
      throttleSpnt = Throttle::CalcThrottle(i, 0, false);

      //Need to call this to set the filtered speed 
      Throttle::SpeedLimitCommand(throttleSpnt,  speed);

      //Throttle should equal input, not above or below.
      cout << "PotVal: " << i << " throttleSpnt: " << throttleSpnt << endl;
   }
}


static void testThrottleRampThroughOffPedalRegenRegionIncreasingSpeed() {
   float throttleSpnt = 0;
   int speed = 0;
   for(int i = 200; i< 3000; i=i+10) {
      speed = i;
      Param::SetInt(Param::speed, speed);
      throttleSpnt = Throttle::CalcThrottle(i, 0, false);

      //Need to call this to set the filtered speed 
      Throttle::SpeedLimitCommand(throttleSpnt,  speed);

      //Throttle should equal input, not above or below.
      cout << "PotVal: " << i << " throttleSpnt: " << throttleSpnt << " speed: " << speed << endl;
   }
}

static void testThrottleStaticThroughOffPedalRegenRegionConstantSpeed() {
   float throttleSpnt = 0;
   int speed = 2000;
   for(int i = 200; i< 3000; i=i+10) {
      throttleSpnt = Throttle::CalcThrottle(1360, 0, false);

      //Need to call this to set the filtered speed 
      Throttle::SpeedLimitCommand(throttleSpnt,  speed);

      //Throttle should equal input, not above or below.
      cout << "PotVal: " << i << " throttleSpnt: " << throttleSpnt << " speed: " << speed << endl;
   }
}

static void testThrottleStatichOffPedalRegenRegionIncreasingSpeed() {
   float throttleSpnt = 0;
   int speed = 0;
   for(int i = 200; i< 3000; i=i+10) {
      speed = i;
      Param::SetInt(Param::speed, speed);
      throttleSpnt = Throttle::CalcThrottle(1360, 0, false);

      //Need to call this to set the filtered speed 
      Throttle::SpeedLimitCommand(throttleSpnt,  speed);

      //Throttle should equal input, not above or below.
      cout << "PotVal: " << i << " throttleSpnt: " << throttleSpnt << " speed: " << speed << endl;
   }
}

static void testThrottleWithRecordedData() {
   int speed;
   float throttleSpnt = 0;
   int potVal = 0;
   for (int i = 1; i < ROWS; i++) {
      speed = testData[i][SPEED_COL];
      potVal = testData[i][POT_COL];

      Param::SetInt(Param::speed, speed);
      
      throttleSpnt = Throttle::CalcThrottle(potVal, 0, false);

      //Need to call this to set the filtered speed 
      Throttle::SpeedLimitCommand(throttleSpnt,  speed);

      //Throttle should equal input, not above or below.
      cout << "PotVal: " << potVal << " throttleSpnt: " << throttleSpnt << " speed: " << speed << endl;
   }
}



void ThrottleTest::RunTest()
{
   TestSetup();
   // TestThrottleTemperateOverMaxThrottleTo0();
   // TestThrottleTemperateInDerateZoneThrottleTo50Percent();
   // TestThrottleUnderTemperateNoDeRate();
   // TestThrottleTemperateInDerateZoneThrottleButThrottleUnderLimit();
   // TestCalcThrottleIs0WhenThrottleAndBrakePressed();
   // TestCalcThrottleIs0WhenNoThrottleAndBrakePressed();
   // TestCalcThrottleIs0WhenInDeadZone();
   // TestCalcThrottleIsAbove0WhenJustOutOfDeadZone();
   // TestCalcThrottleIs100WhenMax();
   // TestCalcThrottleIs100WhenOverMax();
   // TestIdcLimitCommandWhenUnderIDCMin();
   // TestIdcLimitCommandWhenOverIDCMin();
   // TestIdcLimitCommandWhenOverIDCMinIncreasedWhenCurrentReduces();
   // testThrottleIsNotModifiedWhenUDCAboveMin();
   //testThrottleRampThroughOffPedalRegenRegion();
   //testThrottleRampThroughOffPedalRegenRegionIncreasingSpeed();
   //testThrottleStaticThroughOffPedalRegenRegionConstantSpeed();
   testThrottleWithRecordedData();
}
