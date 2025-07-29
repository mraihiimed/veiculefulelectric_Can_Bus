/*
 * agrisensorcan.h
 *
 *  Created on: Jul 29, 2025
 *      Author: Geek
 */

#ifndef INC_AGRISENSORCAN_H_
#define INC_AGRISENSORCAN_H_

#include <stdint.h>
#include  "vehiculefulelectric.h"

/* Bit flags for sensor diagnostics */
#define SENSOR_SOIL_OK     (1 << 0)
#define SENSOR_FUEL_OK     (1 << 1)
#define SENSOR_PRESSURE_OK (1 << 2)
#define SENSOR_SPEED_OK    (1 << 3)
#define SENSOR_GPS_OK      (1 << 4)

void AgriCAN_EncodeStatusFlags(uint8_t *statusFlags);

/* Scaled GPS encoding */
uint16_t ScaleGPS(float coord);  // e.g. multiply by 10,000

/* Fuel economy lookup */
float FuelEfficiencyLookup(uint8_t index);
void AgriCAN_DecodeTelemetry(FuelActuatorMessage *msg,
                             uint8_t *soilMoisture,
                             uint8_t *fuelLevel,
                             uint8_t *pressure,
                             uint8_t *speed,
                             uint16_t *latitude,
                             uint16_t *longitude);
void AgriCAN_EncodeTelemetry(FuelActuatorMessage *msg,
                             uint8_t soilMoisture,
                             uint8_t fuelLevel,
                             uint8_t pressure,
                             uint8_t speed,
                             uint16_t latitude,
                             uint16_t longitude);

void TestDiagnostics() ;

#endif /* INC_AGRISENSORCAN_H_ */
