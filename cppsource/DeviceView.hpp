/* 
 * File:   DeviceView.hpp
 * Author: Dormouse
 *
 * Created on 08 April 2015, 15:15
 */

#ifndef DEVICEVIEW_HPP
#define	DEVICEVIEW_HPP

#include "GobjectView.hpp"

class DeviceView : public GobjectView
{
public:
    DeviceView() :GobjectView() {}
    virtual void Draw(sf::RenderTarget & rt) = 0;
    virtual ~DeviceView() {}
};

#endif	/* DEVICEVIEW_HPP */

