/* 
 * File:   Serializer.cpp
 * Author: Dormouse
 * 
 * Created on 23 April 2015, 12:44
 */

#include "Serializer.hpp"

#include <iostream>
#include <string>
#include <SFML/System.hpp>// (/Vector2.hpp)
#include "AbstractFactory.hpp"

const char XML_FILE_NAME [] = "tree.xml";

Serializer::Serializer()
{}

void Serializer::OpenFile(pugi::xml_document & doc)
{
    pugi::xml_parse_result result = doc.load_file(XML_FILE_NAME);

    if (!result) {
        std::cout << "Failure Loading XML File: " << result.description() << std::endl;
        std::cout << "Error offset: " << result.offset << std::endl;
    }
}

void Serializer::SaveFile(std::vector<std::shared_ptr<Device> > &devices)
{
    pugi::xml_document doc;
    OpenFile(doc);
    if(doc) {
        SaveNode(doc, devices);
        doc.save_file(XML_FILE_NAME);
    }
}

void Serializer::LoadFile(std::shared_ptr<AbstractFactory> factory)
{
    pugi::xml_document doc;
    OpenFile(doc);
    if(doc) {
        LoadNode(doc, factory);
    }
}

void Serializer::SaveNode(pugi::xml_node & doc, std::vector<std::shared_ptr<Device> > &devices)
{
    doc.remove_child("PLAN");
    pugi::xml_node plan = doc.append_child("PLAN");
    
    for (auto d: devices) {
        if (d->SerialName() == "NEUR")
        {
            pugi::xml_node neur = plan.append_child("NEUR");
            neur.append_attribute("x").set_value(d->GetPosition().x);
            neur.append_attribute("y").set_value(d->GetPosition().y);
        }
        else if (d->SerialName() == "SOCK")
        {
            pugi::xml_node sock = plan.append_child("SOCK");
            sock.append_attribute("x").set_value(d->GetPosition().x);
            sock.append_attribute("y").set_value(d->GetPosition().y);
        }
    }
}

void Serializer::LoadNode(pugi::xml_node & doc, std::shared_ptr<AbstractFactory> factory)
{
    for (pugi::xml_node plan = doc.child("PLAN"); plan; plan = plan.next_sibling("PLAN"))
    {
        for (pugi::xml_node device = plan.child("NEUR"); device; device = device.next_sibling("NEUR"))
        {
            sf::Vector2i pos { device.attribute("x").as_int(), device.attribute("y").as_int() };
            factory->AddNeuron(pos);
        }
        for (pugi::xml_node device = plan.child("SOCK"); device; device = device.next_sibling("SOCK"))
        {
            sf::Vector2i pos { device.attribute("x").as_int(), device.attribute("y").as_int() };
            factory->AddSocket(pos);
        }
    }
}

void PrintNode(pugi::xml_node & doc)
{
    for (pugi::xml_node plan = doc.child("PLAN"); plan; plan = plan.next_sibling("PLAN"))
    {
        std::cout << "Anchor Tag: " << plan.attribute("anch").value() << std::endl;
        std::cout << "Width:      " << plan.attribute("w").as_int() << std::endl;
        std::cout << "Height:     " << plan.attribute("h").as_bool() << std::endl;

        for (pugi::xml_node device = plan.child("NEUR"); device; device = device.next_sibling("NEUR"))
        {
            std::cout << "Device Type:" << device.name() << std::endl;
            std::cout << "ID:         " << device.attribute("i").as_int() << std::endl;
            std::cout << "x pos:      " << device.attribute("x").as_int() << std::endl;
            std::cout << "y pos:      " << device.attribute("y").as_int() << std::endl;
            std::cout << "threshold:  " << device.attribute("thr").as_int() << std::endl;
            if (device.attribute("n")) std::cout << "name:       " << device.attribute("n").value() << std::endl;
        }
        for (pugi::xml_node device = plan.child("SOCK"); device; device = device.next_sibling("SOCK"))
        {
            std::cout << "Device Type:" << device.name() << std::endl;
            std::cout << "ID:         " << device.attribute("i").as_int() << std::endl;
            std::cout << "x pos:      " << device.attribute("x").as_int() << std::endl;
            std::cout << "y pos:      " << device.attribute("y").as_int() << std::endl;
            if (device.attribute("n")) std::cout << "name:       " << device.attribute("n").value() << std::endl;
        }
        for (pugi::xml_node device = plan.child("WIRE"); device; device = device.next_sibling("WIRE"))
        {
            std::cout << "Device Type:" << device.name() << std::endl;
            std::cout << "to device:  " << device.attribute("t").as_int() << std::endl;
            if (device.attribute("w")) std::cout << "weight:     " << device.attribute("w").as_int() << std::endl;
        }
    }
    
}