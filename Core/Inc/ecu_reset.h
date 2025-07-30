/*
 * ecu_reset.h
 *
 *  Created on: Jul 29, 2025
 *      Author: Geek
 */

#ifndef ECU_RESET_H_
#define ECU_RESET_H_
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

void HandleECUReset(uint8_t* request, uint8_t* response);
uint8_t GetCurrentSession(void);
#endif /* ECU_RESET_H_ */
