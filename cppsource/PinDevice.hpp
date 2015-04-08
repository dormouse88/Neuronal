/* 
 * File:   PinDevice.hpp
 * Author: Dormouse
 *
 * Created on 06 April 2015, 19:09
 */

#ifndef PINDEVICE_HPP
#define	PINDEVICE_HPP

#include <SFML/System.hpp>  //for sf::Vector
class Wire;

class PinDevice {
public:
    PinDevice(sf::Vector2i pos_p, bool multiInput_p);
    virtual ~PinDevice() {;}
    bool operator==(const PinDevice& rhs) const    { return this->GetPosition() == rhs.GetPosition(); }    
    
    sf::Vector2i GetPosition() const {return pos;}
    void SetPosition(sf::Vector2i p) {pos = p;}
   
    void RegisterIn(Wire* wp);
    void RegisterOut(Wire* wp);
    void DeRegister(Wire* wp);

    //Not-saved
    std::vector<Wire*> inWires;
    std::vector<Wire*> outWires;
private:
    bool multiInput;
    sf::Vector2i pos;
};

#endif	/* PINDEVICE_HPP */

