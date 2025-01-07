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
void UIutils::addButton(Vector2f pos, Vector2f sizeRect, function<void()> command, string name)
{
    button newButton;
    newButton.pos = pos;
    newButton.buttonSize = sizeRect;
    newButton.buttonRect.setPosition(pos);
    newButton.buttonRect.setSize(sizeRect);
    newButton.command = command;
    newButton.buttonNameStr = name;
    newButton.font = font;
    buttons.push_back(newButton);
}
void UIutils::addDropDown(Vector2f pos, Vector2f sizeRect, vector<string> elements, int maxElementPage)
{
    dropDown newDropDown;
    newDropDown.pos = pos;
    newDropDown.sizeRect = sizeRect;
    newDropDown.dropRect.setPosition(pos);
    newDropDown.dropRect.setSize(sizeRect);
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
    newSlider.backgroundRect.setPosition(pos);
    newSlider.backgroundRect.setSize(sizeRect);
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
    newTextZone.textZoneRect.setPosition(pos);
    newTextZone.textZoneRect.setSize(sizeRect);
    newTextZone.txtSize = txtSize;
    newTextZone.font = font;
}
void UIutils::updateElements(Event e, RenderWindow &window)
{

    if(e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
    {
        Vector2i mousepos = Mouse::getPosition(window);
        for(int i = 0; i < buttons.size(); i++)
            buttons[i].onClick(mousepos);
        for(int i = 0; i < dropDowns.size(); i++)
            cout << dropDowns[i].getSelected(mousepos, true) << endl;
        for(int i = 0; i < textZones.size(); i++)
            textZones[i].updateShouldWrite(mousepos);
        for(int i = 0; i < sliders.size(); i++)
            sliders[i].updateValue(mousepos);
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
        Vector2i mousepos = Mouse::getPosition(window);
        for(int i = 0; i < dropDowns.size(); i++)
            dropDowns[i].scroll(e.mouseWheel.delta, mousepos);
    }
}
void UIutils::displayElements(RenderWindow &window)
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
