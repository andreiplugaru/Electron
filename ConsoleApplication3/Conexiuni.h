#include <SFML/Graphics/VertexArray.hpp>
#include "Structuri.h"
#pragma once

using namespace sf;
VertexArray creareConexiuni(Legatura leg);

void modificareConexiuni(Vector2f coordonateInAfara, int nrLegaturi, Legatura* legaturi, int idComponenta, Transform TransformPunctConexiune);

bool verificareExistentaConexiune(Legatura legaturi[], int nrLegaturi, int idComponenta, int idPunctConexiune);
