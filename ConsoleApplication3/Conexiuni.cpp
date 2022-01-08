#pragma once
#include <SFML/Graphics.hpp>
#include "Conexiuni.h"
using namespace sf;
VertexArray creareConexiuni(Legatura leg, Color culoareLinii)
{
    VertexArray linii(LinesStrip);
    Vertex punct;
    punct.color = culoareLinii;
    float distanta = leg.punct2.x - leg.punct1.x;
    punct.position = leg.punct1;
    linii.append(punct);

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

void modificareConexiuni(Vector2f coordonateInAfara, Circuit* circuit,  Transform TransformPunctConexiune)
{
    TransformPunctConexiune.translate(coordonateInAfara.x, coordonateInAfara.y);
    for (int i = 0; i < circuit->nrLegaturi; i++)
    {
        if (circuit->legaturi[i].idComponenta1 == circuit->componentaSelectata)
        {
            circuit->legaturi[i].punct1 = TransformPunctConexiune.transformPoint(circuit->legaturi[i].punct1);
        }
        else if (circuit->legaturi[i].idComponenta2 == circuit->componentaSelectata)
        {
            circuit->legaturi[i].punct2 = TransformPunctConexiune.transformPoint(circuit->legaturi[i].punct2);
        }
    }
}

bool verificareExistentaConexiune(Legatura legaturi[], int nrLegaturi, int idComponenta, int idPunctConexiune)
{
    for (int i = 0; i < nrLegaturi; i++)
    {
       // if ((legaturi[i].idComponenta1 == idComponenta || legaturi[i].idComponenta2 == idComponenta) && (legaturi[i].nrPunctComponenta1 == idPunctConexiune || legaturi[i].nrPunctComponenta2 == idPunctConexiune))
        if ((legaturi[i].idComponenta1 == idComponenta && legaturi[i].nrPunctComponenta1 == idPunctConexiune) || (legaturi[i].idComponenta2 == idComponenta && legaturi[i].nrPunctComponenta2 == idPunctConexiune))
        {
            for (int j = i; j < nrLegaturi - 1; j++)
                legaturi[j] = legaturi[j + 1];
            return true;
        }
    }
    return false;
}
