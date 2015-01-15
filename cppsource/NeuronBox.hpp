/* 
 * File:   NeuronBox.hpp
 * Author: Dormouse
 *
 * Created on 14 January 2015, 16:57
 */

#ifndef NEURONBOX_HPP
#define	NEURONBOX_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Neuron.hpp"

class NeuronBox {
public:
    NeuronBox();
    NeuronBox(const NeuronBox& rhs) = delete;
    void Add(int x, int y);

    std::vector<std::unique_ptr<Neuron> > n_vector;
private:
    sf::Font font;

};

#endif	/* NEURONBOX_HPP */

