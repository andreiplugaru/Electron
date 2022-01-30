#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Structuri.h"
#include "Defineuri.h"
#include "Stiva.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
enum Mod  { Salvare, Deschidere};
void FereastraEroare(char mesajEroare[])
{
    Image image;
    image.loadFromFile("icons\\eroare.png");
    
    sf::Font font;
    font.loadFromFile("fonts\\RobotoSlab-Font.ttf");
    sf::Text t;
    t.setFillColor(sf::Color::White);
    t.setFont(font);
    t.setString(mesajEroare);

    sf::RenderWindow ErrWindow(sf::VideoMode(t.getLocalBounds().width+10, t.getLocalBounds().height + 10, 32), "Eroare!", sf::Style::Close);
    ErrWindow.setFramerateLimit(30);
    ErrWindow.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());

    while (ErrWindow.isOpen())
    {
        sf::Event event;

        while (ErrWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
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
    image.loadFromFile("icons\\succes.png");

    sf::Font font;
    font.loadFromFile("fonts\\RobotoSlab-Font.ttf");
    sf::Text t;
    t.setFillColor(sf::Color::White);
    t.setFont(font);
    t.setString(mesajSucces);

    sf::RenderWindow ErrWindow(sf::VideoMode(t.getLocalBounds().width + 10, t.getLocalBounds().height + 10, 32), "Succes!", sf::Style::Close);
    ErrWindow.setFramerateLimit(30);
    ErrWindow.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
  
    while (ErrWindow.isOpen())
    {
        sf::Event event;

        while (ErrWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
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
    if (!font.loadFromFile("fonts\\RobotoSlab-Font.ttf"))
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
    image.loadFromFile("icons\\folder.png");
    sf::RenderWindow window(sf::VideoMode(500, 40), "Inserare nume fisier(fara extensie)", Style::Close);
    window.setFramerateLimit(30);
    window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
   
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::TextEntered)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
                {
                    if (i > 0)
                    {
                        nume[i - 1] = init;
                        nume[i] = '\0';
                        i--;
                    }
                }
                else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                {
                    if (i < MAXNUME-2)
                    {
                        nume[i] = (char)(event.key.code);
                        nume[i + 1] = init;
                        nume[i + 2] = '\0';
                        i++;
                    }
                }
                else
                {
                    nume[i] = '\0';
                    window.close();
                }
            }
        }
        window.clear();
        t.setString(nume);
        window.draw(t);
        window.display();
    }
    
    if (i > 0)
    {
        return true;
    }
    return false;
}
void salvareCircuit(Circuit* circuit, char adresaFisier[], bool afisareMesaj)
{
    char numeFisier[MAXNUME];
    if (adresaFisier[0] == '\0' && deschidereFereastra(Salvare, numeFisier))
    {
        strcat(adresaFisier, ADRESA_SALVARI);
        strcat(adresaFisier, numeFisier);
        strcat(adresaFisier, ".bin");

    }
        FILE* f = fopen(adresaFisier, "wb");
        if (f != NULL)
        {
            fwrite(&circuit->nrComponente, sizeof(unsigned int), 1, f);
            fwrite(circuit->componente, sizeof(Componenta), circuit->nrComponente, f);
            fwrite(&circuit->nrLegaturi, sizeof(unsigned int), 1, f);
            fwrite(circuit->legaturi, sizeof(Legatura), circuit->nrLegaturi, f);
            fwrite(&circuit->sizeCoef, sizeof(float), 1, f);
            fclose(f);
            if (afisareMesaj)
            {
                char mesaj[] = "Fiser salvat cu succes!";
                FereastraSucces(mesaj);
            }
            
        }
        else
        {
            adresaFisier[0] = '\0';
            char mesaj[] = "Eroare la salvarea fisierului!";
            FereastraEroare(mesaj);
        }
}
bool deschidereCircuit(Circuit* circuit, char adresaFisier[], bool afisareMesaj)
{
    char numeFisier[MAXNUME];
    if (adresaFisier[0] == '\0' && deschidereFereastra(Deschidere, numeFisier))
    {
        strcat(adresaFisier, ADRESA_SALVARI);
        strcat(adresaFisier, numeFisier);
        strcat(adresaFisier, ".bin");
       
    }

    FILE* f = fopen(adresaFisier, "rb");
    if (f != NULL)
    {
        fread(&circuit->nrComponente, sizeof(unsigned int), 1, f);
        fread(circuit->componente, sizeof(Componenta), circuit->nrComponente, f);
        fread(&circuit->nrLegaturi, sizeof(unsigned int), 1, f);
        fread(circuit->legaturi, sizeof(Legatura), circuit->nrLegaturi, f);
        fread(&circuit->sizeCoef, sizeof(float), 1, f);
        
        fclose(f);

        if (afisareMesaj)
        {
            char mesaj[] = "Fisier deschis cu succes!";
            FereastraSucces(mesaj);
            return true;
        }
    }
    else
    {
        adresaFisier[0] = '\0';
        char mesaj[] = "Eroare la deschiderea fisierului!";
        FereastraEroare(mesaj);
        return false;
    }
    
}
void salvareIstoricRedo(Circuit* circuit)
{
    if (circuit->topUndo->data != 1 && circuit->topUndo->data != 0 && circuit->topRedo != circuit->topUndo)
    {
        int nr = prim(circuit->topUndo);
     
        push(circuit->topRedo, nr);
      
    }
}
void deschidereIstoricRedo(Circuit* circuit)
{
    if (!esteVida(circuit->topRedo))
    {
        char numarFisier[MAX_NUMAR_STIVA];
        char nume[MAXNUME] = "istoric\\temp";
       
        _itoa(prim(circuit->topRedo), numarFisier, 10);

        strcat(nume, numarFisier);
        strcat(nume, ".bin");
        deschidereCircuit(circuit, nume, false);
        pop(circuit->topRedo);
        circuit->componentaSelectata = -1;
    }
}
int max(int n1, int n2)
{
    return n1 > n2 ? n1 : n2;
}
void salvareIstoric(Circuit* circuit)
{
    char numarFisier[MAX_NUMAR_STIVA];
    char nume[MAXNUME] = "istoric\\temp";
    int nr1 = maxStiva(circuit->topUndo);
    int nr2 = maxStiva(circuit->topRedo);
    _itoa(max(nr1, nr2) + 1, numarFisier, 10);
    strcat(nume, numarFisier);
    strcat(nume, ".bin");
    salvareCircuit(circuit, nume, false);
    push(circuit->topUndo, max(nr1, nr2) + 1);
    circuit->esteSalvat = false;
}
void exporta(RenderWindow &window)
{
    char adresaCompleta[MAX_LUNGIME_FISIER] = ADRESA_SCREENS;
    char numeFisier[MAXNUME];
    char extensie[] = ".png";
    
    sf::Texture texture;
    texture.create(window.getSize().x, window.getSize().y);
    texture.update(window);
    if (deschidereFereastra(Salvare, numeFisier))
    {
        strcat(numeFisier, extensie);
        strcat(adresaCompleta, numeFisier);
        if (texture.copyToImage().saveToFile(adresaCompleta))
        {
            char mesaj[] = "Fisier exportat cu succes!";
            FereastraSucces(mesaj);
        }
        else
        {
            char mesaj[] = "Eroare la salvarea fisierului!";
            FereastraEroare(mesaj);
        }
    }
    else
    {
        char mesaj[] = "Eroare la salvarea fisierului!";
        FereastraEroare(mesaj);
    }
}
void deschidereIstoric(Circuit* circuit)
{
    if (!areDoarPrimElement(circuit->topUndo))
    {
        char numarFisier[MAX_NUMAR_STIVA];
        char nume[MAXNUME] = "istoric\\temp";
        pop(circuit->topUndo);
        _itoa(prim(circuit->topUndo), numarFisier, 10);
        strcat(nume, numarFisier);
        strcat(nume, ".bin");
        deschidereCircuit(circuit, nume, false);   
        circuit->componentaSelectata = -1;
    } 
}
void stergereIstoric(Circuit* circuit)
{
    while (circuit->topUndo != NULL)
    {
        char numarFisier[MAX_NUMAR_STIVA];
        char nume[MAXNUME] = "istoric\\temp";
        pop(circuit->topUndo);
        _itoa(prim(circuit->topUndo), numarFisier, 10);
        strcat(nume, numarFisier);
        strcat(nume, ".bin");
        remove(nume);     
    }
    while (circuit->topRedo != NULL)
    {
        char numarFisier[MAX_NUMAR_STIVA];
        char nume[MAXNUME] = "istoric\\temp";
        _itoa(prim(circuit->topRedo), numarFisier, 10);
        strcat(nume, numarFisier);
        strcat(nume, ".bin");
        remove(nume);
        pop(circuit->topRedo);
    }
}
