#define _CRT_SECURE_NO_WARNINGS
#include "Structuri.h"

enum StatusuriLocale {
	NicioModificare, ModificareCoefScalare, ModificareDistantaMiscare, ModificareUnghiRotireText
};
void FereastraEroareSetari(char mesajEroare[])
{
	Image image;
	image.loadFromFile("icons\\eroare.png");

	sf::Font font;
	font.loadFromFile("fonts\\RobotoSlab-Font.ttf");
	sf::Text t;
	t.setFillColor(sf::Color::White);
	t.setFont(font);
	t.setString(mesajEroare);

	sf::RenderWindow ErrWindow(sf::VideoMode(t.getLocalBounds().width + 10, t.getLocalBounds().height + 10, 32), "Eroare!", sf::Style::Close);
	ErrWindow.setFramerateLimit(30);
	ErrWindow.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());

	sf::String ErrMessage;
	bool running = true;


	while (ErrWindow.isOpen()) {
		sf::Event event;

		while (ErrWindow.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				ErrWindow.close();
			}

		}

		ErrWindow.clear(sf::Color::Black);
		ErrWindow.draw(t);
		ErrWindow.display();
	}
}
bool esteCifraSetari(char c)
{
	return c >= 48 && c <= 57;
}
bool verificareInteriorPatrat(RectangleShape patrat, Vector2i coordonateMouse)
{
	return (coordonateMouse.x >= patrat.getGlobalBounds().left && coordonateMouse.x <= patrat.getGlobalBounds().left + patrat.getGlobalBounds().width
		&& coordonateMouse.y >= patrat.getGlobalBounds().top && coordonateMouse.y <= patrat.getGlobalBounds().top + patrat.getGlobalBounds().height);
}
void spatii(FILE* fd, int numar)
{
	for (int i = 1; i <= numar; i++)
		fprintf(fd, "  ");
}
void salvareJSON(Optiuni optiuni, char numeOptiuniSalvate[][40])
{
	FILE* f = fopen("setari.json", "w");
	fprintf(f, "{\n");
	spatii(f, 1);
	fprintf(f, "\"%s\": %f,\n", numeOptiuniSalvate[0], optiuni.coefScalare);
	spatii(f, 1);
	fprintf(f, "\"%s\": %d,\n", numeOptiuniSalvate[1], optiuni.distantaMiscare);
	spatii(f, 1);
	fprintf(f, "\"%s\": %d\n,", numeOptiuniSalvate[2], optiuni.unghiRotire);
	spatii(f, 1);
	fprintf(f, "\"%s\": %d\n", numeOptiuniSalvate[3], optiuni.culoareConexiuni.toInteger());
	fprintf(f, "}");
	fclose(f);
}

bool salvareDate(Circuit* circuit, Optiuni optiuni, char numeOptiuniSalvate[][40])
{
	if (optiuni.coefScalare >= MAX_COEF_SCALARE)
	{
		char numar[3];
		_itoa(MAX_COEF_SCALARE, numar, 10);
		char mesaj[60] = "Coeficientul de micsorare nu poate fi mai mare decât ";
		strcat(mesaj, numar);
		FereastraEroareSetari(mesaj);
		return false;
	}
	else if (optiuni.distantaMiscare >= LATIME_FEREASTRA)
	{
		char numar[5];
		_itoa(LATIME_FEREASTRA, numar, 10);
		char mesaj[60] = "Distanta de miscare nu poate fi mai mare decât ";
		strcat(mesaj, numar);
		FereastraEroareSetari(mesaj);
		return false;
	}
	else if (optiuni.unghiRotire >= MAX_UNGHI_ROTIRE)
	{
		char numar[5];
		_itoa(MAX_UNGHI_ROTIRE, numar, 10);
		char mesaj[60] = "Unghiul de rotire nu poate fi mai mare decât ";
		strcat(mesaj, numar);
		FereastraEroareSetari(mesaj);
		return false;
	}
	salvareJSON(optiuni, numeOptiuniSalvate);
	circuit->optiuni = optiuni;
	//(*componenta).rezistenta = atoi(rezistenta);
	//(*componenta).voltaj = atoi(voltaj);
	return true;
}
void floatToCharArr(float valoare, char rezultat[])
{
	float f1, f2;
	int i1, i2;

	f1 = floor(valoare);
	f2 = valoare - f1;
	i1 = (int)f1;
	i2 = (int)100 * f2;
	sprintf(rezultat, "%d.%d", i1, i2);
}
void afisareFereastraSetari(Circuit* circuit, char numeOptiuniSalvate[][40])
{
	Image image;
	image.loadFromFile("icons\\gears.png");
	sf::Font font;
	font.loadFromFile("fonts\\Courier.ttf");
	sf::Text t;
	t.setFillColor(sf::Color::White);
	t.setFont(font);
	t.setString(numeOptiuniSalvate[0]);
	unsigned int latimeMaxNumeCamp = t.getLocalBounds().width;
	t.setString("Rgp");

	StatusuriLocale status = NicioModificare;
	//circuit->
	
	Vector2f dimButon, dimTextBox;
	char coefScalare[MAXNUMAR] =  "0", distantaMiscare[MAXNUMAR] = "0", unghiRotire[MAXNUMAR] = "0";
	floatToCharArr(circuit->optiuni.coefScalare, coefScalare);
	_itoa(circuit->optiuni.distantaMiscare, distantaMiscare, 10);
	_itoa(circuit->optiuni.unghiRotire, unghiRotire, 10);


	unsigned inaltimeFereastra = 250;
	//t.setString("123456789012345678901234567890");
	int test = t.getGlobalBounds().width;
	unsigned latimeMaxCasetaText = t.getGlobalBounds().width * MAXNUMAR / 3 + 2 * OFFSET;
	unsigned latimeFereastra = 250 + latimeMaxNumeCamp + 2 * OFFSET;
	sf::RenderWindow window(sf::VideoMode(latimeFereastra, inaltimeFereastra, 32), "Setari", sf::Style::Close);
	window.setFramerateLimit(30);
	window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());

	sf::String ErrMessage;
	bool running = true;

	int inaltimeCastaText = t.getLocalBounds().height;
	RectangleShape butonSalvare;
	RectangleShape textBox;
	textBox.setOutlineColor(Color::White);
	textBox.setOutlineThickness(1);
	textBox.setFillColor(Color::Transparent);

	dimTextBox.x = latimeMaxCasetaText;
	dimTextBox.y = inaltimeCastaText + OFFSET / 2;
	textBox.setSize(dimTextBox);
	char init = '_';
	RectangleShape partratAlb;
	RectangleShape patratRosu;
	RectangleShape patratVerde;
	Optiuni optiuniModificate;
	if (circuit->optiuni.culoareConexiuni == Color::Red)
	{
		patratRosu.setOutlineColor(Color::Blue);
		patratRosu.setOutlineThickness(3);
		partratAlb.setOutlineThickness(0);
		patratVerde.setOutlineThickness(0);
	}
	else if (circuit->optiuni.culoareConexiuni == Color::White)
	{
		partratAlb.setOutlineColor(Color::Blue);
		partratAlb.setOutlineThickness(3);
		patratRosu.setOutlineThickness(0);
		patratVerde.setOutlineThickness(0);
	}
	else if (circuit->optiuni.culoareConexiuni == Color::Green)
	{
		patratVerde.setOutlineColor(Color::Blue);
		patratVerde.setOutlineThickness(3);
		partratAlb.setOutlineThickness(0);
		patratRosu.setOutlineThickness(0);
	}
	optiuniModificate.culoareConexiuni = circuit->optiuni.culoareConexiuni;
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
				if (verificareInteriorPatrat(patratRosu, coordonateMouse))
				{
					optiuniModificate.culoareConexiuni = Color::Red;
					patratRosu.setOutlineColor(Color::Blue);
					patratRosu.setOutlineThickness(3);
					partratAlb.setOutlineThickness(0);
					patratVerde.setOutlineThickness(0);
				}
				else if (verificareInteriorPatrat(partratAlb, coordonateMouse))
				{
					optiuniModificate.culoareConexiuni = Color::White;
					partratAlb.setOutlineColor(Color::Blue);
					partratAlb.setOutlineThickness(3);
					patratRosu.setOutlineThickness(0);
					patratVerde.setOutlineThickness(0);
				}
				else if (verificareInteriorPatrat(patratVerde, coordonateMouse))
				{
					optiuniModificate.culoareConexiuni = Color::Green;
					patratVerde.setOutlineColor(Color::Blue);
					patratVerde.setOutlineThickness(3);
					partratAlb.setOutlineThickness(0);
					patratRosu.setOutlineThickness(0);
				}
				else if (verificareInteriorPatrat(butonSalvare, coordonateMouse))
				{
					if (status == ModificareCoefScalare)
						coefScalare[strlen(coefScalare) - 1] = '\0';
					else if (status == ModificareDistantaMiscare)
						distantaMiscare[strlen(distantaMiscare) - 1] = '\0';
					else if (status == ModificareUnghiRotireText)
						unghiRotire[strlen(unghiRotire) - 1] = '\0';

					
					optiuniModificate.coefScalare = atof(coefScalare);
					optiuniModificate.distantaMiscare = atof(distantaMiscare);
					optiuniModificate.unghiRotire= atof(unghiRotire);
					if(salvareDate(circuit, optiuniModificate, numeOptiuniSalvate))
						window.close();
					status = NicioModificare;
				}
				else if (coordonateMouse.x >= latimeMaxNumeCamp + OFFSET && coordonateMouse.x <= latimeMaxNumeCamp + OFFSET + latimeMaxCasetaText)
				{
					if (coordonateMouse.y >= OFFSET / 2)
					{
						if (coordonateMouse.y < inaltimeCastaText + OFFSET) //caseta coefScalare
						{
							if (status == ModificareDistantaMiscare)
								distantaMiscare[strlen(distantaMiscare) - 1] = '\0';
							else if (status == ModificareUnghiRotireText)
								unghiRotire[strlen(unghiRotire) - 1] = '\0';

							if (status != ModificareCoefScalare)
							{
								unsigned lungime = strlen(coefScalare);
								coefScalare[lungime] = init;
								coefScalare[lungime + 1] = '\0';
							}
							status = ModificareCoefScalare;
						}
						else if (coordonateMouse.y < inaltimeCastaText * 2 + 2 * OFFSET) //caseta distantaMiscare
						{
							if (status == ModificareCoefScalare)
								coefScalare[strlen(coefScalare) - 1] = '\0';
							else if (status == ModificareUnghiRotireText)
								unghiRotire[strlen(unghiRotire) - 1] = '\0';

							if (status != ModificareDistantaMiscare)
							{
								unsigned lungime = strlen(distantaMiscare);
								distantaMiscare[lungime] = init;
								distantaMiscare[lungime + 1] = '\0';
							}
							status = ModificareDistantaMiscare;
						}
						else if (coordonateMouse.y < inaltimeCastaText * 3 + 3 * OFFSET) //caseta unghiRotire
						{
							if (status == ModificareCoefScalare)
								coefScalare[strlen(coefScalare) - 1] = '\0';
							else if (status == ModificareDistantaMiscare)
								distantaMiscare[strlen(distantaMiscare) - 1] = '\0';

							if (status != ModificareUnghiRotireText)
							{
								unsigned lungime = strlen(unghiRotire);
								unghiRotire[lungime] = init;
								unghiRotire[lungime + 1] = '\0';
							}
							status = ModificareUnghiRotireText;
						}
						else
						{

							if (status == ModificareCoefScalare)
								coefScalare[strlen(coefScalare) - 1] = '\0';
							else if (status == ModificareDistantaMiscare)
								distantaMiscare[strlen(distantaMiscare) - 1] = '\0';
							else if (status == ModificareUnghiRotireText)
								unghiRotire[strlen(unghiRotire) - 1] = '\0';
							status = NicioModificare;
						}
					}
				}
				else
				{

					if (status == ModificareCoefScalare)
						coefScalare[strlen(coefScalare) - 1] = '\0';
					else if (status == ModificareDistantaMiscare)
						distantaMiscare[strlen(distantaMiscare) - 1] = '\0';
					else if (status == ModificareUnghiRotireText)
						unghiRotire[strlen(unghiRotire) - 1] = '\0';
					status = NicioModificare;
				}

			}
			if (event.type == sf::Event::TextEntered)
			{
				if (status == ModificareCoefScalare)
				{
					unsigned lungimeCoefScalare = strlen(coefScalare);
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && lungimeCoefScalare > 1)
					{
						coefScalare[lungimeCoefScalare - 2] = init;
						coefScalare[lungimeCoefScalare - 1] = '\0';
					}
					else if (lungimeCoefScalare < MAXNUMAR -1)
					{
						char c = (char)(event.key.code);
						if (esteCifraSetari(event.key.code) ||  (c == '.' && strchr(coefScalare, c) == NULL))
						{
							
							coefScalare[lungimeCoefScalare - 1] = c;
							coefScalare[lungimeCoefScalare] = init;
							coefScalare[lungimeCoefScalare + 1] = '\0';
						}
					}
				}
				else if (status == ModificareDistantaMiscare)
				{
					unsigned lungimeDistantaMiscare = strlen(distantaMiscare);
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && lungimeDistantaMiscare > 1)
					{
						distantaMiscare[lungimeDistantaMiscare - 2] = init;
						distantaMiscare[lungimeDistantaMiscare - 1] = '\0';
					}
					else if (lungimeDistantaMiscare < MAXNUMAR - 1 && esteCifraSetari(event.key.code))
					{
						char c = (char)(event.key.code);
						distantaMiscare[lungimeDistantaMiscare - 1] = c;
						distantaMiscare[lungimeDistantaMiscare] = init;
						distantaMiscare[lungimeDistantaMiscare + 1] = '\0';
					}
				}
					else if (status == ModificareUnghiRotireText)
					{
						unsigned lungimeUnghiRotire = strlen(unghiRotire);
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && lungimeUnghiRotire > 1)
						{
							unghiRotire[lungimeUnghiRotire - 2] = init;
							unghiRotire[lungimeUnghiRotire - 1] = '\0';
						}
						else if (lungimeUnghiRotire < MAXNUMAR - 1 && esteCifraSetari(event.key.code))
						{
							char c = (char)(event.key.code);
							unghiRotire[lungimeUnghiRotire - 1] = c;
							unghiRotire[lungimeUnghiRotire] = init;
							unghiRotire[lungimeUnghiRotire + 1] = '\0';
						}
					}
			}

		}
		textBox.setPosition(0, 0);
		transformable.setPosition(0, 0);
		transformableValoare.setPosition(0, 0);

		window.clear(sf::Color::Black);
		t.setString(numeOptiuniSalvate[0]);

		window.draw(t, transformable.getTransform());
		t.setString(coefScalare);
		transformableValoare.move(latimeMaxNumeCamp + OFFSET, OFFSET / 5);
		textBox.move(latimeMaxNumeCamp + OFFSET, OFFSET / 2);
		window.draw(textBox);
		window.draw(t, transformableValoare.getTransform());
		transformable.move(0, inaltimeCastaText + OFFSET);

		t.setString(numeOptiuniSalvate[1]);
		window.draw(t, transformable.getTransform());
		t.setString(distantaMiscare);
		transformableValoare.move(0, inaltimeCastaText + OFFSET);
		textBox.move(0, inaltimeCastaText + OFFSET);

		window.draw(textBox);
		window.draw(t, transformableValoare.getTransform());
		transformable.move(0, inaltimeCastaText + OFFSET);

			t.setString(numeOptiuniSalvate[2]);
			window.draw(t, transformable.getTransform());
			textBox.move(0, inaltimeCastaText + OFFSET);
			transformableValoare.move(0, inaltimeCastaText + OFFSET);
			t.setString(unghiRotire);
			window.draw(t, transformableValoare.getTransform());
			window.draw(textBox);
			transformable.move(0, inaltimeCastaText + OFFSET);

			t.setString(numeOptiuniSalvate[3]);
			window.draw(t, transformable.getTransform());
			transformableValoare.move(0, inaltimeCastaText + 2*OFFSET);
			transformable.move(0, inaltimeCastaText + 2*OFFSET);

			Vector2f dimensiunePatrate(50, 50);
			Vector2f pozitiePatrate = transformableValoare.getPosition();
			
			partratAlb.setSize(dimensiunePatrate);
			partratAlb.setFillColor(Color::White);
			partratAlb.setPosition(pozitiePatrate);
			window.draw(partratAlb);

			pozitiePatrate.x += 100;
			patratRosu.setSize(dimensiunePatrate);
			patratRosu.setFillColor(Color::Red);
			patratRosu.setPosition(pozitiePatrate);
			window.draw(patratRosu);

			pozitiePatrate.x += 100;
			patratVerde.setSize(dimensiunePatrate);
			patratVerde.setFillColor(Color::Green);
			patratVerde.setPosition(pozitiePatrate);
			window.draw(patratVerde);

		t.setString("Salvare");
		transformable.move((latimeFereastra - t.getLocalBounds().width) / 2, inaltimeCastaText);

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