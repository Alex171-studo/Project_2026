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
            printf("⚠️  Energie insuffisante! Requis: %.2fJ | Disponible: %.2fJ | Paquet ID %d stocke en memoire.\n", 
                   cout_energie, c->batterie, c->buffer_tete->id);
            break; 
        }

        Paquet* temp = c->buffer_tete;
        c->buffer_tete = temp->suivant;
        
        c->batterie -= cout_energie;
        if (c->batterie < 0) c->batterie = 0;

        printf("✓ Paquet ID %d transmis avec succes! Energie consommee: %.2fJ | Batterie restante: %.2fJ\n",
               temp->id, cout_energie, c->batterie);

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

    ecrire_log(c);

    return (c->batterie > 0);
}

void lancer_simulation(Capteur* c) {
    float cout_energie = calculer_energie_transmission(c->x, c->y);
    printf("\n========== DEMARRAGE SIMULATION ==========\n");
    printf("Position Capteur: (%.2f, %.2f)\n", c->x, c->y);
    printf("Cout Energetique par Transmission: %.2fJ\n", cout_energie);
    printf("Batterie Initiale: %.2fJ\n", c->batterie);
    
    if (cout_energie > c->batterie) {
        printf("\n⚠️  ATTENTION: Le cout energetique (%.2fJ) depasse la batterie (%.2fJ).\n", cout_energie, c->batterie);
        printf("   Les paquets seront stockes en memoire jusqu'a saturation du buffer.\n");
    } else {
        printf("\n✓ Transmission possible. Nombre max de transmissions: ~%d\n", (int)(c->batterie / cout_energie));
    }
    printf("==========================================\n\n");
    
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
