/* 
 * File:   View.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 11:56
 */

#ifndef VIEW_HPP
#define	VIEW_HPP

#include <SFML/Graphics.hpp>
#include "ViewResources.hpp"
#include "Model.hpp"
#include "ModelListener.hpp"
#include "Gobject.hpp"
#include <vector>
#include "NeuronView.hpp"
#include "WireView.hpp"
#include "Cursor.hpp"


class View : public ModelListener
{
public:
    View(Model & model_p);
    View(const View&) = delete;
    virtual ~View() {}
    virtual void OnNotify(bool added, Neuron * rp);
    virtual void OnNotify(bool added, const Wire & cr);
    void Draw();

    void Zoom(float zoomFactor);
    void Pan(sf::Vector2f moveBy);

    sf::Vector2f GetCursorOnePos() const { return cursorOne.GetPos(); }
    void SetCursorOnePos(sf::Vector2f pos);
    sf::Vector2f GetCursorTwoPos() const { return cursorTwo.GetPos(); }
    void SetCursorTwoPos(sf::Vector2f pos);
    
    sf::RenderWindow & GetWindow() {return window;}
    sf::View & GetView()           {return view;}
    
private:
    const Model & theModel;
    std::vector<std::unique_ptr<NeuronView> > neuronViews;
    std::vector<std::unique_ptr<WireView> > wireViews;
    
    sf::RenderWindow window;
    sf::View view;
    ViewResources vRes;
    Cursor cursorOne;
    Cursor cursorTwo;
};

#endif	/* VIEW_HPP */

