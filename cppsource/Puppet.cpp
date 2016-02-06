/* 
 * File:   Puppet.cpp
 * Author: Dormouse
 * 
 * Created on 11 December 2015, 14:05
 */

#include "Puppet.hpp"
#include <cassert>
#include "Arena.hpp"  //fwd dec
#include "AllXPuts.hpp"

//////////////////////////////////////Entity....
ArenaEntity::ArenaEntity(ArenaPoint s_pos, Orientation s_ori, std::shared_ptr<Arena> ar)
    :pos_(s_pos)
    ,ori_(s_ori)
    ,arena_(ar)
    ,alive_(true)
{}

void ArenaEntity::WalkForward()
{
    pos_ =  GetAdjacentPos(OriEnums::FRONT);
}
void ArenaEntity::TurnLeft()
{
    ori_--;
    if (ori_ < 0) ori_ = 3;
}
void ArenaEntity::TurnRight()
{
    ori_++;
    if (ori_ > 3) ori_ = 0;
}
void ArenaEntity::Die()
{
    alive_ = false;
    pos_ = {0,0}; //GetInitialPos(); //(temporary solution)
}
ArenaPoint ArenaEntity::GetAdjacentPos(Orientation tile)
{
    if      ( (ori_ + tile) % 4 == 0) return ArenaPoint { pos_.x,       pos_.y - 1 };
    else if ( (ori_ + tile) % 4 == 1) return ArenaPoint { pos_.x + 1,   pos_.y };
    else if ( (ori_ + tile) % 4 == 2) return ArenaPoint { pos_.x,       pos_.y + 1 };
    else if ( (ori_ + tile) % 4 == 3) return ArenaPoint { pos_.x - 1,   pos_.y };
    else assert(false);
}







//////////////////////////// Puppet
Puppet::Puppet(ArenaPoint s_pos, Orientation s_ori, std::shared_ptr<Arena> ar, std::shared_ptr<BaseReferer> brain)
    :ArenaEntity(s_pos, s_ori, ar)
    ,brain_(brain)
{}

const int TIME_OUT_TICKS = 2000;
void Puppet::Act()
{
    for (int i = 0; i<TIME_OUT_TICKS and not brain_->IsAnyOutputOn(); i++)
    {
        brain_->TickOnce();
    }

    auto outs = brain_->GetOutputs();

    brain_->TickOnce();  //This is one possibility (an automatic after-tick).
                         //An alternative that wouldn't progress innerTime automatically would be a check on the loop above that at least 1 innerTime has passed

    //Perform an action based on the output information from the brain...
    //Since it is possible to receive requests for multiple commands at once, an order of priority will have to be established and made explicit.
    //As an example, a preliminary order might be: (Moving or Turning) > Marking Scents > Reading Scents > Waiting
    auto lamb = [&outs] (std::string n) { return outs.count(n) > 0 and outs.at(n) == Charge::ON; };
    if (lamb(L_FOOT.name) and lamb(R_FOOT.name)) {
        WalkForward();
    }
    else if (lamb(L_FOOT.name)) {
        TurnLeft();
    }
    else if (lamb(R_FOOT.name)) {
        TurnRight();
    }
}

void Puppet::Sense()
{
    //sends the new sense data to the brain from the arena...
    auto arenaLock = arena_.lock();
    brain_->SetInputState( F_WHISK.name, arenaLock->WhiskerDetect( GetAdjacentPos(OriEnums::FRONT) ) );
    brain_->SetInputState( L_WHISK.name, arenaLock->WhiskerDetect( GetAdjacentPos(OriEnums::LEFT) ) );
    brain_->SetInputState( R_WHISK.name, arenaLock->WhiskerDetect( GetAdjacentPos(OriEnums::RIGHT) ) );
}







////////////////////////////////////////

//Mouse
void Mouse::Draw(sf::RenderTarget & rt)
{
    sf::ConvexShape shape;
    {
        // resize it to 5 points
        shape.setPointCount(5);
        // define the points
        const int LONG = 75;
        const int SHORT = 55;
        shape.setPoint(0, sf::Vector2f(0, -LONG));
        shape.setPoint(1, sf::Vector2f(SHORT, -SHORT));
        shape.setPoint(2, sf::Vector2f(SHORT, SHORT));
        shape.setPoint(3, sf::Vector2f(-SHORT, SHORT));
        shape.setPoint(4, sf::Vector2f(-SHORT, -SHORT));
    }
    shape.setRotation( 90.f * GetActualOri() );

    auto fr = arena_.lock()->GetCellBounds(GetActualPos().x, GetActualPos().y);
    shape.setPosition( fr.left + (fr.width/2.f), fr.top + (fr.height/2.f) );

    shape.setFillColor( MOUSE_COLOR_1 );
    rt.draw(shape);
}
//Cat
void Cat::Draw(sf::RenderTarget & rt)
{
    sf::ConvexShape shape;
    {
        // resize it to 5 points
        shape.setPointCount(5);
        // define the points
        const int MULT = 14;
        const int USUAL = MULT * 3;
        const int EXTRA = MULT * 1;
        shape.setPoint(0, sf::Vector2f(0, -USUAL -EXTRA));
        shape.setPoint(1, sf::Vector2f(USUAL, -USUAL));
        shape.setPoint(2, sf::Vector2f(USUAL, USUAL));
        shape.setPoint(3, sf::Vector2f(-USUAL, USUAL));
        shape.setPoint(4, sf::Vector2f(-USUAL, -USUAL));
    }
    shape.setRotation( 90.f * GetActualOri() );

    auto fr = arena_.lock()->GetCellBounds(GetActualPos().x, GetActualPos().y);
    shape.setPosition( fr.left + (fr.width/2.f), fr.top + (fr.height/2.f) );

    shape.setFillColor( CAT_COLOR_1 );
    rt.draw(shape);
    
    sf::Text planText;
    planText.setFont(ViewResources::GetInstance().font);
    planText.setColor(sf::Color::White);
    planText.setCharacterSize(34);
    planText.setString( patch::to_string( GetBrain()->GetSubPlan()->GetPlanID() ) );
    planText.setPosition( fr.left + (fr.width/2.f) -(planText.getGlobalBounds().width/2.f), fr.top + (fr.height/2.f) -(planText.getGlobalBounds().height/2.f) );
    rt.draw(planText);
}

