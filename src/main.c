#include <stdio.h>
#include <stdlib.h>
#include "../inc/sensor.h"
#include "../inc/simulation.h"
#include "../inc/persistence.h"

void effacer_ecran() {
    printf("\033[H\033[J"); 
}

void afficher_entete() {
    printf("========================================\n");
    printf("   ECO-SENSING : IoT Network Simulator  \n");
    printf("========================================\n");
}

void afficher_menu() {
    printf("\n--- Menu Principal ---\n");
    printf("1. Nouvelle Simulation (Init Capteur)\n");
    printf("2. Charger Etat Precedent\n");
    printf("3. Voir Etat Capteur\n");
    printf("4. Lancer Simulation (Auto - jusqu'a mort)\n");
    printf("5. Executer Une Etape (Debug Manuel)\n");
    printf("6. Sauvegarder Etat\n");
    printf("7. Quitter\n");
    printf("Choisir une option: ");
}

int main() {
    srand(time(NULL));
    Capteur* capteur_courant = NULL;
    int choix;
    
    while (1) {
        afficher_entete();
        if (capteur_courant) {
            printf("Etat Actuel: Batterie %.2f J | Buffer %d/5\n", 
                   capteur_courant->batterie, capteur_courant->buffer_usage);
        } else {
            printf("Etat Actuel: AUCUN CAPTEUR INITIALISE\n");
        }
        
        afficher_menu();
        if (scanf("%d", &choix) != 1) {
            while(getchar() != '\n');
            continue;
        }

        switch (choix) {
            case 1:
                if (capteur_courant) liberer_capteur(capteur_courant);
                float x, y, b;
                printf("Entrer Position X Capteur: ");
                if (scanf("%f", &x) != 1) { x = 0; while(getchar() != '\n'); }
                printf("Entrer Position Y Capteur: ");
                if (scanf("%f", &y) != 1) { y = 0; while(getchar() != '\n'); }
                printf("Entrer Niveau Batterie Initial (Joules, defaut 100): ");
                if (scanf("%f", &b) != 1) { b = 100; while(getchar() != '\n'); }
                
                capteur_courant = creer_capteur(x, y, b);
                reinitialiser_temps_simulation();
                
                FILE* f = fopen("log.txt", "w");
                if (f) { fprintf(f, "--- Nouveau Log ---\n"); fclose(f); }
                
                printf("Capteur Initialise a (%.2f, %.2f) avec %.2f J\n", x, y, b);
                break;

            case 2:
                if (capteur_courant) liberer_capteur(capteur_courant);
                capteur_courant = charger_etat();
                if (capteur_courant) reinitialiser_temps_simulation();
                break;

            case 3:
                if (capteur_courant) afficher_etat_capteur(capteur_courant);
                else printf("Erreur: Aucun capteur initialise.\n");
                break;

            case 4:
                if (capteur_courant) {
                    lancer_simulation(capteur_courant);
                } else {
                    printf("Erreur: Aucun capteur initialise.\n");
                }
                break;

            case 5:
                if (capteur_courant) {
                    int envoyes = 0;
                    int vivant = etape_simulation(capteur_courant, &envoyes);
                    afficher_etat_capteur(capteur_courant);
                    if (!vivant) printf("Le Capteur est MORT (Batterie 0).\n");
                } else {
                    printf("Erreur: Aucun capteur initialise.\n");
                }
                break;

            case 6:
                if (capteur_courant) {
                    sauvegarder_etat(capteur_courant);
                } else {
                    printf("Erreur: Aucun capteur initialise.\n");
                }
                break;

            case 7:
                if (capteur_courant) {
                    printf("Voulez-vous sauvegarder avant de quitter? (1=Oui, 0=Non): ");
                    int choix_save;
                    if (scanf("%d", &choix_save) == 1 && choix_save == 1) {
                        sauvegarder_etat(capteur_courant);
                    }
                    liberer_capteur(capteur_courant);
                }
                printf("Au Revoir!\n");
                return 0;

            default:
                printf("Option Invalide.\n");
        }
        
        printf("\nAppuyez sur Entree pour continuer...");
        while(getchar() != '\n'); 
        getchar();
    }
}
