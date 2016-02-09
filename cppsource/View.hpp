/* 
 * File:   View.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 11:56
 */

#ifndef VIEW_HPP
#define	VIEW_HPP

#include <memory>
#include <SFML/Graphics.hpp>
#include "ViewResources.hpp"
#include "Model.hpp"
#include "Cursor.hpp"


class ViewPanel
{
public:
    ViewPanel();
    void SetPlanID(PlanID, std::shared_ptr<const PlanGroupData>);
    void Draw(sf::RenderTarget &rt);

private:
    std::shared_ptr<PlanGroupData> planGroupData;
    PlanID pid_;
    
    sf::RectangleShape mainOutlineBox;
    sf::Text parentPlanText;
    sf::Text mainPlanText;
    sf::Text kidsPlanText;
};


#include <string>
#include <deque>
class Marquee
{
public:
    Marquee();
    void PostMessage(std::string s);
    void Draw(sf::RenderTarget &rt);
private:
    std::deque<std::string> messageDeque;
    std::vector<sf::Text> texts;
};




class View
{
public:
    View(Model & model_p);
    View(const View&) = delete;
    ~View() {}
    
    void Draw();
private:
    void DrawMain();
    void DrawBar();
public:    

    void Zoom(float zoomFactor);
    void Pan(sf::Vector2f moveBy);
    void Resize(sf::Vector2f newSize);
    void CentreOn(VectorWorld point);
    void Clamp();
    void SetHighlightingMode(int x)             { highlightingMode = x; }
    
    ViewPanel viewPanel;

    void PostMessage(std::string message)       { marquee.PostMessage(message);}
    Marquee marquee;
    
    void SetTextEntering(bool on, std::string text = "")     { barText1.setString(text); drawTextEntering = on; }
    bool drawTextEntering;

    sf::RenderWindow & GetWindow()                          {return window;}
    std::shared_ptr<Arena> GetArena()                       {return theModel.GetArena(); }
    PlanShp GetViewBasePlan()             {return theModel.GetMouseBrain()->GetSubPlan(); }
    
private:
    sf::RenderWindow window;
    sf::View mainView;
    sf::View mainOverlay;
    sf::View barOverlay;

    sf::RectangleShape mainOverlayBox;
    sf::Text barText1;

    Model & theModel;

    int highlightingMode;
public:
    Cursor cursorOne;
    Cursor cursorTwo;
};

#endif	/* VIEW_HPP */

