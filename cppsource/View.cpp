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






bool GetMouseEventPos(sf::Event & event, sf::Vector2i & returnPos)
{
    if (event.type == sf::Event::MouseButtonPressed or event.type == sf::Event::MouseWheelMoved)
    {
        if (event.type == sf::Event::MouseButtonPressed)
        {
            returnPos.x = event.mouseButton.x;
            returnPos.y = event.mouseButton.y;
        }
        if (event.type == sf::Event::MouseWheelMoved)
        {
            returnPos.x = event.mouseWheel.x;
            returnPos.y = event.mouseWheel.y;
        }
        return true;
    }
    return false;
}

///////////////////     class View      ////////////////////////////////////////////////

const unsigned BAR_WIDTH = 720;
const unsigned BAR_HEIGHT = 180;
const sf::Vector2u MINIMUM_WINDOW_SIZE { BAR_WIDTH, BAR_HEIGHT + 300 };
const sf::Vector2u INITIAL_WINDOW_SIZE { 1400, 900 };

//const sf::Vector2f INITIAL_BAR_SIZE    { (float)INITIAL_WINDOW_SIZE.x, (float)BAR_HEIGHT };
//const sf::Vector2f INITIAL_FIELD_SIZE  { (float)INITIAL_WINDOW_SIZE.x, (float)INITIAL_WINDOW_SIZE.y - (float)BAR_HEIGHT };

PaneGroup::PaneGroup(Model & model_p)
    :theModel(model_p)
    ,window(sf::VideoMode(INITIAL_WINDOW_SIZE.x, INITIAL_WINDOW_SIZE.y), "Neuronal", sf::Style::Default, sf::ContextSettings{0,0,8} )
    ,paneLevel(model_p.GetArena(), uiObjects)
    ,paneBrain(model_p.GetMouseBrain(), uiObjects)
    ,paneBar(model_p, uiObjects)
    ,paneText(uiObjects)
    ,quitYet_(false)
    //,textEnterer_(nullptr)
    //,isMouseCursorSet_(false)
{
    window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(3);
    SizingsRefresh();
    //window.setView(paneBrain.view);
}

void PaneGroup::Draw()
{
//    cursorOne.Revalidate();
//    cursorTwo.Revalidate();
    window.clear();

    if (fieldMode)
        paneBrain.Draw(window);
    else
        paneLevel.Draw(window);
    paneBar.Draw(window);

    window.display();
    //window.setView(brainView);
}

bool PaneGroup::HandleInput()
{
    HandleInputEvents();
    //temp//HandleInputState();
    return quitYet_;
}

void PaneGroup::HandleInputEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            quitYet_ = true;
        if (event.type == sf::Event::Resized)
            SizingsRefresh();
        
        //Mouse Events
        sf::Vector2i mousePixelPos;
        bool isMouseEvent = GetMouseEventPos(event, mousePixelPos);
        if (isMouseEvent)
        {
            BasePane * allPanes[] { &paneLevel, &paneBrain, &paneBar };
            for (auto * p: allPanes)
            {
                sf::IntRect vp = window.getViewport(p->view);
                if (vp.contains(mousePixelPos))
                {
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePixelPos, p->view);
                    p->HandleMouse(event, worldPos);
                }
            }
        }

        if (uiObjects.textEnterer_)
        {
            paneText.Handle(event);
        }
        else
        {
            //Keyboard Events
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Num0)
                {
                    ToggleFieldMode();
                }
            }
            paneBrain.Handle(event);
        }
    }
}



void PaneGroup::SizingsRefresh()
{
    sf::Vector2u winSize = window.getSize();
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

    const float barRatio = (float)BAR_HEIGHT / (float)winSize.y ;
    const sf::FloatRect barViewport   { 0.0f, 1.f - barRatio,   1.0f, barRatio };
    const sf::FloatRect fieldViewport { 0.0f, 0.0f,             1.0f, 1.f - barRatio };

    paneBrain.view.setViewport(fieldViewport);
    paneLevel.view.setViewport(fieldViewport);
    paneBar.view.setViewport(barViewport);

    //could use...
        //sf::IntRect window.getViewport(brainView);
    paneBrain.view.setSize( sf::Vector2f{(float)winSize.x, (float)winSize.y - (float)BAR_HEIGHT} );
    paneLevel.view.setSize( sf::Vector2f{(float)winSize.x, (float)winSize.y - (float)BAR_HEIGHT} );
    paneBar.view.setSize( sf::Vector2f{(float)winSize.x, (float)BAR_HEIGHT} );
    paneBar.view.setCenter( paneBar.view.getSize() / 2.f );
    //could use...
        //view.reset(FloatRect);
}






void BaseAreaPane::Zoom(float zoomFactor)
{
    view.zoom( zoomFactor );
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
            //model_.OuterTick();
            arena->TimeAdvance();
        }
    }
}
void PaneLevel::HandleMouse(sf::Event & event, sf::Vector2f worldPos)
{
    
}

void PaneLevel::Draw(sf::RenderWindow & window)
{
    window.setView(view);
    assert(arena);
    arena->Draw(window);
}





PaneBrain::PaneBrain(Shp<BaseReferer> br, UIObjects & uio)
    :brain(br)
    ,uiObjects(uio)
    ,highlightingMode(1)
{
    uiObjects.cursorOne.SetToPlan( brain->GetSubPlan() );
    uiObjects.cursorTwo.SetToPlan( brain->GetSubPlan() );
}

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
            //model_.InnerTick();
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
        auto & cursorOne = uiObjects.cursorOne;
        auto & cursorTwo = uiObjects.cursorTwo;
        PlanShp plan1;
        if (cursorOne.GetState() != CursorState::ABSENT)
        {
            plan1 = cursorOne.GetPlan();
            //temp//EventsPlan(plan1);
        }
        if (cursorOne.GetState() == CursorState::PORT)
        {
            plan1 = cursorOne.GetPlan();
            PortLocation pl = cursorOne.GetPort();
            if (event.key.code == sf::Keyboard::G)
            {
                assert(not uiObjects.textEnterer_);
                uiObjects.textEnterer_ = std::make_shared<TextEnterer>();
                auto bound = std::bind( &ChipPlan::RenamePortTag, plan1, pl, std::placeholders::_1 );
                uiObjects.textEnterer_->SetDispatchTarget( bound );
            }
        }
        if (cursorOne.GetState() == CursorState::LOCATED)
        {
            PlanPos pos1 = cursorOne.GetPlanPos();
            //temp//EventsLocated(pos1);
        }
        Shp<WiringPair> wp = RetrieveWiringPair(cursorOne, cursorTwo);
        if (wp)
        {
            //temp//EventsBothWirable(wp);
        }
    }
//    cu1.Revalidate();
//    cu2.Revalidate();
}
void PaneBrain::HandleMouse(sf::Event & event, sf::Vector2f worldPos)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            uiObjects.cursorOne.SetPosWorld(worldPos);
        }
        if (event.mouseButton.button == sf::Mouse::Right)
        {
            uiObjects.cursorTwo.SetPosWorld(worldPos);
        }
    }
    if (event.type == sf::Event::MouseWheelMoved)
    {
        if (event.mouseWheel.delta > 0)
        {
            CentreOn(worldPos);
        }
        Zoom( 1.f + (-0.4f * event.mouseWheel.delta) );
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

//    PlanShp ap;
//    if (cursorOne.GetState() != CursorState::ABSENT)
//        ap = cursorOne.GetPlan();
//    else
//        ap = GetViewBasePlan();
//    assert(ap);
    nameFilter.setString( theModel.GetNameFilter() );
    window.draw(nameFilter);

    marquee.Draw(window);
    
    viewPanel.SetPlanID( 0 /*ap->GetPlanID()*/, theModel.GetPlanGroupData() );
    viewPanel.Draw(window);
}


PaneText::PaneText(UIObjects & uio)
    :uiObjects(uio)
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
    if (textEnterer_)
        SetTextEntering(true, textEnterer_->GetText());
    else
        SetTextEntering(false);
}

void PaneText::HandleMouse(sf::Event & event, sf::Vector2f worldPos)
{
    
}

void PaneText::Draw(sf::RenderWindow & window)
{
    //textEntering.setString( "text1: " + patch::to_string(ap->GetPlanID()) );
    if (drawTextEntering)
    {
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
