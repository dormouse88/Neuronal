/* 
 * File:   Neuron.hpp
 * Author: Dormouse
 *
 * Created on 13 January 2015, 16:05
 */

#ifndef NEURON_HPP
#define	NEURON_HPP

#include <SFML/Graphics.hpp>
#include "BasicTypes.hpp"
#include "ViewResources.hpp"
#include "Device.hpp"


class Neuron : public Device
{
public:
    Neuron(PlanID serial, sf::Vector2i pos, CWeight threshold, bool hasBulb, PlanShp cont);
    virtual ~Neuron() {;}

    //PlanOwned...
    virtual std::string SerialName() const override         { return "NEUR";}
    virtual void Draw(sf::RenderTarget & rt) override;
    virtual void Handle(HandleCode code) override;
    
    //Wirable...
    virtual void StructuralRefresh() override;
    virtual void ReCalculateCharge(Tag) override;
    virtual Charge GetOutgoingCharge(Tag) override;
    virtual VectorWorld GetWireAttachPos(WireAttachSide, Tag) const override;
    virtual bool IsWeightedIn() const override              {return true;}
    
    //Device...
    virtual void PreInnerStep() override;
    virtual void InnerStep() override;
    virtual VectorDumb GetPlodedSize() override          { return VectorDumb {1, 1}; }
    
    bool IsSimple() const                       {return threshold_ == 1;}
    CWeight GetThreshold() const                {return threshold_;}   //for serializer
    bool HasBulb() const                        {return hasBulb_;}     //for serializer
//    bool IsThresholdMet() const                 {return receivedSum_ >= threshold_; }  //for drawing only
//    bool GetFiring() const                      {return calculatedCharge_;}                      //for drawing only
//    CWeight GetReceivedCharge() const               {return receivedSum_;}                 //for drawing only
//    bool HasBulb() const                        {return hasBulb_;}                     //for drawing only
//    bool GetBulbCharge() const                  {return outgoingCharge_;}                   //for drawing only
    
private:
    void InitVisuals();
    
    bool hasBulb_;
    Charge outgoingCharge_;
    Charge intermediateCharge_;
    Charge calculatedCharge_;
    CWeight threshold_;
    CWeight totalIncoming_;

    sf::ConvexShape simpleShape;
    sf::ConvexShape fullShape;
    sf::ConvexShape bulbShape;
    sf::Text thresholdText;
    sf::Text receivedText;
};

#endif	/* NEURON_HPP */

