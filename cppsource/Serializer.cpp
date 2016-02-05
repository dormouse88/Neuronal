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
#include "UserData.hpp"  //fwd dec
#include "Arena.hpp" //fwd dec

const char XML_USER_FILENAME [] = "tree.xml";
const char XML_LEVELS_FILENAME [] = "levels1.xml";

const int  XMLD_NEUR_THR {1};
const bool XMLD_NEUR_BULB {true};
const int XMLD_WIRE_FS {0};
const int XMLD_WIRE_TS {0};
const int XMLD_WIRE_W {1};

Serializer::Serializer()
    :userDoc_()
    ,levelsDoc_()
{
    srand(time(nullptr));
    OpenFile(userDoc_, XML_USER_FILENAME);
    OpenFile(levelsDoc_, XML_LEVELS_FILENAME);
}

void Serializer::LoadLevel(int num, std::shared_ptr<Arena> a, std::shared_ptr<BlobFactory> f)
{
    //Find level in the XML...
    pugi::xml_node xmlLevel = levelsDoc_.find_child_by_attribute("LEVEL", "num", patch::to_string( num ).c_str() );
    
    if (xmlLevel)
    {
        //level number
        a->levelNum = num;
    }
    {
        //arena dimensions
        a->minCorner = ArenaPoint{ xmlLevel.attribute("min_x").as_int(), xmlLevel.attribute("min_y").as_int() };
        a->maxCorner = ArenaPoint{ xmlLevel.attribute("max_x").as_int(), xmlLevel.attribute("max_y").as_int() };
    }
    {
        //mouse xputs
        XPutFilter filter = std::make_shared<std::set<std::string> >();
        for ( pugi::xml_node n: xmlLevel.children("INPUT") ) {
            filter->insert( n.attribute("name").as_string() );
        }
        for ( pugi::xml_node n: xmlLevel.children("OUTPUT") ) {
            filter->insert( n.attribute("name").as_string() );
        }
        a->mouseSpawnGroup.DefineMouseXPuts( filter );
    }
    {
        //mouse spawners
        for ( pugi::xml_node n: xmlLevel.children("MSPAWN") ) {
            int x = n.attribute("pos_x").as_int();
            int y = n.attribute("pos_y").as_int();
            int ori = n.attribute("ori").as_int();
            a->MakeMouseSpawner(ArenaPoint{x,y}, ori);
        }
    }
    {
        //cat spawners (with brains)
        for ( pugi::xml_node n: xmlLevel.children("CSPAWN") ) {
            int x = n.attribute("pos_x").as_int();
            int y = n.attribute("pos_y").as_int();
            int ori = n.attribute("ori").as_int();
            int min_t = n.attribute("min_t").as_int();
            int max_t = n.attribute("max_t").as_int();
            int planNum = n.attribute("cat_plan").as_int();
            auto spawner = a->MakeCatSpawner(ArenaPoint{x,y}, ori, TimeRange{min_t,max_t}, planNum);
            spawner->DefineCatXPuts();
            auto brain = spawner->GetCatBrain();
            if (planNum != 0)
            {
                auto catPlan = LoadLevelPlan( num, planNum, f );
                assert(catPlan);
                brain->SetSubPlan(catPlan, brain);
            }
        }
    }
    {
        //goal spawners
        
    }
}



bool Serializer::SaveUserPlan(PlanShp plan_p)
{
    bool saved = SavePlanRecursively(userDoc_.child("PLANS"), plan_p);
    userDoc_.save_file(XML_USER_FILENAME);
    return saved;
}
PlanShp Serializer::LoadUserPlan(int planID, std::shared_ptr<BlobFactory> factory)
{
    return LoadPlanRecursively(userDoc_.child("PLANS"), planID, factory);
}

PlanShp Serializer::LoadLevelPlan(int levelNum, int planID, std::shared_ptr<BlobFactory> factory)
{
    pugi::xml_node con = levelsDoc_.find_child_by_attribute("LEVEL", "num", patch::to_string( levelNum ).c_str() );
    return LoadPlanRecursively(con, planID, factory);
}

void Serializer::LoadUserData(std::shared_ptr<UserData> ud)
{
    //Ancestry...
    for (pugi::xml_node node: userDoc_.child("RELATIVES").children("REL"))
    {
        int id = node.attribute("i").as_int(0);
        int anc = node.attribute("anc").as_int(0);
        ud->MakeAncestryEntry(id, anc);
    }
    //And names...
    for (pugi::xml_node node: userDoc_.child("NAMES").children("NAME"))
    {
        int i = node.attribute("i").as_int(0);
        std::string name = node.attribute("n").as_string(0);
        ud->namesByID.insert( std::make_pair(i, name) );
        ud->namesByName.insert( std::make_pair(name, i) );
    }
}

void Serializer::SaveAddAncestryEntry(int id, int anc)
{
    assert(id > 0);
    pugi::xml_node entry = userDoc_.child("RELATIVES").append_child("REL");
    entry.append_attribute("i") = id;
    entry.append_attribute("anc") = anc;
    userDoc_.save_file(XML_USER_FILENAME);
}

void Serializer::SaveRemoveName(int planID)
{
    pugi::xml_node p = GetNameNodeByID(planID);
    assert(p);
    userDoc_.child("NAMES").remove_child(p);
    userDoc_.save_file(XML_USER_FILENAME);
}

void Serializer::SaveAddName(int planID, std::string name)
{
    assert(not GetNameNodeByID(planID) and not GetNameNodeByName(name));
    pugi::xml_node names = userDoc_.child("NAMES");
    pugi::xml_node entry = names.append_child("NAME");
    entry.append_attribute("i") = planID;
    entry.append_attribute("n") = name.c_str();
    userDoc_.save_file(XML_USER_FILENAME);
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////PRIVATE:


void Serializer::OpenFile(pugi::xml_document & doc, const char * fileName)
{
    pugi::xml_parse_result result = doc.load_file(fileName);

    if (!result) {
        std::cout << "Failure Loading XML File: " << result.description() << std::endl;
        std::cout << "Error offset: " << result.offset << std::endl;
    }
}

pugi::xml_node Serializer::GetNameNodeByID(int planID) {
    return userDoc_.child("NAMES").find_child_by_attribute("NAME", "i", patch::to_string( planID ).c_str() );
}
pugi::xml_node Serializer::GetNameNodeByName(std::string name) {
    return userDoc_.child("NAMES").find_child_by_attribute("NAME", "n", name.c_str() );
}


bool Serializer::SavePlanRecursively(pugi::xml_node container, PlanShp plan_p)
{
    if (plan_p->IsModified())
    {
        int oldID = plan_p->planID;
        //check highest used planID in the XML and use next number as new planID...
        int newID = 0;
        for (int i = 1; i<100000; i++) {
            if ( not container.find_child_by_attribute("PLAN", "i", patch::to_string( i ).c_str() ) ) {
                newID = i;
                break;
            }
        }
        assert(newID > 0);
        
        //update the plan in memory with the new ID...
        plan_p->planID = newID;

        //create the new plan in the xml...
        pugi::xml_node plan = container.append_child("PLAN");
        plan.append_attribute("i") = newID;

        for (auto d: plan_p->devices) {
            if (d->SerialName() == "NEUR")
            {
                auto n = std::dynamic_pointer_cast<Neuron>(d);
                pugi::xml_node dev = plan.append_child("NEUR");
                dev.append_attribute("i").set_value(d->GetSerial() );
                dev.append_attribute("x").set_value(d->GetSmartPos().x);
                dev.append_attribute("y").set_value(d->GetSmartPos().y);
                if (n->GetThreshold() != XMLD_NEUR_THR)
                    dev.append_attribute("thr").set_value(n->GetThreshold() );
                dev.append_attribute("bulb").set_value( n->HasBulb() ? 1 : 0 );     //(can stop saving bulb attribute for default value once legacy xml has all gone far away)
            }
            else if (d->SerialName() == "HAND")
            {
                auto h = std::dynamic_pointer_cast<ChipHandle>(d);
                auto p = h->GetSubPlan();
                //holy recursion batman!?!...
                if (p) SavePlanRecursively(container, p);
                pugi::xml_node dev = plan.append_child("HAND");
                dev.append_attribute("i").set_value(d->GetSerial() );
                dev.append_attribute("x").set_value(d->GetSmartPos().x);
                dev.append_attribute("y").set_value(d->GetSmartPos().y);
                int pID = 0;
                if (p) pID = p->planID;
                dev.append_attribute("link").set_value(pID);
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
            if (w->GetWeight() != XMLD_WIRE_W)
                wire.append_attribute("w").set_value(w->GetWeight());
            if (w->GetFromTag() != XMLD_WIRE_FS)
                wire.append_attribute("fs").set_value(w->GetFromTag());
            if (w->GetToTag() != XMLD_WIRE_TS)
                wire.append_attribute("ts").set_value(w->GetToTag());
        }
        plan_p->modified = false;
        return true;
    }
    return false;
}

PlanShp Serializer::LoadPlanRecursively(pugi::xml_node container, int planID, std::shared_ptr<BlobFactory> factory)
{
    PlanShp memPlan = nullptr;
    
    //Find plan with matching planID in the XML...
    pugi::xml_node xmlPlan = container.find_child_by_attribute("PLAN", "i", patch::to_string( planID ).c_str() );
    
    if (xmlPlan and planID != 0)
    {
        memPlan = factory->MakePlan();
        memPlan->planID = planID;

        for (pugi::xml_node device = xmlPlan.child("HAND"); device; device = device.next_sibling("HAND"))
        {
            sf::Vector2i pos { device.attribute("x").as_int(), device.attribute("y").as_int() };
            int link { device.attribute("link").as_int() };
            int serial { device.attribute("i").as_int() };
            //creates handle with default empty plan...
            auto handle = factory->AddHandle(memPlan, serial, pos);
            //Recursively load subplans and assign them...
            if (link != 0) {
                auto subPlan = LoadPlanRecursively(container, link, factory);
                handle->SetSubPlan(subPlan, handle);
            }
        }
        for (pugi::xml_node device = xmlPlan.child("NEUR"); device; device = device.next_sibling("NEUR"))
        {
            sf::Vector2i pos { device.attribute("x").as_int(), device.attribute("y").as_int() };
            int threshold { device.attribute("thr").as_int(XMLD_NEUR_THR) };
            bool hasBulb { device.attribute("bulb").as_bool(XMLD_NEUR_BULB) };
            int serial { device.attribute("i").as_int() };
            factory->AddNeuron(memPlan, serial, pos, threshold, hasBulb);
        }
        for (pugi::xml_node device = xmlPlan.child("JUMP"); device; device = device.next_sibling("JUMP"))
        {
            sf::Vector2i pos { device.attribute("x").as_int(), device.attribute("y").as_int() };
            int serial { device.attribute("i").as_int() };
            auto n = factory->AddNeuron(memPlan, serial, pos, 1, false);
        }
        for (pugi::xml_node wire = xmlPlan.child("WIRE"); wire; wire = wire.next_sibling("WIRE"))
        {
            int from { wire.attribute("f").as_int() };
            int to { wire.attribute("t").as_int() };
            int fromSlot { wire.attribute("fs").as_int(XMLD_WIRE_FS) };
            int toSlot { wire.attribute("ts").as_int(XMLD_WIRE_TS) };
            int weight { wire.attribute("w").as_int(XMLD_WIRE_W) };
            
            std::shared_ptr<Wirable> fromDev = memPlan;
            if (from != 0)
                fromDev = memPlan->GetDevice(from);
            std::shared_ptr<Wirable> toDev = memPlan;
            if (to != 0)
                toDev = memPlan->GetDevice(to);
            Shp<WiringPair> wp = std::make_shared<WiringPair>(memPlan, fromDev, toDev, fromSlot, toSlot);
            factory->AddWire(wp, weight);
        }
        memPlan->modified = false;
    }//if (xmlPlan)
    return memPlan;
}

