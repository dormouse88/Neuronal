/* 
 * File:   Neuron.hpp
 * Author: Dormouse
 *
 * Created on 13 January 2015, 16:05
 */

#ifndef NEURON_HPP
#define	NEURON_HPP

#include <SFML/Graphics.hpp>
#include "ViewResources.hpp"
#include "Device.hpp"
class Neuron;

class NeuronView : public DeviceView
{
public:
    NeuronView(const Neuron & n);
    virtual ~NeuronView() {}

    void Draw(sf::RenderTarget & rt, const Neuron & n);
private:
    sf::RectangleShape shape;
    sf::Text thresholdText;
};



class Neuron : public Device
{
public:
    Neuron(int serial_p, sf::Vector2i pos_p, int threshold_p);
    virtual ~Neuron() {;}

    virtual std::string SerialName() const { return "NEUR";}
    virtual void ReceiveCharge(bool charge, int weight, int slot);
    virtual void LogicAct();
    virtual void LogicCalculate();

    bool GetFiring() const                      {return firing;}
    void SetFiring(bool f)                      {firing = f;}
    int GetThreshold() const {return threshold;}
    void ModifyThreshold(int v) {threshold += v;}

    virtual sf::Vector2f GetWireAttachPos(WireAttachSide was) const override;
    virtual void Draw(sf::RenderTarget & rt) override;
    virtual void Handle(int code) override;
    
private:
    bool firing;
    int threshold;
    int receivedCharge;
    NeuronView v;
};

#endif	/* NEURON_HPP */

