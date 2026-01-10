#include "../inc/persistence.h"
#include <stdio.h>
#include <stdlib.h>

int sauvegarder_etat(const Capteur* c) {
    if (!c) return -1;

    FILE* f = fopen(SAVE_FILENAME, "wb");
    if (!f) {
        perror("Erreur ouverture fichier sauvegarde");
        return -1;
    }

    if (fwrite(&c->batterie, sizeof(float), 1, f) != 1 ||
        fwrite(&c->x, sizeof(float), 1, f) != 1 ||
        fwrite(&c->y, sizeof(float), 1, f) != 1 ||
        fwrite(&c->buffer_usage, sizeof(int), 1, f) != 1) {
        
        perror("Erreur ecriture metadonnees capteur");
        fclose(f);
        return -1;
    }

    Paquet* courant = c->buffer_tete;
    while (courant != NULL) {
        if (fwrite(&courant->id, sizeof(int), 1, f) != 1 ||
            fwrite(&courant->valeur, sizeof(float), 1, f) != 1 ||
            fwrite(&courant->timestamp, sizeof(long), 1, f) != 1) {
            perror("Erreur ecriture paquet");
            fclose(f);
            return -1;
        }
        courant = courant->suivant;
    }

    printf("Etat sauvegarde avec succes dans %s.\n", SAVE_FILENAME);
    fclose(f);
    return 0;
}

Capteur* charger_etat() {
    FILE* f = fopen(SAVE_FILENAME, "rb");
    if (!f) {
        printf("Aucun fichier de sauvegarde trouve.\n");
        return NULL;
    }

    Capteur* c = (Capteur*)malloc(sizeof(Capteur));
    if (!c) {
        fclose(f);
        return NULL;
    }

    if (fread(&c->batterie, sizeof(float), 1, f) != 1 ||
        fread(&c->x, sizeof(float), 1, f) != 1 ||
        fread(&c->y, sizeof(float), 1, f) != 1 ||
        fread(&c->buffer_usage, sizeof(int), 1, f) != 1) {
        
        printf("Erreur lecture valeurs capteur.\n");
        free(c);
        fclose(f);
        return NULL;
    }

    c->buffer_tete = NULL;
    
    int compte = c->buffer_usage;
    c->buffer_usage = 0; 

    Paquet* dernier = NULL;
    for (int i = 0; i < compte; i++) {
        Paquet* p = (Paquet*)malloc(sizeof(Paquet));
        if (fread(&p->id, sizeof(int), 1, f) != 1 ||
            fread(&p->valeur, sizeof(float), 1, f) != 1 ||
            fread(&p->timestamp, sizeof(long), 1, f) != 1) {
            printf("Erreur lecture paquet %d.\n", i);
            free(p);
            break;
        }
        p->suivant = NULL;

        if (c->buffer_tete == NULL) {
            c->buffer_tete = p;
            dernier = p;
        } else {
            dernier->suivant = p;
            dernier = p;
        }
        c->buffer_usage++;
    }

    printf("Etat charge avec succes. Taille buffer: %d\n", c->buffer_usage);
    fclose(f);
    return c;
}
