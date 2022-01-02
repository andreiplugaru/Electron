#include <SFML/Graphics.hpp>
#include "Defineuri.h"
#include "Structuri.h"
#include <string> 
using namespace sf;
char optiuniMeniu[NUMAR_OPTIUNI_MENIU][15] = { "MENIU","SALVARE" ,"SALVARE CA","DESCHIDERE","RESETARE","AJUTOR","Setari" };
float latimeCasutaOptiuni;
void desenareMeniuOptiuni(RenderWindow& window, Font font)
{
    float offsetX = 10;
    latimeCasutaOptiuni = (1920 - (NUMAR_OPTIUNI_MENIU + 1) * 10) / NUMAR_OPTIUNI_MENIU;
    Vector2f dimCasuta;
    dimCasuta.x = latimeCasutaOptiuni;
    dimCasuta.y = INALTIME_MENIU_OPTIUNI - 20;

  /*  Text text;
    text.setFont(font);
    text.setString(std::to_string(latimeCasuta2));
    text.setCharacterSize(25);
    text.setStyle(sf::Text::Bold);
    text.setPosition(300,400);
    text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
    window.draw(text);*/
    Text text;
   
    for (int i = 0; i < NUMAR_OPTIUNI_MENIU; i++)
    {
        RectangleShape casuta;
        sf::Color albastru(0, 80, 190);
        casuta.setPosition(latimeCasutaOptiuni * i + 10 * (i + 1), 10);
        casuta.setSize(dimCasuta);
        casuta.setFillColor(albastru);
        window.draw(casuta);

      
        text.setFont(font); 
        text.setString(optiuniMeniu[i]);
        text.setCharacterSize(25); 
        text.setStyle(sf::Text::Bold);
        text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
        text.setPosition(latimeCasutaOptiuni / 2 + latimeCasutaOptiuni * i + 10 * (i + 1), 20);
      
        window.draw(text);
        

    }
}
int optiuneMeniuSelectata(Vector2i coordonateMouse)
{
    for (int i = 0; i < NUMAR_OPTIUNI_MENIU; i++)
        if (coordonateMouse.x > i * latimeCasutaOptiuni + (i + 1) * 10 && coordonateMouse.x < (i + 1) * latimeCasutaOptiuni + (i + 1) * 10)
            return i;

    return - 1;
}