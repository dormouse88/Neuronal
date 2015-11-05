/* 
 * File:   App.cpp
 * Author: Dormouse
 * 
 * Created on 13 January 2015, 15:04
 */

#include "App.hpp"
#include <iostream> //del

App::App()
    :factory(),
    model(),
    view(model),
    serializer(),
    controller(factory, serializer, model, view)
{}

void App::Run()
{
    bool quitYet = false;
    while (quitYet == false)
    {
        quitYet = controller.HandleInput();
        view.Draw();
    }
}

