/*
 * This file is part of the stm32-... project.
 *
 * Copyright (C) 2021 Johannes Huebner <dev@johanneshuebner.com>
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

#include "outlanderheater.h"
#include "params.h"


OutlanderHeater::OutlanderHeater()
{
   //ctor
}

void OutlanderHeater::SetPower(uint16_t power, bool HeatReq) {
   if (power > 0 && HeatReq && currentTemperature <= targetTemperature) {

         uint8_t canData[8];
         canData[0] = 0x00;
         canData[1] = 0x00;
         canData[2] = 0x14;
         canData[3] = 0x21;
         canData[4] = 0x90;
         canData[5] = 0xFE;
         canData[6] = 0x0C;
         canData[7] = 0x10;
         can->Send(0x285, canData, 8);

         canData[0] = 0x03;
         canData[1] = 0x50;
         canData[3] = 0x4D;
         canData[4] = 0x00;
         canData[5] = 0x00;
         canData[6] = 0x00;
         canData[7] = 0x00;

         if (currentTemperature < targetTemperature - 10) {
            canData[2] = 0xA2;
         } else {
            canData[2] = 0x32;
         }

         can->Send(0x188, canData, 8);

   }
}

void OutlanderHeater::SetTargetTemperature(float temp) {
   targetTemperature = temp;
}

void OutlanderHeater::SetCanInterface(CanHardware* c)
{
   can = c;
   can->RegisterUserMessage(0x398);//Outlander Heater Msg
}

void OutlanderHeater::DecodeCAN(int id, uint32_t data[2]) {
   if (0x398 == id) {
      int8_t temp1 = data[3] - 40;
      int8_t temp2 = data[4] - 40;
      if (temp2 > temp1) {
         currentTemperature = temp2;
      } else {
         currentTemperature = temp1;
      }
      Param::SetInt(Param::HeatActual, currentTemperature);
      active = data[5] > 0;
      
      if (data[6] == 0x09) {
         hvPresent = false;
      } else if (data[6] == 0x00) {
         hvPresent = true;
      }
   }
}


