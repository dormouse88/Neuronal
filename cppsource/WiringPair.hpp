/* 
 * File:   WiringPair.hpp
 * Author: Dormouse
 *
 * Created on 05 February 2016, 17:08
 */

#ifndef WIRINGPAIR_HPP
#define	WIRINGPAIR_HPP

#include "Wirable.hpp"

struct WiringPair
{
    WiringPair(PlanShp p, WirableShp f, WirableShp t) :plan(p), from(f), to(t), fromTag(NULL_TAG), toTag(NULL_TAG) {}
    WiringPair(PlanShp p, WirableShp f, WirableShp t, Tag fTag, Tag tTag) :plan(p), from(f), to(t), fromTag(fTag), toTag(tTag) {}
    PlanShp plan;
    WirableShp from;
    WirableShp to;
    Tag fromTag;
    Tag toTag;
    bool IsWired() const { return from->HasWireTo(fromTag, *to, toTag); }
    bool CanAddWire() const { return from->CanRegisterExactWire(fromTag, *to, toTag); } //{ return (IsWired() == false and from->CanRegisterAnyWire(InOut::OUT, fromTag) and to->CanRegisterAnyWire(InOut::IN, toTag) and from != to) ;}
};



#endif	/* WIRINGPAIR_HPP */

