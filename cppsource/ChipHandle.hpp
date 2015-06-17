/* 
 * File:   ChipHandle.hpp
 * Author: Dormouse
 *
 * Created on 17 June 2015, 17:00
 */

#ifndef CHIPHANDLE_HPP
#define	CHIPHANDLE_HPP

#include <memory>
#include <vector>
#include <string>
#include <SFML/System.hpp>  //for sf::Vector
#include "Gobject.hpp"
#include "ChipPlan.hpp"

class ChipHandle : public Gobject
{
public:
    ChipHandle(int serial_p, sf::Vector2i pos_p) :pos(pos_p), serial(serial_p) {}
    virtual ~ChipHandle() {}
    bool operator==(const ChipHandle& rhs) const    { return this == &rhs; }

    sf::Vector2i GetPosition() const            {return pos;}
    void SetPosition(sf::Vector2i p)            {pos = p;}
    int GetSerial() const                       {return serial;}

private:
    int serial;
    sf::Vector2i pos;
    std::shared_ptr<ChipPlan> plan;
};

#endif	/* CHIPHANDLE_HPP */

