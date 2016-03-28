/* 
 * File:   TextEnterer.cpp
 * Author: Dormouse
 * 
 * Created on 28 March 2016, 13:29
 */

#include "TextEnterer.hpp"

void TextEnterer::Append(sf::Uint32 ch)
{
    if (textEntryCooldownTimer_.getElapsedTime() > sf::milliseconds(200) )
    {
        //A few conversions...
        if (ch >= 0x61 and ch <= 0x7A)  //convert lowercase to capitals
            ch -= 0x20;
        if (ch == 0x20)                 //convert space to underscore
            ch = 0x5F;
        //Accepted characters...
        if (
                (ch >= 0x30 and ch <= 0x39) or  //numbers
                (ch >= 0x41 and ch <= 0x5A) or  //capitals
                (ch == 0x2B) or                 //+
                (ch == 0x5F)                    //_
            )
        {
            textBeingEntered_.append( sf::String{ch} );
        }
    }
}

