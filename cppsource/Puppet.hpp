/* 
 * File:   Puppet.hpp
 * Author: Dormouse
 *
 * Created on 11 December 2015, 14:05
 */

#ifndef PUPPET_HPP
#define	PUPPET_HPP

#include <memory>
#include <SFML/Graphics.hpp>
#include "BaseReferer.hpp"
#include "BlobFactory.hpp"

class Puppet
{
public:
    Puppet();
    void InitBrain();
    void Act();
    
    virtual void Draw(sf::RenderTarget & rt);// override;
    std::shared_ptr<BaseReferer> GetBR()            {return inner;}
    
private:
    void Left() {
        orientation--;
        if (orientation < 0) orientation = 3;
    }
    void Right() {
        orientation++;
        if (orientation > 3) orientation = 0;
    }
    void Forward() {
        if (orientation == 0) yPos--;
        else if (orientation == 1) xPos++;
        else if (orientation == 2) yPos++;
        else if (orientation == 3) xPos--;
    }
private:
    std::shared_ptr<BaseReferer> inner;
    
    int xPos;
    int yPos;
    int orientation;
};



#endif	/* PUPPET_HPP */
