#include <stdint.h>
#include <string.h>
#include "ina219.h"

#include "stm32F4xx_hal.h"


extern I2C_HandleTypeDef hi2c1;



// The following multipliers are used to convert raw current and power
// values to mA and mW, taking into account the current config settings
uint32_t ina219_currentDivider_mA;
uint32_t ina219_powerMultiplier_mW;

uint32_t ina219_calValue;

#define BUFFERLEN 4096
int16_t contBuffer[BUFFERLEN];
unsigned int bufferPos;

/* Holds currently type of continuous measurement (voltage, power, current).
 * Used to avoid floating point numbers prolification */
int measureType;

/**************************************************************************/
/*!
 @brief  Sends a single command byte over I2C
 */
/**************************************************************************/
uint8_t adrs_219 = 0x41; // you can call this from main function when necessary especially when using multiple INA219 on board
void wireWriteRegister (uint8_t reg, uint16_t value)
{
    uint8_t i2c_temp[2];
    i2c_temp[0] = value>>8;
    i2c_temp[1] = value;
    HAL_I2C_Mem_Write(&hi2c1, adrs_219<<1, (uint16_t)reg, 1, i2c_temp, 2, 0xffffffff);
    HAL_Delay(1);
}

/**************************************************************************/
/*!
 @brief  Reads a 16 bit values over I2C
 */
/**************************************************************************/
void wireReadRegister(uint8_t reg, uint16_t *value)
{
    uint8_t i2c_temp[2];
    HAL_I2C_Mem_Read(&hi2c1, adrs_219<<1, (uint16_t)reg, 1,i2c_temp, 2, 0xffffffff);
    HAL_Delay(1);
    *value = ((uint16_t)i2c_temp[0]<<8 )|(uint16_t)i2c_temp[1];
}

    ina219_calValue = 5120;  // Rsh=0.04R,

    // Set multipliers to convert raw current/power values
    ina219_currentDivider_mA = 6; // Current LSB = 100uA per bit (1000/100 = 10)
    ina219_powerMultiplier_mW = 3;     // Power LSB = 1mW per bit (2/1)

    // Set Calibration register to 'Cal' calculated above
    wireWriteRegister(INA219_REG_CALIBRATION, ina219_calValue);

    // Set Config register to take into account the settings above
    uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V
            | INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT
            | INA219_CONFIG_SADCRES_12BIT_1S_532US
            | INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
    wireWriteRegister(INA219_REG_CONFIG, config);
}

    ina219_calValue = 10240;

    // Set multipliers to convert raw current/power values
    ina219_currentDivider_mA = 25;  // Current LSB = 40uA per bit (1000/40 = 25)
    ina219_powerMultiplier_mW = 1;         // Power LSB = 800mW per bit

    // Set Calibration register to 'Cal' calculated above
    wireWriteRegister(INA219_REG_CALIBRATION, ina219_calValue);

    // Set Config register to take into account the settings above
    uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V
            | INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT
            | INA219_CONFIG_SADCRES_12BIT_1S_532US
            | INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
    wireWriteRegister(INA219_REG_CONFIG, config);
}
    // 5. Compute the calibration register
    // Cal = trunc (0.04096 / (Current_LSB * RSHUNT))
    // Cal = 8192 (0x2000)

    ina219_calValue = 8192;

    // Set multipliers to convert raw current/power values
    ina219_currentDivider_mA = 20;  // Current LSB = 50uA per bit (1000/50 = 20)
    ina219_powerMultiplier_mW = 1;     // Power LSB = 1mW per bit

    // Set Calibration register to 'Cal' calculated above
    wireWriteRegister(INA219_REG_CALIBRATION, ina219_calValue);

    // Set Config register to take into account the settings above
    uint16_t config = INA219_CONFIG_BVOLTAGERANGE_16V
            | INA219_CONFIG_GAIN_1_40MV | INA219_CONFIG_BADCRES_12BIT
            | INA219_CONFIG_SADCRES_12BIT_1S_532US
            | INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
    wireWriteRegister(INA219_REG_CONFIG, config);
}

int16_t getBusVoltage_raw() {
    uint16_t value;
    wireReadRegister(INA219_REG_BUSVOLTAGE, &value);

    // Shift to the right 3 to drop CNVR and OVF and multiply by LSB
    return (int16_t) ((value >> 3) * 4);
}

int16_t getShuntVoltage_raw() {
    uint16_t value;
    wireReadRegister(INA219_REG_SHUNTVOLTAGE, &value);
    return (int16_t) value;
}

int16_t getCurrent_raw() {
    uint16_t value;

    wireWriteRegister(INA219_REG_CALIBRATION, ina219_calValue);

    // Now we can safely read the CURRENT register!
    wireReadRegister(INA219_REG_CURRENT, &value);

    return (int16_t) value;
}

int16_t getPower_raw() {
    uint16_t value;
    wireWriteRegister(INA219_REG_CALIBRATION, ina219_calValue);

    // Now we can safely read the POWER register!
    wireReadRegister(INA219_REG_POWER, &value);

    return (int16_t) value;
}

float getShuntVoltage_mV() {
    int16_t value;
    value = getShuntVoltage_raw();
    return value * 0.01;
}

float getBusVoltage_V() {
    int16_t value = getBusVoltage_raw();
    return value * 0.001;

float getCurrent_mA() {
    float valueDec = getCurrent_raw();
    valueDec /= ina219_currentDivider_mA;
    return valueDec;
}

float getPower_mW() {
    float valueDec = getPower_raw();
    valueDec *= ina219_powerMultiplier_mW;
    return valueDec;
}

int contMeasureInit(uint8_t reg) {
    HAL_StatusTypeDef status;
    measureType = reg;

    /* Set register pointer to desired register */
    status = HAL_I2C_Master_Transmit(&hi2c1, adrs_219 << 1, &reg, 1,
            0xffffffff);
    if (status != HAL_OK)
        while (1)
            ;

    bufferPos = 0;
    memset(contBuffer, 0, sizeof(contBuffer));

    return 0;
}

int contMeasureUpdate(void) {
    HAL_StatusTypeDef status;
    uint8_t measure[2];

    status = HAL_I2C_Master_Receive(&hi2c1, adrs_219 << 1,
            (uint8_t*) &measure, 2, 0xffffffff);
    if (status != HAL_OK)
        while (1)
            ;

    /* Change endinanness */
    if (bufferPos < BUFFERLEN)
        contBuffer[bufferPos++] = ina219_powerMultiplier_mW
                * (((uint16_t) measure[0] << 8) | (uint16_t) measure[1]);

    return bufferPos;
}


float convertMeasure(int rawValue) {
    float val = rawValue;

    switch (measureType) {
    case INA219_REG_SHUNTVOLTAGE:
        val *= 0.01f;
        break;

    case INA219_REG_BUSVOLTAGE:
        val *= 0.001f;
        break;

    case INA219_REG_POWER:
        val *= ina219_powerMultiplier_mW;
        break;

    case INA219_REG_CURRENT:
        val /= ina219_currentDivider_mA;
        break;

    default:
        val = -1;
    }

    return val;
}

int getNSamples(void) {
    return bufferPos;
}
