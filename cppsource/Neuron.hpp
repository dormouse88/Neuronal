/* 
 * File:   Neuron.hpp
 * Author: Dormouse
 *
 * Created on 13 January 2015, 16:05
 */

#ifndef NEURON_HPP
#define	NEURON_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
class Wire;

class Neuron {
public:
    Neuron(int x, int y, const sf::Font & font_p);
    void Draw(sf::RenderTarget & rt);
    void StepPartOne();
    void StepPartTwo();
    void SetThreshold(unsigned t) {threshold = t;}
    sf::Vector2i GetPosition() {return pos;}

    void RegisterIn(Wire* wp)  {inWires.emplace_back(wp);}
    void RegisterOut(Wire* wp) {outWires.emplace_back(wp);}

    //Not-saved
    std::vector<Wire*> inWires;
    std::vector<Wire*> outWires;
    bool firing;
private:
    //Saved
    unsigned id;
    unsigned threshold;
    sf::Vector2i pos;

    //Visuals
    sf::RectangleShape shape;
    //std::shared_ptr<sf::Font> font;
    sf::Text thresholdText;
};

#endif	/* NEURON_HPP */

