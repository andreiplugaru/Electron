#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <math.h>
#include "Conexiuni.h"
#include "Circuit.h"
#include "Meniu.h"
#include "Parametri.h"
#include "Stiva.h"
#include "Slidere.h"
#include "FereastraAvertizare.h"
#include "TransformariComponente.h"
#include "Events.h"
char numeOptiuniSalvate[NUMAR_OPTIUNI][40] = { "Coeficient micsorare", "Distanta miscare", "Unghi rotire" , "Culoare Conexiune" };
char locatieFisiere[13][20] = { "PS\\BATERIE.PS","PS\\AMPLOP.PS" ,"PS\\CONDENS.PS","PS\\DIODA.PS","PS\\NOD.PS","PS\\POLARIZ.PS","PS\\REZIST.PS","PS\\SERVOMOT.PS","PS\\SINU.PS","PS\\TRANZNPN.PS","PS\\TRANZPNP.PS","PS\\ZENNER.PS","PS\\STOP.PS" };
using namespace sf;
using namespace std;
void coordonateArc(ElCircuit& element, Vertex origine, float raza, PunctMinMax extreme, float offset)
{
	for (int i = 90; i >= -90; i--)
	{
		Vertex punct;
		float angle = i / (180 / PI);
		punct.position.x = origine.position.x + (cos(angle) * raza) * SIZECOEFINITIAL + offset;
		punct.position.y = origine.position.y + (sin(angle) * raza) * SIZECOEFINITIAL + offset;
		element.arc.append(punct);
	}
}
PunctMinMax aflareCoordonateMinimeMaxime(char locatieFisier[])
{
	FILE* f = fopen(locatieFisier, "r");
	Punct minim;
	Punct maxim;
	PunctMinMax extreme;
	float xmin = 0, ymin = 0, xmax = 0, ymax = 0;
	char denumire[30];
	int nr = 0;
	fgets(denumire, 30, f);
	fscanf(f, "%d", &nr);
	for (int i = 0; i < nr; i++)
	{
		float x, y;
		fscanf(f, "%f", &x);
		fscanf(f, "%f", &y);
		if (i == 0)
		{
			xmin = x;
			ymin = y;
		}
		if (x < xmin)
		{
			xmin = x;
		}
		if (y < ymin)
		{
			ymin = y;
		}
		if (x > xmax)
		{
			xmax = x;
		}
		if (y > ymax)
		{
			ymax = y;
		}
	}
	int nrLinii = 0;
	fscanf(f, "%d", &nrLinii);
	for (int i = 0; i < nrLinii; i++)
	{
		char tip;
		fscanf(f, "\n%c", &tip);

		float x, y;
		if (tip == 'R')
		{
			fscanf(f, "%f", &x);
			fscanf(f, "%f", &y);
			if (x < xmin)
			{
				xmin = x;
			}
			if (y < ymin)
			{
				ymin = y;
			}
			if (x > xmax)
			{
				xmax = x;
			}
			if (y > ymax)
			{
				ymax = y;
			}
			float dimX, dimY;
			fscanf(f, "%f", &dimX);
			fscanf(f, "%f", &dimY);
			if (x + dimX > xmax)
			{
				xmax = x + dimX;
			}
			if (y + dimY > ymax)
			{
				ymax = y + dimY;
			}

		}
		else if (tip != 'O')
		{
			fscanf(f, "%f", &x);
			fscanf(f, "%f", &y);


			if (x < xmin)
			{
				xmin = x;
			}
			if (y < ymin)
			{
				ymin = y;
			}
			if (x > xmax)
			{
				xmax = x;
			}
			if (y > ymax)
			{
				ymax = y;
			}

			fscanf(f, "%f", &x);
			fscanf(f, "%f", &y);

			if (x < xmin)
			{
				xmin = x;
			}
			if (y < ymin)
			{
				ymin = y;
			}
			if (x > xmax)
			{
				xmax = x;
			}
			if (y > ymax)
			{
				ymax = y;
			}
		}
		else
		{
			float rx, ry;
			fscanf(f, "%f", &x);
			fscanf(f, "%f", &y);
			fscanf(f, "%f", &rx);
			fscanf(f, "%f", &ry);
			if (x - rx < xmin)
			{
				xmin = x - rx;
			}
			if (y - ry < ymin)
			{
				ymin = y - ry;
			}
			if (x + rx > xmax)
			{
				xmax = x + rx;
			}
			if (y + ry > ymax)
			{
				ymax = y + ry;
			}
		}

	}
	extreme.minim.x = xmin;
	extreme.minim.y = ymin;
	extreme.maxim.x = xmax;
	extreme.maxim.y = ymax;
	fclose(f);
	return extreme;
}
ElCircuit citireInformatii(char locatieFisier[])
{
	ElCircuit element;
	element.arc.setPrimitiveType(LinesStrip);
	FILE* f = fopen(locatieFisier, "r");
	PunctMinMax extreme = aflareCoordonateMinimeMaxime(locatieFisier);
	fgets(element.denumire, MAX_NUME_COMPONENTA, f);
	element.denumire[strlen(element.denumire) - 1] = '\0';
	fscanf(f, "%d", &element.nrLegaturi);

	for (int j = 0; j < element.nrLegaturi; j++)
	{
		float v;
		fscanf(f, "%f", &v);
		element.legaturi[j].x = (v + abs(extreme.minim.x)) * SIZECOEFINITIAL;
		fscanf(f, "%f", &v);
		element.legaturi[j].y = (v + abs(extreme.minim.y)) * SIZECOEFINITIAL;
	}
	element.dimensiuni.x = abs(extreme.maxim.x - extreme.minim.x) * SIZECOEFINITIAL;
	element.dimensiuni.y = abs(extreme.maxim.y - extreme.minim.y) * SIZECOEFINITIAL;

	fscanf(f, "%d", &element.nrLinii);
	element.linii.setPrimitiveType(Lines);
	for (int j = 0; j < element.nrLinii; j++)
	{
		char tip;
		fscanf(f, "\n%c", &tip);
		if (tip == 'L')
		{
			Vertex Punct;
			float coord;

			fscanf(f, "%f", &coord);
			Punct.position.x = (coord + abs(extreme.minim.x)) * SIZECOEFINITIAL;

			fscanf(f, "%f", &coord);
			Punct.position.y = (coord + abs(extreme.minim.y)) * SIZECOEFINITIAL;

			element.linii.append(Punct);

			fscanf(f, "%f", &coord);
			Punct.position.x = (coord + abs(extreme.minim.x)) * SIZECOEFINITIAL;

			fscanf(f, "%f", &coord);
			Punct.position.y = (coord + abs(extreme.minim.y)) * SIZECOEFINITIAL;

			element.linii.append(Punct);
		}
		if (tip == 'A')
		{
			Vertex punct;
			Vertex Origine;
			float xorigine, yorigine, raza, offset;
			fscanf(f, "%f", &xorigine);
			Origine.position.x = (xorigine + abs(extreme.minim.x)) * SIZECOEFINITIAL;
			fscanf(f, "%f", &yorigine);
			Origine.position.y = (yorigine + abs(extreme.minim.y)) * SIZECOEFINITIAL;
			fscanf(f, "%f", &raza);
			fscanf(f, "%f", &offset);
			offset = offset * SIZECOEFINITIAL;
			coordonateArc(element, Origine, raza, extreme, offset);
		}
		if (tip == 'O')
		{
			Vector2f pozitie;
			Vector2f dimensiuni;
			float rx, ry, x, y, raza, scalex = 1, scaley = 1;
			fscanf(f, "%f", &pozitie.x);
			fscanf(f, "%f", &pozitie.y);
			pozitie.x = (pozitie.x + abs(extreme.minim.x)) * SIZECOEFINITIAL;
			pozitie.y = (pozitie.y + abs(extreme.minim.y)) * SIZECOEFINITIAL;

			fscanf(f, "%f", &rx);
			fscanf(f, "%f", &ry);
			if (ry > rx)
			{
				raza = rx;
				scaley = ry / rx;
			}
			else
			{
				raza = ry;
				scalex = rx / ry;
			}
			element.elipsa.setOrigin(raza * SIZECOEFINITIAL, raza * SIZECOEFINITIAL);
			element.elipsa.setPosition(pozitie);
			element.elipsa.setRadius(raza * SIZECOEFINITIAL);
			element.elipsa.scale(scalex, scaley);
			element.elipsa.setFillColor(Color::Transparent);
			element.elipsa.setOutlineThickness(1);
			dimensiuni.x = raza * rx + 1;
			dimensiuni.y = raza * ry + 1;
			if (dimensiuni.x > element.dimensiuni.x)
				element.dimensiuni.x = dimensiuni.x;

			if (dimensiuni.y > element.dimensiuni.y)
				element.dimensiuni.y = dimensiuni.y;
		}
		if (tip == 'R')
		{
			float x, y;
			Vector2f dimensiuni;
			fscanf(f, "%f", &x);
			x = (x + abs(extreme.minim.x)) * SIZECOEFINITIAL;

			fscanf(f, "%f", &y);
			y = (y + abs(extreme.minim.y)) * SIZECOEFINITIAL;

			fscanf(f, "%f", &dimensiuni.x);
			dimensiuni.x = (dimensiuni.x + abs(extreme.minim.x)) * SIZECOEFINITIAL - x;

			fscanf(f, "%f", &dimensiuni.y);
			dimensiuni.y = (dimensiuni.y + abs(extreme.minim.y)) * SIZECOEFINITIAL - y;

			element.dreptunghi.setPosition(x, y);			
			if (strcmp(element.denumire, "Nod") == 0)
			{
				element.dreptunghi.setFillColor(Color::White);
			}
			else
			{
				element.dreptunghi.setFillColor(Color::Transparent);
			}
			element.dreptunghi.setOutlineThickness(1);
			element.dreptunghi.setSize(dimensiuni);

			if (dimensiuni.x > element.dimensiuni.x)
				element.dimensiuni.x = dimensiuni.x;

			if (dimensiuni.y > element.dimensiuni.y)
				element.dimensiuni.y = dimensiuni.y;


		}
	}
	element.dimensiuni.x += 2 * LATIME_CHENAR;
	element.dimensiuni.y += 2 * LATIME_CHENAR;
	element.origine.x = element.dimensiuni.x / 2;
	element.origine.y = element.dimensiuni.y / 2;
	element.chenar.setFillColor(Color::Transparent);
	element.chenar.setOutlineThickness(LATIME_CHENAR);
	element.chenar.setOutlineColor(Color::Blue);
	element.chenar.setOrigin(LATIME_CHENAR, LATIME_CHENAR);
	element.chenar.setSize(Vector2f(element.dimensiuni.x, element.dimensiuni.y));
	fclose(f);
	return element;

}
void citireJSON(Optiuni* optiuni)
{
	char c;
	char rand[40];
	char sep[] = "\":,}";
	FILE* f = fopen("setari.json", "r");
	fgets(rand, 40, f);


	while (!feof(f))
	{
		fscanf(f, "%c ", &c);
		fgets(rand, 40, f);
		char* token = strtok(rand, sep);		
		if (strcmp(token, numeOptiuniSalvate[0]) == 0)
		{
			token = strtok(NULL, sep);
			optiuni->coefScalare = atof(token);
		}
		else if (strcmp(token, numeOptiuniSalvate[1]) == 0)
		{
			token = strtok(NULL, sep);
			optiuni->distantaMiscare = atoi(token);
		}
		else if (strcmp(token, numeOptiuniSalvate[2]) == 0)
		{
			token = strtok(NULL, sep);
			if (token != NULL)
				optiuni->unghiRotire = atoi(token);
		}
		else if (strcmp(token, numeOptiuniSalvate[3]) == 0)
		{
			token = strtok(NULL, sep);
			if (token != NULL)
			{
				Color culoare(atoi(token));
				optiuni->culoareConexiuni = culoare;
			}
		}
	}
}
void resetareDateComponenta(Componenta* comp)
{
	(*comp) = { };
}
PunctMinMax puncteMinimSiMaxim(Vector2f puncte[], int nrPuncte)
{
	PunctMinMax coordonate = { 0 };
	for (int i = 0; i < nrPuncte; i++)
	{
		if (i == 0)
		{
			coordonate.minim.x = puncte[i].x;
			coordonate.minim.y = puncte[i].y;
		}
		if (puncte[i].x < coordonate.minim.x)
		{
			coordonate.minim.x = puncte[i].x;
		}
		if (puncte[i].y < coordonate.minim.y)
		{
			coordonate.minim.y = puncte[i].y;
		}
		if (puncte[i].x > coordonate.maxim.x)
		{
			coordonate.maxim.x = puncte[i].x;
		}
		if (puncte[i].y > coordonate.maxim.y)
		{
			coordonate.maxim.y = puncte[i].y;
		}
	}
	return coordonate;
}
void stergereComponenta(Circuit* circuit)
{
	for (int i = circuit->componentaSelectata; i < circuit->nrComponente; i++)
	{
		circuit->componente[i] = circuit->componente[i + 1];

		if (i != circuit->componentaSelectata)
		{
			for (int j = 0; j < circuit->nrLegaturi; j++)
			{
				if (circuit->legaturi[j].idComponenta1 == i)
				{
					circuit->legaturi[j].idComponenta1--;
				}
				else if (circuit->legaturi[j].idComponenta2 == i)
				{
					circuit->legaturi[j].idComponenta2--;
				}
			}
		}
	}
}
void afisareDoarComponente(RenderWindow& window, Circuit* circuit)
{
	Color gri(32, 32, 32);
	window.clear(gri);
	for (int j = 0; j < circuit->nrComponente; j++)
	{
		for (int i = 0; i < circuit->nrLegaturi; i++)
		{
			VertexArray linii = creareConexiuni(circuit->legaturi[i], circuit->optiuni.culoareConexiuni);
			linii.setPrimitiveType(LinesStrip);
			window.draw(linii);
		}
		window.draw(circuit->elemente[circuit->componente[j].idElement].linii, circuit->componente[j].transformari.getTransform());
		window.draw(circuit->elemente[circuit->componente[j].idElement].dreptunghi, circuit->componente[j].transformari.getTransform());
		window.draw(circuit->elemente[circuit->componente[j].idElement].elipsa, circuit->componente[j].transformari.getTransform());
		window.draw(circuit->elemente[circuit->componente[j].idElement].arc, circuit->componente[j].transformari.getTransform());

		if (circuit->componente[j].activ || circuit->componentaSelectata == j)
			window.draw(circuit->elemente[circuit->componente[j].idElement].chenar, circuit->componente[j].transformari.getTransform());
	}
}
void afisareElemente(Circuit* circuit)
{
	Color gri(32, 32, 32);
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8.0;
	Image image;
	image.loadFromFile("icons\\Electron.png");
	RenderWindow window(VideoMode(1920, 1080), "Electron", Style::Titlebar, settings);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);
	window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
	citireJSON(&circuit->optiuni);

	for (int i = 0; i < 12; i++)
		circuit->elemente[i] = citireInformatii(locatieFisiere[i]);
	sf::Font font;
	font.loadFromFile("fonts\\RobotoSlab-Font.ttf");
	sf::Text text;
	text.setFont(font);
	circuit->topRedo = NULL;
	circuit->topUndo = NULL;
	salvareIstoric(circuit);
	circuit->esteSalvat = true;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyPressed:
			{
				if (Keyboard::isKeyPressed(sf::Keyboard::Y) && Keyboard::isKeyPressed(sf::Keyboard::LControl))//redo
				{
					if (circuit->topRedo != ((*circuit)).topUndo)
					{
						int nr = prim(circuit->topRedo);
						if (nr != 0 && circuit->topRedo != circuit->topUndo)
							push(circuit->topUndo, nr);
					}

					deschidereIstoricRedo(circuit);
				}
				if (Keyboard::isKeyPressed(sf::Keyboard::Z) && Keyboard::isKeyPressed(sf::Keyboard::LControl))//undo
				{
					salvareIstoricRedo(circuit);
					deschidereIstoric(circuit);
				}
				if (Keyboard::isKeyPressed(sf::Keyboard::S) && Keyboard::isKeyPressed(sf::Keyboard::LControl))
				{
					salvareCircuit(circuit, circuit->numeFisier, true);
					circuit->esteSalvat = true;
				}
				else if (Keyboard::isKeyPressed(sf::Keyboard::O) && Keyboard::isKeyPressed(sf::Keyboard::LControl))
				{
					circuit->numeFisier[0] = '\0';
					deschidereCircuit(circuit, circuit->numeFisier, true);
					
					circuit->topRedo = NULL;
					circuit->topUndo = NULL;
					circuit->esteSalvat = true;
					salvareIstoric(circuit);
				}
				if (event.key.code == sf::Keyboard::Escape)
				{
					stergereIstoric(circuit);
					window.close();
				}
				else if (event.key.code == sf::Keyboard::E && circuit->componentaSelectata != -1)
				{
					circuit->status = ModificareParametri;
				}
				else if (event.key.code == sf::Keyboard::Delete)
				{
					if (circuit->componentaSelectata != -1)
					{
						stergereLegaturi(circuit);
						stergereComponenta(circuit);						
						circuit->nrComponente--;
						circuit->status = Normal;
						circuit->componentaSelectata = -1;
						salvareIstoric(circuit);
					}
				}
				if (event.key.code == sf::Keyboard::Right)
				{
					if (circuit->componentaSelectata != -1)
					{
						salvareIstoric(circuit);
						miscareTastatura(circuit, circuit->optiuni.distantaMiscare, 0);
					}
				}
				else if (event.key.code == sf::Keyboard::Left)
				{
					if (circuit->componentaSelectata != -1)
					{
						salvareIstoric(circuit);
						miscareTastatura(circuit, -circuit->optiuni.distantaMiscare, 0);
					}

				}
				else if (event.key.code == sf::Keyboard::Up)
				{
					if (circuit->componentaSelectata != -1)
					{
						salvareIstoric(circuit);
						miscareTastatura(circuit, 0, -circuit->optiuni.distantaMiscare);
					}

				}
				else if (event.key.code == sf::Keyboard::Down)
				{
					if (circuit->componentaSelectata != -1)
					{
						salvareIstoric(circuit);
						miscareTastatura(circuit, 0, circuit->optiuni.distantaMiscare);
					}

				}
				else if (event.key.code == sf::Keyboard::R) {
					if (circuit->componentaSelectata != -1)
					{
						salvareIstoric(circuit);
						rotire(circuit);
					}
				}
				else if (event.key.code == sf::Keyboard::Subtract)
				{
					if (circuit->componentaSelectata != -1)
					{
						if (circuit->componente[circuit->componentaSelectata].transformari.getScale().x > 0.5)
						{
							salvareIstoric(circuit);
							redimensionare(circuit, circuit->optiuni.coefScalare, circuit->componentaSelectata);
						}
					}
					else
					{
						redimensionareIntregCircuit(circuit, circuit->optiuni.coefScalare);
					}
				}
				else if (event.key.code == sf::Keyboard::Add)
				{
					if (circuit->componentaSelectata != -1)
					{
						if (circuit->componente[circuit->componentaSelectata].transformari.getScale().x < 2)
						{
							salvareIstoric(circuit);
							redimensionare(circuit, 1 / circuit->optiuni.coefScalare, circuit->componentaSelectata);
						}
					}
					else
					{
						redimensionareIntregCircuit(circuit, 1 / circuit->optiuni.coefScalare);
					}

				}
				break;
			}
			case sf::Event::MouseMoved:
			{
				Vector2i coordonateMouse = Mouse::getPosition(window);
				mouseMoved(circuit, coordonateMouse);
				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				Vector2i coordonateMouse = Mouse::getPosition(window);
				mousePressed(circuit, coordonateMouse, window, event.mouseButton.button == sf::Mouse::Left, numeOptiuniSalvate);
				break;
			}

			case sf::Event::MouseButtonReleased:
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					mouseReleased(circuit);
				}
				break;
			}

			}

			window.clear(gri);
			if (circuit->status == ModificareParametri)
			{
				afisareFereastraParametri(&circuit->componente[circuit->componentaSelectata]);
				circuit->status = Normal;
			}
			desenareMeniuElemente(window, circuit->elemente, NUMAR_ELEMENTE, &circuit->latimeCasuta);
			drawCoefScalareBara(window);
			drawUnghiRotireBara(window);
			if (circuit->componentaSelectata != -1)
			{
				drawSlider(window, pozitieCercCoefScalare(circuit->componente[circuit->componentaSelectata].transformari.getScale().x));
				if (circuit->componente[circuit->componentaSelectata].transformari.getRotation() == 0)
				{
					drawSlider(window, varfUnghiRotire);
				}
				else
				{
					drawSlider(window, pozitieCercUnghiRotire(circuit->componente[circuit->componentaSelectata].transformari.getRotation()));
				}
			}
			else
			{
				drawSlider(window, pozitieCercCoefScalare(circuit->sizeCoef));
				drawSlider(window, varfUnghiRotire);
			}
			desenareMeniuOptiuni(window, font);
			for (int j = 0; j < circuit->nrComponente; j++)
			{
				for (int i = 0; i < circuit->nrLegaturi; i++)
				{
					VertexArray linii = creareConexiuni(circuit->legaturi[i], circuit->optiuni.culoareConexiuni);
					linii.setPrimitiveType(LinesStrip);
					window.draw(linii);
				}
				window.draw(circuit->elemente[circuit->componente[j].idElement].linii, circuit->componente[j].transformari.getTransform());
				window.draw(circuit->elemente[circuit->componente[j].idElement].dreptunghi, circuit->componente[j].transformari.getTransform());
				window.draw(circuit->elemente[circuit->componente[j].idElement].elipsa, circuit->componente[j].transformari.getTransform());
				window.draw(circuit->elemente[circuit->componente[j].idElement].arc, circuit->componente[j].transformari.getTransform());

				if (circuit->componente[j].activ || circuit->componentaSelectata == j)
					window.draw(circuit->elemente[circuit->componente[j].idElement].chenar, circuit->componente[j].transformari.getTransform());
			}
			window.display();

		}
	}
	delete circuit;
}
