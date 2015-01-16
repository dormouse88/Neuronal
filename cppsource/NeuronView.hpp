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
private:
    const Neuron & neuron_m;
    sf::RectangleShape shape;
    sf::Text thresholdText;
};

#endif	/* NEURONVIEW_HPP */

