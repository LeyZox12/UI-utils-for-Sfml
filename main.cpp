#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>
#include <functional>
using namespace std;
using namespace sf;
typedef Vector2f vec2;
std::string toString(int n);
Font font;
Vector2i mousepos;
RenderWindow window(VideoMode(512,512), "UILib");
Event e;
/*
0=>Button
1=>Slider
2=>Checkbox
3=>Text
4=>Drop⏬
5=>Color picker
*/
bool rectVecCollision(Vector2i vec, RectangleShape rect)
{
    return (rect.getPosition().x < vec.x &&
            rect.getPosition().x + rect.getSize().x > vec.x &&
            rect.getPosition().y < vec.y &&
            rect.getPosition().y + rect.getSize().y > vec.y);
}
struct button
{
    function<void()> command;
    RectangleShape button;
    Vector2f buttonSize;
    Vector2f pos;
    Font font;
    Color buttonColor;
    Color selectedColor;
    Color textColor;
    Text buttonName;
    string buttonNameStr;
    void onClick(Vector2i mousepos)
    {
        if(rectVecCollision(mousepos, button))
            command();
    }
    void display(RenderWindow& window)
    {
        Vector2i mousepos;
        mousepos Mouse::getPosition(window);
        button.setSize(buttonSize);
        if(rectVecCollision(mousepos, button))
            button.setFillColor(selectedColor);
        else
            button.setFillColor(buttonColor);
        button.setPosition(pos);
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
        hasText = true;
    }
    void updateValue(Vector2i mousepos)
    {
        if(rectVecCollision(mousepos, backgroundRect))
        {
            ratioVal = sizeRect.x / maxValue;
            float diffX = mousepos.x - backgroundRect.getPosition().x;
            value = diffX / ratioVal;
            cout << "changedValue to" << value << "\n";
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
    Font font;
    Color rectColor = Color(50,50,50);
    Color textColor = Color::Black;
    Color elementsColor = Color(255,255,255);
    Color selectedColor = Color(200,200,200);
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
    void scroll(int delta)
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
        dropRect.setSize(Vector2f(sizeRect.x, sizeRect.y));
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
            return -1;
        }
        for(int i = offset; i < maxPageElements+offset; i++)
        {
            if(i < elementsAmount &&
                    rectVecCollision(mousepos, elementsRects[i]) &&
                    isDown)
            {
                if(shouldSetVal)
                {
                    value = elementsNames[i];
                    isDown = false;
                }
                return i;
            }
        }
        return -1;
    }
};
void yap()
{
    cout << "button pressed" << endl;
}
int main()
{
    if(!font.loadFromFile("res/font.ttf"))
        cout << "error, couldnt load font.ttf";
    dropDown dd1;
    dd1.font = font;
    dd1.sizeRect = Vector2f(200,50);
    dd1.maxPageElements = 5;
    dd1.pos = Vector2f(0, 0);
    for(int i = 0; i<6; i++)
    {
        dd1.addElement(toString(i));
    }
    button b1;
    b1.pos = 400;

    slider s1;
    s1.pos = Vector2f(200,0);
    s1.sizeRect = Vector2f(200,50);
    s1.font = font;
    s1.hasknob = false;
    s1.maxValue = 200;
    while(window.isOpen())
    {
        mousepos = Mouse::getPosition(window);
        while(window.pollEvent(e))
        {
            if(e.type == Event::Closed)
                window.close();
            if(e.type == Event::MouseButtonPressed)
            {
                dd1.getSelected(mousepos, true);
                s1.updateValue(mousepos);

            }
            if(e.type == Event::MouseWheelMoved)
            {
                dd1.scroll(e.mouseWheel.delta);
            }
        }
        window.clear(Color::Black);
        dd1.display(window);
        s1.display(window);
        window.display();
    }
    return 0;
}
std::string toString(int n )
{
    std::stringstream stream;
    stream<<n;
    std::string strbuffer;
    stream>>strbuffer;
    return strbuffer;

}
