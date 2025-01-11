#include "Preheater.h"
#include "errormessage.h"
#include "digio.h"

Preheater::Preheater()
{
    //ctor
    Preheater::PreheatTicks=0;
    Preheater::PreheatTicks_1Min=0;
    Preheater::initbyPreHeat=false;
}

void Preheater::SetInitByPreHeat(bool initbyPH) {
    Preheater::initbyPreHeat = initbyPH;
}

bool Preheater::GetInitByPreHeat() {
    return Preheater::initbyPreHeat;
}

bool Preheater::GetRunPreHeat() {
    return Preheater::RunPreHeat;
}

void Preheater::Ms10Task() {
    Param::SetInt(Param::opmode, MOD_PREHEAT);
    Param::SetInt(Param::HeatReq, true);
    ErrorMessage::UnpostAll();
    if(!Preheater::RunPreHeat)
    {
        Param::SetInt(Param::opmode, MOD_OFF);
        Param::SetInt(Param::HeatReq, false);
    }
}

void Preheater::Task200Ms(int opmode, unsigned hours, unsigned minutes)
{
    if(Preheater::PreHeatSet == 2)  //0 - Disabled, 1 - Enabled, 2 - Preheat timer
    {
        Param::SetInt(Param::PreheatDebug1, PreheatTicks);
        if(opmode!=MOD_PREHEAT)
        {
            if((PreHeatHrs_tmp==hours)&&(PreHeatMins_tmp==minutes)&&(PreHeatDur_tmp!=0)) {
                RunPreHeat = true;//if we arrive at set preheat time and duration is non zero then initiate preheat
            }
            else {
                RunPreHeat = false;
            } 
        }

        if(opmode==MOD_PREHEAT)
        {
            if(PreheatTicks!=0)
            {
                PreheatTicks--; //decrement charge timer ticks
                PreheatTicks_1Min++;
            }

            if(PreheatTicks==0)
            {
                RunPreHeat=false; //end preheat once timer expires.
                PreheatTicks = (GetInt(Param::Pre_Dur)*300);//recharge the tick timer
            }

            if (PreheatTicks_1Min==300)
            {
                PreheatTicks_1Min=0;
                PreHeatDur_tmp--; //countdown minutes of charge time remaining.
            }
        }

    }

}

void Preheater::ParamsChange() {
    PreHeatHrs_tmp=GetInt(Param::Pre_Hrs);
    PreHeatMins_tmp=GetInt(Param::Pre_Min);
    PreHeatSet = Param::GetInt(Param::Control);//0=disable,1=enable,2=timer. Heater control
    PreheatTicks = (GetInt(Param::Pre_Dur)*300);//number of 200ms ticks that equates to preheat timer in minutes
    PreHeatDur_tmp = GetInt(Param::Pre_Dur);
}