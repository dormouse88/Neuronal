/* 
 * File:   ViewResources.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 17:46
 */

#include "ViewResources.hpp"

ViewResources::ViewResources()
{
    font.loadFromFile("resources/sansation.ttf");
    clock.restart();
}

const ViewResources & ViewResources::GetInstance()
{
    static ViewResources * instance = new ViewResources();
    return *instance;
}
