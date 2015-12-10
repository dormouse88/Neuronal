/* 
 * File:   RefererInterface.hpp
 * Author: Dormouse
 *
 * Created on 19 November 2015, 23:09
 */

#ifndef REFERERINTERFACE_HPP
#define	REFERERINTERFACE_HPP

#include "ChipPlan.hpp"

class RefererInterface
{
public:
    virtual void StepOut(bool charge, int slot) = 0;
    virtual void SetModified() = 0;
    virtual void SetSubPlan(std::shared_ptr<ChipPlan>, std::shared_ptr<RefererInterface>) = 0;
    virtual std::shared_ptr<ChipPlan> GetSubPlan() = 0;
};

#endif	/* REFERERINTERFACE_HPP */

