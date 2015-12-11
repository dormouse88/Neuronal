/* 
 * File:   BaseReferer.hpp
 * Author: Dormouse
 *
 * Created on 11 December 2015, 14:25
 */

#ifndef BASEREFERER_HPP
#define	BASEREFERER_HPP

#include <memory>
#include <map>
#include <vector>
#include <string>
#include "RefererInterface.hpp"
#include "ChipPlan.hpp"

struct SlotData
{
    SlotData(int s, std::string n, bool c) :slot(s), name(n), charge(c) {}
    int slot;
    std::string name;
    bool charge;
};

class BaseReferer : public RefererInterface
{
public:
    BaseReferer();
    
    //RefererInterface...
    virtual void StepOut(bool charge, int slot) override;
    virtual void SetModified() override;
    virtual void SetSubPlan(std::shared_ptr<ChipPlan>, std::shared_ptr<RefererInterface>) override;
    virtual std::shared_ptr<ChipPlan> GetSubPlan() override;

    void DefineXputs(std::vector<std::string> ins, std::vector<std::string> outs);
    void SetInputState(std::string, bool);
    void TickOnce();
    std::map<std::string, bool> RetrieveOutputs();
    
private:
    std::map<std::string, SlotData > inputs;
    std::map<int, SlotData > outputs;
    std::shared_ptr<ChipPlan> basePlan;
    bool outputsReady;
};


#endif	/* BASEREFERER_HPP */

