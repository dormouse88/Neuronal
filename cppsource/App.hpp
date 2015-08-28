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
#include "BlobFactory.hpp"
#include "Serializer.hpp"

class App
{
public:
    App();
    void Run();
private:
    BlobFactory factory;
    Serializer serializer;
    Model model;
    View view;
    Controller controller;
};

#endif	/* APP_HPP */
