/* 
 * File:   Device.hpp
 * Author: Dormouse
 *
 * Created on 06 April 2015, 19:09
 */

#ifndef DEVICE_HPP
#define	DEVICE_HPP

#include <memory>
#include <vector>
#include <SFML/System.hpp>  //for sf::Vector
#include "Gobject.hpp"
class Wire;

class Device : public Gobject
{
public:
    Device(sf::Vector2i pos_p) :pos(pos_p) {}
    virtual ~Device() = 0;
    bool operator==(const Device& rhs) const    { return this->GetPosition() == rhs.GetPosition(); }

    virtual bool IsInstant() = 0;
    virtual void ReceiveCharge(int weight) = 0;
    virtual void PushCharge() = 0;
    virtual void CalculateFiring() = 0;

    sf::Vector2i GetPosition() const {return pos;}
    void SetPosition(sf::Vector2i p) {pos = p;}
    void RegisterIn(std::shared_ptr<Wire> w) { inWires.push_back(w);}
    void RegisterOut(std::shared_ptr<Wire> w) { outWires.push_back(w);}

protected:
    std::vector<std::weak_ptr<Wire> > inWires;
    std::vector<std::weak_ptr<Wire> > outWires;
private:
    sf::Vector2i pos;
};

inline Device::~Device() {}

#endif	/* DEVICE_HPP */

