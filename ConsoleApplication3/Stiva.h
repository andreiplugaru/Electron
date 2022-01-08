#pragma once
#include "Structuri.h"
Nod* creareNod(int data);
bool esteVida(Nod* top);
bool areDoarPrimElement(Nod* top);
void push(Nod*& top, int data);
int pop(Nod*& top);
int prim(Nod* top);
int nrElemente(Nod* top);
int maxStiva(Nod* top);