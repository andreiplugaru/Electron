#pragma once
#include <SFML/Graphics.hpp>
#include "Conexiuni.h"
using namespace sf;
VertexArray creareConexiuni(Legatura leg)
{
    VertexArray linii(Lines); // de schimbat cu linestrip
    Vertex Punct;
    float distanta = leg.punct2.x - leg.punct1.x;
    linii.append(leg.punct1);

    Punct.position.x = leg.punct1.x + distanta / 2;
    Punct.position.y = leg.punct1.y;
    linii.append(Punct);
    linii.append(Punct);

    Punct.position.y = leg.punct2.y;
    linii.append(Punct);
    linii.append(Punct);

    Punct.position.x = leg.punct2.x;
    Punct.position.y = leg.punct2.y;
    linii.append(Punct);

    return linii;
}

void modificareConexiuni(Vector2f coordonateInAfara, int nrLegaturi, Legatura* legaturi, int idComponenta, Transform TransformPunctConexiune)
{
    TransformPunctConexiune.translate(coordonateInAfara.x, coordonateInAfara.y);
    for (int i = 0; i < nrLegaturi; i++)
    {
        if (legaturi[i].idComponenta1 == idComponenta)
        {
            legaturi[i].punct1 = TransformPunctConexiune.transformPoint(legaturi[i].punct1);
        }
        else if (legaturi[i].idComponenta2 == idComponenta)
        {
            legaturi[i].punct2 = TransformPunctConexiune.transformPoint(legaturi[i].punct2);
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
