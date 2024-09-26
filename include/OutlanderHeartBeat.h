/*
 * This file is part of the ZombieVerter project.
 *
 * Copyright (C) 2021-2023  Johannes Huebner <dev@johanneshuebner.com>
 * 	                        Damien Maguire <info@evbmw.com>
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
 *
 *Control of the Mitsubishi Outlander PHEV on board charger (OBC) and DCDC Converter.
 *
 */
#ifndef OUTLANDERHEARTBEAT_H
#define OUTLANDERHEARTBEAT_H

#include <stdint.h>
#include "my_fp.h"
#include "my_math.h"
#include "hwinit.h"
#include "params.h"
#include "chargerhw.h"
#include <libopencm3/stm32/timer.h>

class OutlanderHeartBeat
{

public:
void Task100Ms();
void SetCanInterface(CanHardware* c);
void SetPullInEVSE(bool pullInEVSE);


protected:
    CanHardware* can;
    bool pullInEVSE=false;

};

#endif // OUTLANDERHEARTBEAT_H
