#include <stdio.h>
#include <stdlib.h>
#include "../inc/sensor.h"
#include "../inc/simulation.h"
#include "../inc/persistence.h"

void clearScreen() {
    // Portable ANSI Escape code to clear screen (Works on most Linux/Unix terminals)
    printf("\033[H\033[J"); 
}

void printHeader() {
    printf("========================================\n");
    printf("   ECO-SENSING : IoT Network Simulator  \n");
    printf("========================================\n");
}

void printMenu() {
    printf("\n--- Main Menu ---\n");
    printf("1. New Simulation (Initialize Sensor)\n");
    printf("2. Load Previous State\n");
    printf("3. View Sensor Status\n");
    printf("4. Run Simulation (Auto - until dead)\n");
    printf("5. Run Single Step (Manual Debug)\n");
    printf("6. Save State\n");
    printf("7. Exit\n");
    printf("Select option: ");
}

int main() {
    Sensor* currentSensor = NULL;
    int choice;
    
    // Clear log on start? Or per new sim? 
    // Let's clear on New Sim.

    while (1) {
        // clearScreen(); // Optional, might flicker
        printHeader();
        if (currentSensor) {
            printf("Current Status: Battery %.2f J | Buffer %d/5\n", 
                   currentSensor->battery, currentSensor->bufferUsage);
        } else {
            printf("Current Status: NO SENSOR INITIALIZED\n");
        }
        
        printMenu();
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); // Clear buffer
            continue;
        }

        switch (choice) {
            case 1: // New
                if (currentSensor) freeSensor(currentSensor);
                float x, y;
                printf("Enter Sensor X Position: ");
                scanf("%f", &x);
                printf("Enter Sensor Y Position: ");
                scanf("%f", &y);
                currentSensor = createSensor(x, y);
                
                // Reset log
                FILE* f = fopen("log.txt", "w");
                if (f) { fprintf(f, "--- New Log ---\n"); fclose(f); }
                
                printf("Sensor Initialized at (%.2f, %.2f)\n", x, y);
                break;

            case 2: // Load
                if (currentSensor) freeSensor(currentSensor);
                currentSensor = loadState();
                break;

            case 3: // View
                if (currentSensor) printSensorStatus(currentSensor);
                else printf("Error: No sensor initialized.\n");
                break;

            case 4: // Run Auto
                if (currentSensor) {
                    runSimulation(currentSensor);
                    // Autosave after death? Or just let user manage?
                    // Usually dead means dead.
                } else {
                    printf("Error: No sensor initialized.\n");
                }
                break;

            case 5: // Step
                if (currentSensor) {
                    int alive = runSimulationStep(currentSensor);
                    printSensorStatus(currentSensor);
                    if (!alive) printf("Sensor has DIED (Battery 0).\n");
                } else {
                    printf("Error: No sensor initialized.\n");
                }
                break;

            case 6: // Save
                if (currentSensor) {
                    saveState(currentSensor);
                } else {
                    printf("Error: No sensor initialized.\n");
                }
                break;

            case 7: // Exit
                if (currentSensor) {
                    printf("Do you want to save before exiting? (1=Yes, 0=No): ");
                    int saveChoice;
                    if (scanf("%d", &saveChoice) == 1 && saveChoice == 1) {
                        saveState(currentSensor);
                    }
                    freeSensor(currentSensor);
                }
                printf("Exiting Eco-Sensing. Goodbye!\n");
                return 0;

            default:
                printf("Invalid option.\n");
        }
        
        printf("\nPress Enter to continue...");
        while(getchar() != '\n'); 
        getchar(); // Wait for user
    }
}
