#pragma once
#include <SFML/Graphics.hpp>
#include "Defineuri.h"
#include "Structuri.h"
#include <string> 

using namespace sf;
char optiuniMeniu[NUMAR_OPTIUNI_MENIU][15] = { "MENIU","SALVARE" ,"SALVARE CA","DESCHIDERE","EXPORT","RESETARE","AJUTOR","SETARI"  };
float latimeCasutaOptiuni;
void desenareMeniuOptiuni(RenderWindow& window, Font font)
{
    float offsetX = 10;
    latimeCasutaOptiuni = (LATIME_FEREASTRA - (NUMAR_OPTIUNI_MENIU + 1) * SPATIERE_CASUTE) / NUMAR_OPTIUNI_MENIU;
    Vector2f dimCasuta;
    dimCasuta.x = latimeCasutaOptiuni;
    dimCasuta.y = INALTIME_MENIU_OPTIUNI - 20;
    Text text;
   
    for (int i = 0; i < NUMAR_OPTIUNI_MENIU; i++)
    {
        RectangleShape casuta;
        sf::Color albastru(0, 80, 190);
        casuta.setPosition(latimeCasutaOptiuni * i + SPATIERE_CASUTE * (i + 1), 10);
        casuta.setSize(dimCasuta);
        casuta.setFillColor(albastru);
        window.draw(casuta);

      
        text.setFont(font); 
        text.setString(optiuniMeniu[i]);
        text.setCharacterSize(25); 
        text.setStyle(sf::Text::Bold);
        text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
        text.setPosition(latimeCasutaOptiuni / 2 + latimeCasutaOptiuni * i + SPATIERE_CASUTE * (i + 1), 20);
      
        window.draw(text);
        
    }
}
int optiuneMeniuSelectata(Vector2i coordonateMouse)
{
    for (int i = 0; i < NUMAR_OPTIUNI_MENIU; i++)
        if (coordonateMouse.x > i * latimeCasutaOptiuni + (i + 1) * SPATIERE_CASUTE && coordonateMouse.x < (i + 1) * latimeCasutaOptiuni + (i + 1) * SPATIERE_CASUTE)
            return i;

    return - 1;
}
void desenareMeniuElemente(RenderWindow& window, ElCircuit elemente[], int nrElemente, float* latimeCasuta)
{
	float offsetX = 10;
	float scale = 0.5;

	*latimeCasuta = (1920 - (nrElemente + 1) * 10) / nrElemente;
	Vector2f dimCasuta;
	dimCasuta.x = *latimeCasuta;
	dimCasuta.y = INALTIME_MENIU_ELEMENTE;
	for (int i = 0; i < 12; i++)
	{
		Transformable t;
		RectangleShape casuta;
		ElCircuit elementLocal = elemente[i];
		sf::Color albastru(0, 102, 204);
		casuta.setPosition(*latimeCasuta * i + 10 * (i + 1), INALTIME_MENIU_OPTIUNI);
		casuta.setSize(dimCasuta);
		casuta.setFillColor(albastru);
		window.draw(casuta);
		t.setScale(scale, scale);
		t.move(*latimeCasuta * i + (i + 1) * 10 - elemente[i].origine.x * scale + *latimeCasuta / 2, INALTIME_MENIU_OPTIUNI - elemente[i].origine.y * scale + 50 / 2);

		if (elemente[i].linii.getVertexCount() != 0)
		{
			window.draw(elemente[i].linii, t.getTransform());
		}
		if (elemente[i].dreptunghi.getPointCount() != 0)
		{
			window.draw(elemente[i].dreptunghi, t.getTransform());
		}
		if (elemente[i].arc.getVertexCount() != 0)
		{
			window.draw(elemente[i].arc, t.getTransform());
		}
		if (elemente[i].elipsa.getRadius() != 0)
		{
			window.draw(elemente[i].elipsa, t.getTransform());
		}
	}
}