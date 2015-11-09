/* 
 * File:   Serializer.cpp
 * Author: Dormouse
 * 
 * Created on 23 April 2015, 12:44
 */

#include "Serializer.hpp"
#include "miscUtil.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <SFML/System.hpp>// (/Vector2.hpp)

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

void Serializer::SaveFile(std::shared_ptr<ChipPlan> plan)
{
    pugi::xml_document doc;
    OpenFile(doc);
    if(doc) {
        SaveNode(doc, plan);
        doc.save_file(XML_FILE_NAME);
    }
}

std::shared_ptr<ChipPlan> Serializer::LoadFile(BlobFactory & factory_p, int planID)
{
    pugi::xml_document doc;
    OpenFile(doc);
    if(doc) {
        return LoadNode(doc, factory_p, planID);
    }
}

void Serializer::SaveNode(pugi::xml_node & doc, std::shared_ptr<ChipPlan> plan_p)
{
    if (plan_p->IsModified())
    {
        //check highest used planID in the XML and use next number as new planID
        pugi::xml_node db = doc.child("PLAN_DB");
        int newID = db.attribute("HIGHEST_ID").as_int() + 1;
        db.attribute("HIGHEST_ID") = newID;
        //update the plan in memory with the new ID...
        plan_p->planID = newID;

        //create the new plan in the xml...
        pugi::xml_node plan = doc.append_child("PLAN");
        plan.append_attribute("i") = newID;

        for (auto d: plan_p->devices) {
            if (d->SerialName() == "NEUR")
            {
                auto n = std::dynamic_pointer_cast<Neuron>(d);
                pugi::xml_node dev = plan.append_child("NEUR");
                dev.append_attribute("i").set_value(d->GetSerial() );
                dev.append_attribute("x").set_value(d->GetPosInPlan().x);
                dev.append_attribute("y").set_value(d->GetPosInPlan().y);
                dev.append_attribute("thr").set_value(n->GetThreshold() );
            }
            else if (d->SerialName() == "HAND")
            {
                auto h = std::dynamic_pointer_cast<ChipHandle>(d);
                auto p = h->GetPlan();
                //holy recursion batman!?!...
                if (p) SaveNode(doc, p);
                pugi::xml_node dev = plan.append_child("HAND");
                dev.append_attribute("i").set_value(d->GetSerial() );
                dev.append_attribute("x").set_value(d->GetPosInPlan().x);
                dev.append_attribute("y").set_value(d->GetPosInPlan().y);
                int pID = 0;
                if (p) pID = p->planID;
                dev.append_attribute("link").set_value(pID);
            }
            else if (d->SerialName() == "JUMP")
            {
                pugi::xml_node dev = plan.append_child("JUMP");
                dev.append_attribute("i").set_value(d->GetSerial() );
                dev.append_attribute("x").set_value(d->GetPosInPlan().x);
                dev.append_attribute("y").set_value(d->GetPosInPlan().y);
            }
        }
        for (auto w: plan_p->wires) {
            //Rather ugly. RTTI using raw pointers.
            //Needed a way to get serial numbers of devices.
            //Had to try casting Wirable references down to device references...
            auto * fd = dynamic_cast<const Device*>( &(w->GetFrom()) );
            auto * td = dynamic_cast<const Device*>( &(w->GetTo()) );
            int fserial = 0;
            int tserial = 0;
            if (fd) fserial = fd->GetSerial();
            if (td) tserial = td->GetSerial();
            pugi::xml_node wire = plan.append_child("WIRE");
            wire.append_attribute("f").set_value(fserial);
            wire.append_attribute("t").set_value(tserial);
            wire.append_attribute("w").set_value(w->GetWeight());
            wire.append_attribute("fs").set_value(w->GetFromSlot());
            wire.append_attribute("ts").set_value(w->GetToSlot());
        }
    }
    plan_p->modified = false;
}

std::shared_ptr<ChipPlan> Serializer::LoadNode(pugi::xml_node & doc, BlobFactory & factory_p, int planID)
{
    std::shared_ptr<ChipPlan> memPlan = nullptr;
    
    //Find plan with matching planID in the XML...
    pugi::xml_node db = doc.child("PLAN_DB");
    pugi::xml_node xmlPlan = doc.find_child_by_attribute("PLAN", "i", patch::to_string( planID ).c_str() );
    
    if (xmlPlan)
    {
        memPlan = std::make_shared<ChipPlan> ();
        memPlan->planID = planID;
        for (pugi::xml_node device = xmlPlan.child("HAND"); device; device = device.next_sibling("HAND"))
        {
            sf::Vector2i pos { device.attribute("x").as_int(), device.attribute("y").as_int() };
            int link { device.attribute("link").as_int() };
            int serial { device.attribute("i").as_int() };
            //creates handle with default empty plan...
            auto handle = factory_p.AddHandle(memPlan, serial, pos);
            //Recursively load subplans and assign them...
            if (link != 0) {
                auto subPlan = LoadNode(doc, factory_p, link);
                handle->SetPlan(subPlan);
                subPlan->RegisterReferer(handle);
            }
        }
        for (pugi::xml_node device = xmlPlan.child("NEUR"); device; device = device.next_sibling("NEUR"))
        {
            sf::Vector2i pos { device.attribute("x").as_int(), device.attribute("y").as_int() };
            int threshold { device.attribute("thr").as_int() };
            int serial { device.attribute("i").as_int() };
            factory_p.AddNeuron(memPlan, serial, pos, threshold);
        }
        for (pugi::xml_node device = xmlPlan.child("JUMP"); device; device = device.next_sibling("JUMP"))
        {
            sf::Vector2i pos { device.attribute("x").as_int(), device.attribute("y").as_int() };
            int serial { device.attribute("i").as_int() };
            factory_p.AddJumper(memPlan, serial, pos);
        }
        for (pugi::xml_node wire = xmlPlan.child("WIRE"); wire; wire = wire.next_sibling("WIRE"))
        {
            int from { wire.attribute("f").as_int() };
            int to { wire.attribute("t").as_int() };
            int fromSlot { wire.attribute("fs").as_int() };
            int toSlot { wire.attribute("ts").as_int() };
            int weight { wire.attribute("w").as_int() };
            
            std::shared_ptr<Wirable> fromDev = memPlan;
            if (from != 0) fromDev = memPlan->GetDevice(from);
            std::shared_ptr<Wirable> toDev = memPlan;
            if (to != 0) toDev = memPlan->GetDevice(to);
            factory_p.AddWire(memPlan, *fromDev, fromSlot, *toDev, toSlot, weight);
        }
        memPlan->modified = false;
    }//if (xmlPlan)
    return memPlan;
}


void PrintNode(pugi::xml_node & doc)
{
//    for (pugi::xml_node plan = doc.child("PLAN"); plan; plan = plan.next_sibling("PLAN"))
//    {
//        std::cout << "Anchor Tag: " << plan.attribute("anch").value() << std::endl;
//        std::cout << "Width:      " << plan.attribute("w").as_int() << std::endl;
//        std::cout << "Height:     " << plan.attribute("h").as_bool() << std::endl;
//
//        for (pugi::xml_node device = plan.child("NEUR"); device; device = device.next_sibling("NEUR"))
//        {
//            std::cout << "Device Type:" << device.name() << std::endl;
//            std::cout << "ID:         " << device.attribute("i").as_int() << std::endl;
//            std::cout << "x pos:      " << device.attribute("x").as_int() << std::endl;
//            std::cout << "y pos:      " << device.attribute("y").as_int() << std::endl;
//            std::cout << "threshold:  " << device.attribute("thr").as_int() << std::endl;
//            if (device.attribute("n")) std::cout << "name:       " << device.attribute("n").value() << std::endl;
//        }
//        for (pugi::xml_node device = plan.child("SOCK"); device; device = device.next_sibling("SOCK"))
//        {
//            std::cout << "Device Type:" << device.name() << std::endl;
//            std::cout << "ID:         " << device.attribute("i").as_int() << std::endl;
//            std::cout << "x pos:      " << device.attribute("x").as_int() << std::endl;
//            std::cout << "y pos:      " << device.attribute("y").as_int() << std::endl;
//            if (device.attribute("n")) std::cout << "name:       " << device.attribute("n").value() << std::endl;
//        }
//        for (pugi::xml_node device = plan.child("WIRE"); device; device = device.next_sibling("WIRE"))
//        {
//            std::cout << "Device Type:" << device.name() << std::endl;
//            std::cout << "to device:  " << device.attribute("t").as_int() << std::endl;
//            if (device.attribute("w")) std::cout << "weight:     " << device.attribute("w").as_int() << std::endl;
//        }
//    }
}