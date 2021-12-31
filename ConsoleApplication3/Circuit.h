#pragma once
#include "Structuri.h"
enum Mod { Salvare, Deschidere };
void salvareCircuit(Componenta* comp, int nrComponente, Legatura* legaturi, int nrLegaturi, char numeFisier[]);
void deschidereCircuit(Componenta* comp, int* nrComponente, Legatura* legaturi, int* nrLegaturi, char numeFisier[]);
bool deschidereFereastra(Mod mod);
void FereastraSucces(char mesajSucces[]);
void FereastraEroare(char mesajEroare[]);