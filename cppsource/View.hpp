/* 
 * File:   View.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 11:56
 */

#ifndef VIEW_HPP
#define	VIEW_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "ViewResources.hpp"
#include "Model.hpp"
#include "DeviceView.hpp"
#include "WireView.hpp"
#include "Cursor.hpp"


class View
{
public:
    View(Model & model_p);
    View(const View&) = delete;
    ~View() {}
    void DebugInfo();
    
    void ImportDevice(std::shared_ptr<DeviceView> device);
    void ExpelDevices();
    void ImportWire(std::shared_ptr<WireView> wire);
    void ExpelWires();
    
    void Draw();

    void Zoom(float zoomFactor);
    void Pan(sf::Vector2f moveBy);
    void Resize(sf::Vector2f newSize);

    sf::Vector2f GetCursorOnePos() const { return cursorOne.GetPos(); }
    void SetCursorOnePos(sf::Vector2f pos);
    sf::Vector2f GetCursorTwoPos() const { return cursorTwo.GetPos(); }
    void SetCursorTwoPos(sf::Vector2f pos);
    
    sf::RenderWindow & GetWindow() {return window;}
    sf::View & GetMainView()           {return mainView;}
    
private:
    sf::RenderWindow window;
    sf::View mainView;
    sf::View barView;

    const Model & theModel;
    std::vector<std::shared_ptr<DeviceView> > deviceViews;
    std::vector<std::shared_ptr<WireView> > wireViews;

    Cursor cursorOne;
    Cursor cursorTwo;
};

#endif	/* VIEW_HPP */

