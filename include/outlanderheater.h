/*
 * This file is part of the stm32-vcu project.
 *
 * Copyright (C) 2021 Damien Maguire
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
#ifndef OUTLANDERHEATER_H
#define OUTLANDERHEATER_H

#include <heater.h>

class OutlanderHeater: public Heater
{
public:
   OutlanderHeater();
   void SetPower(uint16_t power, bool HeatReq);
   void SetTargetTemperature(float temp);
   void DecodeCAN(int, uint32_t*);
   void SetCanInterface(CanHardware* c);


private:
   int8_t currentTemperature;
   bool hvPresent;
   bool active;
   float targetTemperature;
};

#endif