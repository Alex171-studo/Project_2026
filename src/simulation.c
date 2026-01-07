#include "../inc/simulation.h"
#include <stdio.h>
#include <math.h>
#include <unistd.h> // for sleep()

// Global simulation time tracker (local to this run)
static int simTime = 0;

float calculateTransmissionEnergy(float x, float y) {
    // Distance to (0,0)
    // Constraint: Use sqrt and pow from <math.h> as requested in the subject
    float d = sqrt(pow(x, 2) + pow(y, 2));
    
    // Formula: E = E_elec + E_amp * d^2
    // We use pow(d, 2) to be strictly compliant with "math & algo" requirements
    return E_ELEC + (E_AMP * pow(d, 2));
}

int attemptTransmission(Sensor* sensor) {
    if (!sensor || sensor->bufferHead == NULL) {
        return 0; // Nothing to send
    }

    float energyCostPerPacket = calculateTransmissionEnergy(sensor->x, sensor->y);
    int packetsSent = 0;

    // Send all packets in buffer
    while (sensor->bufferHead != NULL) {
        // Check if enough battery
        if (sensor->battery < energyCostPerPacket) {
            printf("Battery low! Cannot transmit packet ID %d\n", sensor->bufferHead->id);
            break; 
        }

        // "Send" packet (Remove from queue)
        Packet* temp = sensor->bufferHead;
        sensor->bufferHead = temp->next;
        
        // Consume energy
        sensor->battery -= energyCostPerPacket;
        if (sensor->battery < 0) sensor->battery = 0;

        // Free memory
        free(temp);
        sensor->bufferUsage--;
        packetsSent++;
    }
    
    return packetsSent;
}

void logToCrashFile(const Sensor* sensor) {
    FILE* f = fopen("log.txt", "a");
    if (f) {
        // Format: Temps: 10s | Batterie: 45.2J | Paquets en attente: 3
        fprintf(f, "Temps: %ds | Batterie: %.2fJ | Paquets en attente: %d\n", 
                simTime, sensor->battery, sensor->bufferUsage);
        fclose(f);
    }
}

int runSimulationStep(Sensor* sensor) {
    if (sensor->battery <= 0) return 0;

    simTime++;

    // 1. Produce Data
    producePacket(sensor);

    // 2. Attempt Transmission
    int sent = attemptTransmission(sensor);
    if (sent > 0) {
        printf("Transmission: %d packets sent to Base Station.\n", sent);
    }

    // 3. Log
    logToCrashFile(sensor);

    return (sensor->battery > 0);
}

void runSimulation(Sensor* sensor) {
    printf("Starting Simulation...\n");
    // Clear previous log logic? 
    // Usually a new run might append or overwrite. Requirement implies "crash test log", maybe overwrite?
    // I'll overwrite at start of runSimulation if Time is 0, or just append. 
    // To be safe and clean, maybe clear log.txt at start of MAIN execution, not here.
    
    // totalPackets removed as it was unused. 
    // If stats are needed, they can be calculated from bufferUsage or a separate counter.

    while (sensor->battery > 0) {
        int alive = runSimulationStep(sensor);
        printSensorStatus(sensor);
        
        if (!alive) break;
        
        sleep(1); // Slow down simulation for visibility (1 sec per step)
    }

    printf("Simulation Ended. Battery Dead.\n");
}
