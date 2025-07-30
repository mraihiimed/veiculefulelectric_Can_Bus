/*
 * ecu_reset.c
 *
 *  Created on: Jul 29, 2025
 *      Author: Geek
 */


#include "ecu_reset.h"
#include "stm32f4xx_hal.h"


void HandleECUReset(uint8_t* request, uint8_t* response) {
    uint8_t subFunction = request[1];

    switch (subFunction) {
        case 0x01: // Hard Reset
        case 0x02: // Key Off-On Reset
        case 0x03: // Soft Reset
            response[0] = 0x51; // Positive response SID = 0x11 + 0x40
            response[1] = subFunction;

            // Optional: delay before reset
            HAL_Delay(100);

            // Trigger MCU reset
            NVIC_SystemReset();
            break;

        default:
            response[0] = 0x7F;
            response[1] = 0x11;
            response[2] = 0x12; // NRC: Sub-function not supported
            printf("Unsupported ECU reset sub-function: 0x%02X\r\n", subFunction);
            break;
    }
}
