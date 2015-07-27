/* 
 * File:   ChipHandle.cpp
 * Author: Dormouse
 * 
 * Created on 17 June 2015, 17:00
 */

#include "ChipHandle.hpp"

//ChipHandle::ChipHandle() {}

void ChipHandle::ReceiveCharge(bool charge, int weight, int slot)
{
    plan->StepIn(charge, slot);
}

void ChipHandle::StepOut(bool charge, int slot)
{
    PushCharge(charge, slot);
}

void ChipHandle::LogicAct()
{
    plan->PassOnAct();
}
void ChipHandle::LogicCalculate()
{
    plan->PassOnCalculate();
}

