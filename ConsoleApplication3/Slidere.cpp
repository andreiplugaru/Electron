#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Slidere.h"
#define varfCoefScalare 640
#define bazaCoefScalare 840
#define varfUnghiRotire 310
#define bazaUnghiRotire 510
#define axaOxSlidere 1850
#define spatiereTextBox 
#define spatiereValoareButoane
using namespace sf;

void drawCoefScalareBara(RenderWindow& window)
{
    float mijlocSlider = varfCoefScalare + (bazaCoefScalare - varfCoefScalare) / 2;
    VertexArray linie;
    linie.setPrimitiveType(Lines);
    Vertex punct1;
    Vertex punct2;
    punct1.position.x = axaOxSlidere;
    punct1.position.y = varfCoefScalare;
    punct2.position.x = axaOxSlidere;
    punct2.position.y = bazaCoefScalare;
    linie.append(punct1);
    linie.append(punct2);

    sf::Font font;
    font.loadFromFile("fonts\\RobotoSlab-Font.ttf");
    sf::Text text;
    text.setString("Marime");
    text.setFont(font);
    text.setCharacterSize(25);
    text.setPosition(axaOxSlidere - 49.5, varfCoefScalare - 70);
    sf::Text multipliermic;
    multipliermic.setString("x0.5");
    multipliermic.setFont(font);
    multipliermic.setCharacterSize(20);
    multipliermic.setPosition(axaOxSlidere - 24.5, varfCoefScalare - 30);
    sf::Text multipliermare;
    multipliermare.setString("x2.0");
    multipliermare.setFont(font);
    multipliermare.setCharacterSize(20);
    multipliermare.setPosition(axaOxSlidere - 24.5, bazaCoefScalare + 10);
    window.draw(linie);
    window.draw(text);
    window.draw(multipliermic);
    window.draw(multipliermare);

}
void drawUnghiRotireBara(RenderWindow& window)
{
    float mijlocSlider = varfUnghiRotire + (bazaUnghiRotire - varfUnghiRotire) / 2;
    VertexArray linie;
    linie.setPrimitiveType(Lines);
    Vertex punct1;
    Vertex punct2;
    punct1.position.x = axaOxSlidere;
    punct1.position.y = varfUnghiRotire;
    punct2.position.x = axaOxSlidere;
    punct2.position.y = bazaUnghiRotire;
    linie.append(punct1);
    linie.append(punct2);

    sf::Font font;
    font.loadFromFile("fonts\\RobotoSlab-Font.ttf");
    sf::Text text;
    text.setString("Rotatie");
    text.setFont(font);
    text.setCharacterSize(25);
    text.setPosition(axaOxSlidere - 44.5, varfUnghiRotire - 70);
    sf::Text unghi0;
    unghi0.setString("0");
    unghi0.setFont(font);
    unghi0.setCharacterSize(20);
    unghi0.setPosition(axaOxSlidere - 7.5, varfUnghiRotire - 30);
    sf::Text unghi360;
    unghi360.setString("360");
    unghi360.setFont(font);
    unghi360.setCharacterSize(20);
    unghi360.setPosition(axaOxSlidere - 20, bazaUnghiRotire + 5);

    window.draw(linie);
    window.draw(text);
    window.draw(unghi0);
    window.draw(unghi360);
}
float valoareUnghiRotire(Vector2i coordMouse)
{
    float valoareUnghiRotire = 0;
    valoareUnghiRotire = (coordMouse.y - varfUnghiRotire) * 1.8;
    return valoareUnghiRotire;
}
float valoareCoefScalare(Vector2i coordMouse)
{
    float coefScalare = 0;
    coefScalare = (0.3 * coordMouse.y) / 40 - 4.3;
    return coefScalare;
}
float pozitieCercCoefScalare(float coefScalare)
{
    if (coefScalare < 0.5)
    {
        coefScalare = 0.5;
    }
    else if (coefScalare > 2.0)
    {
        coefScalare = 2;
    }
    float pozitieY;
    pozitieY = (40 * (coefScalare + 4.3)) / 0.3;
    return pozitieY;
}
float  pozitieCercUnghiRotire(float unghiRotire)
{
    float pozitieY;
    pozitieY = unghiRotire / 1.8 + varfUnghiRotire;
    return pozitieY;
}
void checkSizeCoef(Vector2f& sizecoef)
{
    if (sizecoef.x <= 0.5)
    {
        sizecoef.x = 0.5;
    }
    if (sizecoef.y <= 0.5)
    {
        sizecoef.y = 0.5;
    }
}
void drawSlider(RenderWindow& window, float pozitieCerc)
{
    CircleShape slider;
    slider.setRadius(10);
    slider.setOrigin(slider.getRadius(), slider.getRadius());
    slider.setPosition(axaOxSlidere - 0.5, pozitieCerc);
    slider.setFillColor(sf::Color::White);

    window.draw(slider);
}
