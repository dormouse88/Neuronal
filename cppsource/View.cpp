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

void ViewPanel::SetPlanID(int plan, std::shared_ptr<const UserData> uData)
{
    planID = plan;
    auto userData = uData;
    
    int parent = 0;
    int numKids = 0;
    auto relatives = userData->GetRelatives(planID);
    if (relatives) {
        parent = relatives->parent;
        numKids = relatives->kids.size();
    }
    mainPlanText.setString( patch::to_string(planID) + ": " + userData->GetNameByID(planID) );
    parentPlanText.setString( patch::to_string(parent) + ": " + userData->GetNameByID(parent) );
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


const sf::FloatRect MAIN_VIEWPORT {0.f, 0.f, 1.f, 0.8f};
const sf::FloatRect BAR_VIEWPORT {0.f, 0.8f, 1.f, 0.2f};
const int BAR_HEIGHT = 180.f;
//This is all very shonky. Need to consider what is flexible and what is fixed on screen.
const sf::Vector2i INITIAL_WINDOW_SIZE { 1400, 900 };
const sf::Vector2f INITIAL_MAINVIEW_SIZE { (float)INITIAL_WINDOW_SIZE.x, (float)INITIAL_WINDOW_SIZE.y - BAR_HEIGHT };

View::View(Model & model_p)
   :theModel(model_p)
    ,window(sf::VideoMode(INITIAL_WINDOW_SIZE.x, INITIAL_WINDOW_SIZE.y), "Neuronal", sf::Style::Default, sf::ContextSettings{0,0,8} )
    ,mainView(sf::Vector2f{0.f, 0.f}, INITIAL_MAINVIEW_SIZE )
    ,mainOverlay(sf::FloatRect{0.f,0.f, INITIAL_MAINVIEW_SIZE.x, INITIAL_MAINVIEW_SIZE.y})
    ,barOverlay(sf::FloatRect{0.f,0.f, INITIAL_MAINVIEW_SIZE.x, BAR_HEIGHT})
    ,cursorOne(model_p.GetBasePlan()->GetGrid())
    ,cursorTwo(model_p.GetBasePlan()->GetGrid(), sf::Color::Cyan)
    ,highlightingMode(1)
{
    window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(3);
    mainView.setViewport(MAIN_VIEWPORT);
    mainOverlay.setViewport(MAIN_VIEWPORT);
    barOverlay.setViewport(BAR_VIEWPORT);
    window.setView(mainView);

    mainOverlayBox.setSize(sf::Vector2f{1400.f, 720.f} );
    
    barText1.setFont(ViewResources::GetInstance().font);
    barText1.setCharacterSize(25.f);
    barText1.setPosition(30.f, 30.f);
    barText1.setColor( sf::Color::Green );
    }


void View::Draw()
{
    cursorOne.Revalidate();
    cursorTwo.Revalidate();
    window.clear();

    DrawMain();
    DrawBar();

    window.display();
    window.setView(mainView);
}

void View::DrawMain() //Main Port...
{
    window.setView(mainView);
    auto ap = GetViewBasePlan();
    assert(ap);
    
    //Draw all plans/devices/wires...
    ap->Draw(window);

    //Highlighting modes...
    if (highlightingMode != 1)  //dim everything that's been drawn so far...
    {
        window.setView(mainOverlay);
        mainOverlayBox.setFillColor( sf::Color{0,0,0,210} );
        window.draw(mainOverlayBox);
        window.setView(mainView);
    }
    cursorTwo.Draw(window);
    cursorOne.Draw(window);
    //draw selected Gobjects again in full brightness...
    if (highlightingMode == 2)
    {
        auto d1 = ChipPlanFunc::GetDevice(cursorOne.GetPlanPos());
        if (d1) d1->Draw(window);
        auto d2 = ChipPlanFunc::GetDevice(cursorTwo.GetPlanPos());
        if (d2) d2->Draw(window);
        auto w1 = ChipPlanFunc::GetWire(cursorOne.GetPlanPos(), cursorTwo.GetPlanPos());
        if (w1) w1->Draw(window);
    }
    if (highlightingMode == 3)
    { //this whole GetWires() business is shonky...
        auto d1 = ChipPlanFunc::GetDevice(cursorOne.GetPlanPos());
        if (d1) {
            d1->Draw(window);
            auto vec = ChipPlanFunc::GetWires(cursorOne.GetPlanPos(), true, false);  //ap->GetWires(d1, true, true);
            for (auto x: vec) {
                x->Draw(window);
            }
        }
    }
}

void View::DrawBar() //Bar Port...
{
    window.setView(barOverlay);
    sf::RectangleShape bar;
    bar.setSize( barOverlay.getSize() );
    bar.setPosition( barOverlay.getCenter() - barOverlay.getSize()/2.f );
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

    
    auto ap = cursorOne.GetPlanPos().GetPlan();
    if (not ap) ap = GetViewBasePlan();
    assert(ap);
    
    //barText1.setString( "text1: " + patch::to_string(ap->GetPlanID()) );
    if (drawTextEntering) window.draw(barText1);

    marquee.Draw(window);
    
    viewPanel.SetPlanID( ap->GetPlanID(), theModel.GetUserData() );
    viewPanel.Draw(window);
}

void View::Zoom(float zoomFactor)
{
    mainView.zoom( zoomFactor );
    Clamp();
    window.setView(mainView);
}
void View::Pan(sf::Vector2f moveBy)
{
    mainView.move(moveBy);
    Clamp();
    window.setView(mainView);
}
void View::Resize(sf::Vector2f newSize)
{
    mainView.setSize( sf::Vector2f{newSize.x * MAIN_VIEWPORT.width, newSize.y * MAIN_VIEWPORT.height} ); //This is all very shonky. Need to consider what is flexible and what is fixed on screen.
    Clamp();
    window.setView(mainView);
}
void View::CentreOn(VectorWorld point)
{
    mainView.setCenter(point);
}


void View::Clamp()
{
    auto clampPlan = GetViewBasePlan(); //cursorOne.GetPlanPos().GetPlan();
    if (clampPlan)
    {
        RectWorld b = clampPlan->GetWorldPaddedBound();
        auto center = mainView.getCenter();
        auto viewSize = mainView.getSize();
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
        mainView.setCenter(tl + (size/2.f) );
    }
}




//DEL
//    //Create a string to represent the stack of plans...
//    std::string planNumStr;
//    std::shared_ptr<ChipPlan> cont = ap;
//    std::shared_ptr<ChipHandle> ref = nullptr;
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
