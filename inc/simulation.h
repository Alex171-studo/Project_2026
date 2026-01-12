#ifndef SIMULATION_H
#define SIMULATION_H

#include "sensor.h"

#define STATION_X 0.0
#define STATION_Y 0.0
#define E_ELEC 0.05
#define E_AMP 0.01

void lancer_simulation(Capteur* c);
void reinitialiser_temps_simulation();
int etape_simulation(Capteur* c, int* paquets_envoyes);
float calculer_energie_transmission(float x, float y);
int tenter_transmission(Capteur* c);

#endif
