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
    Vertex conexiuni[MAX];
    Transformable transformari;
    char nume[MAX_NUME_COMPONENTA] = { 0 };
    bool areConexiuni[MAX] = { false };
    int idElement;
    unsigned rezistenta = 0;
    unsigned voltaj = 0;
    bool activ;
};
struct ElCircuit {
    VertexArray linii;
    CircleShape elipsa;
    VertexArray arc;
    RectangleShape dreptunghi;
    Vector2f origine;
    RectangleShape chenar;
    Vector2f dimensiuni;
    Vector2f legaturi[MAX];
    char denumire[MAXNUME]; 
    float sizeCoef = 50;
    unsigned nrLegaturi;
    unsigned nrLinii;
   
};
struct Legatura
{
    Vector2f punct1;
    Vector2f punct2;
    unsigned int nrPunctComponenta1;
    unsigned int nrPunctComponenta2;
    unsigned int idComponenta1;
    unsigned int idComponenta2;
};

enum Statusuri {
    CreareConexiune,
    Normal,
    InserareElement,
    ModificarePozitieComponenta,
    ModificareParametri,
    ModificareCoeficientScalare,
    ModificareUnghiRotire
};

struct PunctMinMax
{
    Punct minim;
    Punct maxim;
};
struct Optiuni
{
    float coefScalare;
    int distantaMiscare;
    int unghiRotire;
    Color culoareConexiuni;
};
struct Nod {
    int data;
    Nod* next;
};
struct Circuit 
{
    Componenta *componente;
    ElCircuit *elemente;
    Legatura *legaturi;
    Optiuni optiuni;
    Nod* topUndo;
    Nod* topRedo;
    int componentaSelectata = -1;
    unsigned nrComponente = 0;
    unsigned nrLegaturi =  0;
    float latimeCasuta;
    float sizeCoef = 1;
    char numeFisier[MAX_LUNGIME_FISIER] = {};
    bool esteSalvat = true;
    Statusuri status = Normal;
};
