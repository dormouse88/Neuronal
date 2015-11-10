/* 
 * File:   Controller.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 12:42
 */

#ifndef CONTROLLER_HPP
#define	CONTROLLER_HPP

#include "Model.hpp"
#include "View.hpp"
#include "BlobFactory.hpp"
#include "Serializer.hpp"

class Controller
{
public:
    Controller(BlobFactory & factory_p, Serializer & serializer_p, Model & model_p, View & view_p);
    Controller(const Controller&) = delete;
    void DebugInfo();
    bool ReplaceActivePlan(std::shared_ptr<ChipPlan> newPlan, std::shared_ptr<ChipPlan> activePlan);
    bool WipePlan(std::shared_ptr<ChipPlan> activePlan, bool forced);
    bool LoadPlan(int num, std::shared_ptr<ChipPlan> activePlan);
    bool HandleInput();

private:
    BlobFactory & theFactory;
    Serializer & theSerializer;
    Model & theModel;
    View & theView;

    sf::Vector2f mouseCursorWorldPos;
    bool mouseCursorSet;
};

#endif	/* CONTROLLER_HPP */

