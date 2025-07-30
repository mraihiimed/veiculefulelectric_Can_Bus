/*
 * vehiculefulelectric.c
 *
 *  Created on: Jul 28, 2025
 *      Author: Geek
 */


#include "vehiculefulelectric.h"

void FuelActuator_EncodeSignals(FuelActuatorMessage *msg, const FuelActuatorData *data) {
    msg->data[0] = data->vehicleSpeed & 0xFF;
    msg->data[1] = (data->vehicleSpeed >> 8) & 0xFF;
    msg->data[2] = data->timestampLSB;
    msg->data[3] = (uint8_t)data->speedQuality;
    msg->data[4] = data->engineRPM & 0xFF;
    msg->data[5] = (data->engineRPM >> 8) & 0xFF;
    msg->data[6] = data->oilTemp;
    msg->data[7] = data->engineLoad;
    msg->dlc = 8;
}


void FuelActuator_DecodeSignals(FuelActuatorMessage *msg, FuelActuatorData *data) {
    data->vehicleSpeed = msg->data[0] | (msg->data[1] << 8);
    data->timestampLSB = msg->data[2];
    data->speedQuality = (SpeedQuality)msg->data[3];
    data->engineRPM = msg->data[4] | (msg->data[5] << 8);
    data->oilTemp = msg->data[6];
    data->engineLoad = msg->data[7];
}


void FuelActuator_CAN_Init(CAN_HandleTypeDef *hcan) {
    hcan->Instance = CAN1;
    hcan->Init.Prescaler = 6;
    hcan->Init.Mode = CAN_MODE_NORMAL;
    hcan->Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan->Init.TimeSeg1 = CAN_BS1_13TQ;
    hcan->Init.TimeSeg2 = CAN_BS2_2TQ;
    hcan->Init.AutoBusOff = DISABLE;
    hcan->Init.AutoWakeUp = DISABLE;
    hcan->Init.AutoRetransmission = ENABLE;
    hcan->Init.ReceiveFifoLocked = DISABLE;
    hcan->Init.TransmitFifoPriority = DISABLE;

    if (HAL_CAN_Init(hcan) != HAL_OK) {
        Error_Handler();
    }

    HAL_CAN_Start(hcan);
}

void FuelActuator_SendCommand(CAN_HandleTypeDef *hcan, FuelActuatorMessage *msg) {
    CAN_TxHeaderTypeDef txHeader;
    uint32_t txMailbox;

    txHeader.StdId = msg->id;
    txHeader.IDE = CAN_ID_STD;
    txHeader.RTR = CAN_RTR_DATA;
    txHeader.DLC = msg->dlc;

    if (HAL_CAN_AddTxMessage(hcan, &txHeader, msg->data, &txMailbox) != HAL_OK) {
        Error_Handler();
    }

    while (HAL_CAN_IsTxMessagePending(hcan, txMailbox));
}
