/*
 * This file is part of the W1209 firmware replacement project
 * (https://github.com/mister-grumbler/w1209-firmware).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Control functions for EEPROM storage of persistent application parameters.
 *
 * The list of aplication parameters with default values:
 * Name |Def| Description
 * -----+---+---------------------------------------------
 * P0 - | C | Cooling/Heating
 *            (relay ON when temperature is over(C)/below(H) threshold value)
 * P1 - | 2 | 0.1 ... 15.0 - Hysteresis
 * P2 - |110| 110 ... -45 - Maximum allowed temperature value
 * P3 - |-50| -50 ... 105 Minimum allowed temperature value
 * P4 - | 0 | 7.0 ... -7.0 Correction of temperature value
 * P5 - | 0 | 0 ... 10 Relay switching delay in minutes
 * P6 - |Off| On/Off Indication of overheating
 * P  - | 0 | Reserve
 * P  - | 0 | Reserve
 * TH - | 28| Threshold value
 * TM - | 10| Timer value
 */

#include "params.h"
#include "stm8s003/prom.h"
#include "buttons.h"

/* Definitions for EEPROM */
#define EEPROM_BASE_ADDR        0x4000
#define EEPROM_PARAMS_OFFSET    100

static unsigned char paramId;
static int paramCache[PARAM_COUNT];
const int paramMin[] = {0, 1, -45, -50, -70, 0, 0, 0, 0, -500, 0};
const int paramMax[] = {1, 150, 110, 105, 70, 10, 1, 0, 0, 1100, 240};
// const int paramDefault[] = {0, 20, 110, -50, 0, 0, 0, 0, 0, 280};
const int paramDefault[] = {1, 5, 80, 10, 0, 0, 0, 0, 0, 280, 100}; // timer-termostat

/**
 * @brief Check values in the EEPROM to be correct then load them into
 * parameters' cache.
 */
void initParamsEEPROM()
{
    if (getButton2() && getButton3() ) {
        // Restore parameters to default values
        for (paramId = 0; paramId < PARAM_COUNT; paramId++) {
            paramCache[paramId] = paramDefault[paramId];
        }

        storeParams();
    } else {
        // Load parameters from EEPROM
        for (paramId = 0; paramId < PARAM_COUNT; paramId++) {
            paramCache[paramId] = * (int*) (EEPROM_BASE_ADDR + EEPROM_PARAMS_OFFSET
                                            + (paramId * sizeof paramCache[0]) );
        }
    }

    paramId = 0;
}

/**
 * @brief
 * @param id
 * @return
 */
int getParamById (unsigned char id)
{
    if (id < PARAM_COUNT) {
        return paramCache[id];
    }

    return -1;
}

/**
 * @brief
 * @param id
 * @param val
 */
void setParamById (unsigned char id, int val)
{
    if (id < PARAM_COUNT) {
        paramCache[id] = val;
    }
}

/**
 * @brief
 * @return
 */
int getParam()
{
    return paramCache[paramId];
}

/**
 * @brief
 * @param val
 */
void setParam (int val)
{
    paramCache[paramId] = val;
}

/**
 * @brief Incrementing the value of the currently selected parameter.
 */
void incParam()
{
    if (paramId == PARAM_RELAY_MODE || paramId == PARAM_OVERHEAT_INDICATION) {
        paramCache[paramId] = ~paramCache[paramId] & 0x0001;
    } else if (paramId == PARAM_TIMER) {
        switch (paramCache[paramId])
        {
        case 59:
            paramCache[paramId] = 100;
            break;
        case 159:
            paramCache[paramId] = 200;
            break;
        case 259:
            paramCache[paramId] = 300;
            break;
        case 359:
            paramCache[paramId] = 400;
            break;
        case 459:
            paramCache[paramId] = 500;
            break;
        case 559:
            paramCache[paramId] = 600;
            break;
        case 659:
            paramCache[paramId] = 700;
            break;
        case 759:
            paramCache[paramId] = 800;
            break;
        case 859:
            paramCache[paramId] = 900;
            break;
        case 959:
            paramCache[paramId] = 0;
            break;
        default:
            paramCache[paramId]++;
            break;
        }
    } else if (paramCache[paramId] < paramMax[paramId]) {
        paramCache[paramId]++;
    }
}

/**
 * @brief Decrementing the value of the currently selected parameter.
 */
void decParam()
{
    if (paramId == PARAM_RELAY_MODE || paramId == PARAM_OVERHEAT_INDICATION) {
        paramCache[paramId] = ~paramCache[paramId] & 0x0001;
        } else if (paramId == PARAM_TIMER) {
        switch (paramCache[paramId])
        {
        case 0:
            paramCache[paramId] = 959;
            break;
        case 100:
            paramCache[paramId] = 59;
            break;
        case 200:
            paramCache[paramId] = 159;
            break;
        case 300:
            paramCache[paramId] = 259;
            break;
        case 400:
            paramCache[paramId] = 359;
            break;
        case 500:
            paramCache[paramId] = 459;
            break;
        case 600:
            paramCache[paramId] = 559;
            break;
        case 700:
            paramCache[paramId] = 659;
            break;
        case 800:
            paramCache[paramId] = 759;
            break;
        case 900:
            paramCache[paramId] = 859;
            break;
        default:
            paramCache[paramId]--;
            break;
        }
    } else if (paramCache[paramId] > paramMin[paramId]) {
        paramCache[paramId]--;
    }
}

/**
 * @brief
 * @return
 */
unsigned char getParamId()
{
    return paramId;
}

/**
 * @brief
 * @param val
 */
void setParamId (unsigned char val)
{
    if (val < PARAM_COUNT) {
        paramId = val;
    }
}

/**
 * @brief
 */
void incParamId()
{
    if (paramId < 6) {
        paramId++;
    } else {
        paramId = 0;
    }
}

/**
 * @brief
 */
void decParamId()
{
    if (paramId > 0) {
        paramId--;
    } else {
        paramId = 6;
    }
}

/**
 * @brief Converts the current value of the selected parameter to a string.
 * @param id
 *  The identifier of the parameter to be processed.
 * @param strBuff
 *  A pointer to a string buffer where the result should be placed.
 */
void paramToString (unsigned char id, unsigned char* strBuff)
{
    switch (id) {
    case PARAM_RELAY_MODE:
        if (paramCache[id]) {
            ( (unsigned char*) strBuff) [0] = 'H';
        } else {
            ( (unsigned char*) strBuff) [0] = 'C';
        }

        ( (unsigned char*) strBuff) [1] = 0;
        break;

    case PARAM_RELAY_HYSTERESIS:
        itofpa (paramCache[id], strBuff, 0);
        break;

    case PARAM_MAX_TEMPERATURE:
        itofpa (paramCache[id], strBuff, 6);
        break;

    case PARAM_MIN_TEMPERATURE:
        itofpa (paramCache[id], strBuff, 6);
        break;

    case PARAM_TEMPERATURE_CORRECTION:
        itofpa (paramCache[id], strBuff, 0);
        break;

    case PARAM_RELAY_DELAY:
        itofpa (paramCache[id], strBuff, 6);
        break;

    case PARAM_OVERHEAT_INDICATION:
        ( (unsigned char*) strBuff) [0] = 'O';

        if (paramCache[id]) {
            ( (unsigned char*) strBuff) [1] = 'N';
            ( (unsigned char*) strBuff) [2] = ' ';
        } else {
            ( (unsigned char*) strBuff) [1] = 'F';
            ( (unsigned char*) strBuff) [2] = 'F';
        }

        ( (unsigned char*) strBuff) [3] = 0;
        break;

    case PARAM_TEMPER:
        itofpa (paramCache[id], strBuff, 0);
        break;
    
    case PARAM_TIMER:
        itofpa (paramCache[id], strBuff, 1);
        break;

    default: // Display "OFF" to all unknown ID
        ( (unsigned char*) strBuff) [0] = 'O';
        ( (unsigned char*) strBuff) [1] = 'F';
        ( (unsigned char*) strBuff) [2] = 'F';
        ( (unsigned char*) strBuff) [3] = 0;
    }
}

/**
 * @brief Stores updated parameters from paramCache into EEPROM.
 */
void storeParams()
{
    unsigned char i;

    //  Check if the EEPROM is write-protected.  If it is then unlock the EEPROM.
    if ( (FLASH_IAPSR & 0x08) == 0) {
        FLASH_DUKR = 0xAE;
        FLASH_DUKR = 0x56;
    }

    //  Write to the EEPROM parameters which value is changed.
    for (i = 0; i < PARAM_COUNT; i++) {
        if (paramCache[i] != (* (int*) (EEPROM_BASE_ADDR + EEPROM_PARAMS_OFFSET
                                        + (i * sizeof paramCache[0]) ) ) ) {
            * (int*) (EEPROM_BASE_ADDR + EEPROM_PARAMS_OFFSET
                      + (i * sizeof paramCache[0]) ) = paramCache[i];
        }
    }

    //  Now write protect the EEPROM.
    FLASH_IAPSR &= ~0x08;
}
/**
 * @brief
 * @param val
 * @param offset
 */
static void writeEEPROM (unsigned char val, unsigned char offset)
{
    //  Check if the EEPROM is write-protected.  If it is then unlock the EEPROM.
    if ( (FLASH_IAPSR & 0x08) == 0) {
        FLASH_DUKR = 0xAE;
        FLASH_DUKR = 0x56;
    }

    //  Write the data to the EEPROM.
    (* (unsigned char*) (EEPROM_BASE_ADDR + offset) ) = val;

    //  Now write protect the EEPROM.
    FLASH_IAPSR &= ~0x08;
}

/**
 * @brief Construction of a string representation of the given value.
 *  To emulate a floating-point value, a decimal point can be inserted
 *  before a certain digit.
 *  When the decimal point is not needed, set pointPosition to 6 or more.
 *  a) Если нужно короткое значение, то установить pointPosition в значение -2 или меньше
 *  b) Если нужна точка после правой цифры, то установить pointPosition в значение -1
 * @param val
 *  the value to be processed.
 * @param str
 *  pointer to buffer for constructed string.
 * @param pointPosition
 *  put the decimal point in front of specified digit.
 */
void itofpa (int val, unsigned char* str, int pointPosition)
{
    unsigned char i, l, buffer[] = {0, 0, 0, 0, 0, 0};
    bool minus = false;

    // Специальное формирование нулевого значения со включенной или ненужной точкой
    if (val == 0 && pointPosition >= -1) {
        ( (unsigned char*) str) [0] = '0';
        if (pointPosition == 1) ( (unsigned char*) str) [1] = '.'; 
        else ( (unsigned char*) str) [1] = '0';
        if (pointPosition == 0) ( (unsigned char*) str) [2] = '.'; 
        else ( (unsigned char*) str) [2] = '0';
        if (pointPosition == -1) ( (unsigned char*) str) [3] = '.';
        else ( (unsigned char*) str) [3] = '0';
        ( (unsigned char*) str) [4] = 0;
        return;
    }

    // Специальное формирование сокращенного нулевого значения
    if (val == 0 && pointPosition < -1) {
        ( (unsigned char*) str) [0] = '0';
        ( (unsigned char*) str) [1] = 0;
        return;
    }
    // Correction for processing of negative value
    if (val < 0) {
        minus = true;
        val = -val;
    }

    // Forming the reverse string учитывая положение точки
    for (i = 0; val != 0 || i <= pointPosition; i++) {
        buffer[i] = '0' + (val % 10);

        if (i == pointPosition) {
            i++;
            buffer[i] = '.';
        }

        val /= 10;
    }

    // Add leading '0' in case of ".x" result
    if (buffer[i - 1] == '.') {
        buffer[i] = '0';
        i++;
    }

    // Add '-' sign for negative values
    if (minus) {
        buffer[i] = '-';
        i++;
    }

    // Reversing to get the result string
    for (l = i; i > 0; i--) {
        ( (unsigned char*) str) [l - i] = buffer[i - 1];
    }

    // Put null at the end of string
    ( (unsigned char*) str) [l] = 0;
}
