/* 
 * File:   Neuron.hpp
 * Author: Dormouse
 *
 * Created on 13 January 2015, 16:05
 */

#ifndef NEURON_HPP
#define	NEURON_HPP

#include <SFML/System.hpp>  //for sf::Vector
#include "Gobject.hpp"
class Wire;

class Neuron// : public Gobject
{
public:
    Neuron(sf::Vector2i pos_p);
    bool operator==(const Neuron& rhs) const    { return this->GetPosition() == rhs.GetPosition(); }    

    void StepPartOne();
    void StepPartTwo();
    int GetThreshold() const {return threshold;}
    void SetThreshold(int t) {threshold = t;}
    void ModifyThreshold(int v) {threshold += v;}
    sf::Vector2i GetPosition() const {return pos;}
    void SetPosition(sf::Vector2i p) {pos = p;}

    void RegisterIn(Wire* wp)  {inWires.emplace_back(wp);}
    void RegisterOut(Wire* wp) {outWires.emplace_back(wp);}
    void DeRegister(Wire* wp);

    //Not-saved
    std::vector<Wire*> inWires;
    std::vector<Wire*> outWires;
    bool firing;
private:
    //Saved
    //unsigned id;
    int threshold;
    sf::Vector2i pos;
};

#endif	/* NEURON_HPP */

