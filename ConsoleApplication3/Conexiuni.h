#include <SFML/Graphics/VertexArray.hpp>
#include "Structuri.h"
#pragma once

using namespace sf;
VertexArray creareConexiuni(Legatura leg, Color culoareLinii);

void modificareConexiuni(Vector2f coordonateInAfara, Circuit* circuit, Transform TransformPunctConexiune);

bool verificareExistentaConexiune(Legatura legaturi[], int nrLegaturi, int idComponenta, int idPunctConexiune);
