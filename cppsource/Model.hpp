/* 
 * File:   Model.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 11:53
 */

#ifndef MODEL_HPP
#define	MODEL_HPP

#include <memory>
#include "BlobFactory.hpp"
#include "Serializer.hpp"
#include "ChipPlan.hpp"

class Model : public RefererInterface
{
public:
    Model();
    Model(const Model&) = delete;

    void Logic();
    
    virtual void StepOut(bool charge, int slot) override;
    virtual void SetModified() override;
    virtual void SwapIn(std::shared_ptr<ChipPlan>) override;
    
    void SetBasePlan(std::shared_ptr<ChipPlan> newPlan)         {basePlan = newPlan;}
    std::shared_ptr<ChipPlan> GetBasePlan()                     {return basePlan;}

    std::shared_ptr<ChipPlan> WipePlan(std::shared_ptr<ChipPlan> plan, bool forced);
    std::shared_ptr<ChipPlan> LoadPlan(int num, std::shared_ptr<ChipPlan> plan);
    void SavePlan(PlanPos pos);
private:
    Serializer serializer;
    std::shared_ptr<ChipPlan> basePlan;
};


#endif	/* MODEL_HPP */

