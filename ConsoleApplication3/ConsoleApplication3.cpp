#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <math.h>
#define MAX 100
#define SIN(x) sin(x * 3.141592653589 / 180)
#define COS(x) cos(x * 3.141592653589 / 180)
#define SIZECOEFINITIAL 50
#define INALTIMEMENIUELEMENTE 150
#define PI 3.141592
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
    int idElement;
    float offsetX;
    float offsetY;
    
    float unghiRotire;
    bool areConexiuni[MAX] = { false };
    int conexiuneActiva;
    Vertex conexiuni[MAX];
    VertexArray liniiPtConexiuni[MAX];
    Transformable transformari;
    RectangleShape chenar;
    bool activ;
};
struct elCircuit {
    char denumire[30];
    VertexArray linii;
    CircleShape elipsa;
    VertexArray arc;
    RectangleShape patrat;
    Vector2f origine;
    unsigned nrLegaturi;
    unsigned nrLinii;
    float inaltime;
    float latime;
    Vector2f legaturi[MAX];
    float sizeCoef = 50;
   
};
struct Legatura
{
    Vector2f pPornire;
    Vector2f pSfarsit;
};
struct ComponentaActiva
{
    int id = -1;
};
enum statusuri{
    DesenareLinie,
    Normal,
    InserareElement,
    InteractiuneComponenta
};
float latimeCasuta;
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
    VertexArray linii(Lines); // de schimbat cu linestrip
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
   
    latimeCasuta = (1920 - (nrElemente + 1)*10) / nrElemente;
    Vector2f dimCasuta;
    dimCasuta.x = latimeCasuta;
    dimCasuta.y = 50;
    t.translate(-latimeCasuta + (latimeCasuta - elemente[0].latime * scale) / 2, 0);
    for (int i = 0; i < nrElemente; i++)
    {
        RectangleShape casuta;
        sf::Color albastru(0, 102, 204);
        casuta.setPosition(latimeCasuta * i + 10*(i+1), 100);
        casuta.setSize(dimCasuta);
        casuta.setFillColor(albastru);
        window.draw(casuta);
        t.translate(offsetX + latimeCasuta,0);
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
void setareChenar(Componenta *c, elCircuit tipElement)
{
    (*c).chenar.setFillColor(Color::Transparent);
    (*c).chenar.setOutlineThickness(5.f);
    (*c).chenar.setOutlineColor(Color::Transparent);
    (*c).chenar.setSize(Vector2f(tipElement.latime, tipElement.inaltime));
}
void afisareElement(FILE* f)
{
    int conexiuneActiva = -1;
    int i = 0;
    int nrComponenteInserate = 0;
    elCircuit element =  citireCoordonate(f);
    Componenta comp[MAX];
    Legatura legaturi[MAX];
    RenderWindow window(VideoMode(1920, 1080), "SFML works!");
    
    window.setFramerateLimit(60);
 
    float xmin = 300, xmax = -300, ymin = 300, ymax = -300;
    statusuri status = Normal;

   

    element.origine.x =element.latime / 2;
    element.origine.y = element.inaltime / 2;
  
    VertexArray linie(Lines);
    Vertex punctPornire;

    elCircuit elemente[13];
    for (int i = 0; i < 13; i++) elemente[i] = element;
    
 //   comp.transformari.move(element.origine.x, 300 + element.origine.y);
   
    VertexArray conexiuni(Lines);
    ComponentaActiva compActiv;
     
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Right) {
                    comp[compActiv.id].transformari.move(50, 0);
                    for(int i = 0; i < element.nrLegaturi; i++)
                        if (comp[compActiv.id].areConexiuni[i])
                        {
                            Transform TransformPunctConexiune;
                            TransformPunctConexiune.translate(50, 0);
                            modificareConexiuni(&comp[compActiv.id], TransformPunctConexiune, i);
                        }
                }
                if (event.key.code == sf::Keyboard::Left) {
                    comp[compActiv.id].transformari.move(-50, 0);
                    for (int i = 0; i < element.nrLegaturi; i++)
                        if (comp[compActiv.id].areConexiuni[i])
                        {
                            Transform TransformPunctConexiune;
                            TransformPunctConexiune.translate(-50, 0);
                            modificareConexiuni(&comp[compActiv.id], TransformPunctConexiune, i);
                        }
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    comp[compActiv.id].transformari.move(0, -50);
                    for (int i = 0; i < element.nrLegaturi; i++)
                        if (comp[compActiv.id].areConexiuni[i])
                        {
                            Transform TransformPunctConexiune;
                            TransformPunctConexiune.translate(0, -50);
                            modificareConexiuni(&comp[compActiv.id], TransformPunctConexiune, i);
                        }
                   
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    comp[compActiv.id].transformari.move(0, 50);
                    for (int i = 0; i < element.nrLegaturi; i++)
                        if (comp[compActiv.id].areConexiuni[i])
                        {
                            Transform TransformPunctConexiune;
                            TransformPunctConexiune.translate(0, 50);
                            modificareConexiuni(&comp[compActiv.id], TransformPunctConexiune, i);
                        }
                }
                else if (event.key.code == sf::Keyboard::R) {
                    comp[compActiv.id].transformari.rotate(10);
                    for (int i = 0; i < element.nrLegaturi; i++)
                        if (comp[compActiv.id].areConexiuni[i])
                        {
                            Transform TransformPunctConexiune;
                            TransformPunctConexiune.rotate(10, comp[compActiv.id].transformari.getTransform().transformPoint(elemente[comp[compActiv.id].idElement].origine));
                            modificareConexiuni(&comp[compActiv.id], TransformPunctConexiune, i);
                        }
                }
                else if (event.key.code == sf::Keyboard::M) {
                  /*  Vector2f size = element.chenar.getSize();
                    size.x *= 0.5;
                    size.y *= 0.4;
                   */
                    comp[compActiv.id].transformari.scale(0.5, 0.5);

                    for (int i = 0; i < element.nrLegaturi; i++)
                        if (comp[compActiv.id].areConexiuni[i])
                        {
                            Transform TransformPunctConexiune;
                            TransformPunctConexiune.scale(0.5, 0.5, comp[compActiv.id].transformari.getTransform().transformPoint(elemente[comp[compActiv.id].idElement].origine).x, comp[compActiv.id].transformari.getTransform().transformPoint(elemente[comp[compActiv.id].idElement].origine).y);
                            modificareConexiuni(&comp[compActiv.id], TransformPunctConexiune, i);
                        }
                }
                else if (event.key.code == sf::Keyboard::E) {
                    window.close();
                }
                break;
                
            case sf::Event::MouseMoved:
            {
                Vector2i coordonateMouse = Mouse::getPosition(window);
                if (status == Normal)
                {
                    punct max, min;
                    for (int i = 0; i < nrComponenteInserate; i++)
                    {
                        
                            coordonateDupaRotire(comp[i].chenar, &min, &max, comp[i].transformari.getTransform());
                            if (coordonateMouse.x >= min.x && coordonateMouse.y >= min.y
                                && coordonateMouse.x <= max.x && coordonateMouse.y <= max.y)
                            {
                                comp[i].activ = true;
                                comp[i].chenar.setOutlineColor(Color::Blue);
                            }
                            else
                            {
                                comp[i].activ = false;
                                if (compActiv.id != i)
                                    comp[i].chenar.setOutlineColor(Color::Transparent);
                            }
                        
                    }
                }
                else if (status == DesenareLinie)
                {
                    if (comp[compActiv.id].conexiuneActiva != -1)
                    {
                        VertexArray liniiTemp(Lines);
                        liniiTemp = creareConexiuni((Vector2f)coordonateMouse, comp[compActiv.id].liniiPtConexiuni[comp[compActiv.id].conexiuneActiva][0]);
                        comp[compActiv.id].liniiPtConexiuni[comp[compActiv.id].conexiuneActiva].clear();
                        comp[compActiv.id].liniiPtConexiuni[comp[compActiv.id].conexiuneActiva] = liniiTemp;
                    }
                   
                }
                if(status == InserareElement)
                {
                    comp[nrComponenteInserate-1].transformari.setPosition(coordonateMouse.x, coordonateMouse.y);
                }
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                Vector2i coordonateMouse = Mouse::getPosition(window);
                if (status == Normal)
                {
                    if (coordonateMouse.y >= 50 && coordonateMouse.y <= INALTIMEMENIUELEMENTE)
                    {
                        for(int i = 0; i < 12; i++)
                            if (coordonateMouse.x > i * latimeCasuta && coordonateMouse.x < (i + 1) * latimeCasuta)
                            {
                                status = InserareElement;
                                comp[nrComponenteInserate].idElement = i;
                                comp[nrComponenteInserate].transformari.setOrigin(element.origine.x, element.origine.y);
                                nrComponenteInserate++;
                            }
                    }
                    else
                    {
                        for (int i = 0; i < nrComponenteInserate; i++)
                        {
                            if (comp[i].activ)
                            {
                                compActiv.id = i;
                                comp[compActiv.id].chenar.setOutlineColor(Color::Blue);
                                break;
                               
                            }
                            else
                                comp[i].chenar.setOutlineColor(Color::Transparent);
                            compActiv.id = -1;
                        }
                    }
                    if(compActiv.id!=-1)
                        for (int i = 0; i < elemente[comp[compActiv.id].idElement].nrLegaturi; i++)
                        {
                            Vertex punctTransfomrat = comp[compActiv.id].transformari.getTransform().transformPoint(elemente[comp[compActiv.id].idElement].legaturi[i].x, elemente[comp[compActiv.id].idElement].legaturi[i].y);
                        
                                if (abs(punctTransfomrat.position.x - coordonateMouse.x) < 30 && abs(punctTransfomrat.position.y - coordonateMouse.y) < 30)
                                {
                                    comp[compActiv.id].conexiuneActiva = i;
                                    comp[compActiv.id].areConexiuni[i] = true;
                                    comp[compActiv.id].liniiPtConexiuni[comp[compActiv.id].conexiuneActiva].setPrimitiveType(Lines);

                                    status = DesenareLinie;
                                    comp[compActiv.id].liniiPtConexiuni[comp[compActiv.id].conexiuneActiva].append(punctTransfomrat);
                               
                                }
                        }
                }
                else if(status == DesenareLinie)
                {
                    conexiuneActiva = -1;
                    status = Normal;
                }
                else if(status == InserareElement)
                {
                    setareChenar(&comp[nrComponenteInserate - 1], elemente[comp[nrComponenteInserate - 1].idElement]);
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
        for (int j = 0; j < nrComponenteInserate; j++)
        {
            for (int i = 0; i < elemente[comp[j].idElement].nrLegaturi; i++)
                if (comp[j].areConexiuni[i])
                    window.draw(comp[j].liniiPtConexiuni[i]);

            window.draw(elemente[comp[j].idElement].linii, comp[j].transformari.getTransform());
            window.draw(comp[j].chenar, comp[j].transformari.getTransform());
        }

        sf::VertexArray player(LineStrip);
        double degrees = 360.0 / 32.0;

        for (double i = 0; i < 32; i++)
        {
            float value = (PI * 2) / i;
            float y = sin(value) * degrees*10;
            float x = cos(value) * degrees*10;
            Vertex p;
            p.position.x = 300+x;
            p.position.y = 300+y;
            player.append(p);
        }

        window.draw(player);

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
