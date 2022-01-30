#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Structuri.h"
#include <iostream>
#include <charconv>
#include <string.h>


enum StatusuriLocale
{
	NicioModificare, 
	ModificareNume, 
	ModificareRezistenta,
	ModificareVoltaj
};
bool esteCaracter(char c)
{
	return c >= 33 && c <= 126;
}
bool esteCifra(char c)
{
	return c >= 48 && c <= 57;
}
bool salvareDate(Componenta* componenta, char rezistenta[], char voltaj[])
{
	(*componenta).rezistenta = atoi(rezistenta);
	(*componenta).voltaj = atoi(voltaj);
	return true;
}
void shrek()
{
	Color gri(32, 32, 32);
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8.0;
	RenderWindow window(VideoMode(800, 450), "Shrek", Style::Close, settings);
	window.setFramerateLimit(60);
	Texture image;
	image.loadFromFile("shrek.jpg");
	Sprite s(image);
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		window.clear(sf::Color::White);
		window.draw(s);
		window.display();
	}	
}
void afisareFereastraParametri(Componenta* componenta)
{
	Image image;
	image.loadFromFile("icons\\gears.png");
	sf::Font font;
	font.loadFromFile("fonts\\Courier.ttf");
	sf::Text t;
	t.setFillColor(sf::Color::White);
	t.setFont(font);
	t.setString("Rezistenta");
	unsigned int latimeMaxNumeCamp = t.getLocalBounds().width;
	t.setString("Rgp");

	StatusuriLocale status = NicioModificare;

	Vector2f dimButon, dimTextBox;
	char rezistenta[MAXNUMAR] = "0", voltaj[MAXNUMAR] = "0";
	_itoa(componenta->rezistenta, rezistenta, 10);
	_itoa(componenta->voltaj, voltaj, 10);

	unsigned inaltimeFereastra = 200;
	int test = t.getGlobalBounds().width;
	unsigned latimeMaxCasetaText = t.getGlobalBounds().width * MAX_NUME_COMPONENTA  / 3 + 2 * OFFSET;
	unsigned latimeFereastra = latimeMaxCasetaText + latimeMaxNumeCamp + 2 * OFFSET;
	sf::RenderWindow window(sf::VideoMode(latimeFereastra, inaltimeFereastra, 32), "Parametri", sf::Style::Close);
	window.setFramerateLimit(30);
	window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());

	int inaltimeCastaText = t.getLocalBounds().height;
	RectangleShape butonSalvare;
	
	butonSalvare.setOutlineColor(Color::White);
	butonSalvare.setOutlineThickness(2);
	butonSalvare.setFillColor(Color::Transparent);
	RectangleShape textBox;
	textBox.setOutlineColor(Color::White);
	textBox.setOutlineThickness(1);
	textBox.setFillColor(Color::Transparent);

	dimTextBox.x = latimeMaxCasetaText;
	dimTextBox.y = inaltimeCastaText + OFFSET / 2;
	textBox.setSize(dimTextBox);
	char init = '_';

	while (window.isOpen()) 
	{
		sf::Event event;
		Transformable transformable, transformableValoare;
		while (window.pollEvent(event)) 
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				Vector2i coordonateMouse = Mouse::getPosition(window);
				if (coordonateMouse.x >= butonSalvare.getGlobalBounds().left && coordonateMouse.x <= butonSalvare.getGlobalBounds().left + butonSalvare.getGlobalBounds().width
					&& coordonateMouse.y >= butonSalvare.getGlobalBounds().top && coordonateMouse.y <= butonSalvare.getGlobalBounds().top + butonSalvare.getGlobalBounds().height)
				{
					butonSalvare.setOutlineColor(Color::Blue);
				}
				else
				{
					butonSalvare.setOutlineColor(Color::White);
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				Vector2i coordonateMouse = Mouse::getPosition(window);
				if (coordonateMouse.x >= butonSalvare.getGlobalBounds().left && coordonateMouse.x <= butonSalvare.getGlobalBounds().left + butonSalvare.getGlobalBounds().width
					&& coordonateMouse.y >= butonSalvare.getGlobalBounds().top && coordonateMouse.y <= butonSalvare.getGlobalBounds().top + butonSalvare.getGlobalBounds().height)
				{
					if (status == ModificareVoltaj)
					{
						voltaj[strlen(voltaj) - 1] = '\0';
					}
					else if (status == ModificareRezistenta)
					{
						rezistenta[strlen(rezistenta) - 1] = '\0';
					}
					else if (status == ModificareNume)
					{
						componenta->nume[strlen(componenta->nume) - 1] = '\0';
					}
					if (salvareDate(componenta, rezistenta, voltaj) == true)
					{
						if (_stricmp(componenta->nume, "shrek") == 0)
						{
							shrek();
						}
						window.close();
					}
				}
				else if (coordonateMouse.x >= latimeMaxNumeCamp + OFFSET && coordonateMouse.x < latimeFereastra)
				{
					if (coordonateMouse.y >= OFFSET / 2)
					{
						if (coordonateMouse.y < inaltimeCastaText + OFFSET) //caseta nume
						{
							if (status == ModificareRezistenta)
								rezistenta[strlen(rezistenta) - 1] = '\0';

							if (status == ModificareVoltaj)
								voltaj[strlen(voltaj) - 1] = '\0';

							if (status != ModificareNume)
							{
								unsigned lungime = strlen(componenta->nume);
								componenta->nume[lungime] = init;
								componenta->nume[lungime + 1] = '\0';
							}
							status = ModificareNume;
						}
						else if (coordonateMouse.y < inaltimeCastaText * 2 + 2 * OFFSET) //caseta rezistenta
						{
							if (status == ModificareNume)
							{
								componenta->nume[strlen(componenta->nume) - 1] = '\0';
							}
							if (status == ModificareVoltaj)
							{
								voltaj[strlen(voltaj) - 1] = '\0';
							}
							if (status != ModificareRezistenta)
							{
								unsigned lungime = strlen(rezistenta);
								rezistenta[lungime] = init;
								rezistenta[lungime + 1] = '\0';
							}
							status = ModificareRezistenta;
						}
						else if (coordonateMouse.y < inaltimeCastaText * 3 + 3 * OFFSET) //caseta voltaj
						{
							if (status == ModificareNume)
							{
								componenta->nume[strlen(componenta->nume) - 1] = '\0';
							}
							if (status == ModificareRezistenta)
							{
								rezistenta[strlen(rezistenta) - 1] = '\0';
							}
							if (status != ModificareVoltaj)
							{
								unsigned lungime = strlen(voltaj);
								voltaj[lungime] = init;
								voltaj[lungime + 1] = '\0';
							}
							status = ModificareVoltaj;
						}
						else
						{
							if (status == ModificareVoltaj)
							{
								voltaj[strlen(voltaj) - 1] = '\0';
							}
							else if (status == ModificareRezistenta)
							{
								rezistenta[strlen(rezistenta) - 1] = '\0';
							}
							else if (status == ModificareNume)
							{
								componenta->nume[strlen(componenta->nume) - 1] = '\0';
							}
							status = NicioModificare;
						}
					}
				}
				else
				{
					if (status == ModificareVoltaj)
					{
						voltaj[strlen(voltaj) - 1] = '\0';
					}
					else if (status == ModificareRezistenta)
					{
						rezistenta[strlen(rezistenta) - 1] = '\0';
					}
					else if (status == ModificareNume)
					{
						componenta->nume[strlen(componenta->nume) - 1] = '\0';
					}
					status = NicioModificare;
				}

			}
			if (event.type == sf::Event::TextEntered)
			{
				if (status == ModificareNume)
				{
					unsigned lungimeNume = strlen(componenta->nume);
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && lungimeNume > 1)
					{
						componenta->nume[lungimeNume - 2] = init;
						componenta->nume[lungimeNume - 1] = '\0';
					}
					else if (lungimeNume < MAX_NUME_COMPONENTA && esteCaracter(event.key.code))
					{
						char c = (char)(event.key.code);
						componenta->nume[lungimeNume - 1] = c;
						componenta->nume[lungimeNume] = init;
						componenta->nume[lungimeNume + 1] = '\0';
					}
				}
				else if (status == ModificareRezistenta)
				{
					unsigned lungimeRezist = strlen(rezistenta);
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && lungimeRezist > 1)
					{
						rezistenta[lungimeRezist - 2] = init;
						rezistenta[lungimeRezist - 1] = '\0';
					}
					else if (lungimeRezist < MAXNUMAR - 1 && esteCifra(event.key.code))
					{
						char c = (char)(event.key.code);
						rezistenta[lungimeRezist - 1] = c;
						rezistenta[lungimeRezist] = init;
						rezistenta[lungimeRezist + 1] = '\0';
					}
				}
				else if (status == ModificareVoltaj)
				{
					unsigned lungimeVoltaj = strlen(voltaj);
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && lungimeVoltaj > 1)
					{
						voltaj[lungimeVoltaj - 2] = init;
						voltaj[lungimeVoltaj - 1] = '\0';
					}
					else if (lungimeVoltaj < MAXNUMAR - 1 && esteCifra(event.key.code))
					{
						char c = (char)(event.key.code);
						voltaj[lungimeVoltaj - 1] = c;
						voltaj[lungimeVoltaj] = init;
						voltaj[lungimeVoltaj + 1] = '\0';
					}
				}
			}

		}
		textBox.setPosition(0, 0);
		transformable.setPosition(0, 0);
		transformableValoare.setPosition(0, 0);

		window.clear(sf::Color::Black);
		t.setString("Nume");

		window.draw(t, transformable.getTransform());
		t.setString(componenta->nume);
		transformableValoare.move(latimeMaxNumeCamp + OFFSET, OFFSET / 5);
		textBox.move(latimeMaxNumeCamp + OFFSET, OFFSET / 2);
		window.draw(textBox);
		window.draw(t, transformableValoare.getTransform());
		transformable.move(0, inaltimeCastaText + OFFSET);

		t.setString("Rezistenta");
		window.draw(t, transformable.getTransform());
		t.setString(rezistenta);
		transformableValoare.move(0, inaltimeCastaText + OFFSET);
		textBox.move(0, inaltimeCastaText + OFFSET);

		window.draw(textBox);
		window.draw(t, transformableValoare.getTransform());
		transformable.move(0, inaltimeCastaText + OFFSET);

		t.setString("Voltaj");
		window.draw(t, transformable.getTransform());
		textBox.move(0, inaltimeCastaText + OFFSET);
		transformableValoare.move(0, inaltimeCastaText + OFFSET);
		t.setString(voltaj);
		window.draw(t, transformableValoare.getTransform());
		window.draw(textBox);
		transformable.move(0, inaltimeCastaText + OFFSET);

		t.setString("Salvare");
		transformable.move((latimeFereastra - t.getLocalBounds().width) / 2, inaltimeCastaText);

		dimButon.x = t.getLocalBounds().width + OFFSET;
		dimButon.y = t.getLocalBounds().height + OFFSET;

		butonSalvare.setPosition((latimeFereastra - t.getLocalBounds().width) / 2, transformable.getPosition().y);
		butonSalvare.setSize(dimButon);
		window.draw(butonSalvare);
		window.draw(t, transformable.getTransform());

		window.display();
	}
}
