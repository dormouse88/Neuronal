/* 
 * File:   DeviceView.hpp
 * Author: Dormouse
 *
 * Created on 08 April 2015, 15:15
 */

#ifndef DEVICEVIEW_HPP
#define	DEVICEVIEW_HPP

class DeviceView
{
public:
    DeviceView() {}
    virtual bool AmIYourDaddy(const PinDevice & n) const = 0;
    virtual ~DeviceView() {}
};

#endif	/* DEVICEVIEW_HPP */

