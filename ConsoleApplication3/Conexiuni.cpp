#pragma once
#include <SFML/Graphics.hpp>
#include "Conexiuni.h"
#include "Structuri.h"
#include "Circuit.h"
#include <iostream>
using namespace sf;

VertexArray creareConexiuni(Legatura leg, Color culoareLinii)
{
    VertexArray linii(LinesStrip);
    Vertex punct;
    punct.color = culoareLinii;
    punct.position = leg.punct1;
    linii.append(punct);

    float distanta = leg.punct2.x - leg.punct1.x;
    punct.position.x = leg.punct1.x + distanta / 2;
    punct.position.y = leg.punct1.y;
    linii.append(punct);

    punct.position.y = leg.punct2.y;
    linii.append(punct);

    punct.position.x = leg.punct2.x;
    punct.position.y = leg.punct2.y;
    linii.append(punct);

    return linii;
}
void modificareConexiuni(Vector2f coordonateInAfara, Circuit* circuit, Transform TransformPunctConexiune, unsigned idComponenta)
{
    TransformPunctConexiune.translate(coordonateInAfara.x, coordonateInAfara.y);
    for (int i = 0; i < circuit->nrLegaturi; i++)
    {
        if (circuit->legaturi[i].idComponenta1 == idComponenta)
        {
            circuit->legaturi[i].punct1 = TransformPunctConexiune.transformPoint(circuit->legaturi[i].punct1);
        }
        else if (circuit->legaturi[i].idComponenta2 == idComponenta)
        {
            circuit->legaturi[i].punct2 = TransformPunctConexiune.transformPoint(circuit->legaturi[i].punct2);
        }
    }
}
bool verificareExistentaConexiune(Legatura legaturi[], int nrLegaturi, int idComponenta, int idPunctConexiune)
{
    for (int i = 0; i < nrLegaturi; i++)
    {
        if ((legaturi[i].idComponenta1 == idComponenta && legaturi[i].nrPunctComponenta1 == idPunctConexiune) || 
            (legaturi[i].idComponenta2 == idComponenta && legaturi[i].nrPunctComponenta2 == idPunctConexiune))
        {
            for (int j = i; j < nrLegaturi; j++)
                legaturi[j] = legaturi[j + 1];
            return true;
        }
    }
    return false;
}
void inserarePunct2Conexiune(Circuit* circuit, Vector2i coordonateMouse)
{
    for (int j = 0; j < circuit->nrComponente; j++)
    {
        for (int i = 0; i < circuit->elemente[circuit->componente[j].idElement].nrLegaturi; i++)
        {
            Vector2f PunctTransfomrat = circuit->componente[j].transformari.getTransform().transformPoint(circuit->elemente[circuit->componente[j].idElement].legaturi[i].x,
                circuit->elemente[circuit->componente[j].idElement].legaturi[i].y);//coordonate punct conexiune dupa transformari

            if (circuit->legaturi[circuit->nrLegaturi - 1].idComponenta1 != j && abs(PunctTransfomrat.x - coordonateMouse.x) < 10 && abs(PunctTransfomrat.y - coordonateMouse.y) < 10)
            {
                if (verificareExistentaConexiune(circuit->legaturi, circuit->nrLegaturi, j, i))
                    circuit->nrLegaturi--;

                circuit->legaturi[circuit->nrLegaturi - 1].idComponenta2 = j;
                circuit->legaturi[circuit->nrLegaturi - 1].nrPunctComponenta2 = i;
                circuit->legaturi[circuit->nrLegaturi - 1].punct2 = PunctTransfomrat;
                circuit->status = Normal;
                salvareIstoric(circuit);
                break;
            }
        }
    }
    if (circuit->status != Normal)
    {
        circuit->legaturi[circuit->nrLegaturi - 1] = {};
        circuit->nrLegaturi--;
        circuit->status = Normal;
    }
}
void stergereLegaturi(Circuit* circuit)
{
    int i = 0;
    while (i < circuit->nrLegaturi)
    {
        if (circuit->legaturi[i].idComponenta1 == circuit->componentaSelectata || circuit->legaturi[i].idComponenta2 == circuit->componentaSelectata)
        {
            for (int j = i; j < circuit->nrLegaturi; j++)
                circuit->legaturi[j] = circuit->legaturi[j + 1];
            
            circuit->nrLegaturi--;
        }
        else
        {
            i++;
        }
    }
   
}
