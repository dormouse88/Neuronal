/* 
 * File:   AllXPuts.hpp
 * Author: Dormouse
 *
 * Created on 12 January 2016, 13:20
 */

#ifndef ALLXPUTS_HPP
#define	ALLXPUTS_HPP

//Is there a better way to handle these SlotData?
//enum class BrainInput { WAIT = 0, F_WHISK = 1, L_WHISK = 2, R_WHISK = 3 };

//senses
const SlotData F_WHISK {"A","F_WHISK"};
const SlotData L_WHISK {"B","L_WHISK"};
const SlotData R_WHISK {"C","R_WHISK"};
//actions
const SlotData WAIT   {"A","WAIT"};
const SlotData L_FOOT {"B","L_FOOT"};
const SlotData R_FOOT {"C","R_FOOT"};
inline XPuts GetAllXPuts()
{
    XPuts ret;
    ret.ins.emplace_back(F_WHISK);
    ret.ins.emplace_back(L_WHISK);
    ret.ins.emplace_back(R_WHISK);
    ret.outs.emplace_back(WAIT);
    ret.outs.emplace_back(L_FOOT);
    ret.outs.emplace_back(R_FOOT);
    return ret;
}

#endif	/* ALLXPUTS_HPP */

