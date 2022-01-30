#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

void desenareMeniuOptiuni(RenderWindow& window, Font font);

int optiuneMeniuSelectata(Vector2i coordonateMouse);

void desenareMeniuElemente(RenderWindow& window, ElCircuit elemente[], int nrElemente, float* latimeCasuta);
