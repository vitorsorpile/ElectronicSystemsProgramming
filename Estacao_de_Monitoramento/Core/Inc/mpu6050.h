#ifndef _MY_MPU6050_
#define _MY_MPU6050_

#include <string.h>
#include <stdint.h>
#include "main.h"

#define MPU6050_ADDR 0xD0
#define WHO_AM_I_REG 0x75
#define PWR_MGMT_1_REG 0x6B
#define SMPLRT_DIV_REG 0x19
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_CONFIG_REG 0x1B
#define GYRO_XOUT_H_REG 0x43


typedef struct {
	int8_t id;
	int8_t Gx[4];
	int8_t Gy[4];
	int8_t Gz[4];
} Message_t;

// MPU6050 structure
typedef struct
{

    int16_t Accel_X_RAW;
    int16_t Accel_Y_RAW;
    int16_t Accel_Z_RAW;
    double Ax;
    double Ay;
    double Az;

    int16_t Gyro_X_RAW;
    int16_t Gyro_Y_RAW;
    int16_t Gyro_Z_RAW;
    float Gx;
    float Gy;
    float Gz;


    float Temperature;

    double KalmanAngleX;
    double KalmanAngleY;
} MPU6050_t;

uint8_t MPU6050_Init(I2C_HandleTypeDef *I2Cx);

void MPU6050_Read_Gyro(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct);

void MPU6050_Read_Temp(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct);

void Prepare_Message(Message_t *msg, MPU6050_t *DataStruct);

HAL_StatusTypeDef Send_Message_via_UART(Message_t *msg, UART_HandleTypeDef *huart);

#endif
