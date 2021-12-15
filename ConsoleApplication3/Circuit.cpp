#define _CRT_SECURE_NO_WARNINGS
#include "Structuri.h"
#include <conio.h>
void salvareCircuit(Componenta* comp, int nrComponente)
{
    char numeFisier[100] = "circuit.bmp";
    FILE* f = fopen(numeFisier, "wb");
    fwrite(&nrComponente, sizeof(int), 1, f);
    fwrite(comp, sizeof(Componenta), nrComponente, f);
    fclose(f);
}
void deschidereCircuit(Componenta *comp, int* nrComponente)
{
      char numeFisier[100] = "circuit.bmp";
    FILE* f = fopen(numeFisier, "rb");
    fread(nrComponente, sizeof(int), 1, f);
    
    int m = sizeof(Componenta);
    fread(comp, sizeof(Componenta), *nrComponente, f);
    for (int i = 0; i < *nrComponente; i++)
    {
        VertexArray r;
        comp[i].chenar.setTexture(NULL);
        comp[i].liniiPtConexiuni[0] = r;
        Color c = comp[i].chenar.getFillColor();
        comp[i].chenar.setFillColor(Color::Transparent);
        comp[i].chenar.setOutlineThickness(0);
        comp[i].chenar.setOutlineColor(Color::Transparent);
        comp[i].chenar.setSize(Vector2f(0, 0));
    }
 //   comp[0].chenar.setOutlineColor(Color::Black);
    fclose(f);
}
