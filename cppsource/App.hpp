/* 
 * File:   App.hpp
 * Author: Dormouse
 *
 * Created on 13 January 2015, 15:04
 */

#ifndef APP_HPP
#define	APP_HPP

#include "BlobFactory.hpp"
#include "Model.hpp"
#include "View.hpp"
#include "Serializer.hpp"
#include "Controller.hpp"

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
