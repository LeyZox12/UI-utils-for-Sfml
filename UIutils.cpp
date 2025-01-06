#include "UIutils.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>
#include <string>
#include <functional>

using namespace sf;
using namespace std;


UIutils::UIutils()
{

}
static bool UIutils::rectVecCollision(Vector2i vec, RectangleShape rect)
{
    return (rect.getPosition().x < vec.x &&
            rect.getPosition().x + rect.getSize().x > vec.x &&
            rect.getPosition().y < vec.y &&
            rect.getPosition().y + rect.getSize().y > vec.y);
}
static string toString(int val)
{
    stringstream stream;
    stream<<val;
    string strbuffer;
    stream>>strbuffer;
    return strbuffer;
}
void UIutils::setWindow(RenderWindow &window)
{
    this->window = window;
}
void UIutils::addButton(Vector2f pos, Vector2f sizeRect, function<void()> command, string name)
{
    button newButton;
    newButton.pos = pos;
    newButton.buttonSize = sizeRect;
    newButton.command = command;
    newButton.buttonName = name;
    newButton.font = font;
    buttons.push_back(newButton);
}
void UIutils::addDropDown(Vector2f pos, Vector2f sizeRect, vector<string> elements, int maxElementPage)
{
    dropDown newDropDown;
    newDropDown.pos = pos;
    newDropDown.sizeRect = sizeRect;
    newDropDown.font = font;
    for(auto& s : elements)
        newDropDown.addElement(s);
    newDropDown.maxPageElements = maxElementPage;
    dropDowns.push_back(newDropDown);
}
void UIutils::addSlider(Vector2f pos, Vector2f sizeRect, float maxVal, bool hasKnob)
{
    slider newSlider;
    newSlider.pos = pos;
    newSlider.sizeRect = sizeRect;
    newSlider.maxValue = maxVal;
    newSlider.hasknob = hasKnob;
    newSlider.font = font;
    sliders.push_back(newSlider);
}
void UIutils::addTextZone(Vector2f pos, Vector2f sizeRect, int txtSize)
{
    textZone newTextZone;
    newTextZone.pos = pos;
    newTextZone.sizeRect = sizeRect;
    newTextZone.txtSize = txtSize;
    newTextZone.font = font;
}
void UIutils::updateElements(Event e)
{
    mousepos = Mouse::getPosition(window);
    if(e.type == MouseButtonPressed && e.mouseButton.button == Mouse::Left)
    {
        for(button b : buttons)
            b.onClick(mousepos);
        for(dropDown : dropDowns)
            d.getSelected(mousepos, true);
        for(textZone t : textZones)
            t.updateShouldWrite(mousepos);
        for(slider s : sliders)
            s.updateValue(mousepos);
    }
    else if(e.type == Event::TextEntered)
    {
        for(textZone t : textZones)
            if(e.text.unicode < 128)
            {
                t.write(e.text.unicode);
            }
    }
    else if(e.type == Event::MouseWheelMoved)
    {
        for(dropDown d : dropDowns)
            d.scroll(e.mouseWheel.delta);
    }
}
void UIutils::displayElements()
{
    for(button b : buttons)
        b.display(window);
    for(dropDown d : dropDowns)
        d.display(window);
    for(textZone t : textZones)
        t.display(window);
    for(slider s : sliders)
        s.display(window);
}
