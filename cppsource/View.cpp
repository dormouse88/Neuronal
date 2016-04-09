/* 
 * File:   View.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 11:56
 */

#include "miscUtil.hpp"
#include "View.hpp"
#include <iostream>
#include <cassert>
#include "ChipHandle.hpp"
#include "ChipPlan.hpp"


ViewPanel::ViewPanel()
{
    mainOutlineBox.setSize(sf::Vector2f{300.f, 140.f} );
    mainOutlineBox.setPosition(300.f, 20.f );
    mainOutlineBox.setFillColor(sf::Color::Transparent);
    mainOutlineBox.setOutlineThickness(2.f);
    mainOutlineBox.setOutlineColor(sf::Color::Yellow);

    parentPlanText.setFont(ViewResources::GetInstance().font);
    parentPlanText.setCharacterSize(25.f);
    parentPlanText.setPosition(320.f, 30.f);
    parentPlanText.setColor( sf::Color::Green );

    mainPlanText.setFont(ViewResources::GetInstance().font);
    mainPlanText.setCharacterSize(29.f);
    mainPlanText.setPosition(320.f, 70.f);
    mainPlanText.setColor( sf::Color::Green );

    kidsPlanText.setFont(ViewResources::GetInstance().font);
    kidsPlanText.setCharacterSize(25.f);
    kidsPlanText.setPosition(320.f, 110.f);
    kidsPlanText.setColor( sf::Color::Green );
}

void ViewPanel::SetPlanID(PlanID pid, std::shared_ptr<const PlanGroupData> pData)
{
    pid_ = pid;
    
    PlanID parent = 0;
    int numKids = 0;
    auto relatives = pData->GetRelatives(pid);
    if (relatives) {
        parent = relatives->parent;
        numKids = relatives->kids.size();
    }
    mainPlanText.setString( patch::to_string(pid) + ": " + pData->GetNameByID(pid) );
    parentPlanText.setString( patch::to_string(parent) + ": " + pData->GetNameByID(parent) );
    kidsPlanText.setString( "< kids: " + patch::to_string(numKids) + "   >" );
}

void ViewPanel::Draw(sf::RenderTarget &rt)
{
    rt.draw( mainOutlineBox );
    rt.draw( parentPlanText );
    rt.draw( mainPlanText );
    rt.draw( kidsPlanText );
}





const int MESSAGES_TO_DISPLAY = 4;
Marquee::Marquee()
{
    for (int i = 0; i< MESSAGES_TO_DISPLAY; i++)
    {
        sf::Text barText;
        barText.setFont(ViewResources::GetInstance().font);
        barText.setCharacterSize(25.f);
        barText.setPosition(670.f, 30.f + (i*25.f) );
        barText.setColor( sf::Color::Magenta );
        texts.emplace_back(barText);
    }
}

void Marquee::PostMessage(std::string message)
{
    messageDeque.push_front(message);
    if (messageDeque.size() > MESSAGES_TO_DISPLAY) messageDeque.pop_back();
}

void Marquee::Draw(sf::RenderTarget &rt)
{
    for (int i = 0; i<texts.size(); i++)
    {
        //auto & x = texts.at(i);
        if (i<messageDeque.size()) texts.at(i).setString( messageDeque.at(i) );
        rt.draw( texts.at(i) );
    }
}










ArenaCursor::ArenaCursor(sf::Color color)
{
    shape_.setFillColor(sf::Color::Transparent);
    shape_.setOutlineColor( color );
    shape_.setOutlineThickness(-4.5f);
}

void ArenaCursor::Draw(sf::RenderTarget & rt)
{
    Shp<Arena> arena = addr_.arena.lock();
    if (addr_.mode == ArenaAddressMode::CELL and arena)
    {
        sf::FloatRect bounds { arena->GetCellBounds(addr_.arenaPoint) };
        shape_.setPosition( bounds.left, bounds.top );
        shape_.setSize( {bounds.width, bounds.height} );
        rt.draw(shape_, sf::RenderStates(sf::BlendAdd));
    }
}





sf::FloatRect ConvertRect(sf::IntRect ir)
{
    return sf::FloatRect { (float)ir.left, (float)ir.top, (float)ir.width, (float)ir.height };
}

//defunct?
//sf::Vector2i GetMouseEventPos(sf::Event & event, sf::RenderWindow & window)
//{
//    if (event.type == sf::Event::MouseButtonPressed)
//    {
//        return sf::Vector2i{ event.mouseButton.x, event.mouseButton.y };
//    }
//    else if (event.type == sf::Event::MouseWheelMoved)
//    {
//        return sf::Vector2i{ event.mouseWheel.x, event.mouseWheel.y };
//    }
//    else
//    {
//        return sf::Mouse::getPosition(window);
//    }
//}

///////////////////     class View      ////////////////////////////////////////////////

const unsigned BAR_WIDTH = 720;
const unsigned BAR_HEIGHT = 180;
const sf::Vector2u MINIMUM_WINDOW_SIZE { BAR_WIDTH, BAR_HEIGHT + 300 };
const sf::Vector2u INITIAL_WINDOW_SIZE { 1400, 900 };

PaneGroup::PaneGroup(Model & model_p)
    :theModel(model_p)
    ,window(sf::VideoMode(INITIAL_WINDOW_SIZE.x, INITIAL_WINDOW_SIZE.y), "Neuronal", sf::Style::Default, sf::ContextSettings{0,0,8} )
    ,fieldMode(1)
    ,paneLevel(model_p.GetArena(), uiObjects)
    ,paneBrain(model_p, uiObjects)
    ,paneBar(model_p, uiObjects)
    ,paneText(uiObjects)
    ,quitYet_(false)
{
    window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(3);
    SizingsRefresh();
}

void PaneGroup::Draw()
{
    window.clear();

    if (fieldMode != 2)
        paneBrain.Draw(window);
    if (fieldMode != 1)
        paneLevel.Draw(window);
    paneBar.Draw(window);
    paneText.Draw(window);

    window.display();
}

bool PaneGroup::HandleInput()
{
    //Get Pane and WorldPos under the mouse...
      //sf::Vector2i mousePixelPos = GetMouseEventPos(event, window);
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    BasePane * pane = nullptr;
    sf::Vector2f mouseWPos;
    BasePane * allPanes[] { &paneLevel, &paneBrain, &paneBar };
    for (auto * p: allPanes)
    {
        sf::IntRect vp = window.getViewport(p->view);
        if (vp.contains(mousePixelPos))
        {
            mouseWPos = window.mapPixelToCoords(mousePixelPos, p->view);
            pane = p;
        }
    }
    
    HandleInputEvents(pane, mouseWPos);
    HandleInputState(pane, mouseWPos);
    return quitYet_;
}

void PaneGroup::HandleInputState(BasePane * pane, sf::Vector2f worldPos)
{

    
//    if ( sf::Mouse::isButtonPressed(sf::Mouse::Middle) )
//    {
//        sf::Vector2i pixelPos{ sf::Mouse::getPosition(view_.GetWindow()) };
//        sf::Vector2f worldPos{ view_.GetWindow().mapPixelToCoords( pixelPos ) };
//        if (!isMouseCursorSet_) {
//            isMouseCursorSet_ = true;
//        }
//        else {
//            view_.Pan( (-worldPos + mouseCursorWorldPos_) /1.00f );
//        }
//        sf::Vector2i newPixelPos{ sf::Mouse::getPosition(view_.GetWindow()) };
//        mouseCursorWorldPos_ = view_.GetWindow().mapPixelToCoords( newPixelPos );
//    }
//    else {
//        isMouseCursorSet_ = false;
//    }
}

void PaneGroup::HandleInputEvents(BasePane * pane, sf::Vector2f worldPos)
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            quitYet_ = true;
        if (event.type == sf::Event::Resized)
            SizingsRefresh();
        
        if (uiObjects.textEnterer_)
        {
            paneText.Handle(event);
        }
        else
        {
            if (pane)
                pane->HandleMouse(event, worldPos);
            
            //Keyboard Events
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Num0)
                {
                    CycleFieldMode();
                }
                if (event.key.code == sf::Keyboard::P)
                {
                    //view brain
                    ArenaPoint addr = uiObjects.arenaCursor.GetArenaPoint();
                    auto ele = theModel.GetArena()->GetEntity( addr );
                    if (ele)
                    {
                        auto puppet = std::dynamic_pointer_cast<Puppet>(ele);
                        if (puppet)
                        {
                            paneBrain.SetBrain( puppet->GetBrain() );
                            uiObjects.cursorOne.SetPlanAddress(PlanAddress{});
                            uiObjects.cursorTwo.SetPlanAddress(PlanAddress{});
                        }
                    }
                }
            }
            
            paneBrain.Handle(event);
            paneLevel.Handle(event);
            paneBar.Handle(event);
        }
    } //while event
}



void PaneGroup::SizingsRefresh()
{
    sf::Vector2u winSize = window.getSize();
    //Disallow tiny windows...
    bool underMin = false;
    if (winSize.x < MINIMUM_WINDOW_SIZE.x)
    {
        winSize.x = MINIMUM_WINDOW_SIZE.x;
        underMin = true;
    }
    if (winSize.y < MINIMUM_WINDOW_SIZE.y)
    {
        winSize.y = MINIMUM_WINDOW_SIZE.y;
        underMin = true;
    }
    if (underMin)
        window.setSize(winSize);

    //Calculate viewPort ratios...
    const float barRatio = (float)BAR_HEIGHT / (float)winSize.y ;
    const sf::FloatRect nullViewport { 0.0f, 0.0f, 0.0f, 0.0f };
    const sf::FloatRect barViewport   { 0.0f, 1.f - barRatio,   1.0f, barRatio };
    const sf::FloatRect fieldViewport { 0.0f, 0.0f,             1.0f, 1.f - barRatio };
    const sf::FloatRect leftField  { fieldViewport.left, fieldViewport.top , fieldViewport.width * 0.5f , fieldViewport.height };
    const sf::FloatRect rightField { fieldViewport.left + (fieldViewport.width * 0.5f), fieldViewport.top , fieldViewport.width * 0.5f , fieldViewport.height };
    

    //Set sf::view (viewports + worldzone)...
    {
        if (fieldMode == 1)
            paneBrain.view.setViewport(fieldViewport);
        if (fieldMode == 2)
            paneBrain.view.setViewport(nullViewport);
        if (fieldMode == 3)
            paneBrain.view.setViewport(leftField);
        sf::FloatRect vp = ConvertRect( window.getViewport(paneBrain.view) );
        paneBrain.view.setSize(vp.width, vp.height);
    }
    {
        if (fieldMode == 1)
            paneLevel.view.setViewport(nullViewport);
        if (fieldMode == 2)
            paneLevel.view.setViewport(fieldViewport);
        if (fieldMode == 3)
            paneLevel.view.setViewport(rightField);
        sf::FloatRect vp = ConvertRect( window.getViewport(paneLevel.view) );
        paneLevel.view.setSize(vp.width, vp.height);
    }
    {
        paneBar.view.setViewport(barViewport);
        sf::FloatRect vp = ConvertRect( window.getViewport(paneBar.view) );
        paneBar.view.setSize(vp.width, vp.height);
        paneBar.view.setCenter( vp.width * 0.5f, vp.height * 0.5f );
    }
}






void BaseAreaPane::Zoom(float zoomFactor, VectorWorld anchorPoint)
{
    //Googlemaps-style zoom: (the same worldPoint stays under mouse cursor)
    VectorWorld worldOffset = view.getCenter() - anchorPoint;
    sf::Vector2f normOffset { worldOffset.x / view.getSize().x, worldOffset.y / view.getSize().y };
    view.zoom( zoomFactor);
    VectorWorld newWorldOffset { normOffset.x * view.getSize().x, normOffset.y * view.getSize().y };
    CentreOn( anchorPoint + newWorldOffset );
    AutoClamp();
}
void BaseAreaPane::Pan(sf::Vector2f moveBy)
{
    view.move(moveBy);
    AutoClamp();
}
void BaseAreaPane::CentreOn(VectorWorld point)
{
    view.setCenter(point);
}
void BaseAreaPane::ClampToRect(RectWorld b)
{
    //Clamps view to a given rectangle
    //Considers both dimensions separately and clamps view to be
    //either contained by or containing the rectangle in that dimension.
    //(Which depends on if it is larger or smaller in that dimension).
    auto center = view.getCenter();
    auto viewSize = view.getSize();
    VectorWorld size = viewSize - VectorWorld{120.f, 120.f};
    VectorWorld tl = center - (size/2.f);
    bool oversizedX = (b.width > size.x) ? true : false;
    bool oversizedY = (b.height > size.y) ? true : false;
    if (oversizedX) {
        if (tl.x          < b.left)            tl.x = b.left;
        if (tl.x + size.x > b.left + b.width) tl.x = b.left + b.width - size.x;
    }
    else {
        if (tl.x          > b.left)            tl.x = b.left;
        if (tl.x + size.x < b.left + b.width) tl.x = b.left + b.width - size.x;
    }
    if (oversizedY) {
        if (tl.y          < b.top)             tl.y = b.top;
        if (tl.y + size.y > b.top + b.height) tl.y = b.top + b.height - size.y;
    }
    else {
        if (tl.y          > b.top)             tl.y = b.top;
        if (tl.y + size.y < b.top + b.height) tl.y = b.top + b.height - size.y;
    }
    view.setCenter(tl + (size/2.f) );
}









PaneLevel::PaneLevel(Shp<Arena> aren, UIObjects & uio)
    :arena(aren)
    ,uiObjects(uio)
{}

void PaneLevel::AutoClamp()
{
    ;
}

void PaneLevel::Handle(sf::Event & event)
{
    //Keyboard Events
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Period)
        {
            arena->TimeAdvance();
        }
            //add cat
            //add mouse
            //add goal
            //remove element
            //edit element (various)
    }
}
void PaneLevel::HandleMouse(sf::Event & event, sf::Vector2f worldPos)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            ArenaPoint ap = arena->GetArenaPoint( worldPos );
            uiObjects.arenaCursor.SetArenaPoint( ap, arena );
        }
        if (event.mouseButton.button == sf::Mouse::Right)
        {
        }
    }
    
    if (event.type == sf::Event::MouseMoved)
    {
        if ( sf::Mouse::isButtonPressed(sf::Mouse::Middle) )
        {
            Pan( -(worldPos - prevMouseWorldPos) * 1.00f );
        }
        else
            prevMouseWorldPos = worldPos;
    }
    if (event.type == sf::Event::MouseWheelMoved)
    {
        Zoom( 1.f + (-0.3f * event.mouseWheel.delta), worldPos );
    }
}

void PaneLevel::Draw(sf::RenderWindow & window)
{
    window.setView(view);
    assert(arena);
    arena->Draw(window);
    uiObjects.arenaCursor.Draw(window);
}





PaneBrain::PaneBrain(Model & model_p, UIObjects & uio)
    :model_(model_p)
    ,brain(model_p.GetArena()->GetMouseBrain())
    ,uiObjects(uio)
    ,highlightingMode(1)
{}

void PaneBrain::AutoClamp()
{
    auto clampPlan = brain->GetSubPlan();
    if (clampPlan)
    {
        RectWorld b = clampPlan->GetWorldPaddedBoundPlusPorts();  //unimportant
        ClampToRect(b);
    }
}

void PaneBrain::Handle(sf::Event & event)
{
    //Keyboard Events
    if (event.type == sf::Event::KeyPressed)
    {
        //requires brain...
        if (event.key.code == sf::Keyboard::Comma)
        {
            if (not brain->IsAnyOutputOn())
                brain->TickOnce();
        }
        if (event.key.code == sf::Keyboard::Num1)
        {
            SetHighlightingMode(1);
        }
        if (event.key.code == sf::Keyboard::Num2)
        {
            SetHighlightingMode(2);
        }
        if (event.key.code == sf::Keyboard::Num3)
        {
            SetHighlightingMode(3);
        }

        //Brain sections...
        PlanAddress pa1 = uiObjects.cursorOne.GetPA();
        PlanAddress pa2 = uiObjects.cursorTwo.GetPA();
        PlanShp plan1 = pa1.plan.lock();
        if (pa1.mode != PlanAddressMode::ABSENT)
        {
            HandlePlan(event, plan1);
        }
        if (pa1.mode == PlanAddressMode::PORT)
        {
            PortLocation pl = pa1.port;
            if (event.key.code == sf::Keyboard::G)
            {
                assert(not uiObjects.textEnterer_);
                uiObjects.textEnterer_ = std::make_shared<TextEnterer>();
                auto bound = std::bind( &ChipPlan::RenamePortTag, plan1, pl, std::placeholders::_1 );
                uiObjects.textEnterer_->SetDispatchTarget( bound );
            }
        }
        if (pa1.mode == PlanAddressMode::CELL)
        {
            PlanPos pp1 { pa1.pos, plan1->GetGrid() };
            HandleLocated(event, pp1);
        }
        Shp<WiringPair> wp = RetrieveWiringPair(pa1, pa2);
        if (wp)
        {
            EventsBothWirable(event, wp);
        }
        MakeValidAndVisible(pa1);
        MakeValidAndVisible(pa2);
        uiObjects.cursorOne.SetPlanAddress( pa1 );
        uiObjects.cursorTwo.SetPlanAddress( pa2 );
    }
}

void PaneBrain::HandlePlan(sf::Event & event, PlanShp plan)
{
    //requires cu1 PLAN
    if (event.key.code == sf::Keyboard::X)
    {
        //Collapse an expanded plan
        auto h = plan->GetHandle();
        if (h)
            h->SetExploded(false);
    }

    if (event.key.code == sf::Keyboard::LBracket)
    {
        //uiObjects.cursorOne.SetToPlan();
    }
    if (event.key.code == sf::Keyboard::RBracket)
    {
        //uiObjects.cursorTwo.SetToPlan();
    }

    if (event.key.code == sf::Keyboard::Q)
    {
        //Save plan
        if (not event.key.shift and (model_.GetPlanGroupData()->GetNameByID(plan->GetPlanID()) != NULL_PLAN_NAME))  //ouch
            model_.SavePlan( plan, PlanNamingMode::TRANSFER );
//        else;
//            model_.SavePlan( plan, PlanNamingMode::AUTONAME );
    }
    if (event.key.code == sf::Keyboard::Tab)
    {
        //Save plan with a name
        uiObjects.textEnterer_ = std::make_shared<TextEnterer>();
        auto bound = std::bind( &Model::SavePlan, &model_, plan, PlanNamingMode::PROVIDED, std::placeholders::_1 );
        uiObjects.textEnterer_->SetDispatchTarget( bound );
    }
    
    if (event.key.code == sf::Keyboard::W)
    {
        //Wipe Plan
        model_.LoadPlan(plan, PlanNav::EMPTY, event.key.shift and event.key.control);
    }

    //Various Load Plan Options...
    if (event.key.code == sf::Keyboard::Numpad1)
    {
        if (event.key.control)
            model_.LoadPlan(plan, PlanNav::PREV_ID);
        else
            model_.LoadPlan(plan, PlanNav::PREV_NAMED_ID);
    }
    if (event.key.code == sf::Keyboard::Numpad3)
    {
        if (event.key.control)
            model_.LoadPlan(plan, PlanNav::NEXT_ID);
        else
            model_.LoadPlan(plan, PlanNav::NEXT_NAMED_ID);
    }
    if (event.key.code == sf::Keyboard::Numpad7)
    {
        model_.LoadPlan(plan, PlanNav::PREV_NAME);
    }
    if (event.key.code == sf::Keyboard::Numpad9)
    {
        model_.LoadPlan(plan, PlanNav::NEXT_NAME);
    }
    if (event.key.code == sf::Keyboard::Numpad8)
    {
        model_.LoadPlan(plan, PlanNav::PARENT);
    }
    if (event.key.code == sf::Keyboard::Numpad2)
    {
        model_.LoadPlan(plan, PlanNav::FIRST_CHILD);
    }
    if (event.key.code == sf::Keyboard::Numpad4)
    {
        model_.LoadPlan(plan, PlanNav::PREV_SIBLING);
    }
    if (event.key.code == sf::Keyboard::Numpad6)
    {
        model_.LoadPlan(plan, PlanNav::NEXT_SIBLING);
    }
    if (event.key.code == sf::Keyboard::Subtract)
    {
        //clear name filter
        assert(not uiObjects.textEnterer_);
        model_.EngageNameFilter(plan, "");
    }
    if (event.key.code == sf::Keyboard::Add)
    {
        //set name filter
        assert(not uiObjects.textEnterer_);
        uiObjects.textEnterer_ = std::make_shared<TextEnterer>();
        auto bound = std::bind( &Model::EngageNameFilter, &model_, plan, std::placeholders::_1 );
        uiObjects.textEnterer_->SetDispatchTarget( bound );
        uiObjects.textEnterer_->SetText( model_.GetNameFilter() );
    }

    if (event.key.code == sf::Keyboard::E)
    {
        //erase plan name
        model_.RemoveName(plan->GetPlanID());
    }
    if (event.key.code == sf::Keyboard::T and (model_.GetPlanName(plan->GetPlanID()) == NULL_PLAN_NAME) )
    {
        //Add plan name
        uiObjects.textEnterer_ = std::make_shared<TextEnterer>();
        auto bound = std::bind( &Model::AddName, &model_, plan->GetPlanID(), std::placeholders::_1 );
        uiObjects.textEnterer_->SetDispatchTarget( bound );
    }
    if (event.key.code == sf::Keyboard::R and (model_.GetPlanName(plan->GetPlanID()) != NULL_PLAN_NAME) )
    {
        //Change plan name
        uiObjects.textEnterer_ = std::make_shared<TextEnterer>();
        auto bound = std::bind( &Model::ChangeName, &model_, plan->GetPlanID(), std::placeholders::_1 );
        uiObjects.textEnterer_->SetDispatchTarget( bound );
        uiObjects.textEnterer_->SetText( model_.GetPlanName(plan->GetPlanID()) );
    }
}

void PaneBrain::HandleLocated(sf::Event & event, PlanPos pos1)
{
    //requires cu1 LOCATED
    if (event.key.code == sf::Keyboard::S)
    {
        auto h = pos1.GetDeviceAsHandle();
        if (h)
            h->SetExploded(true);
    }

    if (event.key.code == sf::Keyboard::N)
    {
        if (event.key.shift == false)
        {
            model_.GetFactory()->AddNeuron(pos1);
        }
        else
        {
            pos1.GetPlan()->RemoveDevice(pos1);
        }
    }
    if (event.key.code == sf::Keyboard::H)
    {
        model_.GetFactory()->AddHandle(pos1);
    }
    //if PlanPos sucessfully selects a device...
    DeviceShp dev1 = pos1.GetDevice();
    if (dev1)
    {
        if (event.key.code == sf::Keyboard::A)
        {
            //view_.PostMessage("Tried to modify a device upwards");
            dev1->Handle(1);
        }
        if (event.key.code == sf::Keyboard::Z)
        {
            dev1->Handle(2);
        }
        if (event.key.code == sf::Keyboard::BackSlash)
        {
            dev1->Handle(3);
        }

        //if cu1 DEVICE + cu2 LOCATED
        PlanAddress pa2 = uiObjects.cursorTwo.GetPA();
        PlanShp plan2 = pa2.plan.lock();
        if (pa2.mode == PlanAddressMode::CELL)
        {
            //EventsBothLocated(pos1, pos2);
            if (event.key.code == sf::Keyboard::M)
            {
                //view_.PostMessage("Tried to move something");
                if (dev1->GetContainer() == plan2)
                    dev1->GetContainer()->SetPosition(dev1, pa2.pos);
                //ChipPlanFunc::SetPosition(pos1, pos2); //allows moving of exploded plans
            }
        }
    }
}

void PaneBrain::EventsBothWirable(sf::Event & event, Shp<WiringPair> wp)
{
    //requires cu1 + cu2 in a VALID WIRING RELATIONSHIP
    if (event.key.code == sf::Keyboard::B)
    {
        if (event.key.shift == false)
        {
            model_.GetFactory()->AddWire(wp, 1);
        }
        else
        {
            auto wire = wp->plan->GetWire(wp);
            if (wire)
                wp->plan->RemoveWire(wire);
        }
    }
    auto wire = wp->plan->GetWire(wp);
    if (wire) 
    {
        if (event.key.code == sf::Keyboard::D) {
            wire->Handle(1);
        }
        if (event.key.code == sf::Keyboard::C) {
            wire->Handle(2);
        }
        if (event.key.code == sf::Keyboard::F) {
            wire->Handle(3);
        }
        if (event.key.code == sf::Keyboard::V) {
            wire->Handle(4);
        }
    }
}



void PaneBrain::HandleMouse(sf::Event & event, sf::Vector2f worldPos)
{
    if (event.type == sf::Event::MouseMoved)
    {
        if ( sf::Mouse::isButtonPressed(sf::Mouse::Middle) )
        {
            Pan( -(worldPos - prevMouseWorldPos) * 1.00f );
        }
        else
            prevMouseWorldPos = worldPos;
    }
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            PlanAddress pa = brain->GetSubPlan()->GetInnermostExplodedPlanAddress(worldPos);
            uiObjects.cursorOne.SetPlanAddress( pa );
        }
        if (event.mouseButton.button == sf::Mouse::Right)
        {
            PlanAddress pa = brain->GetSubPlan()->GetInnermostExplodedPlanAddress(worldPos);
            uiObjects.cursorTwo.SetPlanAddress( pa );
        }
    }
    if (event.type == sf::Event::MouseWheelMoved)
    {
        Zoom( 1.f + (-0.3f * event.mouseWheel.delta), worldPos );
    }
}

void PaneBrain::Draw(sf::RenderWindow & window)
{
    window.setView(view);
    
    //Draw all plans/devices/wires...
    assert(brain);
    brain->DrawBrain(window);

    //Highlighting modes...
    if (highlightingMode != 1)  //dim everything that's been drawn so far...
    {
        //window.setView(fieldOverlay);
        brainOverlayBox.setPosition( view.getCenter() - (view.getSize() / 2.f) );
        brainOverlayBox.setSize( view.getSize() );

        brainOverlayBox.setFillColor( sf::Color{0,0,0,210} );
        window.draw(brainOverlayBox);
        //window.setView(brainView);
    }
    uiObjects.cursorTwo.Draw(window);
    uiObjects.cursorOne.Draw(window);
    //draw selected Gobjects again in full brightness...
    if (highlightingMode == 2)
    {
//        auto d1 = cursorOne.GetPlanPos().GetDevice();
//        if (d1) d1->Draw(window);
//        auto d2 = cursorTwo.GetPlanPos().GetDevice();
//        if (d2) d2->Draw(window);
//        auto w1 = ap->GetWire(d1, d2);
//        if (w1) w1->Draw(window);
    }
    if (highlightingMode == 3)
    { //this whole GetWires() business is shonky...
//        auto d1 = cursorOne.GetPlanPos().GetDevice();
//        if (d1) {
//            d1->Draw(window);
//            auto vec = ChipPlanFunc::GetWires(cursorOne.GetPlanPos(), true, false);  //ap->GetWires(d1, true, true);
//            for (auto x: vec) {
//                x->Draw(window);
//            }
//        }
    }
}


PaneBar::PaneBar(Model & model_p, UIObjects & uio)
    :theModel(model_p)
    ,uiObjects(uio)
{
    nameFilter.setFont(ViewResources::GetInstance().font);
    nameFilter.setCharacterSize(25.f);
    nameFilter.setPosition(38.f, 70.f);
    nameFilter.setColor( sf::Color::Yellow );
}
void PaneBar::Handle(sf::Event & event)
{
    
}
void PaneBar::HandleMouse(sf::Event & event, sf::Vector2f worldPos)
{
    
}
void PaneBar::Draw(sf::RenderWindow & window)
{
    window.setView(view);
    sf::RectangleShape bar;
    bar.setSize( view.getSize() );
    bar.setPosition( view.getCenter() - view.getSize()/2.f );
    bar.setFillColor(sf::Color::Blue);
    window.draw(bar);
//    //Draw the bar using a VertexArray...
//    sf::VertexArray lines(sf::Quads, 4);
//    lines[0].position = sf::Vector2f(0, 0);
//    lines[1].position = sf::Vector2f(1400, 0);
//    lines[2].position = sf::Vector2f(1400, 180);
//    lines[3].position = sf::Vector2f(0, 180);
//    lines[0].color = sf::Color::Red;
//    lines[2].color = sf::Color::Yellow;
//    window.draw(lines);

    PlanShp ap = nullptr;
    if (uiObjects.cursorOne.GetPA().mode != PlanAddressMode::ABSENT)
        ap = uiObjects.cursorOne.GetPA().plan.lock();
    PlanID planID = 0;
    if (ap)
        planID = ap->GetPlanID();

    nameFilter.setString( theModel.GetNameFilter() );
    window.draw(nameFilter);

    marquee.Draw(window);
    
    viewPanel.SetPlanID( planID, theModel.GetPlanGroupData() );
    viewPanel.Draw(window);
}


PaneText::PaneText(UIObjects & uio)
    :uiObjects(uio)
    ,drawTextEntering(false)
{
    textEntering.setFont(ViewResources::GetInstance().font);
    textEntering.setCharacterSize(25.f);
    textEntering.setPosition(30.f, 30.f);
    textEntering.setColor( sf::Color::Green );
}

void PaneText::Handle(sf::Event & event)
{
    auto & textEnterer_ = uiObjects.textEnterer_;
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::BackSpace)
        {
            textEnterer_->BackSpace();
        }
        if (event.key.code == sf::Keyboard::Return or event.key.code == sf::Keyboard::Escape)
        {
            if (event.key.code == sf::Keyboard::Return)
            {
                textEnterer_->Dispatch();
            }
            textEnterer_ = nullptr;
        }
    }
    if (event.type == sf::Event::TextEntered)
    {
        if (textEnterer_)
            textEnterer_->Append(event.text.unicode);
    }
}

void PaneText::HandleMouse(sf::Event & event, sf::Vector2f worldPos)
{
    
}

void PaneText::Draw(sf::RenderWindow & window)
{
    //textEntering.setString( "text1: " + patch::to_string(ap->GetPlanID()) );
    if (uiObjects.textEnterer_)
    {
        textEntering.setString("-->" + uiObjects.textEnterer_->GetText() + "<--");
        window.draw(textEntering);
    }
}
















//DEL
//    //Create a string to represent the stack of plans...
//    std::string planNumStr;
//    PlanShp cont = ap;
//    HandleShp ref = nullptr;
//    while (cont)
//    {
//        //(the string is written in reverse using insert(0,"") to prepend to make the active plan come last)
//        if (cont->IsModified()) planNumStr.insert(0, "*");
//        planNumStr.insert(0, patch::to_string( cont->GetPlanID() ) );
//        //Step out a layer...
//        ref = cont->GetHandle();
//        if (ref) {
//            cont = ref->GetContainer();
//            if (cont) {
//                planNumStr.insert(0, " > ");
//            }
//        }
//        else
//        {
//            cont = nullptr;
//        }
//    }
