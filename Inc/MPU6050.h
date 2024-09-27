/*
 * MPU6050.h
 *
 *  Created on: Sep 24, 2024
 *      Author: zarno
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "stm32f4xx_hal.h"

#define MPU6050_I2C_ADDR 	(0x68 << 1)

#define MPU6050_REG_SELF_TEST_X		0x0D
#define MPU6050_REG_SELF_TEST_Y 	0x0E
#define MPU6050_REG_SELF_TEST_Z 	0x0F
#define MPU6050_REG_SELF_TEST_A 	0x10
#define MPU6050_REG_ACCEL_XOUT_H 	0x3B
#define MPU6050_REG_ACCEL_XOUT_L 	0x3C
#define MPU6050_REG_ACCEL_YOUT_H 	0x3D
#define MPU6050_REG_ACCEL_YOUT_L 	0x3E
#define MPU6050_REG_ACCEL_ZOUT_H 	0x3F
#define MPU6050_REG_ACCEL_ZOUT_L 	0x40
#define MPU6050_REG_TEMP_OUT_H	 	0x41
#define MPU6050_REG_TEMP_OUT_L	 	0x42
#define MPU6050_REG_GYRO_XOUT_H 	0x43
#define MPU6050_REG_GYRO_XOUT_L 	0x44
#define MPU6050_REG_GYRO_YOUT_H 	0x45
#define MPU6050_REG_GYRO_YOUT_L 	0x46
#define MPU6050_REG_GYRO_ZOUT_H 	0x47
#define MPU6050_REG_GYRO_ZOUT_L 	0x48
#define MPU6050_REG_PWR_MGMT_1	 	0x6B
#define MPU6050_REG_PWR_MGMT_2	 	0x6C
#define MPU6050_REG_WHO_AM_I	 	0x75

typedef struct {
	I2C_HandleTypeDef *i2cHandle;

	float acc[3];

	float gyro[3];

	float temp;
} MPU6050;

uint8_t MPU6050_Initialize(MPU6050 *dev, I2C_HandleTypeDef *i2cHandle);

HAL_StatusTypeDef MPU6050_ReadTemperature(MPU6050 *dev);
HAL_StatusTypeDef MPU6050_ReadAccelerations(MPU6050 *dev);
HAL_StatusTypeDef MPU6050_ReadGyro(MPU6050 *dev);

HAL_StatusTypeDef MPU6050_ReadRegister(MPU6050 *dev, uint8_t reg, uint8_t *data);
HAL_StatusTypeDef MPU6050_ReadRegisters(MPU6050 *dev, uint8_t reg, uint8_t *data, uint8_t length);

HAL_StatusTypeDef MPU6050_WriteRegister(MPU6050 *dev, uint8_t reg, uint8_t *data);



#endif /* INC_MPU6050_H_ */
