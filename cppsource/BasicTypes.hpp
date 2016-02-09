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



enum class Charge { MAYBE, ON, OFF };

typedef int CWeight;

typedef std::string Tag; 
const Tag NULL_TAG {""}; //(Empty string is a special value indicating "no tag")

typedef int PortNum;
const PortNum NULL_PORT {0}; //(Zero is a special value indicating "no port")

typedef std::string PlanName;
const PlanName NULL_PLAN_NAME {""};  //(Empty string is a special value indicating "no plan name")

typedef int PlanID;
const PlanID NULL_PID {0}; //(Zero is a special value indicating "no plan id")

typedef int LevelNum;

typedef int HandleCode;

typedef int OTime;



const sf::Vector2f GRID_SIZE { 115.f, 75.f };

// In/Out with regard to wiring. Relates to from/to on wires.
enum class InOut { IN, OUT };

// Refers to the interface between a handle and plan. Head is the first interface encountered where it zooms in. Tail is the way out.
enum class ZoomSide { HEAD, TAIL };

struct PortLocation
{
    ZoomSide side;
    PortNum num;
};

enum class PlanRegion { NONE, WHOLEPLAN, PORTS, CONTENTS };





#endif	/* BASICTYPES_HPP */

