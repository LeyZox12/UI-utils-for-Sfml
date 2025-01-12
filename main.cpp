#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>
#include <functional>
#include <UIutils.h>
using namespace std;
using namespace sf;
typedef Vector2f vec2;

Font font;
Vector2i mousepos;
RenderWindow window(VideoMode(512,512), "UILib");
UIutils ui;
Event e;
void test();

void test()
{
    cout << "button pressed" << endl;
}
int main()
{
    if(!font.loadFromFile("res/font.ttf"))
        cout << "error, couldnt load font.ttf";
    ui.font = font;
    ui.addButton(Vector2f(0,0),Vector2f(200,50), test, "testButton");
    ui.addButton(Vector2f(0,50), Vector2f(200,50), test, "testbuttonColor", Color::Red, Color(150,0,0), Color::White);
    ui.addSlider(Vector2f(200,0), Vector2f(200,50), 200, false);
    vector<string> testValues = {"orange","apple","pear","cherry","strawberry"};
    ui.addDropDown(Vector2f(400,0),Vector2f(200,50),testValues, 3);
    ui.addTextZone(Vector2f(600,0), Vector2f(200,50), 20);
    while(window.isOpen())
    {
        while(window.pollEvent(e))
        {
            if(e.type == Event::Closed)
                window.close();
            if (e.type == Event::Resized)
            {
                FloatRect visibleArea(0, 0, e.size.width, e.size.height);
                window.setView(View(visibleArea));
            }
            ui.updateElements(e, window);

        }
        window.clear(Color::Black);
        ui.displayElements(window);
        window.display();
    }
    return 0;
}
