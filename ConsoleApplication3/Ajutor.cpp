#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Structuri.h"
#include "Defineuri.h"
#include <conio.h>
char comenzi[NUMAR_AJUTOR_COMENZI][70] = { "CTRL+O - Deschidere fisier",
"CTRL+S - Salvare fisier",
"E(pentru componenta selectata) - Editare parametri",
"DELETE(pentru componenta selectata) - Sterge Componenta",
"-(pentru componenta selectata sau intreg circuitul) - Micsorare",
"+(pentru componenta selectata sau intreg circuitul) - Marire",
"R(pentru componenta selectata) - Rotire",
"CTRL-Z - Undo",
"CTRL-Y - Redo" };
using namespace sf;
void deschideFereastraAjutor()
{
    Image image;
    image.loadFromFile("icons\\help.png");
    RenderWindow window(sf::VideoMode(800, 400, 32), "Ajutor", sf::Style::Close);
    window.setFramerateLimit(30);
    window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());

    Texture t;
    t.loadFromFile("background.jpg");
    Sprite s(t);
    sf::Vector2f targetSize(800, 400);

    s.setScale( targetSize.x / s.getLocalBounds().width, targetSize.y / s.getLocalBounds().height);
    sf::Font font;
    font.loadFromFile("fonts\\RobotoSlab-Font.ttf");
    Text text;
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(25);
    text.setFont(font);

    while (window.isOpen())
    {
        sf::Event event;
        text.setPosition(OFFSET, OFFSET);
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
            {
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