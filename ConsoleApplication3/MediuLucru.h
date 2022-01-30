#pragma once
void afisareElemente(Circuit* circuit);

void afisareDoarComponente(RenderWindow& window, Circuit* circuit);

void coordonateArc(ElCircuit& element, Vertex origine, float raza, PunctMinMax extreme, float offset);

PunctMinMax aflareCoordonateMinimeMaxime(char locatieFisier[]);

ElCircuit citireInformatii(char locatieFisier[]);

void citireJSON(Optiuni* optiuni);

PunctMinMax puncteMinimSiMaxim(Vector2f puncte[], int nrPuncte);

void stergereComponenta(Circuit* circuit);

void resetareDateComponenta(Componenta* comp);