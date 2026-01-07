#include "../inc/persistence.h"
#include <stdio.h>
#include <stdlib.h>

int saveState(const Sensor* sensor) {
    if (!sensor) return -1;

    FILE* f = fopen(SAVE_FILENAME, "wb");
    if (!f) {
        perror("Error opening save file");
        return -1;
    }

    // 1. Save Sensor Data (Scalars only)
    // Optimization: Write fields individually to avoid saving memory pointers (garbage on disk)
    if (fwrite(&sensor->battery, sizeof(float), 1, f) != 1 ||
        fwrite(&sensor->x, sizeof(float), 1, f) != 1 ||
        fwrite(&sensor->y, sizeof(float), 1, f) != 1 ||
        fwrite(&sensor->bufferUsage, sizeof(int), 1, f) != 1) {
        
        perror("Error writing sensor metadata");
        fclose(f);
        return -1;
    }

    // 2. Save Packet List
    Packet* current = sensor->bufferHead;
    while (current != NULL) {
        // Write the packet content (excluding next pointer effectively, though we write the struct)
        if (fwrite(current, sizeof(Packet), 1, f) != 1) {
            perror("Error writing packet data");
            fclose(f);
            return -1;
        }
        current = current->next;
    }

    printf("State saved successfully to %s.\n", SAVE_FILENAME);
    fclose(f);
    return 0;
}

Sensor* loadState() {
    FILE* f = fopen(SAVE_FILENAME, "rb");
    if (!f) {
        printf("No save file found.\n");
        return NULL;
    }

    Sensor* s = (Sensor*)malloc(sizeof(Sensor));
    if (!s) {
        fclose(f);
        return NULL;
    }

    // 1. Read Sensor Data (Scalars)
    if (fread(&s->battery, sizeof(float), 1, f) != 1 ||
        fread(&s->x, sizeof(float), 1, f) != 1 ||
        fread(&s->y, sizeof(float), 1, f) != 1 ||
        fread(&s->bufferUsage, sizeof(int), 1, f) != 1) {
        
        printf("Error reading sensor values.\n");
        free(s);
        fclose(f);
        return NULL;
    }

    // Initialize pointers safely
    s->bufferHead = NULL;
    
    // Allow reconstruction loop to run
    int count = s->bufferUsage;
    s->bufferUsage = 0; // Reset internal counter, will be incremented as we add nodes 

    // 2. Read Packets and Reconstruct List
    Packet* tail = NULL;
    for (int i = 0; i < count; i++) {
        Packet* p = (Packet*)malloc(sizeof(Packet));
        if (fread(p, sizeof(Packet), 1, f) != 1) {
            printf("Error reading packet %d.\n", i);
            // Handle partial failure? For now, break.
            free(p);
            break;
        }
        p->next = NULL;

        if (s->bufferHead == NULL) {
            s->bufferHead = p;
            tail = p;
        } else {
            tail->next = p;
            tail = p;
        }
        s->bufferUsage++;
    }

    printf("State loaded successfully. Buffer size: %d\n", s->bufferUsage);
    fclose(f);
    return s;
}
