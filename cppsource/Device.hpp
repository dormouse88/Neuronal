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
#include <iostream>

class Device : public Gobject
{
public:
    Device(sf::Vector2i pos_p, int in = -1, int out = -1) :pos(pos_p), firing(false), inLimit(in), outLimit(out) {}
    virtual ~Device() = 0;
    bool operator==(const Device& rhs) const    { return this == &rhs; }

    virtual std::string SerialName() const = 0;
    virtual bool IsInstant() = 0;
    virtual void ReceiveCharge(int weight) = 0;
    virtual void PushCharge() = 0;
    virtual void CalculateFiring() {}

    bool GetFiring() const {return firing;}
    void SetFiring(bool f) {firing = f;}
    sf::Vector2i GetPosition() const {return pos;}
    void SetPosition(sf::Vector2i p) {pos = p;}
    void RegisterIn(std::shared_ptr<Wire> w)  { inWires.push_back(w);}
    void RegisterOut(std::shared_ptr<Wire> w) { outWires.push_back(w);}

    bool CanAcceptInput() const {
        CleanWireVectors();
        return inLimit  >  inWires.size() or  inLimit==-1 ? true : false;
    }
    bool CanAcceptOutput() const {
        CleanWireVectors();
        return outLimit > outWires.size() or outLimit==-1 ? true : false;
    }
protected:
    int inLimit;
    int outLimit;
    mutable std::vector<std::weak_ptr<Wire> > inWires;
    mutable std::vector<std::weak_ptr<Wire> > outWires;
private:
    void CleanWireVectors() const //this method is necessary since the limits work by checking vector size.
    {
        auto cleanVector = [&] (std::vector<std::weak_ptr<Wire> > & wv) {
            auto remove_func = [] (std::weak_ptr<Wire> & w) { return w.expired(); } ;
            auto new_end = std::remove_if(begin(wv), end(wv), remove_func );
            wv.erase(new_end, end(wv));
        };
        cleanVector(inWires);
        cleanVector(outWires);
    }
    sf::Vector2i pos;
    bool firing;
};

inline Device::~Device() {}

#endif	/* DEVICE_HPP */

