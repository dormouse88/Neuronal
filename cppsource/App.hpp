/* 
 * File:   App.hpp
 * Author: Dormouse
 *
 * Created on 13 January 2015, 15:04
 */

#ifndef APP_HPP
#define	APP_HPP


#include "Model.hpp"
#include "View.hpp"
#include "Controller.hpp"

class App
{
public:
    App();
    void Run();
private:
    Model model;
    PaneGroup paneGroup;
//    View view;
//    Controller controller;
};

#endif	/* APP_HPP */
