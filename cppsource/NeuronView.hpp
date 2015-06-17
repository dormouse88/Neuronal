/* 
 * File:   NeuronView.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 11:57
 */

#ifndef NEURONVIEW_HPP
#define	NEURONVIEW_HPP

#include <SFML/Graphics.hpp>
#include "Neuron.hpp"
#include "ViewResources.hpp"
#include "DeviceView.hpp"

#include <iostream>

class NeuronView : public DeviceView
{
public:
    NeuronView(std::shared_ptr<const Neuron> neuron_p, const ViewResources & vRes_p);
    virtual ~NeuronView() { std::cout << "NVIEW KILLED" << std::endl; }
    virtual void Draw(sf::RenderTarget & rt);
    //virtual bool IsDead() const {return neuron_m.IsDead();}
private:
    std::weak_ptr<const Neuron> neuron_m;
    sf::RectangleShape shape;
    sf::Text thresholdText;
};

#endif	/* NEURONVIEW_HPP */

