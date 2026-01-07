#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "sensor.h"

#define SAVE_FILENAME "save.bin"

/**
 * saveState
 * Saves the current sensor state and its packet buffer to a binary file.
 * @param sensor Pointer to the sensor
 * @return 0 on success, -1 on failure
 */
int saveState(const Sensor* sensor);

/**
 * loadState
 * Restores a sensor state and its packet buffer from a binary file.
 * @return Pointer to the restored Sensor (allocated), or NULL on failure.
 */
Sensor* loadState();

#endif // PERSISTENCE_H
