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
#include <cassert>

const char XML_FILE_NAME [] = "tree.xml";

const int NAME_MAX_LENGTH = 30;
const std::string AUTO_NAME_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

Serializer::Serializer()
    :doc()
{
    srand(time(nullptr));
    OpenFile(doc, XML_FILE_NAME);

    
    for (pugi::xml_node pNode: doc.child("PLANS").children("PLAN"))
    {
        int id = pNode.attribute("i").as_int(0);
        int anc = pNode.attribute("anc").as_int(0);
        //AddAncestryEntry(id, anc);
    }
}


std::shared_ptr<ChipPlan> Serializer::LoadPlan(int planID)
{
    return LoadPlanParts(planID);
}

void Serializer::SavePlan(std::shared_ptr<ChipPlan> plan)
{
    int oldID = plan->GetPlanID();
    std::string oldName = GetNameByID( oldID );
    SavePlanParts(plan);
    if (not oldName.empty()) {
        RemoveName(oldID);
        AddName(plan->GetPlanID(), oldName);
    }
    else {
        AddName(plan->GetPlanID(), GetUnusedAutoName());
    }
    doc.save_file(XML_FILE_NAME);
}

void Serializer::SavePlanAsNew(std::shared_ptr<ChipPlan> plan)
{
    SavePlanParts(plan);
    AddName(plan->GetPlanID(), GetUnusedAutoName());
    doc.save_file(XML_FILE_NAME);
}

int Serializer::GetIDByName(std::string name)
{
    pugi::xml_node xmlPlan = GetNameNodeByName(name);
    return xmlPlan.attribute("i").as_int(0);
}
std::string Serializer::GetNameByID(int planID)
{
    pugi::xml_node xmlPlan = GetNameNodeByID(planID);
    return xmlPlan.attribute("n").as_string("");
}

void Serializer::RemoveName(int planID)
{
    pugi::xml_node p = GetNameNodeByID(planID);
    if (p) {
        doc.child("NAMES").remove_child(p);
        doc.save_file(XML_FILE_NAME);
    }
}

void Serializer::AddName(int planID, std::string name)
{
    if (not GetNameNodeByID(planID) and not GetNameNodeByName(name))
    {
        pugi::xml_node names = doc.child("NAMES");
        pugi::xml_node entry = names.append_child("NAME");
        entry.append_attribute("i") = planID;
        entry.append_attribute("n") = name.c_str();
        doc.save_file(XML_FILE_NAME);
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////PRIVATE:



std::string Serializer::GetUnusedAutoName()
{
    std::string randomName;
    for (int i = 0; i <= NAME_MAX_LENGTH; ++i)
    {
        randomName.push_back( AUTO_NAME_CHARS.at( rand() % AUTO_NAME_CHARS.length() ) );
        pugi::xml_node nameCheck = GetNameNodeByName(randomName);
        if (not nameCheck) {
            return randomName;
        }
    }
    throw "NAME MAX LENGTH Reached! (highly unlikely to reach this)";
}

pugi::xml_node Serializer::GetNameNodeByID(int planID) {
    return doc.child("NAMES").find_child_by_attribute("NAME", "i", patch::to_string( planID ).c_str() );
}
pugi::xml_node Serializer::GetNameNodeByName(std::string name) {
    return doc.child("NAMES").find_child_by_attribute("NAME", "n", name.c_str() );
}

void Serializer::OpenFile(pugi::xml_document & doc, const char * fileName)
{
    pugi::xml_parse_result result = doc.load_file(fileName);

    if (!result) {
        std::cout << "Failure Loading XML File: " << result.description() << std::endl;
        std::cout << "Error offset: " << result.offset << std::endl;
    }
}


void Serializer::SavePlanParts(std::shared_ptr<ChipPlan> plan_p)
{
    if (plan_p->IsModified())
    {
        int oldID = plan_p->planID;
        pugi::xml_node plansNode = doc.child("PLANS");
        //check highest used planID in the XML and use next number as new planID
        pugi::xml_node db = plansNode.child("PLAN_DB");
        int newID = db.attribute("HIGHEST_ID").as_int() + 1;
        db.attribute("HIGHEST_ID") = newID;
        //update the plan in memory with the new ID...
        plan_p->planID = newID;
        plan_p->nameGetter = std::bind(&Serializer::GetNameByID , this, std::placeholders::_1);
        plan_p->relativesGetter = std::bind(&Serializer::GetRelatives , this, std::placeholders::_1);

        //create the new plan in the xml...
        pugi::xml_node plan = plansNode.append_child("PLAN");
        plan.append_attribute("i") = newID;
        if (oldID > 0) {
            plan.append_attribute("anc") = oldID;
            AddAncestryEntry(newID, oldID);
        }

        for (auto d: plan_p->devices) {
            if (d->SerialName() == "NEUR")
            {
                auto n = std::dynamic_pointer_cast<Neuron>(d);
                pugi::xml_node dev = plan.append_child("NEUR");
                dev.append_attribute("i").set_value(d->GetSerial() );
                dev.append_attribute("x").set_value(d->GetSmartPos().x);
                dev.append_attribute("y").set_value(d->GetSmartPos().y);
                dev.append_attribute("thr").set_value(n->GetThreshold() );
            }
            else if (d->SerialName() == "HAND")
            {
                auto h = std::dynamic_pointer_cast<ChipHandle>(d);
                auto p = h->GetPlan();
                //holy recursion batman!?!...
                if (p) SavePlanParts(p);
                pugi::xml_node dev = plan.append_child("HAND");
                dev.append_attribute("i").set_value(d->GetSerial() );
                dev.append_attribute("x").set_value(d->GetSmartPos().x);
                dev.append_attribute("y").set_value(d->GetSmartPos().y);
                int pID = 0;
                if (p) pID = p->planID;
                dev.append_attribute("link").set_value(pID);
            }
            else if (d->SerialName() == "JUMP")
            {
                pugi::xml_node dev = plan.append_child("JUMP");
                dev.append_attribute("i").set_value(d->GetSerial() );
                dev.append_attribute("x").set_value(d->GetSmartPos().x);
                dev.append_attribute("y").set_value(d->GetSmartPos().y);
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

std::shared_ptr<ChipPlan> Serializer::LoadPlanParts(int planID)
{
    std::shared_ptr<ChipPlan> memPlan = nullptr;
    
    pugi::xml_node plansNode = doc.child("PLANS");
    pugi::xml_node db = plansNode.child("PLAN_DB");
    //Find plan with matching planID in the XML...
    pugi::xml_node xmlPlan = plansNode.find_child_by_attribute("PLAN", "i", patch::to_string( planID ).c_str() );
    
    if (xmlPlan)
    {
        memPlan = BlobFactory::MakePlan();
        memPlan->planID = planID;
        memPlan->nameGetter = std::bind(&Serializer::GetNameByID , this, std::placeholders::_1);
        memPlan->relativesGetter = std::bind(&Serializer::GetRelatives , this, std::placeholders::_1);
        

        for (pugi::xml_node device = xmlPlan.child("HAND"); device; device = device.next_sibling("HAND"))
        {
            sf::Vector2i pos { device.attribute("x").as_int(), device.attribute("y").as_int() };
            int link { device.attribute("link").as_int() };
            int serial { device.attribute("i").as_int() };
            //creates handle with default empty plan...
            auto handle = BlobFactory::AddHandle(memPlan, serial, pos);
            //Recursively load subplans and assign them...
            if (link != 0) {
                auto subPlan = LoadPlanParts(link);
                handle->SetPlan(subPlan);
                subPlan->RegisterReferer(handle);
            }
        }
        for (pugi::xml_node device = xmlPlan.child("NEUR"); device; device = device.next_sibling("NEUR"))
        {
            sf::Vector2i pos { device.attribute("x").as_int(), device.attribute("y").as_int() };
            int threshold { device.attribute("thr").as_int() };
            int serial { device.attribute("i").as_int() };
            BlobFactory::AddNeuron(memPlan, serial, pos, threshold);
        }
        for (pugi::xml_node device = xmlPlan.child("JUMP"); device; device = device.next_sibling("JUMP"))
        {
            sf::Vector2i pos { device.attribute("x").as_int(), device.attribute("y").as_int() };
            int serial { device.attribute("i").as_int() };
            BlobFactory::AddJumper(memPlan, serial, pos);
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
            BlobFactory::AddWire(memPlan, *fromDev, fromSlot, *toDev, toSlot, weight);
        }
        memPlan->modified = false;
    }//if (xmlPlan)
    return memPlan;
}

