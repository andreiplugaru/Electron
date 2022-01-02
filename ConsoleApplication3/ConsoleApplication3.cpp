#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <math.h>
#include "Conexiuni.h"
#include "Structuri.h"
#include "Circuit.h"
#include "Meniu.h"
#include "Parametri.h"
#include "Ajutor.h"
char locatieFisiere[13][20] = { "PS\\BATERIE.PS","PS\\AMPLOP.PS" ,"PS\\CONDENS.PS","PS\\DIODA.PS","PS\\NOD.PS","PS\\POLARIZ.PS","PS\\REZIST.PS","PS\\SERVOMOT.PS","PS\\SINU.PS","PS\\TRANZNPN.PS","PS\\TRANZPNP.PS","PS\\ZENNER.PS","PS\\STOP.PS" };
using namespace sf;
using namespace std;

string statusCoordonate;
float latimeCasuta;
Optiuni optiuni;
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
    //fscanf(f, "%s", denumire);
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
    fgets(element.denumire, 30, f);
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
    cout << element.nrLinii << endl;
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
        if (tip == 'A')//arc
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
        if (tip == 'O')//elipsa
        {
            Vector2f pozitie;
            float rx, ry, x, y, raza, scalex = 1, scaley = 1;
            fscanf(f, "%f", &pozitie.x);
            fscanf(f, "%f", &pozitie.y);
            pozitie.x = (pozitie.x  + abs(extreme.minim.x)) * SIZECOEFINITIAL;
            pozitie.y = (pozitie.y + abs(extreme.minim.y)) * SIZECOEFINITIAL;

            fscanf(f, "%f", &rx);
            fscanf(f, "%f", &ry);
            if (ry > rx)
            {
                raza = rx;
                scaley =  ry / rx;
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
            element.elipsa.setOutlineThickness(3);
          //  element.dimensiuni.y = 2 * raza * SIZECOEFINITIAL;
        }
        if (tip == 'R')//rectangle
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
           // element.dreptunghi.setOrigin(dimensiuni.x/2, dimensiuni.y/2);
            element.dreptunghi.setFillColor(Color::Transparent);
            element.dreptunghi.setOutlineThickness(2);
            element.dreptunghi.setSize(dimensiuni);

            if (dimensiuni.x > element.dimensiuni.x)
                element.dimensiuni.x = dimensiuni.x;

            if (dimensiuni.y > element.dimensiuni.y)
                element.dimensiuni.y = dimensiuni.y;
            

        }
    }
    
    element.origine.x = element.dimensiuni.x / 2;
    element.origine.y = element.dimensiuni.y / 2;
    element.chenar.setFillColor(Color::Transparent);
    element.chenar.setOutlineThickness(optiuni.latimeChenar);
    element.chenar.setOutlineColor(Color::Blue);
    element.chenar.setSize(Vector2f(element.dimensiuni.x, element.dimensiuni.y));
    fclose(f);
    return element;

}
PunctMinMax puncteMinimSiMaxim(Vector2f puncte[], int nrPuncte)
{
    PunctMinMax coordonate = {0};
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

void coordonateDupaRotire(RectangleShape patrat, Punct* Min, Punct* Max, Transform tp)
{
    Vector2f p1, p2, p3, p4;
    FloatRect patrat2;

    patrat2 = tp.transformRect(patrat.getGlobalBounds());
    p1.x = patrat2.left;
    p1.y = patrat2.top;

    p2.x = patrat2.left + patrat2.width;
    p2.y = patrat2.top;

    p3.x = patrat2.left + patrat2.width;
    p3.y = patrat2.top + patrat2.height;

    p4.x = patrat2.left;
    p4.y = patrat2.top + patrat2.height;

    (*Max).x = p1.x;

    if (p2.x > (*Max).x)
    {
        (*Max).x = p2.x;
    }
    if (p3.x > (*Max).x)
    {
        (*Max).x = p3.x;
    }
    if (p4.x > (*Max).x)
    {
        (*Max).x = p4.x;
    }

    (*Min).x = p1.x;
    if (p2.x < (*Min).x)
    {
        (*Min).x = p2.x;
    }
    if (p3.x < (*Min).x)
    {
        (*Min).x = p3.x;
    }
    if (p4.x < (*Min).x)
    {
        (*Min).x = p4.x;
    }
    (*Max).y = p1.y;
    if (p2.y > (*Max).y)
    {
        (*Max).y = p2.y;
    }
    if (p3.y > (*Max).y)
    {
        (*Max).y = p3.y;
    }
    if (p4.y > (*Max).y)
    {
        (*Max).y = p4.y;
    }

    (*Min).y = p1.y;
    if (p2.y < (*Min).y)
    {
        (*Min).y = p2.y;
    }
    if (p3.y < (*Min).y)
    {
        (*Min).y = p3.y;
    }
    if (p4.y < (*Min).y)
    {
        (*Min).y = p4.y;
    }
}

Vector2f ieseDinFereastra(Componenta comp, ElCircuit element)// diferenta dintre coorodnate compnenta si marginea intersectatta
{
    Punct max, min;
    coordonateDupaRotire(element.chenar, &min, &max, comp.transformari.getTransform());
   
    Vector2f coordonateInAfara;
    //coordonateChenar = comp.transformari.getTransform().transformRect(comp.chenar.getLocalBounds());
    if (min.y < INALTIME_MENIU_ELEMENTE + INALTIME_MENIU_OPTIUNI)
        coordonateInAfara.y = INALTIME_MENIU_ELEMENTE + INALTIME_MENIU_OPTIUNI - min.y;
     
    if (min.x < 0)
        coordonateInAfara.x = 0 - min.x;

    if (max.y > INALTIME_FEREASTRA)
        coordonateInAfara.y = INALTIME_FEREASTRA - max.y;

    if (max.x > LATIME_FEREASTRA)
        coordonateInAfara.x = LATIME_FEREASTRA - max.x;

   // statusCoordonate = "y:" + to_string(coordonateChenar.top) + ", " + to_string(coordonateChenar.top + coordonateChenar.height) + "\n x:" + to_string(coordonateChenar.left) + ", " + to_string(coordonateChenar.left + coordonateChenar.width);

    return coordonateInAfara;
}
/*void transformareComponenta(Componenta* comp, ElCircuit element, int nrLegaturi, int idComponenta, Legatura* legaturi, Transformable t)
{
    Vector2f coordonateInafaraFereastra = ieseDinFereastra(comp[idComponenta], element);
    if (coordonateInafaraFereastra.x == 0 && coordonateInafaraFereastra.y == 0)
    {

        modificareConexiuni(coordonateInafaraFereastra, nrLegaturi, legaturi, idComponenta, t.getTransform());
    }
    else
        (*comp).transformari.move(coordonateInafaraFereastra);

}*/
VertexArray scalareLinii(VertexArray linii, float coef)
{
    VertexArray liniiModificate(Lines);
    for (int i = 0; i < linii.getVertexCount(); i++)
    {
        Vertex mod;
        mod.position.x = linii[i].position.x * coef;
        mod.position.y = linii[i].position.y * coef;
        liniiModificate.append(mod);
    }
    return liniiModificate;
}
void desenareMeniuElemente(RenderWindow& window, ElCircuit elemente[], int nrElemente)
{
    float offsetX = 10;
    float scale = 0.5;
   
    latimeCasuta = (1920 - (nrElemente + 1)*10) / nrElemente;
    Vector2f dimCasuta;
    dimCasuta.x = latimeCasuta;
    dimCasuta.y = INALTIME_MENIU_ELEMENTE;
    for (int i = 0; i < 12; i++)
    {
        Transformable t;
        RectangleShape casuta;
        ElCircuit elementLocal = elemente[i];
        sf::Color albastru(0, 102, 204);
        casuta.setPosition(latimeCasuta * i + 10 * (i + 1), INALTIME_MENIU_OPTIUNI);
        casuta.setSize(dimCasuta);
        casuta.setFillColor(albastru);
       window.draw(casuta);
     
       Vertex pCentru;
       pCentru.position = elemente[i].origine;
       pCentru.color = Color::Red;
       
        t.setScale(scale, scale);
        t.move(latimeCasuta * i +  (i + 1)*10  - elemente[i].origine.x*scale + latimeCasuta/2, INALTIME_MENIU_OPTIUNI - elemente[i].origine.y * scale + 50 / 2);

        window.draw(&pCentru,1, Points, t.getTransform());
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
void resetareDateComponenta(Componenta* comp)
{
    (*comp) = { };
}
void stergereLegaturi(Legatura legaturi[], unsigned int idComponenta, int *nrLegaturi)
{
    int i = 0;
    while(i < *nrLegaturi)
    {
        if (legaturi[i].idComponenta1 == idComponenta || legaturi[i].idComponenta2 == idComponenta)
        {
            for (int j = i; j < (*nrLegaturi) - 1; j++)

                legaturi[j] = legaturi[j + 1];
            (*nrLegaturi)--;
        }
        else
            i++;
    }
}
void stergereComponenta(Componenta comp[], unsigned int nrComponente, unsigned int idComponenta)
{
    for (int i = idComponenta; i < nrComponente - 1; i++)
    {
        comp[i] = comp[i + 1];
    }
}
/*void setareChenar(Componenta* c, ElCircuit tipElement)
{
    (*c).chenar.setFillColor(Color::Transparent);
    (*c).chenar.setOutlineThickness(optiuni.latimeChenar);
    (*c).chenar.setOutlineColor(Color::Transparent);
    (*c).chenar.setSize(Vector2f(tipElement.dimensiuni.x, tipElement.dimensiuni.y));
}*/

/*void afisareStatus(RenderWindow* w, Componenta status)
{
    sf::Font font;
    if (!font.loadFromFile("Electrolize-Regular.ttf"))
        cout << "ERROR";
    sf::Text text;
    text.setFont(font);
    if (status == DesenareLinie)
        text.setString("DesenareLinie");
    else if (status == Normal)
        text.setString("Normal");
    else if (status == InserareElement)
        text.setString("InserareElement");
    else if (status == InteractiuneComponenta)
        text.setString("InteractiuneComponenta");

    text.setCharacterSize(24);
    (*w).draw(text);
}*/
bool verificareColiziune(Componenta componete[], int compId, int nrComponente, ElCircuit elemente[])
{
    ElCircuit elementVerificat = elemente[componete[compId].idElement];
    for (int i = 0; i < nrComponente; i++)
    {
        if (compId != i)
        {
            ElCircuit elementCurent = elemente[componete[i].idElement];
            if (componete[i].transformari.getTransform().transformRect(elementCurent.chenar.getGlobalBounds()).intersects(
                componete[compId].transformari.getTransform().transformRect(elementVerificat.chenar.getGlobalBounds())))
            {
                cout << "Coliziune";
                return true;
            }
        }
    }
    return false;
}
void miscareTastatura(Componenta componete[], int compId, int nrComponente, ElCircuit elemente[], int nrLegaturi, Legatura legaturi[], float offsetx, float offsety)
{
    Transformable transformPunctConexiune;
    componete[compId].transformari.move(offsetx, offsety);
    if (verificareColiziune(componete, compId, nrComponente, elemente) == false)
    {
        transformPunctConexiune.move(offsetx, offsety);
        Vector2f coordonateInafaraFereastra = ieseDinFereastra(componete[compId], elemente[componete[compId].idElement]);
        if (abs(coordonateInafaraFereastra.x) < optiuni.distantaMiscare && abs(coordonateInafaraFereastra.y) < optiuni.distantaMiscare)
            modificareConexiuni(coordonateInafaraFereastra, nrLegaturi, legaturi, compId, transformPunctConexiune.getTransform());

        componete[compId].transformari.move(coordonateInafaraFereastra);
    }
    else
    {
        componete[compId].transformari.move(-offsetx, -offsety);
    }
}
void afisareElemente()
{
    int nrComponenteInserate = 0;
    int nrLegaturi = 0;
    char numeFisier[MAX_LUNGIME_FISIER] = {};
    optiuni.distantaMiscare = 50;
    Componenta comp[MAX];
    Legatura legaturi[MAX];
    RenderWindow window(VideoMode(1920, 1080), "ELECTRON", Style::Fullscreen);
    window.setFramerateLimit(60);
    Statusuri status = Normal; 
   
    optiuni.latimeChenar = 5;
    optiuni.coefScalare = 0.8;
    optiuni.distantaMiscare = 50;
    optiuni.unghiRotire = 10;
    ElCircuit elemente[NUMAR_ELEMENTE];

     for (int i = 0; i < 12; i++) 
         elemente[i] = citireInformatii(locatieFisiere[i]);
    
    VertexArray conexiuni(Lines);
    ComponentaSelectata compSelectata;
    sf::Font font;
    if (!font.loadFromFile("fonts\\RobotoSlab-Font.ttf"))
        cout << "ERROR";
    sf::Text text;
    text.setFont(font);
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {

            case sf::Event::KeyPressed:
                if (Keyboard::isKeyPressed(sf::Keyboard::S) && Keyboard::isKeyPressed(sf::Keyboard::LControl))
                {
                    salvareCircuit(comp, nrComponenteInserate, legaturi, nrLegaturi, numeFisier);
                }
                else if (Keyboard::isKeyPressed(sf::Keyboard::O) && Keyboard::isKeyPressed(sf::Keyboard::LControl))
                {
                    deschidereCircuit(comp, &nrComponenteInserate, legaturi, &nrLegaturi, numeFisier);
                }
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::E && compSelectata.id != -1)
                {
                    status = ModificareParametri;
                }
                else if (event.key.code == sf::Keyboard::Delete)
                {
                    stergereComponenta(comp, nrComponenteInserate, compSelectata.id);
                    stergereLegaturi(legaturi, compSelectata.id, &nrLegaturi);
                    nrComponenteInserate--;
                    status = Normal;
                }
                if (compSelectata.id == -1) break;
                if (event.key.code == sf::Keyboard::Right) {
                    
                    miscareTastatura(comp, compSelectata.id, nrComponenteInserate, elemente, nrLegaturi, legaturi, optiuni.distantaMiscare, 0);
                }
                else if (event.key.code == sf::Keyboard::Left) {
                    miscareTastatura(comp, compSelectata.id, nrComponenteInserate, elemente, nrLegaturi, legaturi, -optiuni.distantaMiscare, 0);

                }
                else if (event.key.code == sf::Keyboard::Up) {
                    miscareTastatura(comp, compSelectata.id, nrComponenteInserate, elemente, nrLegaturi, legaturi, 0, -optiuni.distantaMiscare);

                }
                else if (event.key.code == sf::Keyboard::Down) {
                    miscareTastatura(comp, compSelectata.id, nrComponenteInserate, elemente, nrLegaturi, legaturi, 0, optiuni.distantaMiscare);

                }
                else if (event.key.code == sf::Keyboard::R) {
                    Transform transformPunctConexiune;
                    comp[compSelectata.id].transformari.rotate(optiuni.unghiRotire);
                    if (verificareColiziune(comp, compSelectata.id, nrComponenteInserate, elemente) == false)
                    {
                        transformPunctConexiune.rotate(optiuni.unghiRotire, comp[compSelectata.id].transformari.getTransform().transformPoint(elemente[comp[compSelectata.id].idElement].origine));
                        Vector2f coordonateInafaraFereastra = ieseDinFereastra(comp[compSelectata.id], elemente[comp[compSelectata.id].idElement]);
                        if (coordonateInafaraFereastra.x == 0 && coordonateInafaraFereastra.y == 0)
                        {
                            modificareConexiuni(coordonateInafaraFereastra, nrLegaturi, legaturi, compSelectata.id, transformPunctConexiune);
                        }
                        else
                            comp[compSelectata.id].transformari.rotate(-optiuni.unghiRotire);
                    }
                    else
                        comp[compSelectata.id].transformari.rotate(-optiuni.unghiRotire);
                }
                else if (event.key.code == sf::Keyboard::Subtract) {
                    Transform transformPunctConexiune;
                    comp[compSelectata.id].transformari.scale(optiuni.coefScalare, optiuni.coefScalare);
                    if (verificareColiziune(comp, compSelectata.id, nrComponenteInserate, elemente) == false)
                    {
                        transformPunctConexiune.scale(optiuni.coefScalare, optiuni.coefScalare,
                            comp[compSelectata.id].transformari.getTransform().transformPoint(elemente[comp[compSelectata.id].idElement].origine).x,
                            comp[compSelectata.id].transformari.getTransform().transformPoint(elemente[comp[compSelectata.id].idElement].origine).y);

                        Vector2f coordonateInafaraFereastra = ieseDinFereastra(comp[compSelectata.id], elemente[comp[compSelectata.id].idElement]);
                        if (coordonateInafaraFereastra.x == 0 && coordonateInafaraFereastra.y == 0)
                        {
                            modificareConexiuni(coordonateInafaraFereastra, nrLegaturi, legaturi, compSelectata.id, transformPunctConexiune);
                        }
                        else
                            comp[compSelectata.id].transformari.scale(1 / optiuni.coefScalare, 1 / optiuni.coefScalare);
                    }
                    else
                        comp[compSelectata.id].transformari.scale(1 / optiuni.coefScalare, 1 / optiuni.coefScalare);
                    verificareColiziune(comp, compSelectata.id, nrComponenteInserate, elemente);
                }
                else if (event.key.code == sf::Keyboard::Add)
                {
                    Transform transformPunctConexiune;
                    transformPunctConexiune.scale(1/optiuni.coefScalare, 1/optiuni.coefScalare,
                        comp[compSelectata.id].transformari.getTransform().transformPoint(elemente[comp[compSelectata.id].idElement].origine).x,
                        comp[compSelectata.id].transformari.getTransform().transformPoint(elemente[comp[compSelectata.id].idElement].origine).y);
                    comp[compSelectata.id].transformari.scale(1 / optiuni.coefScalare, 1 / optiuni.coefScalare);
                    Vector2f coordonateInafaraFereastra = ieseDinFereastra(comp[compSelectata.id], elemente[comp[compSelectata.id].idElement]);
                    if (coordonateInafaraFereastra.x == 0 && coordonateInafaraFereastra.y == 0)
                    {
                        modificareConexiuni(coordonateInafaraFereastra, nrLegaturi, legaturi, compSelectata.id, transformPunctConexiune);
                    }
                    else
                        comp[compSelectata.id].transformari.scale(optiuni.coefScalare, optiuni.coefScalare);
                    verificareColiziune(comp, compSelectata.id, nrComponenteInserate, elemente);
                }
                break;

            case sf::Event::MouseMoved:
            {
                Vector2i coordonateMouse = Mouse::getPosition(window);
                if (status == Normal)
                {
                    for (int i = 0; i < nrComponenteInserate; i++)
                    {
                        Punct max, min;
                        coordonateDupaRotire(elemente[comp[i].idElement].chenar, &min, &max, comp[i].transformari.getTransform());
                        if (coordonateMouse.x >= min.x && coordonateMouse.y >= min.y
                            && coordonateMouse.x <= max.x && coordonateMouse.y <= max.y)
                        {
                            comp[i].activ = true;
                        }
                        else
                        {
                            comp[i].activ = false;
                         }

                    }
                }
                else if (status == DesenareLinie)
                {
                    if (comp[compSelectata.id].conexiuneActiva != -1)
                    {
                        legaturi[nrLegaturi - 1].punct2 = (Vector2f)coordonateMouse;
                    }

                }
                if (status == InserareElement)
                {
                    comp[nrComponenteInserate - 1].transformari.setPosition(coordonateMouse.x, coordonateMouse.y);
                }
                else if (status == ModificarePozitieComponenta)
                {
                    for (int i = 0; i < nrComponenteInserate; i++)
                    {
                        if (comp[i].activ)
                        {
                          Vector2f pozitieVeche = comp[i].transformari.getPosition();
                            comp[i].transformari.setPosition((Vector2f)coordonateMouse);
                            if (verificareColiziune(comp, compSelectata.id, nrComponenteInserate, elemente) == false)
                            {
                                Vector2f coordonateInafaraFereastra = ieseDinFereastra(comp[compSelectata.id], elemente[comp[compSelectata.id].idElement]);

                                comp[compSelectata.id].transformari.move(coordonateInafaraFereastra);

                                for (int j = 0; j < nrLegaturi; j++)
                                {
                                    if (legaturi[j].idComponenta1 == i)
                                    {
                                        Vector2f PunctTransfomrat = comp[i].transformari.getTransform().transformPoint(elemente[comp[i].idElement].legaturi[legaturi[j].nrPunctComponenta1]);
                                        legaturi[j].punct1 = PunctTransfomrat;
                                    }
                                    else if (legaturi[j].idComponenta2 == i)
                                    {
                                        Vector2f PunctTransfomrat = comp[i].transformari.getTransform().transformPoint(elemente[comp[i].idElement].legaturi[legaturi[j].nrPunctComponenta2]);
                                        legaturi[j].punct2 = PunctTransfomrat;
                                    }

                                }
                            }
                            else
                            {
                                comp[i].transformari.setPosition(pozitieVeche);
                            }
                        
                            break;

                        }
                    }
                }
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
               
                Vector2i coordonateMouse = Mouse::getPosition(window);
                if (status == Normal)
                {
                    if (coordonateMouse.y >= 0 && coordonateMouse.y <= INALTIME_MENIU_OPTIUNI)//coordonate meniu optiuni
                    {
                        unsigned int numarOptiune = optiuneMeniuSelectata(coordonateMouse);
                        if (numarOptiune != -1)
                        {
                            switch (numarOptiune)
                            {
                               case 0: break; //meniu principal
                               case 1: //salvare
                                   if (numeFisier[0] != '\0')
                                       salvareCircuit(comp, nrComponenteInserate, legaturi, nrLegaturi, numeFisier);
                                   else
                                   {
                                       char mesaj[] = "Nu este inserat numele fisierului";
                                       FereastraEroare(mesaj);
                                   }
                                   break;
                               case 2:
                                   numeFisier[0] = '\0';
                                   salvareCircuit(comp, nrComponenteInserate, legaturi, nrLegaturi, numeFisier);
                                   break; //salvare ca
                               case 3: 
                                   deschidereCircuit(comp, &nrComponenteInserate, legaturi, &nrLegaturi, numeFisier);
                                   break; //deschidere
                               case 4:
                                   for (int i = 0; i < nrComponenteInserate; i++)
                                   {
                                       resetareDateComponenta(&comp[i]);
                                       stergereLegaturi(legaturi, i, &nrLegaturi);
                                   }
                                  nrComponenteInserate = 0;
                                   break;
                               case 5: 
                                   deschideFereastraAjutor();
                                   break; //ajutor
                            default:
                                break;
                            } 
                        }
                    }
                    if (coordonateMouse.y >= INALTIME_MENIU_OPTIUNI && coordonateMouse.y <= INALTIME_MENIU_OPTIUNI + INALTIME_MENIU_ELEMENTE) //coordonate meniu elemente
                    {
                        for (int i = 0; i < 12; i++)
                            if (coordonateMouse.x > i * latimeCasuta + (i + 1) * 10 && coordonateMouse.x < (i + 1) * latimeCasuta + (i + 1) * 10)
                            {
                                status = InserareElement;
                                comp[nrComponenteInserate].idElement = i;
                                Transformable t;
                                comp[nrComponenteInserate].transformari = t;
                                comp[nrComponenteInserate].transformari.setOrigin(elemente[comp[nrComponenteInserate].idElement].origine.x, elemente[comp[nrComponenteInserate].idElement].origine.y);
                                comp[nrComponenteInserate].transformari.setPosition(coordonateMouse.x, coordonateMouse.y);
                                strcpy(comp[nrComponenteInserate].nume, "componenta");
                                nrComponenteInserate++;
                            }
                    }
                    else
                    {
                        if (event.mouseButton.button == sf::Mouse::Left && status != ModificarePozitieComponenta)
                            for (int i = 0; i < nrComponenteInserate; i++)
                            {
                                if (comp[i].activ)
                                {
                                    compSelectata.id = i;
                                    comp[i].activ = true;
                                    status = ModificarePozitieComponenta;
                                    break;

                                }
                                else
                                {
                                    comp[i].activ = false;
                                }
                                if(status != ModificarePozitieComponenta)
                                    compSelectata.id = -1;
                            }
                    }
                    if (compSelectata.id != -1)
                        for (int i = 0; i < elemente[comp[compSelectata.id].idElement].nrLegaturi; i++)
                        {
                            Vector2f PunctTransfomrat = comp[compSelectata.id].transformari.getTransform().transformPoint(elemente[comp[compSelectata.id].idElement].legaturi[i].x,
                                elemente[comp[compSelectata.id].idElement].legaturi[i].y);

                            if (abs(PunctTransfomrat.x - coordonateMouse.x) < 10 && abs(PunctTransfomrat.y - coordonateMouse.y) < 10)
                            {
                                if (verificareExistentaConexiune(legaturi, nrLegaturi, compSelectata.id, i))
                                    nrLegaturi--;

                                legaturi[nrLegaturi].idComponenta1 = compSelectata.id;
                                legaturi[nrLegaturi].idComponenta2 = -1;
                                legaturi[nrLegaturi].nrPunctComponenta1 = i;
                                legaturi[nrLegaturi].punct1 = PunctTransfomrat;
                                legaturi[nrLegaturi].punct2 = PunctTransfomrat;
                                nrLegaturi++;
                                status = DesenareLinie;
                            }
                        }
                }
                else if (status == DesenareLinie)
                {
                    for (int j = 0; j < nrComponenteInserate; j++)
                        for (int i = 0; i < elemente[comp[j].idElement].nrLegaturi; i++)
                        {
                            Vector2f PunctTransfomrat = comp[j].transformari.getTransform().transformPoint(elemente[comp[j].idElement].legaturi[i].x,
                                elemente[comp[j].idElement].legaturi[i].y);

                            if (abs(PunctTransfomrat.x - coordonateMouse.x) < 10 && abs(PunctTransfomrat.y - coordonateMouse.y) < 10)
                            {
                                if (verificareExistentaConexiune(legaturi, nrLegaturi, j, i))
                                    nrLegaturi--;
                                legaturi[nrLegaturi - 1].idComponenta2 = j;
                                legaturi[nrLegaturi - 1].nrPunctComponenta2 = i;
                                legaturi[nrLegaturi - 1].punct2 = PunctTransfomrat;
                               // conexiuneActiva = -1;
                                status = Normal;

                                break;
                            }
                        }
                    if (status != Normal)
                    {
                        legaturi[nrLegaturi - 1] = {};
                        nrLegaturi--;
                        status = Normal;
                    }

                }
           
                break;
            }

            case sf::Event::MouseButtonReleased:
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (status == InserareElement)
                    {
                        Vector2f coordonateInafaraFereastra = ieseDinFereastra(comp[nrComponenteInserate - 1], elemente[comp[nrComponenteInserate - 1].idElement]);
                        if(!(coordonateInafaraFereastra.x == 0 && coordonateInafaraFereastra.y == 0))
                             //setareChenar(&comp[nrComponenteInserate - 1], elemente[comp[nrComponenteInserate - 1].idElement]);
                        {
                            resetareDateComponenta(&comp[nrComponenteInserate - 1]);
                            nrComponenteInserate--;
                        }

                        status = Normal;
                    }
                    else if (status == ModificarePozitieComponenta)
                    {
                        status = Normal;
                    }


                }
                break;
            }
            }
        }

        window.clear(sf::Color::Black);
        if (status == ModificareParametri)
        {
            afisareFereastraParametri(&comp[compSelectata.id]);
            status = Normal;
        }
        desenareMeniuElemente(window, elemente, 12);
        desenareMeniuOptiuni(window, font);
        text.setString(statusCoordonate);
        for (int j = 0; j < nrComponenteInserate; j++)
        {
            for (int i = 0; i < nrLegaturi; i++)
            {
                VertexArray linii = creareConexiuni(legaturi[i]);
                linii.setPrimitiveType(LinesStrip);
                window.draw(linii);
            }
            window.draw(elemente[comp[j].idElement].linii, comp[j].transformari.getTransform());
            window.draw(elemente[comp[j].idElement].dreptunghi, comp[j].transformari.getTransform());
            window.draw(elemente[comp[j].idElement].elipsa, comp[j].transformari.getTransform());
            
            if(comp[j].activ || compSelectata.id == j)
             window.draw(elemente[comp[j].idElement].chenar, comp[j].transformari.getTransform());
            window.draw(elemente[comp[j].idElement].arc, comp[j].transformari.getTransform());
        }
        window.draw(text);
        window.display();
    }
}

int main()
{
    afisareElemente();
    return 0;
}
