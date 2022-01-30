#include <SFML/Graphics/VertexArray.hpp>
#include "Structuri.h"
#pragma once

using namespace sf;
VertexArray creareConexiuni(Legatura leg, Color culoareLinii);

void modificareConexiuni(Vector2f coordonateInAfara, Circuit* circuit, Transform TransformPunctConexiune, unsigned idComponenta);

bool verificareExistentaConexiune(Legatura legaturi[], int nrLegaturi, int idComponenta, int idPunctConexiune);

void inserarePunct2Conexiune(Circuit* circuit, Vector2i coordonateMouse);

void stergereLegaturi(Circuit* circuit);
