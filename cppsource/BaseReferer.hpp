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
#include <set>
#include <string>
#include "RefererInterface.hpp"
#include "ChipPlan.hpp"

struct SlotData
{
    SlotData()                                  :slot(7), name(), charge(false) {}
    SlotData(int s, std::string n, bool c)      :slot(s), name(n), charge(c) {}
    int slot;
    std::string name;
    bool charge;
};

struct XPuts
{
    std::vector<SlotData> ins;
    std::vector<SlotData> outs;
};
typedef std::shared_ptr<std::set<std::string> > XPutFilter;

class BaseReferer : public RefererInterface
{
public:
    BaseReferer();
    
    //RefererInterface...
    virtual void StepOutRefresh(int slot) override;
    virtual bool StepOutGetOutgoingCharge(int slot) override;
    virtual void SetModified() override;
    virtual void SetSubPlan(std::shared_ptr<ChipPlan>, std::shared_ptr<RefererInterface>) override;
    virtual std::shared_ptr<ChipPlan> GetSubPlan() override;

    void DefineXputs(XPuts all, XPutFilter filter);
    void SetInputState(std::string, bool);
    void TickOnce();

    void DrawBrain(sf::RenderTarget & rt);
    
    bool IsAnyOutputOn() const;
    std::map<std::string, bool> GetOutputs();
private:
    void RefreshOutputs();

    std::map<std::string, SlotData > inputs;
    std::map<int, SlotData > outputs;
    std::shared_ptr<ChipPlan> basePlan;
};


#endif	/* BASEREFERER_HPP */

