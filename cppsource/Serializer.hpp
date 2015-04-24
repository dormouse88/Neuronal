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
#include "AbstractFactory.hpp"
#include "Device.hpp"

class Serializer
{
public:
    Serializer();
    ~Serializer() {}
    void SaveFile(std::vector<std::shared_ptr<Device> > &devices);
    void LoadFile(std::shared_ptr<AbstractFactory> factory_p);
private:
    void OpenFile(pugi::xml_document & doc);
    void SaveNode(pugi::xml_node & doc, std::vector<std::shared_ptr<Device> > &devices);
    void LoadNode(pugi::xml_node & doc, std::shared_ptr<AbstractFactory> factory);
};

#endif	/* SERIALIZER_HPP */

