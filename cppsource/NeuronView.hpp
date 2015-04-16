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

class NeuronView : public DeviceView
{
public:
    NeuronView(const Neuron & neuron_p, const ViewResources & vRes_p);
    virtual ~NeuronView() {}
    virtual void Draw(sf::RenderTarget & rt);
    virtual bool IsDead() const {return neuron_m.IsDead();}
private:
    const Neuron & neuron_m;
    sf::Vector2f targetPos;
    sf::Vector2f actualPos;
    sf::RectangleShape shape;
    sf::Text thresholdText;
};

#endif	/* NEURONVIEW_HPP */

