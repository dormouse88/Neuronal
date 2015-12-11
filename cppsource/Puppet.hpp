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
    void Left() {}
    void Right() {}
    void Forward() {}
private:
    std::shared_ptr<BaseReferer> inner;
};



#endif	/* PUPPET_HPP */
