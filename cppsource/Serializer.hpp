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
#include "FactoryBase.hpp"
#include "Device.hpp"
#include "Wire.hpp"

class Serializer
{
public:
    Serializer();
    ~Serializer() {}
    void SaveFile(std::vector<std::shared_ptr<Device> > &devices, std::vector<std::shared_ptr<Wire> > &wires);
    void LoadFile(std::shared_ptr<FactoryBase> factory_p);
private:
    void OpenFile(pugi::xml_document & doc);
    void SaveNode(pugi::xml_node & doc, std::vector<std::shared_ptr<Device> > &devices, std::vector<std::shared_ptr<Wire> > &wires);
    void LoadNode(pugi::xml_node & doc, std::shared_ptr<FactoryBase> factory);
};

#endif	/* SERIALIZER_HPP */

