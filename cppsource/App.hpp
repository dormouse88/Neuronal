/* 
 * File:   App.hpp
 * Author: Dormouse
 *
 * Created on 13 January 2015, 15:04
 */

#ifndef APP_HPP
#define	APP_HPP

#include <SFML/Graphics.hpp>
#include "NeuronBox.hpp"
#include <vector>
#include "Wire.hpp"

class App {
public:
    App();
    void Run();
private:
    void HandleInput();
    void Logic();
    void Draw();
    
    sf::RenderWindow window;
    sf::View view;

    
    NeuronBox nBox;
    std::vector<std::unique_ptr<Wire> > wires;
};

#endif	/* APP_HPP */
