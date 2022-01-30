#pragma once
#include "Structuri.h"

Nod* creareNod(int data) 
{
    Nod* nodNou = new Nod();
    nodNou->data = data;
    nodNou->next = NULL;
    return nodNou;
}
void push(Nod*& top, int data) 
{
    Nod* nodNou = creareNod(data);
    nodNou->next = top;
    top = nodNou;
}
int nrElemente(Nod* top)
{
    Nod* nodTemp = top;
    int numar = 0;
    while (nodTemp != NULL)
    {
        numar++;
        nodTemp = nodTemp->next;
    }
    return numar;
}
bool esteVida(Nod* top)
{
    return  top == NULL;
}
bool areDoarPrimElement(Nod* top)
{
    int numar = nrElemente(top);
    return  numar == 0 || numar == 1;
}
int maxStiva(Nod* top) 
{
    Nod* nodTemp = top;
    int max = 0;
    while (nodTemp != NULL)
    {
        if (nodTemp->data > max)
            max = nodTemp->data;
        nodTemp = nodTemp->next;
    }
    return max;
}
int pop(Nod*& top)
{
    
    if (esteVida(top))
        return 0;
    Nod* tmp = top;
    top = top->next;
    int rezultat = tmp->data;
    delete tmp;
    return rezultat;
}
int prim(Nod* top) 
{
    int numar = nrElemente(top);
    if (numar == 0)
        return 0;
    return top->data;
}
