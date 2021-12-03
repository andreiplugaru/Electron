#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <math.h>
#define MAX 100
#define SIN(x) sin(x * 3.141592653589 / 180)
#define COS(x) cos(x * 3.141592653589 / 180)
#define SIZECOEFINITIAL 50
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
struct obiect
{
    float offsetX;
    float offsetY;
    punct mijloc;
    float unghiRotire;
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
    punct legaturi[MAX];
    float sizeCoef = 50;
    RectangleShape chenar;
};
struct legatura
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
  //  string denumire;
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
VertexArray creareConexiuni(legatura l)
{
    VertexArray conexiuni(Lines);
    float distanta = l.pSfarsit.x - l.pPornire.x;

    Vertex punct;
    
    punct.position.x = l.pPornire.x + distanta / 2;
    punct.position.y = l.pPornire.y;
    conexiuni.append(punct);
    conexiuni.append(punct);

    punct.position.x = l.pPornire.x + distanta / 2;
    punct.position.y = l.pSfarsit.y;
    conexiuni.append(punct);
    conexiuni.append(punct);

    punct.position.x = l.pSfarsit.x;
    punct.position.y = l.pSfarsit.y;
    conexiuni.append(punct);

    return conexiuni;
}

void coordonateDupaRotire(RectangleShape patrat, punct* Min, punct* Max, Transform tp)
{
 //   punct Min, Max;
  //  Transform matrix = patrat.getTransform();
    Vector2f p1, p2, p3, p4;
    p1 = tp.transformPoint(patrat.getPoint(0));
    p2 = tp.transformPoint(patrat.getPoint(1));
    p3 = tp.transformPoint(patrat.getPoint(2));
    p4 = tp.transformPoint(patrat.getPoint(3));
    //x
   // patrat.getGlobalBounds 
    if (p1.x > (*Max).x)
    {
        (*Max).x = p1.x;
    }
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
    if (p1.y > (*Max).y)
    {
        (*Max).y = p1.y;
    }
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
      //  t.translate(47 / 2, 0);
      //  t.translate(latimeCasuta, 0);

      /*  VertexArray linieHotar(Lines);
        Vertex p1, p2;
        p1.position.x = 0;
        p1.position.y = 0;
        p1.color = Color::Green;
        p2.position.x = 0;
        p2.position.y = 50;
        p2.color = Color::Green;
        linieHotar.append(p1);
        linieHotar.append(p2);
        */
    }
}
void afisareElement(FILE* f)
{
    int i = 0;
    legatura l1;
    elCircuit Baterie =  citireCoordonate(f);

    RenderWindow window(VideoMode(1920, 1080), "SFML works!");
    
    window.setFramerateLimit(60);
    
    Transform t;
    Transform tp, tm;

 //   VertexArray lines(Lines);

    float xmin = 300, xmax = -300, ymin = 300, ymax = -300;
    statusuri status = Normal;
  
  
    Baterie.chenar.setFillColor(Color::Transparent);
    Baterie.chenar.setOutlineThickness(5.f);
    Baterie.chenar.setOutlineColor(Color::Transparent);
    
    Baterie.chenar.setSize(Vector2f(Baterie.latime, Baterie.inaltime));

    Vector2f origin;
    origin.x =Baterie.latime / 2;
    origin.y = Baterie.inaltime / 2;
  //  Baterie.chenar.setOrigin(origin);
   // tp.translate(200 , 200 );
    Vector2f size = Baterie.chenar.getSize();
    size.x *= SIZECOEFINITIAL;
    size.y *= SIZECOEFINITIAL;
  
    VertexArray linie(Lines);
    Vertex punctPornire;
    elCircuit elemente[13];
    for (int i = 0; i < 13; i++) elemente[i] = Baterie;
    Transformable transformablet;
    
    transformablet.move(origin.x, 300 + origin.y);
    transformablet.setOrigin(origin.x, origin.y);
    tp.translate(0, 300);
    tm.translate(0, 300);
    VertexArray conexiuni(Lines);
    bool areConexiuni = false;
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Right) {
                    transformablet.move(50, 0); 
                    tp.translate(50, 0);
                  //  Baterie.chenar.move(50, 0);
                    Transform TransformPunctConexiune;
                    TransformPunctConexiune.translate(50, 0);
                    l1.pPornire = TransformPunctConexiune.transformPoint(l1.pPornire);
                    if (areConexiuni)
                    {
                        conexiuni = creareConexiuni(l1);
                        linie.clear();
                        linie.append(l1.pPornire);
                        for (int i = 0; i < conexiuni.getVertexCount(); i++)
                        {
                            linie.append(conexiuni[i]);
                        }
                    }
                }
                if (event.key.code == sf::Keyboard::Left) {
                    t.translate(-50, 0);
                    tp.translate(-50, 0);
                    transformablet.move(-50, 0);
                    Transform TransformPunctConexiune;
                    TransformPunctConexiune.translate(-50, 0);
                    l1.pPornire = TransformPunctConexiune.transformPoint(l1.pPornire);
                    if (areConexiuni)
                    {
                        conexiuni = creareConexiuni(l1);
                        linie.clear();
                        linie.append(l1.pPornire);
                        for (int i = 0; i < conexiuni.getVertexCount(); i++)
                        {
                            linie.append(conexiuni[i]);
                        }
                    }
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    t.translate(0, -50);
                    tp.translate(0, -50);
                //    tm.translate(0, -50);
                    transformablet.move(0, -50);
                    if (areConexiuni)
                    {
                        l1.pPornire.y -= 50;
                        conexiuni = creareConexiuni(l1);
                        linie.clear();
                        linie.append(l1.pPornire);
                        for (int i = 0; i < conexiuni.getVertexCount(); i++)
                        {
                            linie.append(conexiuni[i]);
                        }
                    }
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    t.translate(0, 50);
                    tp.translate(0, 50);
                 //   tm.translate(0, 50);
                    transformablet.move(0, 50);
                    if (areConexiuni)
                    {
                        l1.pPornire.y += 50;
                        conexiuni = creareConexiuni(l1);
                        linie.clear();
                        linie.append(l1.pPornire);
                        for (int i = 0; i < conexiuni.getVertexCount(); i++)
                        {
                            linie.append(conexiuni[i]);
                        }
                    }
                }
                else if (event.key.code == sf::Keyboard::R) {
                    //t.rotate(10,origin.x, origin.y);
                    transformablet.rotate(10);
                    tp.rotate(10, origin);
                    tm.rotate(10, origin);
                    Transform TransformPunctConexiune;
                    TransformPunctConexiune.rotate(10, tp.transformPoint(origin));
                    l1.pPornire = TransformPunctConexiune.transformPoint(l1.pPornire);
                    if (areConexiuni)
                    {
                        conexiuni = creareConexiuni(l1);
                        linie.clear();
                        linie.append(l1.pPornire);
                        for (int i = 0; i < conexiuni.getVertexCount(); i++)
                        {
                            linie.append(conexiuni[i]);
                        }
                    }
                }
                else if (event.key.code == sf::Keyboard::M) {
                    Vector2f size = Baterie.chenar.getSize();
                    size.x *= 0.5;
                    size.y *= 0.4;
                    tp.scale(0.5, 0.5, origin.x, origin.y);
                    t.scale(0.5,0.5,origin.x, origin.y);
                    transformablet.scale(0.5, 0.5);
                    Transform TransformPunctConexiune;
                    TransformPunctConexiune.scale(0.5, 0.5, tp.transformPoint(origin).x, tp.transformPoint(origin).y);
                    l1.pPornire = TransformPunctConexiune.transformPoint(l1.pPornire);
                    if (areConexiuni)
                    {
                        conexiuni = creareConexiuni(l1);
                        linie.clear();
                        linie.append(l1.pPornire);
                        for (int i = 0; i < conexiuni.getVertexCount(); i++)
                        {
                            linie.append(conexiuni[i]);
                        }
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
                    coordonateDupaRotire(Baterie.chenar, &min, &max, transformablet.getTransform());

                    if (coordonateMouse.x >= min.x && coordonateMouse.y >= min.y
                        && coordonateMouse.x <= max.x && coordonateMouse.y <= max.y)
                    {
                        Baterie.chenar.setOutlineColor(Color::Blue);
                    }
                    else
                        Baterie.chenar.setOutlineColor(Color::Transparent);
                }
                else
                {
                    l1.pSfarsit.x = coordonateMouse.x;
                    l1.pSfarsit.y = coordonateMouse.y;
                    conexiuni = creareConexiuni(l1);
                    linie.clear();
                    linie.append(l1.pPornire);
                    for (int i = 0; i < conexiuni.getVertexCount(); i++)
                    {
                        linie.append(conexiuni[i]);
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
                    for (int i = 0; i < Baterie.nrLegaturi; i++)
                    {
                        Vertex punctTransfomrat = transformablet.getTransform().transformPoint(Baterie.legaturi[i].x, Baterie.legaturi[i].y);

                        if (abs(punctTransfomrat.position.x - coordonateMouse.x) < 30 && abs(punctTransfomrat.position.y - coordonateMouse.y) < 30)
                        {
                            l1.pPornire.x = punctTransfomrat.position.x;
                            l1.pPornire.y = punctTransfomrat.position.y;
                            status = DesenareLinie;
                            
                            areConexiuni = true;
                        }
                    }
                }
                else
                {
                   
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
        window.draw(linie);
        window.draw(Baterie.linii, transformablet.getTransform());
        
        window.draw(Baterie.chenar, transformablet.getTransform());
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
