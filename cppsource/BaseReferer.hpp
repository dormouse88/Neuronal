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
    SlotData()                                      :tag("NTAG"), name("NNAME"), charge(Charge::MAYBE) {}
    SlotData(Tag t, std::string n)                  :tag(t), name(n), charge(Charge::OFF) {}
    SlotData(Tag t, std::string n, Charge c)        :tag(t), name(n), charge(c) {}
    Tag tag;
    std::string name;
    Charge charge;
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
    virtual void StepOutReCalculateCharge(Tag slot) override;
    virtual Charge StepOutGetOutgoingCharge(Tag slot) override;
    virtual void SetModified() override;
    virtual void SetSubPlan(PlanShp, std::shared_ptr<RefererInterface>) override;
    virtual PlanShp GetSubPlan() override;

    void DefineXputs(XPuts all, XPutFilter filter);
    void SetInputState(std::string, bool);
    void TickOnce();
    bool IsAnyOutputOn() const;
    std::map<std::string, Charge> GetOutputs();

    void DrawBrain(sf::RenderTarget & rt);
    
private:
    void RefreshOutputs();

    std::map<std::string, SlotData > inputs_;
    std::map<Tag, SlotData > outputs_;
    PlanShp subPlan_;
};


#endif	/* BASEREFERER_HPP */

