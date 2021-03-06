/* 
 * File:   RefererInterface.hpp
 * Author: Dormouse
 *
 * Created on 19 November 2015, 23:09
 */

#ifndef REFERERINTERFACE_HPP
#define	REFERERINTERFACE_HPP

#include "BasicTypes.hpp"

class ChipPlan;

class RefererInterface
{
public:
    virtual void StepOutReCalculateCharge(Tag slot) = 0;
    virtual Charge StepOutGetOutgoingCharge(Tag slot) = 0;
    virtual void SetModified() = 0;
    virtual void SetSubPlan(PlanShp, std::shared_ptr<RefererInterface>) = 0;
    virtual PlanShp GetSubPlan() = 0;
};

#endif	/* REFERERINTERFACE_HPP */

