/* 
 * File:   Controller.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 12:42
 */

#ifndef CONTROLLER_HPP
#define	CONTROLLER_HPP

#include "Model.hpp"
#include "View.hpp"
//#include "BlobFactory.hpp"

class Controller
{
public:
    Controller(Model & model_p, View & view_p);
    Controller(const Controller&) = delete;

    bool HandleInput();

private:
    Model & theModel;
    View & theView;

    sf::Vector2f mouseCursorWorldPos;
    bool mouseCursorSet;
};

#endif	/* CONTROLLER_HPP */

