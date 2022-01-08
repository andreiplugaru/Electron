#pragma once
#include "Structuri.h"
enum Mod { Salvare, Deschidere };
void salvareCircuit(Circuit* circuit, char numeFisier[], bool afisareMesaj);
bool deschidereCircuit(Circuit* circuit, char numeFisier[], bool afisareMesaj);
bool deschidereFereastra(Mod mod);
void FereastraSucces(char mesajSucces[]);
void FereastraEroare(char mesajEroare[]);
void salvareIstoric(Circuit* circuit);
void deschidereIstoric(Circuit* circuit);
void stergereIstoric(Circuit* circuit);
void deschidereIstoricRedo(Circuit* circuit);
void salvareIstoricRedo(Circuit* circuit);
void exporta(RenderWindow &window);