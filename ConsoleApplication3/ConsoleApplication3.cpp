#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <math.h>
#define MAX 100
#define SIN(x) sin(x * 3.141592653589 / 180)
#define COS(x) cos(x * 3.141592653589 / 180)
#define SIZECOEFINITIAL 50
#define NULLVECTOR Vector2f x = NAN, y = NAN
using namespace sf;
using namespace std;
struct punct
{
    float x;
    float y;
};
struct linie
{
    punct p1;
    punct p2;
};
struct Componenta
{
    float offsetX;
    float offsetY;
    Vector2f origine;
    float unghiRotire;
    bool areConexiuni[MAX] = { false };
    int conexiuneActiva;
    Vertex conexiuni[MAX];
    VertexArray liniiPtConexiuni[MAX];
    Transformable transformari;
};
struct elCircuit {
    char denumire[30];
    VertexArray linii;
    CircleShape elipsa;
    VertexArray arc;
    RectangleShape patrat;
    unsigned nrLegaturi;
    unsigned nrLinii;
    float inaltime;
    float latime;
    Vector2f legaturi[MAX];
    float sizeCoef = 50;
    RectangleShape chenar;
};
struct Legatura
{
    Vector2f pPornire;
    Vector2f pSfarsit;
};
enum statusuri{
    DesenareLinie,
    Normal
};

elCircuit citireCoordonate(FILE* f)
{
    elCircuit o1;
    int i = 0, nr = 0;
    char denumire[30];
    //string denumire;
    o1.linii.setPrimitiveType(Lines);
    fscanf(f, "%s", denumire);
    fscanf(f, "%d", &nr);
    for (int i = 0; i < nr; i++)
    {
        float v;
        fscanf(f, "%f", &v);
        o1.legaturi[i].x = (v + 2) * SIZECOEFINITIAL;
        fscanf(f, "%f", &v);
        o1.legaturi[i].y = (v + 1) * SIZECOEFINITIAL;
    }
    o1.nrLegaturi = nr;
  
    fscanf(f, "%d", &nr);
    float xmin, xmax = 0, ymin, ymax = 0;
    for (int i = 0; i < nr; i++)
    {
        Vertex punct;

        float v;
        char tip;
        fscanf(f, "\n%c", &tip);

        if (tip == 'L')
        {
            fscanf(f, "%f", &v);
            punct.position.x = (v + 2) * SIZECOEFINITIAL;
            fscanf(f, "%f", &v);
            punct.position.y = (v + 1) * SIZECOEFINITIAL;
            o1.linii.append(punct);

            fscanf(f, "%f", &v);
            punct.position.x = (v + 2) * SIZECOEFINITIAL;
            fscanf(f, "%f", &v);
            punct.position.y = (v + 1) * SIZECOEFINITIAL;
            o1.linii.append(punct);
            if (i == 0)
            {
                xmin = o1.linii[i * 2].position.x;
                ymin = o1.linii[i * 2].position.y;
            }

            if (o1.linii[i * 2].position.x > xmax) xmax = o1.linii[i * 2].position.x;
            if (o1.linii[i * 2 + 1].position.x > xmax) xmax = o1.linii[i * 2 + 1].position.x;

            if (o1.linii[i * 2].position.x < xmin) xmin = o1.linii[i * 2 + 1].position.x;
            if (o1.linii[i * 2 + 1].position.x < xmin) xmin = o1.linii[i * 2 + 1].position.x;

            if (o1.linii[i * 2].position.y > ymax) ymax = o1.linii[i * 2].position.y;
            if (o1.linii[i * 2 + 1].position.y > ymax) ymax = o1.linii[i * 2 + 1].position.y;

            if (o1.linii[i * 2].position.y < ymin) ymin = o1.linii[i * 2].position.y;
            if (o1.linii[i * 2 + 1].position.y < ymin) ymin = o1.linii[i * 2 + 1].position.y;
        }

    }
    strcpy(o1.denumire, denumire);
    
    o1.latime = xmax - xmin;
    o1.inaltime = ymax - ymin;
    o1.nrLinii = nr;
    fclose(f);

    return o1;
}
VertexArray creareConexiuni(Vector2f coordMouse, Vertex PunctPornire)
{
    VertexArray linii(Lines);
    Vertex punct;
    float distanta = coordMouse.x - PunctPornire.position.x;
    linii.append(PunctPornire);

    punct.position.x = PunctPornire.position.x + distanta / 2;
    punct.position.y = PunctPornire.position.y;
    linii.append(punct);
    linii.append(punct);

    punct.position.y = coordMouse.y;
    linii.append(punct);
    linii.append(punct);

    punct.position.x = coordMouse.x;
    punct.position.y = coordMouse.y;
    linii.append(punct);
    
   return linii;
}
void modificareConexiuni(Componenta *comp, Transform TransformPunctConexiune, int nrConexiune)
{
        Vector2f punctPornireTransformat;
        punctPornireTransformat = TransformPunctConexiune.transformPoint((*comp).liniiPtConexiuni[nrConexiune][0].position);

        VertexArray liniiTemp(Lines);
        liniiTemp = creareConexiuni((*comp).liniiPtConexiuni[nrConexiune][5].position, punctPornireTransformat);
        (*comp).liniiPtConexiuni[nrConexiune].clear();
        (*comp).liniiPtConexiuni[nrConexiune] = liniiTemp;
}

void coordonateDupaRotire(RectangleShape patrat, punct* Min, punct* Max, Transform tp)
{
    Vector2f p1, p2, p3, p4;
    FloatRect patrat2;
   
   patrat2 =  tp.transformRect(patrat.getLocalBounds());
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
    //y
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
bool singlePixelTest(const sf::RectangleShape Object1, sf::Vector2i mousePosition) {
    if (Object1.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
        sf::IntRect O1SubRect = Object1.getTextureRect();
        
      //  sf::Uint8* mask1 = Bitmasks.GetMask(Object1.getTexture());

        sf::Vector2f o1v = Object1.getInverseTransform().transformPoint(mousePosition.x, mousePosition.y);
        // Make sure pixels fall within the sprite's subrect
        if (o1v.x > 0 && o1v.y > 0 && o1v.x < O1SubRect.width && o1v.y < O1SubRect.height) {
            return true;
        }
    }
    return false;
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
VertexArray desenareConexiuni(RenderWindow& window, Componenta comp, int nrMaxConexiuniElement, elCircuit element)
{
    for (int i = 0; i < nrMaxConexiuniElement; i++)
    {
        VertexArray linii(Lines);
        if (comp.areConexiuni[i])
        {
            Legatura l;
            l.pPornire = comp.transformari.getTransform().transformPoint(element.legaturi[i]);
            
           // l.pSfarsit = comp.conexiuni[i];
          //  creareConexiuni(l);
            return linii;
        }
    }
    
}
void desenareMeniuElemente(RenderWindow& window, elCircuit elemente[], int nrElemente)
{
    Transform t;
    float offsetY = 100;
    float offsetX = 10;
    t.translate(0, offsetY);
    
    float scale = 0.5;
   
 //  t.translate(47/2, 0);
    float latimeCasuta = (1920 - (nrElemente + 1)*10) / nrElemente;
    Vector2f dimCasuta;
    dimCasuta.x = latimeCasuta;
    dimCasuta.y = 50;
    t.translate(-latimeCasuta + (latimeCasuta - elemente[0].latime * scale) / 2, 0);
    for (int i = 0; i < nrElemente; i++)
    {
        RectangleShape casuta;
        sf::Color albastru(0, 102, 204);
        casuta.setPosition(latimeCasuta * i + 10*(i+1), 100);
        //casuta.setPosition(500, 100);
        casuta.setSize(dimCasuta);
        casuta.setFillColor(albastru);
      //  casuta.setOutlineThickness(5.f);
     //   casuta.setOutlineColor(Color::Blue);
        window.draw(casuta);
        t.translate(offsetX + latimeCasuta,0);
      //  t.translate(offsetX + latimeCasuta + 20, 0);
        if (elemente[i].linii.getVertexCount() != 0)
        {
           window.draw(scalareLinii(elemente[i].linii, scale), t);
        }
        if (elemente[i].patrat.getPointCount() != 0)
        {
            window.draw(elemente[i].patrat, t);
        }
        if (elemente[i].arc.getVertexCount() != 0)
        {
            window.draw(elemente[i].arc, t);
        }
        if (elemente[i].elipsa.getPointCount() != 0)
        {
            window.draw(elemente[i].elipsa, t);
        }
      
    }
}
void afisareElement(FILE* f)
{
    int conexiuneActiva = -1;
    int i = 0;
    elCircuit element =  citireCoordonate(f);
    Componenta comp;
    Legatura legaturi[MAX];
    RenderWindow window(VideoMode(1920, 1080), "SFML works!");
    
    window.setFramerateLimit(60);
 
    float xmin = 300, xmax = -300, ymin = 300, ymax = -300;
    statusuri status = Normal;

    element.chenar.setFillColor(Color::Transparent);
    element.chenar.setOutlineThickness(5.f);
    element.chenar.setOutlineColor(Color::Transparent);
    
    element.chenar.setSize(Vector2f(element.latime, element.inaltime));

    comp.origine.x =element.latime / 2;
    comp.origine.y = element.inaltime / 2;
    Vector2f size = element.chenar.getSize();
    size.x *= SIZECOEFINITIAL;
    size.y *= SIZECOEFINITIAL;
  
    VertexArray linie(Lines);
    Vertex punctPornire;

    elCircuit elemente[13];
    for (int i = 0; i < 13; i++) elemente[i] = element;
    
    comp.transformari.move(comp.origine.x, 300 + comp.origine.y);
    comp.transformari.setOrigin(comp.origine.x, comp.origine.y);
    VertexArray conexiuni(Lines);
     
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Right) {
                    comp.transformari.move(50, 0); 
                    for(int i = 0; i < element.nrLegaturi; i++)
                        if (comp.areConexiuni[i])
                        {
                            Transform TransformPunctConexiune;
                            TransformPunctConexiune.translate(50, 0);
                            modificareConexiuni(&comp, TransformPunctConexiune, i);
                        }
                }
                if (event.key.code == sf::Keyboard::Left) {
                    comp.transformari.move(-50, 0);
                    for (int i = 0; i < element.nrLegaturi; i++)
                        if (comp.areConexiuni[i])
                        {
                            Transform TransformPunctConexiune;
                            TransformPunctConexiune.translate(-50, 0);
                            modificareConexiuni(&comp, TransformPunctConexiune, i);
                        }
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    comp.transformari.move(0, -50);
                    for (int i = 0; i < element.nrLegaturi; i++)
                        if (comp.areConexiuni[i])
                        {
                            Transform TransformPunctConexiune;
                            TransformPunctConexiune.translate(0, -50);
                            modificareConexiuni(&comp, TransformPunctConexiune, i);
                        }
                   
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    comp.transformari.move(0, 50);
                    for (int i = 0; i < element.nrLegaturi; i++)
                        if (comp.areConexiuni[i])
                        {
                            Transform TransformPunctConexiune;
                            TransformPunctConexiune.translate(0, 50);
                            modificareConexiuni(&comp, TransformPunctConexiune, i);
                        }
                }
                else if (event.key.code == sf::Keyboard::R) {
                    comp.transformari.rotate(10);
                    for (int i = 0; i < element.nrLegaturi; i++)
                        if (comp.areConexiuni[i])
                        {
                            Transform TransformPunctConexiune;
                            TransformPunctConexiune.rotate(10, comp.transformari.getTransform().transformPoint(comp.origine));
                            modificareConexiuni(&comp, TransformPunctConexiune, i);
                        }
                }
                else if (event.key.code == sf::Keyboard::M) {
                    Vector2f size = element.chenar.getSize();
                    size.x *= 0.5;
                    size.y *= 0.4;
                   
                    comp.transformari.scale(0.5, 0.5);

                    for (int i = 0; i < element.nrLegaturi; i++)
                        if (comp.areConexiuni[i])
                        {
                            Transform TransformPunctConexiune;
                            TransformPunctConexiune.scale(0.5, 0.5, comp.transformari.getTransform().transformPoint(comp.origine).x, comp.transformari.getTransform().transformPoint(comp.origine).y);
                            modificareConexiuni(&comp, TransformPunctConexiune, i);
                        }
                }
                else if (event.key.code == sf::Keyboard::E) {
                    window.close();
                }
                break;

            case sf::Event::MouseMoved:
            {
                Vector2i coordonateMouse = Mouse::getPosition(window);
                if (status != DesenareLinie)
                {
                    punct max, min;
                    coordonateDupaRotire(element.chenar, &min, &max, comp.transformari.getTransform());
                   if (coordonateMouse.x >= min.x && coordonateMouse.y >= min.y
                       && coordonateMouse.x <= max.x && coordonateMouse.y <= max.y)
                    {
                        element.chenar.setOutlineColor(Color::Blue);
                    }
                    else
                        element.chenar.setOutlineColor(Color::Transparent);
                }
                else
                {
                    if (comp.conexiuneActiva != -1)
                    {
                        VertexArray liniiTemp(Lines);
                        liniiTemp = creareConexiuni((Vector2f)coordonateMouse, comp.liniiPtConexiuni[comp.conexiuneActiva][0]);
                        comp.liniiPtConexiuni[comp.conexiuneActiva].clear();
                        comp.liniiPtConexiuni[comp.conexiuneActiva] = liniiTemp;
                    }
                   
                }
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                Vector2i coordonateMouse = Mouse::getPosition(window);
                if (status != DesenareLinie)
                {
                    linie.clear();
                 
                    for (int i = 0; i < element.nrLegaturi; i++)
                    {
                        Vertex punctTransfomrat = comp.transformari.getTransform().transformPoint(element.legaturi[i].x, element.legaturi[i].y);
                        
                            if (abs(punctTransfomrat.position.x - coordonateMouse.x) < 30 && abs(punctTransfomrat.position.y - coordonateMouse.y) < 30)
                            {
                                comp.conexiuneActiva = i;
                                comp.areConexiuni[i] = true;
                                comp.liniiPtConexiuni[comp.conexiuneActiva].setPrimitiveType(Lines);

                                status = DesenareLinie;
                                comp.liniiPtConexiuni[comp.conexiuneActiva].append(punctTransfomrat);
                               
                            }
                    }
                }
                else
                {
                    conexiuneActiva = -1;
                    status = Normal;
                }
                break;
            }
            }

        }

        window.clear();
        //for i = 0 to n
        Transform transformElBara;
        transformElBara.translate(0, 50);
       
        desenareMeniuElemente(window, elemente, 12);
    //    desenareConexiuni(window, comp, element.nrLegaturi, element);
        for(int i = 0; i < element.nrLegaturi; i++)
            if(comp.areConexiuni[i])
         window.draw(comp.liniiPtConexiuni[i]);
        window.draw(element.linii, comp.transformari.getTransform());
        
        window.draw(element.chenar, comp.transformari.getTransform());
        //window.draw(line2, 2, Lines,t);

        window.display();
    }
    fclose(f);

}

int main()
{
    FILE* f = fopen("BATERIE.PS", "r");
   
    afisareElement(f);
    return 0;
}
