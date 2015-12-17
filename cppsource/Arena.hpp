/* 
 * File:   Arena.hpp
 * Author: Dormouse
 *
 * Created on 16 December 2015, 22:49
 */

#ifndef ARENA_HPP
#define	ARENA_HPP

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>


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
bool operator==(const ArenaPoint & lhs, const ArenaPoint & rhs);

class ArenaStatic;
class ArenaMotile;

class Arena
{
public:
    Arena();
        //InitBoard()
    void RegisterStatic(std::shared_ptr<ArenaStatic> guy);
    void RegisterMotile(std::shared_ptr<ArenaMotile> guy);
    void Draw(sf::RenderTarget &rt) {}
    bool IsInBounds(ArenaPoint);
    bool WhiskerDetect(ArenaPoint);
private:
    std::vector< std::shared_ptr<ArenaStatic> > mots;
    ArenaPoint minCorner;
    ArenaPoint maxCorner;
};

//potential optimization:
    //Store ArenaStatic keyed by location such as std::multimap< ArenaPoint, std::shared_ptr<ArenaStatic> > stats;
    //Store ArenaMotile in a dumb container such as std::vector< std::shared_ptr<ArenaMotile> > mots;


#endif	/* ARENA_HPP */

