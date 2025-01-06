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
/*
0=>Button
1=>Slider
2=>Checkbox
3=>Text
4=>Drop⏬
5=>Color picker
*/


void test()
{
    cout << "button pressed" << endl;
}
int main()
{
    ui.setWindow(window);
    if(!font.loadFromFile("res/font.ttf"))
        cout << "error, couldnt load font.ttf";


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

        }
        window.clear(Color::Black);

        window.display();
    }
    return 0;
}
