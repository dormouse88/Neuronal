/* 
 * File:   Model.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 11:53
 */

#ifndef MODEL_HPP
#define	MODEL_HPP


#include <vector>
#include <memory>
#include <stack>
#include "ChipPlan.hpp"
#include "Device.hpp"
#include "Wire.hpp"
#include "FactoryBase.hpp"
#include "Serializer.hpp"

class Model
{
public:
    Model();
    Model(const Model&) = delete;
    void DebugInfo();
    void SetFactory(std::shared_ptr<FactoryBase> f) {theFactory = f; }
    void Logic();
    void SaveXML();
    void LoadXML();
    
    void ClearEverything();
    void ExpelDevices();
    void ExpelWires();
    
private:
    std::shared_ptr<FactoryBase> theFactory;
    std::shared_ptr<Serializer> theSerializer;
    std::shared_ptr<ChipPlan> basePlan;
};

#endif	/* MODEL_HPP */

