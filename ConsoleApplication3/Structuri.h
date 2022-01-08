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
  //  VertexArray liniiPtConexiuni[MAX];
    Transformable transformari;
    char nume[MAX_NUME_COMPONENTA] = { 0 };
    unsigned rezistenta = 0;
    unsigned voltaj = 0;
    bool activ;
};
struct ElCircuit {
    char denumire[MAXNUME]; // dioada, baterie, polarizator...
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
    Vector2f punct1;
    Vector2f punct2;
    unsigned int nrPunctComponenta1;
    unsigned int nrPunctComponenta2;
    unsigned int idComponenta1;
    unsigned int idComponenta2;
};

enum Statusuri {
    DesenareLinie,
    Normal,
    InserareElement,
    InteractiuneComponenta,
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
  //  unsigned int pasiUndoRedo;
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
    int componentaSelectata = -1;
    unsigned nrComponente = 0;
    unsigned nrLegaturi =  0;
    Nod* topUndo;
    Nod* topRedo;
    char numeFisier[MAX_LUNGIME_FISIER] = {};
    bool esteSalvat = true;
};
//char numeOptiuniSalvate[NUMAR_OPTIUNI][40] = { "Coeficient scalare", "Distanta miscare", "Unghi rotire" };
