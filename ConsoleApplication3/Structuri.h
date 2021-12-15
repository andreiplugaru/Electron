#pragma once
#include <SFML/Graphics.hpp>
#define MAX 100
using namespace sf;

struct Punct
{
    float x;
    float y;
};
struct Componenta
{
    int idElement;
    bool areConexiuni[MAX] = { false };
    int conexiuneActiva;
    Vertex conexiuni[MAX];
    VertexArray liniiPtConexiuni[MAX];
    Transformable transformari;
    RectangleShape chenar;
    bool activ;
};
struct ElCircuit {
    char denumire[30]; // dioada, baterie, polarizator...
    VertexArray linii;
    CircleShape elipsa;
    VertexArray arc;
    RectangleShape dreptunghi;
    Vector2f origine;
    unsigned nrLegaturi;
    unsigned nrLinii;
    Vector2f dimensiuni;
    Vector2f legaturi[MAX];
    float sizeCoef = 50;
};
struct Legatura
{
    VertexArray linii[MAX];
    Vector2f punct1;
    Vector2f punct2;
    unsigned int nrPunctComponenta1;
    unsigned int nrPunctComponenta2;
    unsigned int idComponenta1;
    unsigned int idComponenta2;
};
struct ComponentaSelectata
{
    int id = -1;
};
enum Statusuri {
    DesenareLinie,
    Normal,
    InserareElement,
    InteractiuneComponenta,
    ModificarePozitieComponenta
};

struct PunctMinMax
{
    Punct minim;
    Punct maxim;
};
struct Optiuni
{
    float latimeChenar;
    float coefScalare;
};
