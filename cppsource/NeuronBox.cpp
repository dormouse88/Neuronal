/* 
 * File:   NeuronBox.cpp
 * Author: Dormouse
 * 
 * Created on 14 January 2015, 16:57
 */

#include "NeuronBox.hpp"

NeuronBox::NeuronBox()
{
    font.loadFromFile("sansation.ttf");
}

void NeuronBox::Add(int x, int y)
{
    std::unique_ptr<Neuron> p{new Neuron{x, y, font}};
    n_vector.emplace_back(std::move(p));
}

