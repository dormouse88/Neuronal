/* 
 * File:   Controller.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 12:42
 */

#ifndef CONTROLLER_HPP
#define	CONTROLLER_HPP

#include "View.hpp"
#include "Model.hpp"

class Controller {
public:
    Controller(View & view_p, Model & model_p);
    Controller(const Controller&) = delete;
    bool HandleInput();
private:
    View & theView;
    Model & theModel;
    sf::Vector2f mouseCursorWorldPos;
    bool mouseCursorSet;
};

#endif	/* CONTROLLER_HPP */

