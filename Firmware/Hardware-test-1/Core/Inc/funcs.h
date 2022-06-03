
/*
 * funcs.h
 *
 *  Created on: Jun 3, 2022
 *      Author: Scott Porter
 */

#ifndef INC_FUNCS_H_
#define INC_FUNCS_H_


#include "defs.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c2;

extern RTC_HandleTypeDef hrtc;

extern TIM_HandleTypeDef htim14;

extern UART_HandleTypeDef huart1;

void debugPrint(UART_HandleTypeDef *huart, char _out[]);
void debugPrintln(UART_HandleTypeDef *huart, char _out[]);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim);
void doLoop();

// Returns the humidity and temperature in the passed variables
// Return is 0 upon success, or HAL status if i2C comms failed
uint8_t getHumidityTemp(void);

// Scans for i2C devices on bus
void scanBus(void);

#endif /* INC_FUNCS_H_ */
