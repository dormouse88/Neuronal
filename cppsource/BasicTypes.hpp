/* 
 * File:   BasicTypes.hpp
 * Author: Dormouse
 *
 * Created on 21 January 2016, 16:38
 */

#ifndef BASICTYPES_HPP
#define	BASICTYPES_HPP

#include <memory>

template<typename T>
using Shp = std::shared_ptr<T>;

template<typename T>
using Wep = std::weak_ptr<T>;

class ChipPlan;
typedef Shp<ChipPlan> PlanShp;

class Device;
typedef Shp<Device> DeviceShp;

class Neuron;
typedef Shp<Neuron> NeuronShp;

class ChipHandle;
typedef Shp<ChipHandle> HandleShp;

class Wire;
typedef Shp<Wire> WireShp;

class Wirable;
typedef Shp<Wirable> WirableShp;


#endif	/* BASICTYPES_HPP */

