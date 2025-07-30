/*
 * session_control.c
 *
 *  Created on: Jul 29, 2025
 *      Author: Geek
 */


#include "session_control.h"

static uint8_t currentSession = 0x01; // Default

void HandleSessionControl(uint8_t* request, uint8_t* response) {
    uint8_t subFunction = request[1];

    switch (subFunction) {
        case 0x01: // Default Session
        case 0x02: // Programming Session
        case 0x03: // Extended Diagnostic Session
            currentSession = subFunction;
            response[0] = 0x50; // Positive response SID = 0x10 + 0x40
            response[1] = subFunction;
            break;

        default:
            response[0] = 0x7F;
            response[1] = 0x10;
            response[2] = 0x12; // NRC: Sub-function not supported

            printf("Unsupported session sub-function: 0x%02X\r\n", subFunction);
            break;
    }
}

uint8_t GetCurrentSession(void) {
    return currentSession;
}
