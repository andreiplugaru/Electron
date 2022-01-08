#include <SFML/Graphics.hpp>
#include "Defineuri.h"
#include "Structuri.h"
#include "MediuLucru.h"
#include "Circuit.h"
#include "Ajutor.h"
using namespace sf;
using namespace std;
char textButoane[NR_BUTOANE_START][30] = { "Deschidere Circuit",
"Circuit Nou",
"Ajutor",
"Iesire",
 };
using namespace sf;
bool verificareInteriorButon(RectangleShape patrat, Vector2i coordonateMouse)
{
    return (coordonateMouse.x >= patrat.getGlobalBounds().left && coordonateMouse.x <= patrat.getGlobalBounds().left + patrat.getGlobalBounds().width
        && coordonateMouse.y >= patrat.getGlobalBounds().top && coordonateMouse.y <= patrat.getGlobalBounds().top + patrat.getGlobalBounds().height);
}
void afisareFereastraStart()
{
	Texture t;
	t.loadFromFile("background.png");
    Vector2f marimeButon;
    marimeButon.x = 300;
    marimeButon.y = 50;
	Sprite s(t);
    sf::Vector2f targetSize(LATIME_FEREASTRA, INALTIME_FEREASTRA);

    s.setScale(
        targetSize.x / s.getLocalBounds().width,
        targetSize.y / s.getLocalBounds().height);
	RenderWindow window(VideoMode(1920, 1080), "ELECTRON", Style::Fullscreen);
    window.setFramerateLimit(60);
    RectangleShape butonDeschidereCircuit;
    butonDeschidereCircuit.setSize(marimeButon);
    butonDeschidereCircuit.setFillColor(Color::Transparent);
    butonDeschidereCircuit.setOutlineColor(Color::White);
    butonDeschidereCircuit.setOutlineThickness(3);

    RectangleShape butonCircuitNou;
    butonCircuitNou.setSize(marimeButon);
    butonCircuitNou.setFillColor(Color::Transparent);
    butonCircuitNou.setOutlineColor(Color::White);
    butonCircuitNou.setOutlineThickness(3);

    RectangleShape butonAjutor;
    butonAjutor.setSize(marimeButon);
    butonAjutor.setFillColor(Color::Transparent);
    butonAjutor.setOutlineColor(Color::White);
    butonAjutor.setOutlineThickness(3);

    RectangleShape butonIesire;
    butonIesire.setSize(marimeButon);
    butonIesire.setFillColor(Color::Transparent);
    butonIesire.setOutlineColor(Color::White);
    butonIesire.setOutlineThickness(3);

    sf::Font font;
    font.loadFromFile("fonts\\RobotoSlab-Font.ttf");
    Text text;
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(34);
    text.setFont(font);
 
    while (window.isOpen()) {
        sf::Event event;
       // text.setPosition(OFFSET, OFFSET);
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) 
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                Vector2i coordonateMouse = Mouse::getPosition(window);
                if (verificareInteriorButon(butonDeschidereCircuit, coordonateMouse))
                {
                    butonDeschidereCircuit.setOutlineColor(Color::Blue);
                    butonCircuitNou.setOutlineColor(Color::White);
                    butonAjutor.setOutlineColor(Color::White);
                    butonIesire.setOutlineColor(Color::White);
                }
                else  if (verificareInteriorButon(butonCircuitNou, coordonateMouse))
                {
                    butonDeschidereCircuit.setOutlineColor(Color::White);
                    butonCircuitNou.setOutlineColor(Color::Blue);
                    butonAjutor.setOutlineColor(Color::White);
                    butonIesire.setOutlineColor(Color::White);
                }
                else  if (verificareInteriorButon(butonAjutor, coordonateMouse))
                {
                    butonDeschidereCircuit.setOutlineColor(Color::White);
                    butonCircuitNou.setOutlineColor(Color::White);
                    butonAjutor.setOutlineColor(Color::Blue);
                    butonIesire.setOutlineColor(Color::White);
                }
                else  if (verificareInteriorButon(butonIesire, coordonateMouse))
                {
                    butonDeschidereCircuit.setOutlineColor(Color::White);
                    butonCircuitNou.setOutlineColor(Color::White);
                    butonAjutor.setOutlineColor(Color::White);
                    butonIesire.setOutlineColor(Color::Blue);
                }
                else
                {
                    butonDeschidereCircuit.setOutlineColor(Color::White);
                    butonCircuitNou.setOutlineColor(Color::White);
                    butonAjutor.setOutlineColor(Color::White);
                    butonIesire.setOutlineColor(Color::White);
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                Vector2i coordonateMouse = Mouse::getPosition(window);
                if (verificareInteriorButon(butonDeschidereCircuit, coordonateMouse))
                {
                    Circuit* circuit = new Circuit; // se aloca dinamic deoarece depasea marimea stack ului
                    circuit->componente = new Componenta[MAX];
                    circuit->legaturi = new Legatura[MAX];
                    circuit->elemente = new ElCircuit[NUMAR_ELEMENTE];
                    circuit->numeFisier[0] = '\0';
                    if (deschidereCircuit(circuit, circuit->numeFisier, true))
                    {
                        window.close();
                        afisareElemente(circuit);
                    }
                }
                if (verificareInteriorButon(butonCircuitNou, coordonateMouse))
                {
                    Circuit* circuit = new Circuit; // se aloca dinamic deoarece depasea marimea stack ului
                    circuit->componente = new Componenta[MAX];
                    circuit->legaturi = new Legatura[MAX];
                    circuit->elemente = new ElCircuit[NUMAR_ELEMENTE];
                    window.close();
                    afisareElemente(circuit);
                }
                else  if (verificareInteriorButon(butonAjutor, coordonateMouse))
                {
                    deschideFereastraAjutor();
                }
                else  if (verificareInteriorButon(butonIesire, coordonateMouse))
                {
                    window.close();
                }
            }
        }
        window.clear(sf::Color::White);
        window.draw(s);
        Vector2f pozitieButon;
        Vector2f pozitieText;
        pozitieButon.x = (LATIME_FEREASTRA - marimeButon.x )/ 2 ;
        pozitieButon.y = 300;

        pozitieText.y = pozitieButon.y;
        text.setString(textButoane[0]);
        pozitieText.x = pozitieButon.x + (marimeButon.x - text.getGlobalBounds().width) / 2;
        text.setPosition(pozitieText);
        window.draw(text);
        butonDeschidereCircuit.setPosition(pozitieButon);


        pozitieButon.y += marimeButon.y + 2 * OFFSET;
        pozitieText.y = pozitieButon.y;
        text.setString(textButoane[1]);
        pozitieText.x = pozitieButon.x + (marimeButon.x - text.getGlobalBounds().width) / 2;
        text.setPosition(pozitieText);
        window.draw(text);
        butonCircuitNou.setPosition(pozitieButon);

        pozitieText.y = pozitieButon.y += marimeButon.y + 2 * OFFSET;
        text.setString(textButoane[2]);
        pozitieText.x = pozitieButon.x + (marimeButon.x - text.getGlobalBounds().width) / 2;
        text.setPosition(pozitieText);
        window.draw(text);
        butonAjutor.setPosition(pozitieButon);

        pozitieText.y = pozitieButon.y += marimeButon.y + 2 * OFFSET;
        text.setString(textButoane[3]);
        pozitieText.x = pozitieButon.x + (marimeButon.x - text.getGlobalBounds().width) / 2;
        text.setPosition(pozitieText);
        window.draw(text);
        butonIesire.setPosition(pozitieButon);

     
        window.draw(butonDeschidereCircuit);
        window.draw(butonCircuitNou);
        window.draw(butonAjutor);
        window.draw(butonIesire);
        window.display();
    }
}
int main()
{
    afisareFereastraStart();
}