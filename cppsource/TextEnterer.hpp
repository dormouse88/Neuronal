/* 
 * File:   TextEnterer.hpp
 * Author: Dormouse
 *
 * Created on 28 March 2016, 13:29
 */

#ifndef TEXTENTERER_HPP
#define	TEXTENTERER_HPP

#include <string>
#include <SFML/System.hpp>

class TextEnterer
{
public:
    TextEnterer(){}
    
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


#endif	/* TEXTENTERER_HPP */

