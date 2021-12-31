#define _CRT_SECURE_NO_WARNINGS
#include "Structuri.h"
#include "Defineuri.h"
#include <conio.h>
#include <windows.h>
enum Mod  { Salvare, Deschidere};
void FereastraEroare(char mesajEroare[])
{
    Image image;
    image.loadFromFile("eroare.png");
    
    sf::Font font;
    font.loadFromFile("RobotoSlab-Font.ttf");
    sf::Text t;
    t.setFillColor(sf::Color::White);
    t.setFont(font);
    t.setString(mesajEroare);

    sf::RenderWindow ErrWindow(sf::VideoMode(t.getLocalBounds().width+10, t.getLocalBounds().height + 10, 32), "Eroare!", sf::Style::Close);
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
void FereastraSucces(char mesajSucces[])
{
    Image image;
    image.loadFromFile("succes.png");

    sf::Font font;
    font.loadFromFile("RobotoSlab-Font.ttf");
    sf::Text t;
    t.setFillColor(sf::Color::White);
    t.setFont(font);
    t.setString(mesajSucces);

    sf::RenderWindow ErrWindow(sf::VideoMode(t.getLocalBounds().width + 10, t.getLocalBounds().height + 10, 32), "Succes!", sf::Style::Close);
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
bool deschidereFereastra(Mod mod, char nume[])
{
    sf::Font font;
    if (!font.loadFromFile("RobotoSlab-Font.ttf"))
        return false;
    sf::Text t;
    t.setFillColor(sf::Color::White);
    t.setFont(font);
    char init = '_';
    nume[0] = init;
    nume[1] = '\0';
    int i = 0;
    t.setString(nume);
    Image image;
    image.loadFromFile("folder.png");
    sf::RenderWindow window(sf::VideoMode(500, 40), "Inserare nume fisier(fara extensie)", Style::Close);
    window.setFramerateLimit(30);
    window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::TextEntered) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
                {
                    if (i > 0)
                    {
                        nume[i - 1] = init;
                        nume[i] = '\0';
                        i--;
                    }
                }
                else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && i < MAX_LUNGIME_FISIER)
                {
                    nume[i] = (char)(event.key.code);
                    nume[i + 1] = init;
                    nume[i + 2] = '\0';
                    i++;
                }
                else
                {
                    nume[i] = '\0';
                    char extensie[] = ".bin";
                    strcat(nume, extensie);
                    window.close();
                }
            }
        }
        window.clear();
        t.setString(nume);
        window.draw(t);
        window.display();
    }
    if(i > 0)
        return true;
    return false;
}
void salvareCircuit(Componenta* comp, int nrComponente, Legatura* legaturi, int nrLegaturi, char numeFisier[])
{
    if (numeFisier[0] != '\0' || deschidereFereastra(Salvare, numeFisier))
    {
       
        FILE* f = fopen(numeFisier, "wb");
        if (f != NULL)
        {
            fwrite(&nrComponente, sizeof(int), 1, f);
            fwrite(comp, sizeof(Componenta), nrComponente, f);
            fwrite(&nrLegaturi, sizeof(int), 1, f);
            fwrite(legaturi, sizeof(Legatura), nrLegaturi, f);
            fclose(f);
            char mesaj[] = "Fiser salvat cu succes!";
            FereastraSucces(mesaj);
        }
        else
        {
            numeFisier[0] = '\0';
            char mesaj[] = "Eroare la deschiderea fisierului!";
            FereastraEroare(mesaj);
        }
    }
    else
    {
        char mesaj[] = "Nume incorect!";
        FereastraEroare(mesaj);
    }
    
}
void deschidereCircuit(Componenta *comp, int* nrComponente, Legatura* legaturi, int* nrLegaturi, char numeFisier[])
{
    if (deschidereFereastra(Deschidere, numeFisier))
    {
        FILE* f = fopen(numeFisier, "rb");
        if (f != NULL)
        {
            fread(nrComponente, sizeof(int), 1, f);
            fread(comp, sizeof(Componenta), *nrComponente, f);
            fread(nrLegaturi, sizeof(int), 1, f);
            fread(legaturi, sizeof(Legatura), *nrLegaturi, f);
            fclose(f);
            char mesaj[] = "Fisier deschis cu succes!";
            FereastraSucces(mesaj);
        }
        else
        {
            numeFisier[0] = '\0';
            char mesaj[] = "Eroare la deschiderea fisierului!";
            FereastraEroare(mesaj);
        }
    }
    else
    {
        char mesaj[] = "Nume incorect!";
        FereastraEroare(mesaj);
    }
}
