/*
 * test_FuelActuator_EncodeSignals.h
 *
 *  Created on: Jul 29, 2025
 *      Author: Geek
 */

#ifndef INC_TEST_FUELACTUATOR_ENCODESIGNALS_H_
#define INC_TEST_FUELACTUATOR_ENCODESIGNALS_H_
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

bool arraysEqual(const uint8_t* a, const uint8_t* b, size_t len);
void test_FuelActuator_EncodeSignals(FuelActuatorMessage* msg, const FuelActuatorData* data);
#endif /* INC_TEST_FUELACTUATOR_ENCODESIGNALS_H_ */
