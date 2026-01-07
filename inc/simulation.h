#ifndef SIMULATION_H
#define SIMULATION_H

#include "sensor.h"

// -- Constants --
#define STATION_X 0.0
#define STATION_Y 0.0
#define E_ELEC 0.05
#define E_AMP 0.01

// -- Function Prototypes --

/**
 * runSimulation
 * Starts the simulation loop until battery represents 0.
 * @param sensor Pointer to the sensor
 */
void runSimulation(Sensor* sensor);

/**
 * runSimulationStep
 * Executes a single discrete step of time:
 * 1. Produce Packet
 * 2. Attempt Transmission
 * 3. Log Status
 * @param sensor Pointer to sensor
 * @return 1 if simulation continues, 0 if battery died
 */
int runSimulationStep(Sensor* sensor);

/**
 * calculateTransmissionEnergy
 * Calculates Energy cost = E_elec + E_amp * distance^2
 * @param x Sensor X
 * @param y Sensor Y
 * @return Energy cost in Joules
 */
float calculateTransmissionEnergy(float x, float y);

/**
 * attemptTransmission
 * Tries to send all packets in buffer to the base station.
 * Decreases battery based on cost.
 * @param sensor Pointer to sensor
 * @return Number of packets successfully sent
 */
int attemptTransmission(Sensor* sensor);

#endif // SIMULATION_H
