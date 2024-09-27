/*
 * MPU6050.c
 *
 *  Created on: Sep 24, 2024
 *      Author: zarno
 */
#include "MPU6050.h"


uint8_t MPU6050_Initialize(MPU6050 *dev, I2C_HandleTypeDef *i2cHandle) {
	dev->i2cHandle 	= i2cHandle;

	dev->acc[0]		= 0.0f; /*x*/
	dev->acc[1]		= 0.0f; /*y*/
	dev->acc[2]		= 0.0f; /*z*/

	dev->gyro[0]	= 0.0f; /*x*/
	dev->gyro[1]	= 0.0f; /*y*/
	dev->gyro[2]	= 0.0f; /*z*/

	dev->temp		= 0.0f;

	uint8_t errNum 	= 0;
	HAL_StatusTypeDef status;

	uint8_t regData;
	status = HAL_I2C_IsDeviceReady(dev->i2cHandle, MPU6050_I2C_ADDR, 1, 100);
	errNum += (status != HAL_OK);

	status = MPU6050_ReadRegister(dev, MPU6050_REG_WHO_AM_I, &regData);
	errNum += (status != HAL_OK);

	/*Set to run mode
	 *Reg 107/Sleep 1 = 00100000
	 */
	status = MPU6050_ReadRegister(dev, MPU6050_REG_PWR_MGMT_1, &regData);
	errNum += (status != HAL_OK);

	regData = 0x00;//0x20
	status = MPU6050_WriteRegister(dev, MPU6050_REG_PWR_MGMT_1, &regData);
	errNum += (status != HAL_OK);

	status = MPU6050_ReadRegister(dev, MPU6050_REG_PWR_MGMT_1, &regData);
	errNum += (status != HAL_OK);
	/*Set to 5Hz wake up frequency
	 * Reg 108/Sleep 2 =[7:6]=1 [5:0]=0 01000000
	 */
	status = MPU6050_ReadRegister(dev, MPU6050_REG_PWR_MGMT_2, &regData);
	errNum += (status != HAL_OK);
	regData = 0x40;
	status = MPU6050_WriteRegister(dev, MPU6050_REG_PWR_MGMT_2, &regData);
	errNum += (status != HAL_OK);

	status = MPU6050_ReadRegister(dev, MPU6050_REG_PWR_MGMT_2, &regData);
	errNum += (status != HAL_OK);

	return errNum;
}

HAL_StatusTypeDef MPU6050_ReadTemperature(MPU6050 *dev) {
	/*Read the raw temp registers (16bit)*/
	uint8_t regData[2];
	HAL_StatusTypeDef status = MPU6050_ReadRegisters(dev, MPU6050_REG_TEMP_OUT_H, regData, 2);

	/*Combine register values*/
	uint16_t tempRaw = ((regData[0]<<8)|regData[1]);

	/*Page 30 equation for converting to Deg C*/
	dev->temp = (tempRaw / 340.0) + 36.53;

	return status;
}

HAL_StatusTypeDef MPU6050_ReadAccelerations(MPU6050 *dev) {
	/*Read the raw acc registers (16bit)*/
	uint8_t regData[6];
	HAL_StatusTypeDef status = MPU6050_ReadRegisters(dev, MPU6050_REG_ACCEL_XOUT_H, regData, 6);

	/*Combine register values*/
	int16_t accRawSigned[3];
	accRawSigned[0] = regData[0]<<8|regData[1];
	accRawSigned[1] = regData[2]<<8|regData[3];
	accRawSigned[2] = regData[4]<<8|regData[5];

	/*Page 30 equation for converting to G*/
	dev->acc[0] = accRawSigned[0] / 16384;
	dev->acc[1] = accRawSigned[1] / 16384;
	dev->acc[2] = accRawSigned[2] / 16384;

	return status;
}

HAL_StatusTypeDef MPU6050_ReadGyro(MPU6050 *dev) {
	/*Read the raw gyro registers (16bit)*/
	uint8_t regData[6];
	HAL_StatusTypeDef status = MPU6050_ReadRegisters(dev, MPU6050_REG_GYRO_XOUT_H, regData, 6);

	/*Combine register values*/
	int16_t gyroRawSigned[3];
	gyroRawSigned[0] = regData[0]<<8|regData[1];
	gyroRawSigned[1] = regData[2]<<8|regData[3];
	gyroRawSigned[2] = regData[4]<<8|regData[5];

	/*Page 31 equation for converting to deg/s*/
	dev->gyro[0] = gyroRawSigned[0] / 131;
	dev->gyro[1] = gyroRawSigned[1] / 131;
	dev->gyro[2] = gyroRawSigned[2] / 131;

	return status;
}

HAL_StatusTypeDef MPU6050_ReadRegister(MPU6050 *dev, uint8_t reg, uint8_t *data) {
	return HAL_I2C_Mem_Read( dev->i2cHandle, MPU6050_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef MPU6050_ReadRegisters(MPU6050 *dev, uint8_t reg, uint8_t *data, uint8_t length) {
	return HAL_I2C_Mem_Read( dev->i2cHandle, MPU6050_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY);
}

HAL_StatusTypeDef MPU6050_WriteRegister(MPU6050 *dev, uint8_t reg, uint8_t *data) {
	return HAL_I2C_Mem_Write( dev->i2cHandle, MPU6050_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);
}


