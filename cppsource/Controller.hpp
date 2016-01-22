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
    void HandleInputEvents();
    void HandleInputEventsWritingMode();
    void HandleInputEventsFreeMode();
    void EventsPlan(PlanShp);
    void EventsLocated(PlanPos);
    void EventsBothLocated(PlanPos, PlanPos);
    void EventsBothWirable(PlanShp, WiringPair);
    void HandleInputState();


    void CursorsGoHere(PlanShp);

    Model & model_;
    View & view_;

    Cursor & cu1;
    Cursor & cu2;
    sf::Event event;
    bool quitYet_;

    sf::Vector2f mouseCursorWorldPos_;
    bool isMouseCursorSet_;
    bool isEnteringName_;
    bool isEnteringFilter_;
    std::string textBeingEntered_;
    sf::Clock textEntryCooldownTimer_;
};

#endif	/* CONTROLLER_HPP */

