#ifndef SENSOR_H
#define SENSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BUFFER_SIZE 5
#define BATTERY_INIT 100.0

typedef struct Paquet {
    int id;
    float valeur;
    long timestamp;
    struct Paquet* suivant;
} Paquet;

typedef struct Capteur {
    float batterie;
    float x;
    float y;
    Paquet* buffer_tete;
    int buffer_usage;
} Capteur;

Capteur* creer_capteur(float x, float y);
void liberer_capteur(Capteur* c);
void produire_paquet(Capteur* c);
void afficher_etat_capteur(const Capteur* c);

#endif
