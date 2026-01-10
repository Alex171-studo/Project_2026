#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "sensor.h"

#define SAVE_FILENAME "save.bin"

int sauvegarder_etat(const Capteur* c);
Capteur* charger_etat();

#endif
