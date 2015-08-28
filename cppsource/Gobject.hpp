/* 
 * File:   Gobject.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 17:57
 */

#ifndef GOBJECT_HPP
#define	GOBJECT_HPP

#include <SFML/Graphics.hpp>

class Drawable
{
public:
    virtual ~Drawable() {}
    virtual void Draw(sf::RenderTarget & rt) = 0;
};

class Responsive
{
public:
    virtual ~Responsive() {}
    virtual void Handle(int code) = 0;
};


#include <string>

class Gobject: public Drawable, public Responsive
{
public:
    Gobject() :dead(false) {}
    virtual ~Gobject() = 0;
    virtual std::string SerialName() const = 0;
    void Zingaya() {dead = true;}
    bool IsDead() const {return dead;}
private:
    bool dead;
};

inline Gobject::~Gobject() {}


#endif	/* GOBJECT_HPP */

