#ifndef UIUTILS_H
#define UIUTILS_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>
#include <string>
#include <functional>

using namespace sf;
using namespace std;

class UIutils
{
    public:
        Font font;
        UIutils();
        struct textZone
        {
            RectangleShape textZoneRect;
            Text text;
            Font font;
            int txtSize;
            int lineCharCount;
            Vector2f pos;
            Vector2f sizeRect;
            Color rectColor;
            Color textColor;
            string textStr;
            bool isWriting = false;
            textZone()
            {
                rectColor = Color(255, 255, 255);
                textColor = Color(150, 150, 150);
                textStr = "";
                txtSize = 20;
                lineCharCount = 0;
            }
            bool rectVecCollision(Vector2i vec, RectangleShape rect)
            {
                return (rect.getPosition().x < vec.x &&
                        rect.getPosition().x + rect.getSize().x > vec.x &&
                        rect.getPosition().y < vec.y &&
                        rect.getPosition().y + rect.getSize().y > vec.y);
            }
            string toString(int val)
            {
                stringstream stream;
                stream<<val;
                string strbuffer;
                stream>>strbuffer;
                return strbuffer;
            }
            void updateShouldWrite(Vector2i mousepos)
            {
                if(rectVecCollision(mousepos, textZoneRect))
                    isWriting = true;
                else isWriting = false;
            }
            void write(Uint32 unicode)
            {
                if(isWriting)
                {
                    switch(unicode)
                    {
                        case(8):
                            if(textStr.size() > 0)
                                textStr.erase(textStr.size()-1);
                            break;
                        case(13):
                            textStr.append("\n");
                            break;
                        default:
                            textStr += static_cast<char>(unicode);
                            break;
                    }
                }
            }
            void display(RenderWindow& window)
            {
                textZoneRect.setFillColor(rectColor);
                textZoneRect.setPosition(pos);
                textZoneRect.setSize(sizeRect);
                window.draw(textZoneRect);
                text.setFont(font);
                text.setFillColor(textColor);
                text.setPosition(pos);
                text.setCharacterSize(txtSize);
                text.setString(textStr);
                if(isWriting)
                    text.setString(text.getString() + "|");
                else if(textStr == "")
                    text.setString("Type somthething...");
                window.draw(textZoneRect);
                window.draw(text);
            }
        };
        struct button
        {
            function<void()> command;
            RectangleShape buttonRect;
            Vector2f buttonSize;
            Vector2f pos;
            Font font;
            Color buttonColor;
            Color selectedColor;
            Color textColor;
            Text buttonName;
            string buttonNameStr;
            button()
            {
                buttonColor = Color(50, 50, 50);
                selectedColor = Color(150, 150, 150);
                textColor = Color(255, 255, 255);
            }
            bool rectVecCollision(Vector2i vec, RectangleShape rect)
            {
                return (rect.getPosition().x < vec.x &&
                        rect.getPosition().x + rect.getSize().x > vec.x &&
                        rect.getPosition().y < vec.y &&
                        rect.getPosition().y + rect.getSize().y > vec.y);
            }
            string toString(int val)
            {
                stringstream stream;
                stream<<val;
                string strbuffer;
                stream>>strbuffer;
                return strbuffer;
            }
            void onClick(Vector2i mousepos)
            {
                if(rectVecCollision(mousepos, buttonRect))
                    command();
            }
            void display(RenderWindow& window)
            {
                Vector2i mousepos;
                mousepos = Mouse::getPosition(window);
                buttonRect.setSize(buttonSize);
                if(rectVecCollision(mousepos, buttonRect))
                    buttonRect.setFillColor(selectedColor);
                else
                    buttonRect.setFillColor(buttonColor);
                buttonRect.setPosition(pos);
                window.draw(buttonRect);
                buttonName.setString(buttonNameStr);
                buttonName.setPosition(pos);
                buttonName.setFont(font);
                buttonName.setFillColor(textColor);
                window.draw(buttonName);
            }
        };
        struct slider
        {
            float percent;
            float maxValue;
            float value;
            float ratioVal;
            Vector2f sizeRect;
            Vector2f pos;
            bool hasknob;
            bool hasText;
            bool isValueFixed;
            Font font;
            CircleShape knob;
            Color backgroundColor;
            Color fillColor;
            Color knobColor;
            Color textColor;
            Text valTxt;
            RectangleShape backgroundRect;
            RectangleShape fillRect;
            slider()
            {
                // default parameters
                backgroundColor = Color(50,50,50);
                knobColor = Color(255,255,255);
                fillColor = Color(100,100,100);
                textColor = knobColor;
                value = 0;
                hasText = true;
            }
            bool rectVecCollision(Vector2i vec, RectangleShape rect)
            {
                return (rect.getPosition().x < vec.x &&
                        rect.getPosition().x + rect.getSize().x > vec.x &&
                        rect.getPosition().y < vec.y &&
                        rect.getPosition().y + rect.getSize().y > vec.y);
            }
            string toString(int val)
            {
                stringstream stream;
                stream<<val;
                string strbuffer;
                stream>>strbuffer;
                return strbuffer;
            }
            void updateValue(Vector2i mousepos)
            {
                if(rectVecCollision(mousepos, backgroundRect) && !isValueFixed)
                {
                    ratioVal = sizeRect.x / maxValue;
                    float diffX = mousepos.x - backgroundRect.getPosition().x;
                    value = diffX / ratioVal;
                }
            }
            void display(RenderWindow& window)
            {
                ratioVal = sizeRect.x / maxValue;
                backgroundRect.setFillColor(backgroundColor);
                backgroundRect.setSize(sizeRect);
                fillRect.setFillColor(fillColor);
                knob.setFillColor(knobColor);
                valTxt.setFillColor(textColor);
                backgroundRect.setPosition(pos);
                fillRect.setPosition(pos);
                fillRect.setSize(Vector2f(ratioVal * value, sizeRect.y));
                window.draw(backgroundRect);
                window.draw(fillRect);
                if(hasText)
                {
                    valTxt.setFillColor(textColor);
                    Vector2f position = backgroundRect.getPosition() +  Vector2f(sizeRect.x / 2, 0);
                    valTxt.setPosition(position);
                    valTxt.setFont(font);
                    valTxt.setString(toString(value * ratioVal));
                    window.draw(valTxt);
                }
            }
        };
        struct dropDown
        {
            vector<RectangleShape> elementsRects;
            vector<string> elementsNames;
            string value;
            RectangleShape slider;
            int maxPageElements = 0;
            int offset = 0;
            Vector2f sizeRect;
            Vector2f pos;
            RectangleShape dropRect;
            bool isDown = false;
            bool isSinglePage = true;
            int elementsAmount = 0;
            int currentSelected = -1;
            Font font;
            Color rectColor;
            Color textColor;
            Color elementsColor;
            Color selectedColor;
            dropDown()
            {
                rectColor = Color(50,50,50);
                textColor = Color::Black;
                elementsColor = Color(255,255,255);
                selectedColor = Color(200,200,200);
            }
            bool rectVecCollision(Vector2i vec, RectangleShape rect)
            {
                return (rect.getPosition().x < vec.x &&
                        rect.getPosition().x + rect.getSize().x > vec.x &&
                        rect.getPosition().y < vec.y &&
                        rect.getPosition().y + rect.getSize().y > vec.y);
            }
            string toString(int val)
            {
                stringstream stream;
                stream<<val;
                string strbuffer;
                stream>>strbuffer;
                return strbuffer;
            }
            void addElement(string name)
            {
                RectangleShape defaultRect;
                elementsNames.push_back(name);
                defaultRect.setSize(sizeRect);
                elementsRects.push_back(defaultRect);
                elementsAmount++;
                if(elementsAmount > maxPageElements)
                    isSinglePage = false;
            }
            void scroll(int delta, Vector2i mousepos)
            {
                if(getSelected(mousepos, false) == -1||
                        isSinglePage)
                    return;
                offset -= delta;
                if(offset < 0)
                    offset = 0;
                else if(offset > elementsAmount-maxPageElements)
                    offset = elementsAmount-maxPageElements;
            }
            void display(RenderWindow& window)
            {
                Text name;
                name.setFont(font);
                for(int i = offset; i < maxPageElements+offset; i++)
                {
                    if(i < elementsAmount && isDown)
                    {
                        elementsRects[i].setPosition(pos + Vector2f(0, (i-offset) * sizeRect.y + 5 + sizeRect.y));
                        if(getSelected(Mouse::getPosition(window), false) == i)
                            elementsRects[i].setFillColor(selectedColor);
                        else
                            elementsRects[i].setFillColor(elementsColor);
                        window.draw(elementsRects[i]);
                        name.setFillColor(Color::Black);
                        name.setString(elementsNames[i]);
                        name.setPosition(elementsRects[i].getPosition());
                        window.draw(name);
                    }
                }
                dropRect.setFillColor(rectColor);
                dropRect.setPosition(pos);
                window.draw(dropRect);
                name.setCharacterSize(name.getCharacterSize());
                name.setPosition(pos);
                name.setFillColor(Color::White);
                name.setString(value);
                window.draw(name);
            }
            int getSelected(Vector2i mousepos, bool shouldSetVal)
            {
                if(rectVecCollision(mousepos, dropRect) && shouldSetVal)
                {
                    isDown = !isDown;
                }
                for(int i = offset; i < maxPageElements+offset; i++)
                {
                    elementsRects[i].setPosition(pos + Vector2f(0, (i-offset) * sizeRect.y + 5 + sizeRect.y));
                    if(i < elementsAmount &&
                            rectVecCollision(mousepos, elementsRects[i]) &&
                            isDown)
                    {
                        if(shouldSetVal)
                        {
                            value = elementsNames[i];
                            isDown = false;
                            currentSelected = i;
                        }
                        return i;
                    }
                }
                if(shouldSetVal)
                    currentSelected = -1;
                return -1;
            }
        };
        vector<button> buttons;
        vector<dropDown> dropDowns;
        vector<slider> sliders;
        vector<textZone> textZones;
        void addButton(Vector2f pos, Vector2f sizeRect, function<void()> command, string name);
        void addDropDown(Vector2f pos, Vector2f sizeRect, vector<string> elements, int maxElementPage);
        void addSlider(Vector2f pos, Vector2f sizeRect, float maxVal, bool hasKnob, bool isValueFixed);
        void addTextZone(Vector2f pos, Vector2f sizeRect, int txtSize);
        //Overloaded for custom colors
        void addButton(Vector2f pos, Vector2f sizeRect, function<void()> command, string name, Color buttonColor, Color selectedColor, Color textColor);
        void addDropDown(Vector2f pos, Vector2f sizeRect, vector<string> elements, int maxElementPage, Color baseColor, Color elementColor, Color textColor, Color selectedColor);
        void addSlider(Vector2f pos, Vector2f sizeRect, float maxVal, bool hasKnob, bool isValueFixed, Color backgroundColor, Color fillColor, Color textColor);
        void addTextZone(Vector2f pos, Vector2f sizeRect, int txtSize, Color rectColor, Color textColor);

        void updateElements(Event e, RenderWindow &window);
        void displayElements(RenderWindow &window);
    protected:

    private:
};

#endif // UIUTILS_H
