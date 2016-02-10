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

class TextEnterer
{
public:
    TextEnterer(){}        //textEntryCooldownTimer_.restart();
    
    void Dispatch()                                                     { dispatchTarget_(textBeingEntered_); }
    void SetDispatchTarget(std::function<void(std::string)> dt)         { dispatchTarget_ = dt; }

    void Clear()                        { textBeingEntered_.clear(); }
    void SetText(std::string s)         { textBeingEntered_ = s; }
    std::string GetText() const         { return textBeingEntered_; }
    void Append(sf::Uint32 ch);
    void BackSpace()                    { if (textBeingEntered_.length() > 0) textBeingEntered_.erase(textBeingEntered_.length()-1); }
    
private:    
    std::string textBeingEntered_;
    sf::Clock textEntryCooldownTimer_;
    std::function<void(std::string)> dispatchTarget_;
};


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
    void EventsBothWirable(Shp<WiringPair>);
    void HandleInputState();


    void CursorsGoHere(PlanShp);

    Model & model_;
    View & view_;

    Cursor & cu1;
    Cursor & cu2;
    sf::Event event;
    bool quitYet_;

    std::shared_ptr<TextEnterer> textEnterer_;

    VectorWorld mouseCursorWorldPos_;
    bool isMouseCursorSet_;
};

#endif	/* CONTROLLER_HPP */

