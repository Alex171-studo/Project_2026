#include "../inc/sensor.h"
#include <stdio.h>
#include <stdlib.h>


static int compteur_id_paquet = 1;

Capteur* creer_capteur(float x, float y, float batt_init) {
    Capteur* c = (Capteur*)malloc(sizeof(Capteur));
    if (!c) {
        perror("Echec allocation Capteur");
        exit(EXIT_FAILURE);
    }
    c->batterie = (batt_init > 0) ? batt_init : BATTERY_INIT;
    c->x = x;
    c->y = y;
    c->buffer_tete = NULL;
    c->buffer_usage = 0;
    
    return c;
}

void liberer_capteur(Capteur* c) {
    if (!c) return;

    Paquet* courant = c->buffer_tete;
    while (courant != NULL) {
        Paquet* suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }
    free(c);
}

void produire_paquet(Capteur* c) {
    if (!c) return;
    
    Paquet* nouveau_paquet = (Paquet*)malloc(sizeof(Paquet));
    if (!nouveau_paquet) {
        perror("Echec allocation Paquet");
        return;
    }
    
    nouveau_paquet->id = compteur_id_paquet++;
    nouveau_paquet->valeur = ((float)rand() / (float)RAND_MAX) * 100.0f;
    nouveau_paquet->timestamp = (long)time(NULL);
    nouveau_paquet->suivant = NULL;

    if (c->buffer_usage >= MAX_BUFFER_SIZE) {
        if (c->buffer_tete != NULL) {
            Paquet* vieux_paquet = c->buffer_tete;
            c->buffer_tete = vieux_paquet->suivant;
            
            printf("ALERTE : Mémoire saturée. Suppression du paquet ID [%d] pour libérer de l'espace.\n", vieux_paquet->id);
            
            free(vieux_paquet);
            c->buffer_usage--;
        }
    }

    if (c->buffer_tete == NULL) {
        c->buffer_tete = nouveau_paquet;
    } else {
        Paquet* courant = c->buffer_tete;
        while (courant->suivant != NULL) {
            courant = courant->suivant;
        }
        courant->suivant = nouveau_paquet;
    }
    c->buffer_usage++;
    
    printf("📦 Paquet Genere (ID: %d, Val: %.2f) | Usage: %d/%d\n", 
           nouveau_paquet->id, nouveau_paquet->valeur, c->buffer_usage, MAX_BUFFER_SIZE);
}

void afficher_etat_capteur(const Capteur* c) {
    if (!c) return;
    printf("\n--- Etat Capteur ---\n");
    printf("Batterie: %.2f J\n", c->batterie);
    printf("Position: (%.2f, %.2f)\n", c->x, c->y);
    printf("Buffer Usage: %d/%d\n", c->buffer_usage, MAX_BUFFER_SIZE);
    
    Paquet* p = c->buffer_tete;
    printf("Paquets: [ ");
    while (p != NULL) {
        printf("{ID:%d Val:%.1f} ", p->id, p->valeur);
        p = p->suivant;
    }
    printf("]\n---------------------\n");
}

void fixer_id_compteur(int nouvel_id) {
    if (nouvel_id >= compteur_id_paquet) {
        compteur_id_paquet = nouvel_id + 1;
    }
}
