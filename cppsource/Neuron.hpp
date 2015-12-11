/* 
 * File:   Neuron.hpp
 * Author: Dormouse
 *
 * Created on 13 January 2015, 16:05
 */

#ifndef NEURON_HPP
#define	NEURON_HPP

#include <SFML/Graphics.hpp>
#include "miscUtil.hpp"
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
    sf::Text receivedText;
};



class Neuron : public Device
{
public:
    Neuron(int serial_p, sf::Vector2i pos_p, int threshold_p, std::shared_ptr<ChipPlan> cont);
    virtual ~Neuron() {;}

    //Misc...
    virtual std::string SerialName() const override         { return "NEUR";}
    virtual void Draw(sf::RenderTarget & rt) override;
    virtual void Handle(int code) override;

    //Wirable...
    virtual void ReceiveCharge(bool charge, int weight, int slot) override;
    virtual VectorWorld GetWireAttachPos(WireAttachSide was) const override;
    virtual bool IsWeightedIn() const override              {return true;}
    
    //Device...
    virtual void LogicAct() override;
    virtual void LogicCalculate() override;

    bool IsThresholdMet() const                 {return receivedCharge >= threshold; }    //for drawing + private use only
    bool GetFiring() const                      {return firing;}  //for drawing only
    //RED//void SetFiring(bool f)                      {firing = f;}
    int GetThreshold() const                    {return threshold;}  //for drawing only
    int GetReceivedCharge() const               {return receivedCharge;}  //for drawing only
    
    void ModifyThreshold(int v)                 {threshold += v;}
    
private:
    bool firing;
    int threshold;
    int receivedCharge;
    NeuronView v;
};

#endif	/* NEURON_HPP */

