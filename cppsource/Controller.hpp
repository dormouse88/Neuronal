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

class Controller
{
public:
    Controller(Model & model_p, View & view_p);
    Controller(const Controller&) = delete;

    bool HandleInput();

private:
    void CursorsGoHere(std::shared_ptr<ChipPlan>);

    Model & theModel;
    View & theView;

    sf::Vector2f mouseCursorWorldPos;
    bool mouseCursorSet;
    bool enteringName;
    bool enteringFilter;
    std::string enteringText;
    sf::Clock textEntryCooldown;
};

#endif	/* CONTROLLER_HPP */

