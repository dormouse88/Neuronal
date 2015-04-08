/* 
 * File:   ModelListener.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 16:17
 */

#ifndef MODELLISTENER_HPP
#define	MODELLISTENER_HPP

class ModelListener
{
public:
    ModelListener() {}
    virtual void OnNotifyAdd(Neuron * rp) = 0;
    virtual void OnNotifyAdd(const Wire & cr) = 0;
    virtual void OnNotifyRemove(PinDevice * rp) = 0;
    virtual void OnNotifyRemove(const Wire & cr) = 0;
    virtual ~ModelListener() {}
};

#endif	/* MODELLISTENER_HPP */

