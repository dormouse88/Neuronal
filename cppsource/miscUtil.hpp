/* 
 * File:   miscUtil.hpp
 * Author: Dormouse
 *
 * Created on 05 November 2015, 15:36
 */

#ifndef MISCUTIL_HPP
#define	MISCUTIL_HPP

#include <string>
#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

#endif	/* MISCUTIL_HPP */

