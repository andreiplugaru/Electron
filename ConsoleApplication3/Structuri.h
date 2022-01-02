#pragma once
#include <SFML/Graphics.hpp>
#include "Defineuri.h"
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
    char nume[MAXNUME] = { 0 };
    unsigned rezistenta = 0;
    unsigned voltaj = 0;
    bool activ;
};
struct ElCircuit {
    char denumire[30]; // dioada, baterie, polarizator...
    VertexArray linii;
    CircleShape elipsa;
    VertexArray arc;
    RectangleShape dreptunghi;
    Vector2f origine;
    RectangleShape chenar;
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
    ModificarePozitieComponenta,
    ModificareParametri
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
    int distantaMiscare;
    int unghiRotire;
};
struct Circuit 
{
    Componenta componente[MAX];
    ElCircuit elemente[NUMAR_ELEMENTE];
    Legatura legaturi[MAX];
    unsigned componentaSelectata = -1
};
