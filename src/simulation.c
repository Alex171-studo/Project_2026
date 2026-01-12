#include "../inc/simulation.h"
#include <stdio.h>
#include <math.h>
#include <unistd.h>

static int simulation_temps = 0;

float calculer_energie_transmission(float x, float y) {
    // Le PDF impose d'utiliser sqrt et pow pour le calcul de distance
    double d = sqrt(pow((double)x, 2) + pow((double)y, 2));
    // Formule énergétique : E = E_elec + E_amp * d^2
    return (float)(E_ELEC + (E_AMP * pow(d, 2)));
}

void reinitialiser_temps_simulation() {
    simulation_temps = 0;
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
    int alertes_saturation = 0;

    while (c->batterie > 0) {
        int envoyes = 0;
        int vivant = etape_simulation(c, &envoyes);
        total_transmis += envoyes;
        
        // Sécurité contre la boucle infinie si la transmission est impossible
        if (envoyes == 0 && c->buffer_usage >= MAX_BUFFER_SIZE) {
            alertes_saturation++;
        } else {
            alertes_saturation = 0;
        }

        if (alertes_saturation >= 5) {
            printf("\n⚠️  ARRET DE SECURITE : La transmission est impossible (energie insuffisante).\n");
            printf("   Le capteur a sature son buffer %d fois sans pouvoir envoyer.\n", alertes_saturation);
            break;
        }
        
        afficher_etat_capteur(c);
        
        if (!vivant) break;
        
        sleep(1);
    }

    if (c->batterie <= 0) {
        printf("\n💀 Simulation Terminee. Batterie Vide (0.00J).\n");
    } else {
        printf("\n🛑 Simulation Interrompue. Transmission impossible (Blocage Energetique).\n");
    }
    printf("📊 Total Paquets Transmis: %d\n", total_transmis);
}
