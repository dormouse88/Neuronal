/* 
 * File:   ChipPlan.hpp
 * Author: Dormouse
 *
 * Created on 17 June 2015, 16:57
 */

#ifndef CHIPPLAN_HPP
#define	CHIPPLAN_HPP

#include <memory>
#include <vector>
#include <string>
#include "Gobject.hpp"
class ChipHandle;
//#include "ChipHandle.hpp"
#include "Device.hpp"
#include "Wire.hpp"

class ChipPlan : public Gobject
{
public:
    ChipPlan();
    virtual ~ChipPlan() {}
    bool operator==(const ChipPlan& rhs) const    { return this == &rhs; }

private:
    int planID;
    std::string name;
    std::vector<std::shared_ptr<ChipHandle> > handles;
    std::vector<std::shared_ptr<Device> > devices;
    std::vector<std::shared_ptr<Wire> > wires;
};
#endif	/* CHIPPLAN_HPP */

