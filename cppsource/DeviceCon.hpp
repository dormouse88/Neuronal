/* 
 * File:   DeviceCon.hpp
 * Author: Dormouse
 *
 * Created on 14 April 2015, 23:19
 */

#ifndef DEVICECON_HPP
#define	DEVICECON_HPP

#include "Device.hpp"
#include "GobjectCon.hpp"

class DeviceCon : public GobjectCon
{
public:
    DeviceCon() :GobjectCon() {}
    virtual void Handle(int code) = 0;
    virtual const Device& GetDevice() const = 0;
    virtual ~DeviceCon() {}
};

#endif	/* DEVICECON_HPP */

