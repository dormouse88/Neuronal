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
const SlotData F_WHISK {1,"F_WHISK",false};
const SlotData L_WHISK {2,"L_WHISK",false};
const SlotData R_WHISK {3,"R_WHISK",false};
//actions
const SlotData WAIT {1,"WAIT",false};
const SlotData L_FOOT {2,"L_FOOT",false};
const SlotData R_FOOT {3,"R_FOOT",false};
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

