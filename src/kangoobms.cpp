/*
 * This file is part of the ZombieVerter project.
 *
 * Copyright (C) 2022 Charlie Smurthwaite
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

#include "stm32_vcu.h"
#include "stm32_can.h"
#include <kangoobms.h>
/*
 * This module receives messages from SimpBMS and updates the
 * BMS_MinV, BMS_MaxV, BMS_MinT and BMS_MaxT parameters with the
 * received values. It also implements a timeout to indicate whether
 * the BMS is actively sending data or not. This data can be
 * used to safely stop any charging process if the BMS is not
 * working correctly.
 */

void KangooBMS::SetCanInterface(CanHardware* c)
{
   can = c;
   can->RegisterUserMessage(0x155);
   can->RegisterUserMessage(0x424);
   can->RegisterUserMessage(0x425);
   can->RegisterUserMessage(0x7BB);

}

bool KangooBMS::BMSDataValid() {
   // Return false if primary BMS is not sending data.
   if(timeoutCounter < 1) return false;
   return true;
}

float KangooBMS::GetCurrent() 
{
   return current;
}

// Return the maximum charge current allowed by the BMS.
float KangooBMS::MaxChargeCurrent()
{
   // if(!ChargeAllowed()) return 0;
   // return chargeCurrentLimit / 1000.0;
}

// Process voltage and temperature message from SimpBMS.
void KangooBMS::DecodeCAN(int id, uint8_t *data)
{
   if (id == 0x155)
   {
      // Reset timeout counter to the full timeout value
      timeoutCounter = Param::GetInt(Param::BMS_Timeout) * 10;
      stateOfCharge = (float) ((data[4] << 8) + data[5]) * 0.0025;
      //maxCharging = data[0] * 300;

      current = (float) ((((data[1] << 8) & 0x0F) + data[2]) * 0.25) - 500;

      Param::SetFloat(Param::SOC, stateOfCharge);
   } else if (id == 0x424) {

      minTempC = (uint8_t)(data[4]) - 40;
      maxTempC = (uint8_t)(data[7]) - 40;

   } else if (id == 0x425) {
      minCellV = (((float)(((data[6] & 0x01) << 8) + data[7]) + 100) * 10);
      maxCellV = ((float)(((data[4] & 0x03) << 7) + ((data[5] >> 1) + 100)) * 10);
      remainingKHW = (float)(data[1] * 0.1);
   }
}

void KangooBMS::Task100Ms() {
   // Decrement timeout counter.
   if(timeoutCounter > 0) timeoutCounter--;

   if(Param::GetInt(Param::opmode) != MOD_OFF) {
      //send can message
      uint8_t bytes[8];
      bytes[0]=0x07;
      bytes[1]=0x1D;
      bytes[2]=0x00;
      bytes[3]=0x02;
      bytes[4]=0x5D;
      bytes[5]=0x80;
      bytes[6]=0x5D;
      bytes[7]=0xD8;

      can->Send(0x423, (uint32_t*)bytes, 8);

      if(BMSDataValid()) {
         Param::SetFloat(Param::BMS_Vmin, minCellV);
         Param::SetFloat(Param::BMS_Vmax, maxCellV);
         Param::SetFloat(Param::BMS_Tmin, minTempC);
         Param::SetFloat(Param::BMS_Tmax, maxTempC);
         Param::SetFloat(Param::KWh, remainingKHW);
      }
      else
      {
         Param::SetFloat(Param::BMS_Vmin, 0);
         Param::SetFloat(Param::BMS_Vmax, 0);
         Param::SetFloat(Param::BMS_Tmin, 0);
         Param::SetFloat(Param::BMS_Tmax, 0);
         Param::SetFloat(Param::KWh, 0);
      }

   }



}
