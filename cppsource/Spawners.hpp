/* 
 * File:   Spawners.hpp
 * Author: Dormouse
 *
 * Created on 12 January 2016, 13:04
 */

#ifndef SPAWNERS_HPP
#define	SPAWNERS_HPP

#include <memory>
#include <SFML/Graphics.hpp>
#include "BaseReferer.hpp"
#include "ArenaBasics.hpp"
#include "BlobFactory.hpp"
#include "AllXPuts.hpp"

struct MouseSpawner
{
    MouseSpawner(ArenaPoint s_pos, Orientation s_ori)
        :pos_(s_pos)
        ,ori_(s_ori)
    {}
    ArenaPoint pos_;
    Orientation ori_;
};

class CatSpawner
{
public:
    CatSpawner(ArenaPoint s_pos, Orientation s_ori, TimeRange timeRange, int planNum, std::shared_ptr<BlobFactory> factory)
        :pos_(s_pos)
        ,ori_(s_ori)
        ,timeRange_(timeRange)
        ,timeExact_(timeRange.start)
        ,planNum_(planNum)
        ,brain_(factory->MakeBrain())
    {}
    void DefineCatXPuts()                           { brain_->DefineXputs(GetAllXPuts(), nullptr); }
    std::shared_ptr<BaseReferer> GetCatBrain()      { return brain_; }
    ArenaPoint pos_;
    Orientation ori_;
    TimeRange timeRange_;
    TimeExact timeExact_;
    int planNum_;
    std::shared_ptr<BaseReferer> brain_;
};



#endif	/* SPAWNERS_HPP */

