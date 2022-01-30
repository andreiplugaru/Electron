#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Structuri.h"

void rotire(Circuit* circuit);

Vector2f ieseDinFereastra(Componenta comp, ElCircuit element);

bool verificareColiziune(Circuit* circuit, int idComponentaVerificata);

void rotireSlider(Circuit* circuit,Vector2i coordonateMouse);

void redimensionare(Circuit* circuit, float coeficient, unsigned idComponenta);

void redimensionareSlider(Circuit* circuit, Vector2i coordonateMouse,unsigned idComponenta);

void coordonateDupaTransformari(RectangleShape patrat, Punct* Min, Punct* Max, Transform tp);

VertexArray scalareLinii(VertexArray linii, float coef);

void miscareTastatura(Circuit* circuit, float offsetx, float offsety);

void miscareComponenta(Circuit* circuit, Vector2i coordonateMouse);

void redimensionareIntregCircuit(Circuit* circuit, float coeficient);

void redimensionareIntregCircuitSlider(Circuit* circuit, Vector2i coordMouse);


