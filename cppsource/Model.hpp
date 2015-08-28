/* 
 * File:   Model.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 11:53
 */

#ifndef MODEL_HPP
#define	MODEL_HPP

#include <memory>
#include "ChipPlan.hpp"

class Model
{
public:
    Model();
    Model(const Model&) = delete;
    void DebugInfo();
    void Logic();
    
    std::shared_ptr<ChipPlan> GetBasePlan() {return basePlan;}
private:
    std::shared_ptr<ChipPlan> basePlan;
};

#endif	/* MODEL_HPP */

