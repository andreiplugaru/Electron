#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <math.h>
#define MAX 100
#define SIN(x) sin(x * 3.141592653589 / 180)
#define COS(x) cos(x * 3.141592653589 / 180)
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
    VertexArray linii; // se pune .setPrimitiveType(Lines); 
    VertexArray triunghiuri;
    CircleShape cercuri;
    RectangleShape chenar;
    float offsetX;
    float offsetY;
    punct mijloc;
    float unghiRotire;
    float sizeCoef = 50;
    unsigned nrLegaturi;
    unsigned nrLinii;
    float inaltime;
    float latime;
    punct legaturi[MAX];
};
enum statusuri{
    DesenareLinie,
    Normal
};
/*void rotire(VertexArray* linii, punct centru, int unghi)
{
   for(int i = 0; i < (*linii).getVertexCount(); i++)
   {
        // Shifting the pivot point to the origin
        // and the given points accordingly
        int x_shifted = (*linii)[i].position.x - centru.x;
        int y_shifted = (*linii)[i].position.y - centru.y;

        // Calculating the rotated point co-ordinates
        // and shifting it back
        (*linii)[i].position.x = centru.x
            + (x_shifted * COS(unghi)
                - y_shifted * SIN(unghi));

        (*linii)[i].position.y = centru.y
            + (x_shifted * SIN(unghi)
                + y_shifted * COS(unghi));
        i++;
    }
}
*/
obiect citireCoordonate(FILE* f)
{
    obiect o1;
    int i = 0, nr = 0;
    o1.linii.setPrimitiveType(Lines);
    
    fscanf(f, "%d", &nr);
    for (int i = 0; i < nr; i++)
    {
       /* fscanf(f, "%f", &o1.legaturi[i].x);
        fscanf(f, "%f", &o1.legaturi[i].y);
        */
        float v;
        fscanf(f, "%f", &v);
        o1.legaturi[i].x = (v + 2)*o1.sizeCoef;
        fscanf(f, "%f", &v);
        o1.legaturi[i].y = (v + 1) * o1.sizeCoef;

    }
    o1.nrLegaturi = nr;
  
    fscanf(f, "%d", &nr);
    float xmin, xmax = 0, ymin, ymax = 0;
    for (int i = 0; i < nr; i++)
    {
        Vertex punct;

       /* fscanf(f, "%f", &punct.position.x);
        fscanf(f, "%f", &punct.position.y);
        o1.linii.append(punct);

        fscanf(f, "%f", &punct.position.x);
        fscanf(f, "%f", &punct.position.y);
        */
        float v;
        fscanf(f, "%f", &v);
        punct.position.x = (v + 2) * o1.sizeCoef;
        fscanf(f, "%f", &v);
        punct.position.y = (v + 1) * o1.sizeCoef;
        o1.linii.append(punct);

        fscanf(f, "%f", &v);
        punct.position.x = (v + 2) * o1.sizeCoef;
        fscanf(f, "%f", &v);
        punct.position.y = (v + 1) * o1.sizeCoef;
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
    o1.latime = xmax - xmin;
    o1.inaltime = ymax - ymin;
    o1.nrLinii = nr;
    fclose(f);

    return o1;
}
VertexArray creareConexiuni(Vector2i coordonateMouse, Vertex legatura)
{
    VertexArray conexiuni(Lines);
    float distanta = coordonateMouse.x - legatura.position.x;

    Vertex punct;

    punct.position.x = legatura.position.x + distanta / 2;
    punct.position.y = legatura.position.y;
    conexiuni.append(punct);
    conexiuni.append(punct);

    punct.position.x = legatura.position.x + distanta / 2;
    punct.position.y = coordonateMouse.y;
    conexiuni.append(punct);
    conexiuni.append(punct);

    punct.position.x = coordonateMouse.x;
    punct.position.y = coordonateMouse.y;
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
void afisareElement(FILE* f)
{
    int i = 0;
 //   linie l[MAX];
   // punct legaturi[MAX];
  //  int nrLinii = 0, nrLegaturi = 0;
    obiect Baterie =  citireCoordonate(f);

    RenderWindow window(VideoMode(600, 600), "SFML works!", Style::Fullscreen);
    
    window.setFramerateLimit(60);
    
    Transform t;
    Transform tp, tm;

 //   VertexArray lines(Lines);

    float xmin = 300, xmax = -300, ymin = 300, ymax = -300;
    statusuri status = Normal;
   /* for (int i = 0; i < nrLinii; i++)
    {
        lines.append(Vertex((Vector2f(l[i].p1.x, l[i].p1.y))));
        lines.append(Vertex((Vector2f(l[i].p2.x, l[i].p2.y))));
       

    }*/
   
  //  RectangleShape Baterie.chenar;
  
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
    size.x *= Baterie.sizeCoef;
    size.y *= Baterie.sizeCoef;
   // Baterie.chenar.setSize(size);
   // tp.scale(Baterie.sizeCoef, Baterie.sizeCoef);
  //  tp.translate(Baterie.latime / 2 * Baterie.sizeCoef, Baterie.inaltime / 2 * Baterie.sizeCoef);
   // Baterie.chenar.setPosition(xmin * 50 + 200 + latime / 2, ymin * 50 + 200 + inaltime / 2);
 
    VertexArray linie(Lines);
    Vertex punctPornire;
   // t.translate(200,200);
   // t.scale(Baterie.sizeCoef, Baterie.sizeCoef);
   // tm.scale(Baterie.sizeCoef, Baterie.sizeCoef);
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Right) {
                    t.translate(50, 0);
                    tp.translate(50, 0);
                    tm.translate(50, 0);
                   
                  //  Baterie.chenar.setOrigin(origin.x + 10, origin.y);
                //   tp.translate(10, 0);
                }
                if (event.key.code == sf::Keyboard::Left) {
                    t.translate(-50, 0);
                    tp.translate(-50, 0);
                    tm.translate(-50, 0);
                    Vector2f origin = Baterie.chenar.getOrigin();
                 //   Baterie.chenar.setOrigin(origin.x - 10, origin.y);
                   // tp.translate(-10, 0);


                }
                else if (event.key.code == sf::Keyboard::Up) {
                    t.translate(0, -50);
                    tp.translate(0, -50);
                    tm.translate(0, -50);
                    Vector2f origin = Baterie.chenar.getOrigin();
                   // Baterie.chenar.setOrigin(origin.x, origin.y - 10);
                 //   tp.translate(0, -10);

                }
                else if (event.key.code == sf::Keyboard::Down) {
                    t.translate(0, 50);
                    tp.translate(0, 50);
                    tm.translate(0, 50);
                    Vector2f origin = Baterie.chenar.getOrigin();
                
                }
                else if (event.key.code == sf::Keyboard::R) {
                    
                   // FloatRect dimBaterie.chenar = Baterie.chenar.getGlobalBounds();
              //      dimBaterie.chenar = tp.transformRect(dimBaterie.chenar);
                //    Baterie.chenar.
                   // cout << dimBaterie.chenar.top;
                  //  punct max, min;
                //    coordonateDupaRotire(Baterie.chenar, &min, &max, tp);

                    t.rotate(10,origin.x, origin.y);
                    tp.rotate(10, origin);
                    tm.rotate(10);
                   // tp.rotate(10, (max.x - min.x) / 2, (max.y - min.y) / 2);
                   /* punct centru;
                    centru.x = xmin * 50 + 200 + latime / 2;
                    centru.y = ymin * 50 + 200 + inaltime / 2;
                    //rotire(&lines, centru, 10);*/
                }
                else if (event.key.code == sf::Keyboard::M) {
                    Vector2f size = Baterie.chenar.getSize();
                    size.x *= 0.5;
                    size.y *= 0.4;
                  // Baterie.chenar.setSize(size);
                    //Baterie.chenar.scale(0.5, 0.5, origin.x, origin.y);
                    tp.scale(0.5, 0.5, origin.x, origin.y);
                    t.scale(0.5,0.5,origin.x, origin.y);
                    
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
                   // FloatRect Baterie.chenarBounds = coveringRect(Baterie.chenar, tp);
                    //    Baterie.chenar.getRotation()
                  

                    punct max, min;
                    coordonateDupaRotire(Baterie.chenar, &min, &max, tp);
                    //  Vector2f punctRotire = tm.transformPoint((Vector2f)coordonateMouse);
                    cout << "Mouse";
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
                    VertexArray conexiuni = creareConexiuni(coordonateMouse, linie[0]);
                    linie.clear();
                    linie.append(punctPornire);
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
                        Vertex punctTransfomrat = t.transformPoint(Baterie.legaturi[i].x , Baterie.legaturi[i].y);

                        if (abs(punctTransfomrat.position.x- coordonateMouse.x) < 25 && abs(punctTransfomrat.position.y- coordonateMouse.y) < 25)
                        {
                            status = DesenareLinie;
                            punctPornire.position.x = punctTransfomrat.position.x;
                            punctPornire.position.y = punctTransfomrat.position.y;
                            linie.append(punctPornire);
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
        window.draw(linie, tm);
        window.draw(Baterie.linii, t);
        
        window.draw(Baterie.chenar,tp);
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
