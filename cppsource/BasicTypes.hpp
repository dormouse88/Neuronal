/* 
 * File:   BasicTypes.hpp
 * Author: Dormouse
 *
 * Created on 21 January 2016, 16:38
 */

#ifndef BASICTYPES_HPP
#define	BASICTYPES_HPP

#include <memory>
#include <cassert>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <SFML/System.hpp>  //for sf::Vector
#include <SFML/Graphics.hpp> //for sf::Rect
#include "miscUtil.hpp"


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


typedef int Smart;
typedef int Dumb;
typedef float World;

typedef sf::Vector2<Dumb> VectorDumb;
typedef sf::Vector2<Smart> VectorSmart;
typedef sf::Vector2<World> VectorWorld;

typedef sf::Rect<Dumb> RectDumb;
typedef sf::Rect<Smart> RectSmart;
typedef sf::Rect<World> RectWorld;



typedef int Tag;



enum class InOut { IN, OUT };

struct Port
{
    int num;
    Tag tag;
};


#endif	/* BASICTYPES_HPP */

