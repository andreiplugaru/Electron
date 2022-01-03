#pragma once
#include "Structuri.h"
enum Mod { Salvare, Deschidere };
void salvareCircuit(Circuit* circuit, char numeFisier[]);
void deschidereCircuit(Circuit* circuit, char numeFisier[]);
bool deschidereFereastra(Mod mod);
void FereastraSucces(char mesajSucces[]);
void FereastraEroare(char mesajEroare[]);