#define _CRT_SECURE_NO_WARNINGS
#include "Structuri.h"
#include "Defineuri.h"
#include <conio.h>
char comenzi[NUMAR_AJUTOR_COMENZI][55] = { "CTRL+O - Deschidere fisier",
"CTRL+O - Salvare fisier",
"E(pentru componenta selectata) - Editare parametri",
"-(pentru componenta selectata) - Micsorare",
"+(pentru componenta selectata) - Marire",
"R(pentru componenta selectata) - Rotire" };
using namespace sf;
void deschideFereastraAjutor()
{
    RenderWindow window(sf::VideoMode(740, 400, 32), "Ajutor", sf::Style::Close);
    window.setFramerateLimit(30);
 //   ErrWindow.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
   
    Texture t;
    t.loadFromFile("background.jpg");
    Sprite s(t);
    sf::Font font;
    font.loadFromFile("fonts\\RobotoSlab-Font.ttf");
    Text text;
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(25);
    text.setFont(font);
 
    while (window.isOpen()) {
        sf::Event event;
        text.setPosition(OFFSET, OFFSET);
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

        }
        window.clear(sf::Color::White);
        window.draw(s);
        for (int i = 0; i < NUMAR_AJUTOR_COMENZI; i++)
        {
            text.setString(comenzi[i]);
            window.draw(text);
            text.move(0, text.getLocalBounds().height + OFFSET);
        }
        
    
        window.display();
    }
}