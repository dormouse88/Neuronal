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

class NeuronView
{
public:
    NeuronView(const Neuron & n);

    void Draw(sf::RenderTarget & rt, const Neuron & n);
private:
    VectorWorld perceivedPos_;
    sf::ConvexShape simpleShape;
    sf::ConvexShape fullShape;
    sf::ConvexShape bulbShape;
    sf::Text thresholdText;
    sf::Text receivedText;
};

//enum class Charge { ON, OFF };
typedef bool Charge;

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

    bool IsThresholdMet() const                 {return receivedSum_ >= threshold_; }  //for drawing only
    bool GetFiring() const                      {return charge_;}                      //for drawing only
    int GetThreshold() const                    {return threshold_;}                   //for drawing only
    int GetReceivedCharge() const               {return receivedSum_;}                 //for drawing only
    bool HasBulb() const                        {return hasBulb_;}                     //for drawing only
    bool IsSimple() const                       {return threshold_ == 1;}               //for drawing only
    bool GetBulbCharge() const                  {return bulbCharge_;}                   //for drawing only
    
    void ModifyThreshold(int v)                 {threshold_ += v;}
    
private:
    void FireIfReady();
    bool ReceivedAll() const                    { return receivedNum_ == GetInWiresNum(); }

    bool hasBulb_;
    Charge bulbCharge_;
    Charge charge_;
    int threshold_;
    int receivedSum_;
    int receivedNum_;

    NeuronView v;
};

#endif	/* NEURON_HPP */

