/*
 * agrisensorcan.c
 *
 *  Created on: Jul 29, 2025
 *      Author: Geek
 */


#include "agrisensorcan.h"
#include <stdio.h>

/*Simulated check results (replace with real sensor logic) */
extern int soilSensorIsOK();
extern int fuelSensorIsOK();
extern int pressureSensorIsOK();
extern int speedSensorIsOK();
extern int gpsSensorIsOK();

/*Check the API if the are OK*/

int soilSensorIsOK() { return 1; }
int fuelSensorIsOK() { return 1; }
int pressureSensorIsOK() { return 1; }
int speedSensorIsOK() { return 1; }
int gpsSensorIsOK() { return 1; }

/* Bit flag encoder */
void AgriCAN_EncodeStatusFlags(uint8_t *statusFlags) {
    *statusFlags = 0;

    if (soilSensorIsOK())     *statusFlags |= SENSOR_SOIL_OK;
    if (fuelSensorIsOK())     *statusFlags |= SENSOR_FUEL_OK;
    if (pressureSensorIsOK()) *statusFlags |= SENSOR_PRESSURE_OK;
    if (speedSensorIsOK())    *statusFlags |= SENSOR_SPEED_OK;
    if (gpsSensorIsOK())      *statusFlags |= SENSOR_GPS_OK;
}
void AgriCAN_EncodeTelemetry(FuelActuatorMessage *msg,
                             uint8_t soilMoisture,
                             uint8_t fuelLevel,
                             uint8_t pressure,
                             uint8_t speed,
                             uint16_t latitude,
                             uint16_t longitude) {
    msg->data[0] = soilMoisture;
    msg->data[1] = fuelLevel;
    msg->data[2] = pressure;
    msg->data[3] = speed;
    msg->data[4] = latitude & 0xFF;
    msg->data[5] = (latitude >> 8) & 0xFF;
    msg->data[6] = longitude & 0xFF;
    msg->data[7] = (longitude >> 8) & 0xFF;
    msg->dlc = 8;
}
void AgriCAN_DecodeTelemetry(FuelActuatorMessage *msg,
                             uint8_t *soilMoisture,
                             uint8_t *fuelLevel,
                             uint8_t *pressure,
                             uint8_t *speed,
                             uint16_t *latitude,
                             uint16_t *longitude) {
    *soilMoisture = msg->data[0];
    *fuelLevel = msg->data[1];
    *pressure = msg->data[2];
    *speed = msg->data[3];
    *latitude = msg->data[4] | (msg->data[5] << 8);
    *longitude = msg->data[6] | (msg->data[7] << 8);
}

/* Scaled GPS coordinate (e.g. latitude/longitude) */
uint16_t ScaleGPS(float coord) {
    return (uint16_t)(coord * 10000.0f);  // Keeps 4 decimal precision
}

/* Fuel efficiency lookup table */
static const float FuelEfficiencyTable[256] = {
	    0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f,
	    1.0f, 1.1f, 1.2f, 1.3f, 1.4f, 1.5f, 1.6f, 1.7f, 1.8f, 1.9f,
	    2.0f, 2.1f, 2.2f, 2.3f, 2.4f, 2.5f, 2.6f, 2.7f, 2.8f, 2.9f,
	    3.0f, 3.1f, 3.2f, 3.3f, 3.4f, 3.5f, 3.6f, 3.7f, 3.8f, 3.9f,
	    4.0f, 4.1f, 4.2f, 4.3f, 4.4f, 4.5f, 4.6f, 4.7f, 4.8f, 4.9f,
	    5.0f, 5.1f, 5.2f, 5.3f, 5.4f, 5.5f, 5.6f, 5.7f, 5.8f, 5.9f,
	    6.0f, 6.1f, 6.2f, 6.3f, 6.4f, 6.5f, 6.6f, 6.7f, 6.8f, 6.9f,
	    7.0f, 7.1f, 7.2f, 7.3f, 7.4f, 7.5f, 7.6f, 7.7f, 7.8f, 7.9f,
	    8.0f, 8.1f, 8.2f, 8.3f, 8.4f, 8.5f, 8.6f, 8.7f, 8.8f, 8.9f,
	    9.0f, 9.1f, 9.2f, 9.3f, 9.4f, 9.5f, 9.6f, 9.7f, 9.8f, 9.9f,
	    10.0f, 10.1f, 10.2f, 10.3f, 10.4f, 10.5f, 10.6f, 10.7f, 10.8f, 10.9f,
	    11.0f, 11.1f, 11.2f, 11.3f, 11.4f, 11.5f, 11.6f, 11.7f, 11.8f, 11.9f,
	    12.0f, 12.1f, 12.2f, 12.3f, 12.4f, 12.5f, 12.6f, 12.7f, 12.8f, 12.9f,
	    13.0f, 13.1f, 13.2f, 13.3f, 13.4f, 13.5f, 13.6f, 13.7f, 13.8f, 13.9f,
	    14.0f, 14.1f, 14.2f, 14.3f, 14.4f, 14.5f, 14.6f, 14.7f, 14.8f, 14.9f,
	    15.0f, 15.1f, 15.2f, 15.3f, 15.4f, 15.5f, 15.6f, 15.7f, 15.8f, 15.9f,
	    16.0f, 16.1f, 16.2f, 16.3f, 16.4f, 16.5f, 16.6f, 16.7f, 16.8f, 16.9f,
	    17.0f, 17.1f, 17.2f, 17.3f, 17.4f, 17.5f, 17.6f, 17.7f, 17.8f, 17.9f,
	    18.0f, 18.1f, 18.2f, 18.3f, 18.4f, 18.5f, 18.6f, 18.7f, 18.8f, 18.9f,
	    19.0f, 19.1f, 19.2f, 19.3f, 19.4f, 19.5f, 19.6f, 19.7f, 19.8f, 19.9f,
	    20.0f, 20.1f, 20.2f, 20.3f, 20.4f, 20.5f, 20.6f, 20.7f, 20.8f, 20.9f,
	    21.0f, 21.1f, 21.2f, 21.3f, 21.4f, 21.5f, 21.6f, 21.7f, 21.8f, 21.9f,
	    22.0f, 22.1f, 22.2f, 22.3f, 22.4f, 22.5f, 22.6f, 22.7f, 22.8f, 22.9f,
	    23.0f, 23.1f, 23.2f, 23.3f, 23.4f, 23.5f, 23.6f, 23.7f, 23.8f, 23.9f,
	    24.0f, 24.1f, 24.2f, 24.3f, 24.4f, 24.5f, 24.6f, 24.7f, 24.8f, 24.9f,
	    25.0f, 25.1f, 25.2f, 25.3f, 25.4f, 25.5f
	};


float FuelEfficiencyLookup(uint8_t index) {
    if (index >= 256) return 0.0f;
    return FuelEfficiencyTable[index];
}


void TestDiagnostics() {
    uint8_t flags;
    AgriCAN_EncodeStatusFlags(&flags);
    printf("Status Flags = 0x%02X\r\n", flags);

    float lat = 52.515;
    uint16_t scaledLat = ScaleGPS(lat);
    printf("Scaled GPS = %u\r\n", scaledLat);

    float economy = FuelEfficiencyLookup(10);
    printf("Fuel Efficiency = %.2f\r\n", economy);
}
