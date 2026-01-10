#include "../inc/simulation.h"
#include <stdio.h>
#include <math.h>
#include <unistd.h>

static int simulation_temps = 0;

float calculer_energie_transmission(float x, float y) {
    float distance_squared = (x * x) + (y * y);
    return E_ELEC + (E_AMP * distance_squared);
}

int tenter_transmission(Capteur* c) {
    if (!c || c->buffer_tete == NULL) {
        return 0;
    }

    float cout_energie = calculer_energie_transmission(c->x, c->y);
    int paquets_envoyes = 0;

    while (c->buffer_tete != NULL) {
        if (c->batterie < cout_energie) {
            printf("Batterie faible! Impossible de transmettre paquet ID %d\n", c->buffer_tete->id);
            break; 
        }

        Paquet* temp = c->buffer_tete;
        c->buffer_tete = temp->suivant;
        
        c->batterie -= cout_energie;
        if (c->batterie < 0) c->batterie = 0;

        free(temp);
        c->buffer_usage--;
        paquets_envoyes++;
    }
    
    return paquets_envoyes;
}

void ecrire_log(const Capteur* c) {
    FILE* f = fopen("log.txt", "a");
    if (f) {
        fprintf(f, "Temps: %ds | Batterie: %.2fJ | Paquets en attente: %d\n", 
                simulation_temps, c->batterie, c->buffer_usage);
        fclose(f);
    }
}

int etape_simulation(Capteur* c, int* paquets_envoyes_ptr) {
    if (c->batterie <= 0) return 0;

    simulation_temps++;

    produire_paquet(c);

    int envoyes = tenter_transmission(c);
    if (paquets_envoyes_ptr) *paquets_envoyes_ptr = envoyes;
    
    if (envoyes > 0) {
        printf("Transmission: %d paquets envoyes a la Station.\n", envoyes);
    }

    ecrire_log(c);

    return (c->batterie > 0);
}

void lancer_simulation(Capteur* c) {
    printf("Demarrage Simulation...\n");
    int total_transmis = 0;

    while (c->batterie > 0) {
        int envoyes = 0;
        int vivant = etape_simulation(c, &envoyes);
        total_transmis += envoyes;
        
        afficher_etat_capteur(c);
        
        if (!vivant) break;
        
        sleep(1);
    }

    printf("Simulation Terminee. Batterie Vide.\n");
    printf("Total Paquets Transmis: %d\n", total_transmis);
}
