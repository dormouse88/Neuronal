/* 
 * File:   View.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 11:56
 */

#ifndef VIEW_HPP
#define	VIEW_HPP

#include <memory>
#include <stack>
#include <SFML/Graphics.hpp>
#include "ViewResources.hpp"
#include "Model.hpp"
#include "Cursor.hpp"


class View
{
public:
    View(Model & model_p);
    View(const View&) = delete;
    ~View() {}
    void DebugInfo();
    
    void Draw();

    void Zoom(float zoomFactor);
    void Pan(sf::Vector2f moveBy);
    void Resize(sf::Vector2f newSize);
    void SetHighlightingMode(int x) {highlightingMode = x;}
    
    void PopPlan()                                  {if (activePlan.size() > 1) activePlan.pop();}
    void PushPlan(std::weak_ptr<ChipPlan> plan)     {activePlan.push(plan);}

    sf::RenderWindow & GetWindow()          {return window;}
    sf::View & GetMainView()                {return mainView;}
    std::weak_ptr<ChipPlan> GetActivePlan() {return activePlan.top();}
    void SetActivePlan(std::weak_ptr<ChipPlan> p) {activePlan.top() = p;}
    
private:
    sf::RenderWindow window;
    sf::View mainView;
    sf::View mainOverlay;
    sf::View barOverlay;

    sf::RectangleShape overlayBox;
    sf::Text planNumText;

    const Model & theModel;
    std::stack< std::weak_ptr<ChipPlan> > activePlan;

    int highlightingMode;
public:
    Cursor cursorOne;
    Cursor cursorTwo;
    std::weak_ptr<Device> device1;
    std::weak_ptr<Device> device2;
    std::weak_ptr<Wire> wire1;
    int xmlPlan;
};

#endif	/* VIEW_HPP */

