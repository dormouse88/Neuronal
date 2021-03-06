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


class RelativesViewer
{
public:
    RelativesViewer();
    void SetPlanID(PlanID, std::shared_ptr<const PlansDirectory>);
    void Draw(sf::RenderTarget &rt);

private:
    std::shared_ptr<PlansDirectory> plansDir;
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




enum class ArenaAddressMode { ABSENT, CELL };
struct ArenaAddress
{
    ArenaAddressMode mode;
    ArenaPoint arenaPoint;
    std::weak_ptr<Arena> arena;
};

class ArenaCursor
{
public:
    ArenaCursor(sf::Color);
    void Draw(sf::RenderTarget & rt);
    ArenaAddressMode GetMode() { return addr_.mode; }
    ArenaPoint GetArenaPoint() { assert(addr_.mode == ArenaAddressMode::CELL); return addr_.arenaPoint; }
    void SetArenaPoint(ArenaPoint ap, Shp<Arena> ar) { addr_.mode = ArenaAddressMode::CELL; addr_.arenaPoint = ap; addr_.arena = ar;}
private:
    ArenaAddress addr_;
    sf::RectangleShape shape_;
};

class PuppetCursor
{
public:
    PuppetCursor(sf::Color);
    void Draw(sf::RenderTarget &);
    void UpdateMonitorTarget( Shp<Puppet> p, Shp<Arena> ar ) {brainMonitored_ = p; arena_ = ar;}
    Shp<BaseReferer> GetBrain() {auto b = brainMonitored_.lock(); if (b) return b->GetBrain(); else return nullptr;}
private:
    sf::RectangleShape shape_;
    std::weak_ptr<Puppet> brainMonitored_;
    std::weak_ptr<Arena> arena_;
};



struct UIObjects //eventually refactor this away (textEnterer stays common, cursors move to their respective panes)
{
    UIObjects()
        :cursorOne( sf::Color::Yellow )
        ,cursorTwo( sf::Color::Cyan )
        ,arenaCursor( sf::Color::Green )
        ,puppetCursor(sf::Color::Yellow)
    {}
    BrainCursor cursorOne;
    BrainCursor cursorTwo;
    ArenaCursor arenaCursor;
    PuppetCursor puppetCursor;
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
    void Zoom(float zoomFactor, VectorWorld anchorPoint);
    void Pan(sf::Vector2f moveBy);
    void CentreOn(VectorWorld point);
    void ClampToRect(RectWorld);
protected:
    sf::Vector2f prevMouseWorldPos;
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
    PaneBrain(Model &, UIObjects &);
    virtual void Draw(sf::RenderWindow &) override;
    virtual void Handle(sf::Event &) override;
    virtual void HandleMouse(sf::Event &, sf::Vector2f) override;
    virtual void AutoClamp() override;
    void SetHighlightingMode(int x)             { highlightingMode = x; }
private:
    void HandlePlan(sf::Event &, PlanShp);
    void HandleLocated(sf::Event &, PlanPos);
    void EventsBothWirable(sf::Event &, Shp<WiringPair>);

    Model & model_;
    Shp<BaseReferer> brain;
    UIObjects & uiObjects;
    sf::RectangleShape brainOverlayBox;
    int highlightingMode;
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
    RelativesViewer viewPanel;
    sf::Text nameFilter;
};

class PaneText : public BasePane
{
public:
    PaneText(UIObjects &);
    virtual void Draw(sf::RenderWindow &) override;
    virtual void Handle(sf::Event &) override;
    virtual void HandleMouse(sf::Event &, sf::Vector2f) override;
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
private:
    void HandleInputState(BasePane * pane, sf::Vector2f worldPos);
    void HandleInputEvents(BasePane * pane, sf::Vector2f worldPos);
    void CycleFieldMode();
private:
    void SizingsRefresh();
    Model & theModel;
    sf::RenderWindow window;
    UIObjects uiObjects;
    int fieldMode;
    
    PaneLevel paneLevel;
    PaneBrain paneBrain;
    PaneBar paneBar;
    PaneText paneText;
    
    bool quitYet_; //scrappy
};



#endif	/* VIEW_HPP */

