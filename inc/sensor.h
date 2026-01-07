#ifndef SENSOR_H
#define SENSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// -- Constants --
#define MAX_BUFFER_SIZE 5
#define BATTERY_INIT 100.0 // Starting Joules (Example)

// -- Data Structures --

/**
 * Packet
 * Represents a single measurement data unit.
 * dynamically allocated in the heap.
 */
typedef struct Packet {
    int id;                 // Unique identifier
    float value;            // Measurement value (e.g. humidity)
    long timestamp;         // Time of creation
    struct Packet* next;    // Pointer to the next packet (Linked List)
} Packet;

/**
 * Sensor
 * Represents the IoT device state.
 */
typedef struct Sensor {
    float battery;          // Energy in Joules
    float x;                // X Position
    float y;                // Y Position
    Packet* bufferHead;     // Head of the linked list (Oldest packet)
    int bufferUsage;        // Current count of packets
} Sensor;

// -- Function Prototypes --

/**
 * createSensor
 * Initializes a new sensor at a given position.
 * @param x Initial X coordinate
 * @param y Initial Y coordinate
 * @return Pointer to the newly allocated Sensor
 */
Sensor* createSensor(float x, float y);

/**
 * freeSensor
 * Frees all memory associated with the sensor, including its buffer.
 * @param sensor Pointer to the sensor to free
 */
void freeSensor(Sensor* sensor);

/**
 * producePacket
 * Generates a new random packet and adds it to the sensor's buffer.
 * Handles the logic for buffer overflow (removing the oldest if > 5).
 * @param sensor Pointer to the sensor
 */
void producePacket(Sensor* sensor);

/**
 * printSensorStatus
 * debug helper to show current state.
 * @param sensor Pointer to the sensor
 */
void printSensorStatus(const Sensor* sensor);

#endif // SENSOR_H
