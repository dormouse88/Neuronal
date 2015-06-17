/* 
 * File:   WireView.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 11:57
 */

#ifndef WIREVIEW_HPP
#define	WIREVIEW_HPP

#include <SFML/Graphics.hpp>
#include "GobjectView.hpp"
#include "Wire.hpp"
#include "ViewResources.hpp"

class WireView : public GobjectView
{
public:
    WireView(std::shared_ptr<const Wire> wire_p, const ViewResources & vRes_p);
    virtual void Draw(sf::RenderTarget & rt);
    //virtual bool IsDead() const {return wire_m.IsDead();}
private:
    std::weak_ptr<const Wire> wire_m;
    sf::Text weightText;
};

#endif	/* WIREVIEW_HPP */

