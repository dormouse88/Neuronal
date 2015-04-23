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
    DeviceView(const Device & device_p)
        :GobjectView(device_p), device_m(device_p)
        {
            targetPos = mapGridToCoords(device_m.GetPosition());
            actualPos = targetPos;
        }
    virtual ~DeviceView() {}
    virtual void Draw(sf::RenderTarget & rt) = 0;
    void UpdatePos()
    {
        targetPos = mapGridToCoords(device_m.GetPosition());
        actualPos += (targetPos - actualPos) * 0.003f;
    }
protected:
    sf::Vector2f targetPos;
    sf::Vector2f actualPos;
private:
    const Device & device_m;
};


#endif	/* DEVICEVIEW_HPP */

