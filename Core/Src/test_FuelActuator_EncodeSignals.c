/*
 * test_FuelActuator_EncodeSignals.c
 *
 *  Created on: Jul 29, 2025
 *      Author: Geek
 */


#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "vehiculefulelectric.h"

// Assuming these are defined elsewhere
#define PAYLOAD_SIZE 8  // or whatever your actual payload size is
#define QUALITY_MEASURED 0x01

extern void FuelActuator_EncodeSignals(FuelActuatorMessage* msg, const FuelActuatorData* data);

bool arraysEqual(const uint8_t* a, const uint8_t* b, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

void test_FuelActuator_EncodeSignals() {
    FuelActuatorData actuatorData = {
        .vehicleSpeed = 120,
        .timestampLSB = 0x7F,
        .speedQuality = QUALITY_MEASURED,
        .engineRPM = 3000,
        .oilTemp = 85,
        .engineLoad = 75
    };

    //FuelActuatorMessage msg = { .id = 0x321, .payload = {0} };
    FuelActuatorMessage msg = { .id = 0x321, .dlc = 8, .data = {0} };


    FuelActuator_EncodeSignals(&msg, &actuatorData);

    // Define what the expected payload should be
    uint8_t expected_data[8] = {
        0x78,  // example: encoded vehicleSpeed (120)
        0x7F,  // timestamp
        0x01,  // quality
        0x0B, 0xB8,  // engineRPM (3000) split into 2 bytes
        0x55,  // oilTemp
        0x4B,  // engineLoad
        0x00   // padding or reserved
    };

    if (arraysEqual(msg.data, expected_data, 8)) {
        printf("✅ Test passed: Payload matches expected output.\n");
    } else {
        printf("❌ Test failed: Payload mismatch.\n");

        printf("Expected: ");
        for (int i = 0; i < 8; i++) {
            printf("%02X ", expected_data[i]);
        }

        printf("\nActual:   ");
        for (int i = 0; i < 8; i++) {
            printf("%02X ", msg.data[i]);
        }
        printf("\n");
    }
}
