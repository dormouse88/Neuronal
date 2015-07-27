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
#include "Gobject.hpp"
#include "Wirable.hpp"
class ChipHandle;
#include "Device.hpp"
#include "Wire.hpp"

class ChipPlan : public Wirable
{
public:
    ChipPlan();
    virtual ~ChipPlan() {}

    virtual void ReceiveCharge(bool charge, int weight, int slot);

    void StepIn(bool charge, int slot);
    void PassOnAct();
    void PassOnCalculate();


    void SetPosition(Device & d, sf::Vector2i newPos);
    int GetFreeSerial() const;
    bool IsSerialFree(int serial) const;
    bool IsPositionFree(sf::Vector2i pos) const;
    bool IsWiringFree(Wirable & from, int fromSlot, Wirable & to, int toSlot) const;

    void ImportDevice(std::shared_ptr<Device> device);
    void ImportWire(std::shared_ptr<Wire> wire);

    std::shared_ptr<Device> GetDevice(sf::Vector2i pos);
    std::shared_ptr<Device> GetDevice(int serial);
    std::shared_ptr<Wire> GetWire(const Wirable& from, const Wirable& to);
    std::vector<std::shared_ptr<Wire> > GetWires(std::shared_ptr<Wirable> device, bool from, bool to); 

private:
    int planID;
    std::weak_ptr<ChipHandle> referer;  //need to be able to pass charge back out
    std::vector<std::shared_ptr<Device> > devices;
    std::vector<std::shared_ptr<Wire> > wires;
};
#endif	/* CHIPPLAN_HPP */

