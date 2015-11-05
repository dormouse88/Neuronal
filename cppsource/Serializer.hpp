/* 
 * File:   Serializer.hpp
 * Author: Dormouse
 *
 * Created on 23 April 2015, 12:44
 */

#ifndef SERIALIZER_HPP
#define	SERIALIZER_HPP

#include <memory>
#include <vector>
#include "../external/pugixml-1.6/src/pugixml.hpp"
#include "BlobFactory.hpp"
#include "Device.hpp"
#include "Wire.hpp"
#include "ChipPlan.hpp"

class Serializer
{
public:
    Serializer();
    ~Serializer() {}
    void SaveFile(std::shared_ptr<ChipPlan> plan);
    std::shared_ptr<ChipPlan> LoadFile(BlobFactory & factory_p, int planID);
private:
    void OpenFile(pugi::xml_document & doc);
    void SaveNode(pugi::xml_node & doc, std::shared_ptr<ChipPlan> plan_p);
    std::shared_ptr<ChipPlan> LoadNode(pugi::xml_node & doc, BlobFactory & factory_p, int planID);
};

#endif	/* SERIALIZER_HPP */

