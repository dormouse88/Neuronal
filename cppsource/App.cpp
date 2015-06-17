/* 
 * File:   App.cpp
 * Author: Dormouse
 * 
 * Created on 13 January 2015, 15:04
 */

#include "App.hpp"
#include <iostream> //del

App::App()
    :model(),
    view(model),
    controller(model, view)
{
    factory = std::make_shared<MVCFactory> (model, view, controller);
    controller.SetFactory(factory);
    model.SetFactory(factory);
}

void App::Run()
{
    bool quitYet = false;
    while (quitYet == false)
    {
        quitYet = controller.HandleInput();
        view.Draw();
    }
}

