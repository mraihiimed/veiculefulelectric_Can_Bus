/*
 * vehiculefulelectric.h
 *
 *  Created on: Jul 28, 2025
 *      Author: Geek
 */

#ifndef INC_VEHICULEFULELECTRIC_H_
#define INC_VEHICULEFULELECTRIC_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

/* CAN message structure */
typedef struct {
    uint32_t id;
    uint8_t dlc;
    uint8_t data[8];
} FuelActuatorMessage;

typedef enum {
    QUALITY_INVALID = 0,
    QUALITY_ESTIMATED = 1,
    QUALITY_MEASURED = 2
} SpeedQuality;

/* New Data Struct for CAN Encoding */
typedef struct {
    uint16_t vehicleSpeed;
    uint8_t timestampLSB;
    SpeedQuality speedQuality;
    uint16_t engineRPM;
    uint8_t oilTemp;
    uint8_t engineLoad;
} FuelActuatorData;

/* Initialization functions */
void FuelActuator_CAN_Init(CAN_HandleTypeDef *hcan);
void FuelActuator_SendCommand(CAN_HandleTypeDef *hcan, FuelActuatorMessage *msg);

/* Updated encoding and decoding with new struct */
void FuelActuator_EncodeSignals(FuelActuatorMessage *msg, const FuelActuatorData *data);
void FuelActuator_DecodeSignals(FuelActuatorMessage *msg, FuelActuatorData *data);

void Error_Handler(void);

#endif /* INC_VEHICULEFULELECTRIC_H_ */
