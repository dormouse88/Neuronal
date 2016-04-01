/* 
 * File:   ArenaBasics.hpp
 * Author: Dormouse
 *
 * Created on 08 January 2016, 14:47
 */

#ifndef ARENABASICS_HPP
#define	ARENABASICS_HPP

#include <cassert>

typedef int Orientation;
//enum class OriEnum { FRONT = 0, RIGHT = 1, BACK = 2, LEFT = 3 };
namespace OriEnums {
    const int FRONT = 0;
    const int RIGHT = 1;
    const int BACK = 2;
    const int LEFT = 3;
};

struct ArenaPoint
{
    ArenaPoint() :x(0), y(0) {}
    ArenaPoint(int p_x, int p_y) :x(p_x), y(p_y) {}
    int x;
    int y;
};

enum class ArenaAddressMode { ABSENT, CELL };
struct ArenaAddress
{
    ArenaAddressMode mode;
    ArenaPoint arenaPoint;
};

inline bool operator==(const ArenaPoint & lhs, const ArenaPoint & rhs)
{
    return (lhs.x == rhs.x and lhs.y == rhs.y);
}

typedef int TimeExact;
struct TimeRange
{
    TimeRange(TimeExact s, TimeExact e) :start(s), end(e) { assert(e>=s); } //both s and e are inclusive
    TimeExact start;
    TimeExact end;
};

#endif	/* ARENABASICS_HPP */

