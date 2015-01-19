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
    virtual void OnNotify(bool added, Neuron * rp) = 0;
    virtual void OnNotify(bool added, const Wire & cr) = 0;    
    virtual ~ModelListener() {}
};

#endif	/* MODELLISTENER_HPP */

