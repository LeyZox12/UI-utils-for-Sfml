#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>

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
struct dropDown
{
    vector<RectangleShape> elementsRects;
    vector<string> elementsNames;
    string value;
    RectangleShape slider;
    int maxPageElements = 0;
    int offset = 0;
    Vector2f sizeR;
    RectangleShape dropRect;
    bool isDown = false;
    bool isSinglePage = true;
    int elementsAmount = 0;
    void addElement(string name)
    {
        RectangleShape defaultRect;
        elementsNames.push_back(name);
        defaultRect.setSize(sizeR);
        elementsRects.push_back(defaultRect);
        elementsAmount++;
        if(elementsAmount > maxPageElements)
            isSinglePage = false;
    }
    void scroll(int delta)
    {
        cout << isSinglePage << endl;
        if(isSinglePage)
           return;
        offset -= delta;
        if(offset < 0)
            offset = 0;
        else if(offset > elementsAmount-maxPageElements)
            offset = elementsAmount-maxPageElements;

    }
    void display(RenderWindow& window, Vector2f pos)
    {
        Text name;
        name.setFont(font);
        if(isDown)
            for(int i = offset; i < maxPageElements+offset; i++)
            {
                if(i < elementsAmount)
                {
                    elementsRects[i].setPosition(pos + Vector2f(0, (i-offset) * sizeR.y + 5 + sizeR.y));
                    window.draw(elementsRects[i]);
                    name.setFillColor(Color::Black);
                    name.setString(elementsNames[i]);
                    name.setPosition(elementsRects[i].getPosition());
                    window.draw(name);
                }
            }
        dropRect.setSize(Vector2f(sizeR.x, sizeR.y));
        dropRect.setFillColor(Color(50,50,50));
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
        if(isDown)
        {
            for(int i = offset; i < maxPageElements+offset; i++)
            {
                if(i < elementsAmount && rectVecCollision(mousepos, elementsRects[i]))
                {
                    if(shouldSetVal)
                    {
                        value = elementsNames[i];
                        isDown = false;
                    }
                    return i;
                }
            }
        }
        return -1;
    }
};

int main()
{
    if(!font.loadFromFile("res/font.ttf"))
        cout << "error, couldnt load font.ttf";
    dropDown dd1;
    dd1.sizeR = Vector2f(200,50);
    dd1.maxPageElements = 5;
    for(int i = 0; i<6; i++)
    {
        dd1.addElement(toString(i));
    }
    dropDown dd2;
    dd2.sizeR = Vector2f(200,50);
    dd2.maxPageElements = 5;
    for(int i = 0; i<6; i++)
    {
        dd2.addElement(toString(6-i));
    }


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
                dd2.getSelected(mousepos, true);
            }
            if(e.type == Event::MouseWheelMoved && dd1.getSelected(mousepos, false) != -1)
            {
                dd1.scroll(e.mouseWheel.delta);
                dd2.scroll(e.mouseWheel.delta);
            }
        }
        window.clear(Color::Black);
        dd1.display(window, Vector2f(0, 0));
        dd2.display(window, Vector2f(200, 0));
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
