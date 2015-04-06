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

class NeuronView {
public:
    NeuronView(const Neuron & neuron_p, const ViewResources & vRes_p);
    void Draw(sf::RenderTarget & rt);
    bool AmIYourDaddy(const Neuron & n) const {return n==neuron_m;}
private:
    const Neuron & neuron_m;
    sf::Vector2f targetPos;
    sf::Vector2f actualPos;
    sf::RectangleShape shape;
    sf::Text thresholdText;
};

#endif	/* NEURONVIEW_HPP */

