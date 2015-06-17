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
    DeviceView(std::shared_ptr<const Device> device_p)
        :GobjectView(device_p), device_m(device_p)
        {
            targetPos = mapGridToCoords(device_p->GetPosition());
            actualPos = targetPos;
        }
    virtual ~DeviceView() {}
    virtual void Draw(sf::RenderTarget & rt) = 0;
    void UpdatePos()
    {
        std::shared_ptr<const Device> m {device_m.lock()};
        if (m) {
            targetPos = mapGridToCoords(m->GetPosition());
            actualPos += (targetPos - actualPos) * 0.003f;
        }
    }
protected:
    sf::Vector2f targetPos;
    sf::Vector2f actualPos;
private:
    std::weak_ptr<const Device> device_m;
};


#endif	/* DEVICEVIEW_HPP */

