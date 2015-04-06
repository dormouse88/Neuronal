/* 
 * File:   WireView.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 11:57
 */

#ifndef WIREVIEW_HPP
#define	WIREVIEW_HPP

#include <SFML/Graphics.hpp>
#include "Wire.hpp"
#include "ViewResources.hpp"

class WireView {
public:
    WireView(const Wire & wire_p, const ViewResources & vRes_p);
    void Draw(sf::RenderTarget & rt);
    bool AmIYourDaddy(const Wire & w) const {return w==wire_m;}
private:
    const Wire & wire_m;
    sf::Text weightText;
};

#endif	/* WIREVIEW_HPP */

