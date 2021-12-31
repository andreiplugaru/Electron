#define _CRT_SECURE_NO_WARNINGS
#include "Structuri.h"
#include <iostream>
#include <charconv>


enum StatusuriLocale {
	NicioModificare, ModificareNume, ModificareRezistenta, ModificareVoltaj
};
bool esteCaracter(char c)
{
	return c >= 33 && c <= 126;
}
bool esteCifra(char c)
{
	return c >= 48 && c <= 57;
}
bool salvareDate(Componenta *componenta, char rezistenta[], char voltaj[])
{
	(*componenta).rezistenta = atoi(rezistenta);
	(*componenta).voltaj = atoi(voltaj);
	return true;
}
void afisareFereastraParametri(Componenta* componenta)
{
	sf::Font font;
	font.loadFromFile("Courier.ttf");
	sf::Text t;
	t.setFillColor(sf::Color::White);
	t.setFont(font);
	t.setString("Rezistentag");
	StatusuriLocale status = NicioModificare;
	Vector2f dimButon, dimTextBox;
	std::string s = std::to_string((*componenta).rezistenta);
	char rezistenta[MAXNUMAR] = "0", voltaj[MAXNUMAR] = "0";
	_itoa((*componenta).rezistenta, rezistenta,10);
	_itoa((*componenta).voltaj, voltaj, 10);
	unsigned latimeFereastra = t.getLocalBounds().width * 3;
	unsigned inaltimeFereastra = 200;
	unsigned latimeMaxText = t.getLocalBounds().width;
	sf::RenderWindow window(sf::VideoMode(latimeFereastra, inaltimeFereastra, 32), "Parametri", sf::Style::Close);
	window.setFramerateLimit(30);
	//  ErrWindow.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());

	sf::String ErrMessage;
	bool running = true;
	
	int inaltime = t.getLocalBounds().height;
	RectangleShape butonSalvare;
	RectangleShape textBox;
	textBox.setOutlineColor(Color::White);
	textBox.setOutlineThickness(1);
	textBox.setFillColor(Color::Transparent);

	dimTextBox.x = latimeMaxText * 2 - OFFSET*2;
	dimTextBox.y = inaltime + OFFSET/2;
	textBox.setSize(dimTextBox);
	char init = '_';
	while (window.isOpen()) {
		sf::Event event;
		Transformable transformable, transformableValoare;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) 
			{
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				Vector2i coordonateMouse = Mouse::getPosition(window);
				if (coordonateMouse.x >= butonSalvare.getGlobalBounds().left && coordonateMouse.x <= butonSalvare.getGlobalBounds().left + butonSalvare.getGlobalBounds().width
					&& coordonateMouse.y >= butonSalvare.getGlobalBounds().top && coordonateMouse.y <= butonSalvare.getGlobalBounds().top + butonSalvare.getGlobalBounds().height)
				{
					if (status == ModificareVoltaj)
						voltaj[strlen(voltaj) - 1] = '\0';
					else if (status == ModificareRezistenta)
						rezistenta[strlen(rezistenta) - 1] = '\0';
					else if (status == ModificareNume)
						(*componenta).nume[strlen((*componenta).nume) - 1] = '\0';
					if (salvareDate(componenta, rezistenta, voltaj) == true)
						window.close();
				}
				else if (coordonateMouse.x >= latimeMaxText + OFFSET && coordonateMouse.x < latimeFereastra)
				{
					if (coordonateMouse.y >= OFFSET / 2)
					{
						if (coordonateMouse.y < inaltime + OFFSET / 2) //caseta nume
						{
							if (status == ModificareRezistenta)
								rezistenta[strlen(rezistenta) - 1] = '\0';

							if (status == ModificareVoltaj)
								voltaj[strlen(voltaj) - 1] = '\0';

							if (status != ModificareNume)
							{
								(*componenta).nume[strlen((*componenta).nume)] = init;
								(*componenta).nume[strlen((*componenta).nume) + 1] = '\0';
							}
							status = ModificareNume;
						}
						else if (coordonateMouse.y < inaltime * 2 + 3 * OFFSET / 2) //caseta rezistenta
						{
							if (status == ModificareNume)
								(*componenta).nume[strlen((*componenta).nume) - 1] = '\0';

							if (status == ModificareVoltaj)
								voltaj[strlen(voltaj) - 1] = '\0';

							if (status != ModificareRezistenta)
							{
								rezistenta[strlen(rezistenta)] = init;
								rezistenta[strlen(rezistenta) + 1] = '\0';
							}
							status = ModificareRezistenta;
						}
						else if (coordonateMouse.y < inaltime * 3 + 4 * OFFSET / 2) //caseta voltaj
						{
							if (status == ModificareNume)
								(*componenta).nume[strlen((*componenta).nume) - 1] = '\0';

							if (status == ModificareRezistenta)
								rezistenta[strlen(rezistenta) - 1] = '\0';

							if (status != ModificareVoltaj)
							{
								voltaj[strlen(voltaj)] = init;
								voltaj[strlen(voltaj) + 1] = '\0';
							}
							status = ModificareVoltaj;
						}
						else
						{
							if (status == ModificareVoltaj)
								voltaj[strlen(voltaj) - 1] = '\0';
							else if (status == ModificareRezistenta)
								rezistenta[strlen(rezistenta) - 1] = '\0';
							else if (status == ModificareNume)
								(*componenta).nume[strlen((*componenta).nume) - 1] = '\0';
							status = NicioModificare;
						}
					}
				}
				 
		}
			if (event.type == sf::Event::TextEntered)
			{
				if (status == ModificareNume)
				{	
					unsigned lungimeNume = strlen((*componenta).nume);
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && lungimeNume > 1)
						{
							(*componenta).nume[lungimeNume - 2] = init;
							(*componenta).nume[lungimeNume - 1] = '\0';
						}
						else if(lungimeNume < MAXNUME && esteCaracter(event.key.code))
						{
							char c = (char)(event.key.code);
							(*componenta).nume[lungimeNume - 1] = c;
							(*componenta).nume[lungimeNume] = init;
							(*componenta).nume[lungimeNume + 1] = '\0';
						}
				}
				else if (status == ModificareRezistenta)
				{
					unsigned lungimeRezist = strlen(rezistenta);
					if(lungimeRezist <= 5)
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
		t.setString((*componenta).nume);
		transformableValoare.move(latimeMaxText + OFFSET, OFFSET / 5);
		textBox.move(latimeMaxText + OFFSET, OFFSET/2);
		window.draw(textBox);
		window.draw(t, transformableValoare.getTransform());
		transformable.move(0, inaltime + OFFSET);

		t.setString("Rezistenta");
		window.draw(t, transformable.getTransform());
		t.setString(rezistenta);
		transformableValoare.move(0, inaltime + OFFSET);
		textBox.move(0, inaltime + OFFSET);
	
		window.draw(textBox);
		window.draw(t, transformableValoare.getTransform());
		transformable.move(0, inaltime + OFFSET);

		t.setString("Voltaj");
		window.draw(t, transformable.getTransform());
		textBox.move(0, inaltime + OFFSET);
		transformableValoare.move(0, inaltime + OFFSET);
		t.setString(voltaj);
		window.draw(t, transformableValoare.getTransform());
		window.draw(textBox);
		transformable.move(0, inaltime + OFFSET);

		t.setString("Salvare");
		transformable.move((latimeFereastra - t.getLocalBounds().width) / 2, inaltime);

		dimButon.x = t.getLocalBounds().width + OFFSET;
		dimButon.y = t.getLocalBounds().height + OFFSET;

		butonSalvare.setPosition((latimeFereastra - t.getLocalBounds().width) / 2, transformable.getPosition().y);
		butonSalvare.setSize(dimButon);
		butonSalvare.setOutlineColor(Color::White);
		butonSalvare.setOutlineThickness(2);
		butonSalvare.setFillColor(Color::Transparent);
		window.draw(butonSalvare);
		window.draw(t, transformable.getTransform());
		
		window.display();
	}
}
/*   window.draw(t, transform);
		transform.translate(0, 100);

		t.setString("Rezistenta");
		window.draw(t, transform);
		transform.translate(0, 100);

		t.setString("Voltaj");
		window.draw(t, transform);
		transform.translate(0, 100);
*/