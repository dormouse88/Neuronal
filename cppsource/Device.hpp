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
#include <string>
#include <SFML/System.hpp>  //for sf::Vector
#include "Gobject.hpp"
class Wire;

class Device : public Gobject
{
public:
    Device(int serial_p, sf::Vector2i pos_p, int in = -1, int out = -1) :pos(pos_p), serial(serial_p), firing(false), inLimit(in), outLimit(out) {}
    virtual ~Device() {}
    bool operator==(const Device& rhs) const    { return this == &rhs; }

    virtual std::string SerialName() const = 0;
    virtual bool IsInstant() = 0;
    virtual void ReceiveCharge(int weight) = 0;
    virtual void PushCharge() = 0;
    virtual void CalculateFiring() {}

    bool GetFiring() const                      {return firing;}
    void SetFiring(bool f)                      {firing = f;}
    sf::Vector2i GetPosition() const            {return pos;}
    void SetPosition(sf::Vector2i p)            {pos = p;}
    int GetSerial() const                      {return serial;}
    void RegisterIn(std::shared_ptr<Wire> w)    { inWires.push_back(w);}
    void RegisterOut(std::shared_ptr<Wire> w)   { outWires.push_back(w);}

    bool CanAcceptInput() const;
    bool CanAcceptOutput() const;
protected:
    int inLimit;
    int outLimit;
    mutable std::vector<std::weak_ptr<Wire> > inWires;
    mutable std::vector<std::weak_ptr<Wire> > outWires;
private:
    void CleanWireVectors() const;
    int serial;
    sf::Vector2i pos;
    bool firing;
};

#endif	/* DEVICE_HPP */

