#include "../inc/sensor.h"
#include <stdio.h>
#include <stdlib.h>

// Static counter for packet IDs
static int currentPacketId = 1;

Sensor* createSensor(float x, float y) {
    Sensor* s = (Sensor*)malloc(sizeof(Sensor));
    if (!s) {
        perror("Failed to allocate Sensor");
        exit(EXIT_FAILURE); // Fatal error
    }
    s->battery = BATTERY_INIT;
    s->x = x;
    s->y = y;
    s->bufferHead = NULL;
    s->bufferUsage = 0;
    
    // Seed random number generator
    srand(time(NULL));
    
    return s;
}

void freeSensor(Sensor* sensor) {
    if (!sensor) return;

    Packet* current = sensor->bufferHead;
    while (current != NULL) {
        Packet* next = current->next;
        free(current);
        current = next;
    }
    free(sensor);
}

void producePacket(Sensor* sensor) {
    if (!sensor) return;
    
    // 1. Generate new packet data
    Packet* newPacket = (Packet*)malloc(sizeof(Packet));
    if (!newPacket) {
        perror("Failed to allocate Packet");
        return;
    }
    
    newPacket->id = currentPacketId++;
    newPacket->value = (float)(rand() % 1000) / 10.0f; // 0.0 to 100.0
    newPacket->timestamp = (long)time(NULL);
    newPacket->next = NULL;

    // 2. Manage Buffer Saturation
    if (sensor->bufferUsage >= MAX_BUFFER_SIZE) {
        // Buffer full: Remove oldest (Head)
        if (sensor->bufferHead != NULL) {
            Packet* oldHead = sensor->bufferHead;
            sensor->bufferHead = oldHead->next; // Move head forward
            
            printf("ALERTE : Mémoire saturée. Suppression du paquet ID [%d] pour libérer de l'espace.\n", oldHead->id);
            
            free(oldHead); // FREE Memory
            sensor->bufferUsage--;
        }
    }

    // 3. Add to Tail
    if (sensor->bufferHead == NULL) {
        sensor->bufferHead = newPacket;
    } else {
        Packet* current = sensor->bufferHead;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newPacket;
    }
    sensor->bufferUsage++;
    
    printf("Info: Packet Generated (ID: %d, Val: %.2f) | Usage: %d/%d\n", 
           newPacket->id, newPacket->value, sensor->bufferUsage, MAX_BUFFER_SIZE);
}

void printSensorStatus(const Sensor* sensor) {
    if (!sensor) return;
    printf("\n--- Sensor Status ---\n");
    printf("Battery: %.2f J\n", sensor->battery);
    printf("Position: (%.2f, %.2f)\n", sensor->x, sensor->y);
    printf("Buffer Usage: %d/%d\n", sensor->bufferUsage, MAX_BUFFER_SIZE);
    
    Packet* p = sensor->bufferHead;
    printf("Packets: [ ");
    while (p != NULL) {
        printf("{ID:%d Val:%.1f} ", p->id, p->value);
        p = p->next;
    }
    printf("]\n---------------------\n");
}
