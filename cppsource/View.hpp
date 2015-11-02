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
    
    void PopPlan()                                  {if (activePlan.size() > 1) activePlan.pop();}
    void PushPlan(std::weak_ptr<ChipPlan> plan)     {activePlan.push(plan);}

    sf::RenderWindow & GetWindow()          {return window;}
    sf::View & GetMainView()                {return mainView;}
    std::weak_ptr<ChipPlan> GetActivePlan() {return activePlan.top();}
    
private:
    sf::RenderWindow window;
    sf::View mainView;
    sf::View barView;

    const Model & theModel;
    std::stack< std::weak_ptr<ChipPlan> > activePlan;

public:
    Cursor cursorOne;
    Cursor cursorTwo;
};

#endif	/* VIEW_HPP */

