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
#include "Stiva.h"
#include "Slidere.h"
#include "Setari.h"
#include "Start.h"
#include "FereastraAvertizare.h"
char numeOptiuniSalvate[NUMAR_OPTIUNI][40] = { "Coeficient micsorare", "Distanta miscare", "Unghi rotire" , "Culoare Conexiune" };
char locatieFisiere[13][20] = { "PS\\BATERIE.PS","PS\\AMPLOP.PS" ,"PS\\CONDENS.PS","PS\\DIODA.PS","PS\\NOD.PS","PS\\POLARIZ.PS","PS\\REZIST.PS","PS\\SERVOMOT.PS","PS\\SINU.PS","PS\\TRANZNPN.PS","PS\\TRANZPNP.PS","PS\\ZENNER.PS","PS\\STOP.PS" };
using namespace sf;
using namespace std;

string statusCoordonate;
float latimeCasuta;
//Optiuni optiuni;
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
    fgets(element.denumire, MAX_NUME_COMPONENTA, f);
    element.denumire[strlen(element.denumire)-1] = '\0';
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
            Vector2f dimensiuni;
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
            element.elipsa.setOutlineThickness(1);
            dimensiuni.x = raza * rx + 1;
            dimensiuni.y = raza * ry + 1;
            if (dimensiuni.x  > element.dimensiuni.x)
                element.dimensiuni.x = dimensiuni.x;

            if (dimensiuni.y > element.dimensiuni.y)
                element.dimensiuni.y = dimensiuni.y;
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
  //  char optiune[40];
    char sep[] = "\":,}";
    FILE* f = fopen("setari.json", "r");
    fgets(rand, 40, f); //{

  
    while (!feof(f))
    {
        fscanf(f, "%c ", &c);
        fgets(rand, 40, f);
        char* token = strtok(rand, sep);
      //  fscanf(f, " \"%s\":", optiune);
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
            if(token != NULL)
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
    
    if (min.y < INALTIME_MENIU_ELEMENTE + INALTIME_MENIU_OPTIUNI)
        coordonateInAfara.y = INALTIME_MENIU_ELEMENTE + INALTIME_MENIU_OPTIUNI - min.y;
     
    if (min.x < 0)
        coordonateInAfara.x = 0 - min.x;

    if (max.y > INALTIME_FEREASTRA)
        coordonateInAfara.y = INALTIME_FEREASTRA - max.y;

    if (max.x > LATIME_FEREASTRA-140)
        coordonateInAfara.x = LATIME_FEREASTRA - max.x-140;

    return coordonateInAfara;
}

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
void stergereToateLegaturi(Circuit* circuit)
{
  
      //  circuit->legaturi = new circuit->legaturi[MAX];
    
}
void stergereLegaturi(Circuit* circuit)
{
    int i = 0;
    while(i < circuit->nrLegaturi)
    {
        if (circuit->legaturi[i].idComponenta1 == circuit->componentaSelectata || circuit->legaturi[i].idComponenta2 == circuit->componentaSelectata)
        {
            for (int j = i; j < circuit->nrLegaturi - 1; j++)
                circuit->legaturi[j] = circuit->legaturi[j + 1];

            circuit->nrLegaturi--;
        }
        else
            i++;
    }
}
void stergereComponenta(Circuit* circuit)
{
    for (int i = circuit->componentaSelectata; i < circuit->nrComponente - 1; i++)
    {
        circuit->componente[i] = circuit->componente[i + 1];
    }
}
bool verificareColiziune(Circuit* circuit, int idComponentaVerificata)
{
    ElCircuit elementVerificat = circuit->elemente[circuit->componente[idComponentaVerificata].idElement];
    for (int i = 0; i < circuit->nrComponente; i++)
    {
        if (idComponentaVerificata != i)
        {
            ElCircuit elementCurent = circuit->elemente[circuit->componente[i].idElement];
            if (circuit->componente[i].transformari.getTransform().transformRect(elementCurent.chenar.getGlobalBounds()).intersects(
                circuit->componente[idComponentaVerificata].transformari.getTransform().transformRect(elementVerificat.chenar.getGlobalBounds())))
            {
                return true;
            }
        }
    }
    return false;
}
void miscareTastatura(Circuit* circuit, float offsetx, float offsety)
{
    Transformable transformPunctConexiune;
    circuit->componente[circuit->componentaSelectata].transformari.move(offsetx, offsety);
    if (verificareColiziune(circuit, circuit->componentaSelectata) == false)
    {
        transformPunctConexiune.move(offsetx, offsety);
        Vector2f coordonateInafaraFereastra = ieseDinFereastra(circuit->componente[circuit->componentaSelectata], circuit->elemente[circuit->componente[circuit->componentaSelectata].idElement]);
        if (abs(coordonateInafaraFereastra.x) < circuit->optiuni.distantaMiscare && abs(coordonateInafaraFereastra.y) < circuit->optiuni.distantaMiscare)
            modificareConexiuni(coordonateInafaraFereastra, circuit, transformPunctConexiune.getTransform());

        circuit->componente[circuit->componentaSelectata].transformari.move(coordonateInafaraFereastra);
    }
    else
    {
        circuit->componente[circuit->componentaSelectata].transformari.move(-offsetx, -offsety);
    }
}
void rotire(Circuit* circuit)
{
    Transform transformPunctConexiune;
    circuit->componente[circuit->componentaSelectata].transformari.rotate(circuit->optiuni.unghiRotire);
    if (verificareColiziune(circuit, circuit->componentaSelectata) == false)
    {
        transformPunctConexiune.rotate(circuit->optiuni.unghiRotire, circuit->componente[circuit->componentaSelectata].transformari.getTransform().transformPoint(circuit->elemente[circuit->componente[circuit->componentaSelectata].idElement].origine));
        Vector2f coordonateInafaraFereastra = ieseDinFereastra(circuit->componente[circuit->componentaSelectata], circuit->elemente[circuit->componente[circuit->componentaSelectata].idElement]);
        if (coordonateInafaraFereastra.x == 0 && coordonateInafaraFereastra.y == 0)
        {
            modificareConexiuni(coordonateInafaraFereastra, circuit,  transformPunctConexiune);
        }
        else
            circuit->componente[circuit->componentaSelectata].transformari.rotate(-circuit->optiuni.unghiRotire);
    }
    else
        circuit->componente[circuit->componentaSelectata].transformari.rotate(-circuit->optiuni.unghiRotire);
}
void redimensionare(Circuit* circuit, float coeficient)
{
    Transform transformPunctConexiune;
    circuit->componente[circuit->componentaSelectata].transformari.scale(coeficient, coeficient);
    if (verificareColiziune(circuit, circuit->componentaSelectata) == false)
    {
        ElCircuit elementCurent = circuit->elemente[circuit->componente[circuit->componentaSelectata].idElement];
        transformPunctConexiune.scale(coeficient, coeficient,
            circuit->componente[circuit->componentaSelectata].transformari.getTransform().transformPoint(elementCurent.origine).x,
            circuit->componente[circuit->componentaSelectata].transformari.getTransform().transformPoint(elementCurent.origine).y);

        Vector2f coordonateInafaraFereastra = ieseDinFereastra(circuit->componente[circuit->componentaSelectata], elementCurent);
        if (coordonateInafaraFereastra.x == 0 && coordonateInafaraFereastra.y == 0)
        {
            modificareConexiuni(coordonateInafaraFereastra, circuit, transformPunctConexiune);
        }
        else
            circuit->componente[circuit->componentaSelectata].transformari.scale(1 / coeficient, 1 / coeficient);
    }
    else
        circuit->componente[circuit->componentaSelectata].transformari.scale(1 / coeficient, 1 / coeficient);
}
void afisareStiva(Nod* p)
{
    Nod* c = p;
    while (c != NULL)
    {
        std::cout << c->data << "->";
        c = c->next;
    }
}
void afisareDoarComponente(RenderWindow &window, Circuit* circuit)
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
   // 
    Color gri(32, 32, 32);
  //  char circuit->numeFisier[MAX_LUNGIME_FISIER] = {};
    circuit->optiuni.distantaMiscare = 50;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8.0;
    RenderWindow window(VideoMode(1920, 1080), "ELECTRON",Style::Fullscreen, settings);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    Statusuri status = Normal; 
    citireJSON(&circuit->optiuni);
   /* circuit->optiuni.coefScalare = 0.8;
    circuit->optiuni.distantaMiscare = 50;
    circuit->optiuni.unghiRotire = 10;
    */
     for (int i = 0; i < 12; i++) 
         circuit->elemente[i] = citireInformatii(locatieFisiere[i]);
    sf::Font font;
    if (!font.loadFromFile("fonts\\RobotoSlab-Font.ttf"))
        cout << "ERROR";
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
                if (Keyboard::isKeyPressed(sf::Keyboard::Y) && Keyboard::isKeyPressed(sf::Keyboard::LControl))//redo
                {
                      if (circuit->topRedo != ((*circuit)).topUndo)
                    {
                        int nr = prim(circuit->topRedo);
                     //   pop(circuit->topRedo);
                        if(nr != 0)
                            push(circuit->topUndo, nr);
                    }
                     
                    deschidereIstoricRedo(circuit);
                    system("CLS");
                    std::cout << "undo";
                    afisareStiva(circuit->topUndo);
                    std::cout << endl<<"redo";
                    afisareStiva(circuit->topRedo);
                }
                if (Keyboard::isKeyPressed(sf::Keyboard::Z) && Keyboard::isKeyPressed(sf::Keyboard::LControl))//undo
                {
                    salvareIstoricRedo(circuit);
                    deschidereIstoric(circuit);
                      system("CLS");
                    std::cout << "undo";
                    afisareStiva(circuit->topUndo);
                    std::cout << endl << "redo";
                    afisareStiva(circuit->topRedo);
                }
                if (Keyboard::isKeyPressed(sf::Keyboard::S) && Keyboard::isKeyPressed(sf::Keyboard::LControl))
                {
                  //  stergereContinutFereastra(&window);
                    salvareCircuit(circuit, circuit->numeFisier, true);
                    circuit->esteSalvat = true;
                }
                else if (Keyboard::isKeyPressed(sf::Keyboard::O) && Keyboard::isKeyPressed(sf::Keyboard::LControl))
                {
                    circuit->numeFisier[0] = '\0';
                    deschidereCircuit(circuit, circuit->numeFisier, true);
                }
                if (event.key.code == sf::Keyboard::Escape)
                {
                    stergereIstoric(circuit);
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::E && circuit->componentaSelectata != -1)
                {
                    status = ModificareParametri;
                }
                else if (event.key.code == sf::Keyboard::Delete)
                {
                    if (circuit->componentaSelectata != -1)
                    {
                        salvareIstoric(circuit);
                       stergereComponenta(circuit);
                        stergereLegaturi(circuit);
                        circuit->nrComponente--;
                        status = Normal;
                        circuit->componentaSelectata = -1;
                    }
                }
                if (circuit->componentaSelectata == -1) break;
                if (event.key.code == sf::Keyboard::Right) 
                {
                    salvareIstoric(circuit);
                    miscareTastatura(circuit, circuit->optiuni.distantaMiscare, 0);
                }
                else if (event.key.code == sf::Keyboard::Left) 
                {
                    salvareIstoric(circuit);
                    miscareTastatura(circuit, -circuit->optiuni.distantaMiscare, 0);

                }
                else if (event.key.code == sf::Keyboard::Up) 
                {
                    salvareIstoric(circuit);
                    miscareTastatura(circuit, 0, -circuit->optiuni.distantaMiscare);

                }
                else if (event.key.code == sf::Keyboard::Down) 
                {
                    salvareIstoric(circuit);
                    miscareTastatura(circuit, 0, circuit->optiuni.distantaMiscare);

                }
                else if (event.key.code == sf::Keyboard::R) {
                    salvareIstoric(circuit);
                    rotire(circuit);
                }
                else if (event.key.code == sf::Keyboard::Subtract|| event.key.code == sf::Keyboard::N)
                {
                    if (circuit->componente[circuit->componentaSelectata].transformari.getScale().x > 0.5)
                    {
                        salvareIstoric(circuit);
                        redimensionare(circuit, circuit->optiuni.coefScalare); // micsorare
                    }
                }
                else if (event.key.code == sf::Keyboard::Add|| event.key.code == sf::Keyboard::M)
                {
                    if (circuit->componente[circuit->componentaSelectata].transformari.getScale().x < 2)
                    {
                        salvareIstoric(circuit);
                        redimensionare(circuit, 1 / circuit->optiuni.coefScalare); // marire
                    }
                }
                break;

            case sf::Event::MouseMoved:
            {
                Vector2i coordonateMouse = Mouse::getPosition(window);
                if (status == Normal)
                {
                    for (int i = 0; i < circuit->nrComponente; i++)
                    {
                        Punct max, min;
                        coordonateDupaRotire(circuit->elemente[circuit->componente[i].idElement].chenar, &min, &max, circuit->componente[i].transformari.getTransform());
                        if (coordonateMouse.x >= min.x && coordonateMouse.y >= min.y
                            && coordonateMouse.x <= max.x && coordonateMouse.y <= max.y)
                        {
                            circuit->componente[i].activ = true;
                        }
                        else
                        {
                            circuit->componente[i].activ = false;
                         }

                    }
                }
                else if (status == DesenareLinie)
                {
                   
                    if (circuit->componente[circuit->componentaSelectata].conexiuneActiva != -1)
                    {
                     
                    circuit->legaturi[circuit->nrLegaturi - 1].punct2 = (Vector2f)coordonateMouse;
                    }

                }
                else if (status == ModificareUnghiRotire)
                {
                    //problema aici
                    float val = valoareUnghiRotire(coordonateMouse);
                    circuit->componente[circuit->componentaSelectata].transformari.setRotation(val);
                }
                else if (status == ModificareCoeficientScalare)
                {   
                    //problema aici
                    if (coordonateMouse.y >= varfCoefScalare && coordonateMouse.y <= bazaCoefScalare)
                    {
                        float val = valoareCoefScalare(coordonateMouse);
                        circuit->componente[circuit->componentaSelectata].transformari.setScale(val, val);
                    }
                }
                if (status == InserareElement)
                {
                    Vector2f pozitieVeche = circuit->componente[circuit->nrComponente - 1].transformari.getPosition();
                    circuit->componente[circuit->nrComponente - 1].transformari.setPosition(coordonateMouse.x, coordonateMouse.y);
                    if (verificareColiziune(circuit, circuit->nrComponente - 1) == true)
                    {
                        circuit->componente[circuit->nrComponente - 1].transformari.setPosition(pozitieVeche);
                    }
                   
                }
                else if (status == ModificarePozitieComponenta)
                {
                    for (int i = 0; i < circuit->nrComponente; i++)
                    {
                        
                        if (circuit->componente[i].activ)
                        {
                          Vector2f pozitieVeche = circuit->componente[i].transformari.getPosition();
                          circuit->componente[i].transformari.setPosition((Vector2f)coordonateMouse);
                           if (verificareColiziune(circuit, circuit->componentaSelectata) == false)
                            {
                                Vector2f coordonateInafaraFereastra = ieseDinFereastra( circuit->componente[circuit->componentaSelectata], circuit->elemente[ circuit->componente[circuit->componentaSelectata].idElement]);

                                 circuit->componente[circuit->componentaSelectata].transformari.move(coordonateInafaraFereastra);

                                for (int j = 0; j < circuit->nrLegaturi; j++)
                                {
                                    if (circuit->legaturi[j].idComponenta1 == i)
                                    {
                                        Vector2f PunctTransfomrat = circuit->componente[i].transformari.getTransform().transformPoint(circuit->elemente[circuit->componente[i].idElement].legaturi[circuit->legaturi[j].nrPunctComponenta1]);
                                        circuit->legaturi[j].punct1 = PunctTransfomrat;
                                    }
                                    else if (circuit->legaturi[j].idComponenta2 == i)
                                    {
                                        Vector2f PunctTransfomrat = circuit->componente[i].transformari.getTransform().transformPoint(circuit->elemente[circuit->componente[i].idElement].legaturi[circuit->legaturi[j].nrPunctComponenta2]);
                                        circuit->legaturi[j].punct2 = PunctTransfomrat;
                                    }

                                }
                            }
                            else
                            {
                                circuit->componente[i].transformari.setPosition(pozitieVeche);
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
                    if (coordonateMouse.x >= axaOxSlidere - 10 && coordonateMouse.x <= axaOxSlidere + 10)
                    {
                        float pozitieY = pozitieCercCoefScalare(circuit->componente[circuit->componentaSelectata].transformari.getScale().x);
                        if (coordonateMouse.y >= pozitieY - 5 && coordonateMouse.y <= pozitieY + 5)
                        {
                            cout << "modificarestatusscalare";
                            status = ModificareCoeficientScalare;
                        }
                    }
                    else if (coordonateMouse.x >= axaOxSlidere - 10 && coordonateMouse.x <= axaOxSlidere + 10 && coordonateMouse.y <= bazaUnghiRotire && coordonateMouse.y >= varfUnghiRotire)
                    {
                        cout << "modificarestatusrotire";
                        status = ModificareUnghiRotire;
                    }
                    else if (coordonateMouse.y >= 0 && coordonateMouse.y <= INALTIME_MENIU_OPTIUNI)//coordonate meniu optiuni
                    {
                        unsigned int numarOptiune = optiuneMeniuSelectata(coordonateMouse);
                        if (numarOptiune != -1)
                        {
                            switch (numarOptiune)
                            {
                               case 0: 
                                   if (circuit->esteSalvat == true || afisareFereastraAvertizare() == true)
                                   {
                                       window.close();
                                       afisareFereastraStart();
                                   }
                                   break; //meniu principal
                               case 1: //salvare
                                   if (circuit->numeFisier[0] != '\0')
                                   {
                                       salvareCircuit(circuit, circuit->numeFisier, true);
                                       circuit->esteSalvat = true;
                                   }
                                   else
                                   {
                                       char mesaj[] = "Nu este inserat numele fisierului";
                                       FereastraEroare(mesaj);
                                   }
                                   break;
                               case 2:
                                  // stergereContinutFereastra(&window);
                                   circuit->numeFisier[0] = '\0';
                                   salvareCircuit(circuit, circuit->numeFisier, true);
                                   circuit->esteSalvat = true;
                                   break; //salvare ca
                               case 3: 
                                //   stergereContinutFereastra(&window);
                                   circuit->numeFisier[0] = '\0';
                                   deschidereCircuit(circuit, circuit->numeFisier, true);
                                   break; //deschidere
                               case 4:
                                   afisareDoarComponente(window, circuit);
                                   exporta(window);
                                   break;//export
                               case 5:
                                   salvareIstoric(circuit);
                                   circuit->componente = new Componenta[MAX];
                                   circuit->legaturi = new Legatura[MAX];
                                  circuit->nrLegaturi = 0;
                                  circuit->nrComponente = 0;
                                  circuit->componentaSelectata = -1;
                                
                                   break;//resetare
                               
                               case 6: 
                                 //  stergereContinutFereastra(&window);
                                   deschideFereastraAjutor();
                                   break; //ajutor
                               case 7:
                                   afisareFereastraSetari(circuit, numeOptiuniSalvate);
                                   break; //setari
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
                                circuit->componente[circuit->nrComponente].idElement = i;
                                Transformable t;
                                ElCircuit elementCurent = circuit->elemente[circuit->componente[circuit->nrComponente].idElement];
                                circuit->componente[circuit->nrComponente].transformari = t;
                                circuit->componente[circuit->nrComponente].transformari.setOrigin(elementCurent.origine.x, elementCurent.origine.y);
                                circuit->componente[circuit->nrComponente].transformari.setPosition(coordonateMouse.x, coordonateMouse.y);
                                strcpy(circuit->componente[circuit->nrComponente].nume, elementCurent.denumire);
                                circuit->nrComponente++;
                            }
                    }
                    else
                    {
                        if (event.mouseButton.button == sf::Mouse::Left && status != ModificarePozitieComponenta)
                            for (int i = 0; i < circuit->nrComponente; i++)
                            {
                                if (circuit->componente[i].activ)
                                {
                                    circuit->componentaSelectata = i;
                                    circuit->componente[i].activ = true;
                                    status = ModificarePozitieComponenta;
                                    break;

                                }
                                else
                                {
                                    circuit->componente[i].activ = false;
                                }
                                if(status != ModificarePozitieComponenta)
                                    circuit->componentaSelectata = -1;
                            }
                    }
                    if (circuit->componentaSelectata != -1)
                        for (int i = 0; i < circuit->elemente[ circuit->componente[circuit->componentaSelectata].idElement].nrLegaturi; i++)
                        {
                            Vector2f PunctTransfomrat =  circuit->componente[circuit->componentaSelectata].transformari.getTransform().transformPoint(circuit->elemente[circuit->componente[circuit->componentaSelectata].idElement].legaturi[i].x,
                                circuit->elemente[ circuit->componente[circuit->componentaSelectata].idElement].legaturi[i].y);

                            if (abs(PunctTransfomrat.x - coordonateMouse.x) < 10 && abs(PunctTransfomrat.y - coordonateMouse.y) < 10)
                            {
                                if (verificareExistentaConexiune(circuit->legaturi, circuit->nrLegaturi, circuit->componentaSelectata, i))
                                    circuit->nrLegaturi--;

                                circuit->legaturi[circuit->nrLegaturi].idComponenta1 = circuit->componentaSelectata;
                                circuit->legaturi[circuit->nrLegaturi].idComponenta2 = -1;
                                circuit->legaturi[circuit->nrLegaturi].nrPunctComponenta1 = i;
                                circuit->legaturi[circuit->nrLegaturi].punct1 = PunctTransfomrat;
                                circuit->legaturi[circuit->nrLegaturi].punct2 = PunctTransfomrat;
                                circuit->nrLegaturi++;
                                status = DesenareLinie;
                            }
                        }
                }
                else if (status == DesenareLinie)
                {
                    for (int j = 0; j < circuit->nrComponente; j++)
                        for (int i = 0; i < circuit->elemente[circuit->componente[j].idElement].nrLegaturi; i++)
                        {
                            Vector2f PunctTransfomrat = circuit->componente[j].transformari.getTransform().transformPoint(circuit->elemente[circuit->componente[j].idElement].legaturi[i].x,
                                circuit->elemente[circuit->componente[j].idElement].legaturi[i].y);

                            if (abs(PunctTransfomrat.x - coordonateMouse.x) < 10 && abs(PunctTransfomrat.y - coordonateMouse.y) < 10)
                            {
                                if (verificareExistentaConexiune(circuit->legaturi, circuit->nrLegaturi, j, i))
                                    circuit->nrLegaturi--;

                                circuit->legaturi[circuit->nrLegaturi - 1].idComponenta2 = j;
                                circuit->legaturi[circuit->nrLegaturi - 1].nrPunctComponenta2 = i;
                                circuit->legaturi[circuit->nrLegaturi - 1].punct2 = PunctTransfomrat;
                                status = Normal;
                                salvareIstoric(circuit);
                                break;
                            }
                        }
                    if (status != Normal)
                    {
                        circuit->legaturi[circuit->nrLegaturi - 1] = {};
                        circuit->nrLegaturi--;
                        status = Normal;
                    }

                }
                else if (status == ModificareCoeficientScalare)
                {
                Vector2f valcoefscalare;
                valcoefscalare.x = valoareCoefScalare(coordonateMouse);
                valcoefscalare.y = valoareCoefScalare(coordonateMouse);
                checkSizeCoef(valcoefscalare);
                circuit->componente[circuit->componentaSelectata].transformari.setScale(valcoefscalare);
                }
                else if (status == ModificareUnghiRotire)
                {
                float valunghirotire;
                valunghirotire = valoareUnghiRotire(coordonateMouse);

                circuit->componente[circuit->componentaSelectata].transformari.setRotation(valunghirotire);
                }
           
                break;
            }

            case sf::Event::MouseButtonReleased:
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (status == InserareElement)
                    {
                        Vector2f coordonateInafaraFereastra = ieseDinFereastra(circuit->componente[circuit->nrComponente - 1], circuit->elemente[circuit->componente[circuit->nrComponente - 1].idElement]);
                        if(!(coordonateInafaraFereastra.x == 0 && coordonateInafaraFereastra.y == 0))
                             //setareChenar(&comp[circuit->nrComponente - 1], elemente[comp[circuit->nrComponente - 1].idElement]);
                        {
                            resetareDateComponenta(&circuit->componente[circuit->nrComponente - 1]);
                            circuit->nrComponente--;
                        }
                        salvareIstoric(circuit);
                        system("CLS");
                        std::cout << "undo";
                        afisareStiva(circuit->topUndo);
                        std::cout << endl << "redo";
                        afisareStiva(circuit->topRedo);
                        status = Normal;
                    }
                    else if (status == ModificarePozitieComponenta)
                    {
                        salvareIstoric(circuit);
                        system("CLS");
                        std::cout << "undo";
                        afisareStiva(circuit->topUndo);
                        std::cout << endl << "redo";
                        afisareStiva(circuit->topRedo);
                        status = Normal;
                    }
                    else if (status == ModificareCoeficientScalare)
                    {
                        status = Normal;
                    }
                    else if (status == ModificareUnghiRotire)
                    {
                        status = Normal;
                    }


                }
                break;
            }
            case sf::Event::LostFocus:
                cout << "LostFocus";
            }
        }
     
            window.clear(gri);
            if (status == ModificareParametri)
            {
                afisareFereastraParametri(&circuit->componente[circuit->componentaSelectata]);
                status = Normal;
            }
            desenareMeniuElemente(window, circuit->elemente, NUMAR_ELEMENTE);
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
                drawSlider(window, ((bazaCoefScalare - varfCoefScalare) / 2 + varfCoefScalare - 33));
                drawSlider(window, varfUnghiRotire);
            }
            desenareMeniuOptiuni(window, font);
            text.setString(statusCoordonate);
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
            window.draw(text);
            window.display();
        
    }
    delete circuit; // se dealoca memoria
}

