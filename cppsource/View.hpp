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


class View
{
public:
    View(Model & model_p);
    View(const View&) = delete;
    ~View() {}
    
    void Draw();

    void Zoom(float zoomFactor);
    void Pan(sf::Vector2f moveBy);
    void Resize(sf::Vector2f newSize);
    void CentreOn(VectorWorld point);
    void Clamp();
    void SetHighlightingMode(int x)             { highlightingMode = x; }
    
    sf::RenderWindow & GetWindow()                        {return window;}
    std::shared_ptr<ChipPlan> GetViewBasePlan()           {return theModel.GetBasePlan(); }//{return viewBasePlan.lock();}
    
private:
    sf::RenderWindow window;
    sf::View mainView;
    sf::View mainOverlay;
    sf::View barOverlay;

    sf::RectangleShape mainOverlayBox;
    sf::Text planNumText;

    const Model & theModel;
    //std::weak_ptr<ChipPlan> viewBasePlan;

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

