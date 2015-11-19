/* 
 * File:   main.cpp
 * Author: Dormouse
 *
 * Created on 12 January 2015, 21:43
 */
#include "App.hpp"
#include <iostream>

int main()
{
    try
    {
        App app;
        app.Run();
    }
    catch(const char * e)
    {
        std::cout << e << std::endl;
    }
    return 0;
}