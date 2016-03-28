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
#include "TextEnterer.hpp"


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




//class View
//{
//public:
//    View(Model & model_p);
//    View(const View&) = delete;
//    ~View() {}
//    void Draw();
//
////Hmmm...
////    sf::RenderWindow & GetWindow()                          {return window;}
////    std::shared_ptr<Arena> GetArena()                       {return theModel.GetArena(); }
////    PlanShp GetViewBasePlan()             {return theModel.GetMouseBrain()->GetSubPlan(); }
//
//};


struct UIObjects
{
    UIObjects()
        :cursorOne( nullptr, sf::Color::Yellow )
        ,cursorTwo( nullptr, sf::Color::Cyan )
    {}
    Cursor cursorOne;
    Cursor cursorTwo;
    std::shared_ptr<TextEnterer> textEnterer_;
};


class BasePane
{
public:
    virtual void Draw(sf::RenderWindow &) = 0;
    virtual void Handle(sf::Event &) = 0;
    virtual void HandleMouse(sf::Event &, sf::Vector2f) = 0;
protected:
public:
    sf::View view;
};

class BaseAreaPane : public BasePane
{
public:
    virtual void AutoClamp() = 0;
    void Zoom(float zoomFactor);
    void Pan(sf::Vector2f moveBy);
    void CentreOn(VectorWorld point);
    void ClampToRect(RectWorld);
};



class PaneLevel : public BaseAreaPane
{
public:
    PaneLevel(Shp<Arena>, UIObjects &);
    virtual void Draw(sf::RenderWindow &) override;
    virtual void Handle(sf::Event &) override;
    virtual void HandleMouse(sf::Event &, sf::Vector2f) override;
    virtual void AutoClamp() override;
private:
    Shp<Arena> arena;
    UIObjects & uiObjects;
};

class PaneBrain : public BaseAreaPane
{
public:
    PaneBrain(Shp<BaseReferer>, UIObjects &);
    virtual void Draw(sf::RenderWindow &) override;
    virtual void Handle(sf::Event &) override;
    virtual void HandleMouse(sf::Event &, sf::Vector2f) override;
    virtual void AutoClamp() override;
    void SetHighlightingMode(int x)             { highlightingMode = x; }
private:
    Shp<BaseReferer> brain;
    UIObjects & uiObjects;
    sf::RectangleShape brainOverlayBox;
    int highlightingMode;
//    Cursor cursorOne;
//    Cursor cursorTwo;
};

class PaneBar : public BasePane
{
public:
    PaneBar(Model &, UIObjects &);
    virtual void Draw(sf::RenderWindow &) override;
    virtual void Handle(sf::Event &) override;
    virtual void HandleMouse(sf::Event &, sf::Vector2f) override;
    void PostMessage(std::string message)       { marquee.PostMessage(message);}
private:
    Model & theModel;
    UIObjects & uiObjects;
    Marquee marquee;
    ViewPanel viewPanel;
    sf::Text nameFilter;
};

class PaneText : public BasePane
{
public:
    PaneText(UIObjects &);
    virtual void Draw(sf::RenderWindow &) override;
    virtual void Handle(sf::Event &) override;
    virtual void HandleMouse(sf::Event &, sf::Vector2f) override;
    void SetTextEntering(bool on, std::string text = "")     { textEntering.setString("-->" + text + "<--"); drawTextEntering = on; }
private:
    UIObjects & uiObjects;
    sf::Text textEntering;
    bool drawTextEntering;
};



class PaneGroup
{
public:
    PaneGroup(Model & model_p);
    void Draw();
    bool HandleInput();
    void HandleInputEvents();
    void ToggleFieldMode()                      { fieldMode = not fieldMode; }
private:
    void SizingsRefresh();
    Model & theModel;
    UIObjects uiObjects;
    sf::RenderWindow window;
    bool fieldMode;
    
    PaneLevel paneLevel;
    PaneBrain paneBrain;
    PaneBar paneBar;
    PaneText paneText;
    
    bool quitYet_; //scrappy
};



#endif	/* VIEW_HPP */

