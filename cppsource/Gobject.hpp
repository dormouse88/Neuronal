/* 
 * File:   Gobject.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 17:57
 */

#ifndef GOBJECT_HPP
#define	GOBJECT_HPP

class Gobject
{
public:
    Gobject() :dead(false) {}
    virtual ~Gobject() = 0;
    void Zingaya() {dead = true;}
    bool IsDead() const {return dead;}
private:
    bool dead;
};

inline Gobject::~Gobject() {}

#endif	/* GOBJECT_HPP */

