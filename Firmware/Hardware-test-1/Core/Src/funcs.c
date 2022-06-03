/*
 * funcs.c
 *
 *  Created on: Jun 3, 2022
 *      Author: Scott Porter
 */

#include "funcs.h"

I2C_HandleTypeDef hi2c2;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim14;

UART_HandleTypeDef huart1;

RTC_DateTypeDef rtcDate;
RTC_TimeTypeDef rtcTime;
uint32_t pTmr=0;
char s[64];

// LED brightnesses, for timer interrupt to use to generate PWM on LED pins
int8_t ledLevel[8]={0,25,50,75,100,75,50,25};

// Direction of change for LED brightnesses. The brightness is changed at 100hz
int8_t ledDir[8]={1,1,1,1,-1,-1,-1,-1};

// ticker used in timer interrupt for PWM generation
volatile uint8_t I_ticker=0;

// tock increments at 100hz. Useful for synchronising animation
volatile uint8_t I_tocks=0;

// Sensor readings
float temperature=0;
float humidity=0;

// Sends data to the UART, which is connected to USB. It is set up for 115200bps in this program.
void debugPrint(UART_HandleTypeDef *huart, char _out[]){
 HAL_UART_Transmit(huart, (uint8_t *) _out, strlen(_out), 10);
}

// As above, but also sends a CRLF after the text
void debugPrintln(UART_HandleTypeDef *huart, char _out[]){
 HAL_UART_Transmit(huart, (uint8_t *) _out, strlen(_out), 10);
 char newline[2] = "\r\n";
 HAL_UART_Transmit(huart, (uint8_t *) newline, 2, 10);
}


// Interrupt called at 10Khz, mostlu used for PWM, and also increments I_tocks at ~100hz
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if(++I_ticker==101){
		I_ticker=0;
		++I_tocks;
	}
	volatile uint8_t x=0;
	if(ledLevel[0]>I_ticker) x=1;
	if(ledLevel[1]>I_ticker) x|=(1<<1);
	if(ledLevel[2]>I_ticker) x|=(1<<2);
	if(ledLevel[3]>I_ticker) x|=(1<<3);
	if(ledLevel[4]>I_ticker) x|=(1<<4);
	if(ledLevel[5]>I_ticker) x|=(1<<5);
	if(ledLevel[6]>I_ticker) x|=(1<<6);
	if(ledLevel[7]>I_ticker) x|=(1<<7);
	GPIOA->ODR=x;
}

void doLoop(){
	if(I_tocks){
		I_tocks=0;
		// Update the LED brightnesses at 100hz
		for(uint8_t n=0;n<8;n++){
			ledLevel[n]+=ledDir[n];
			if(ledLevel[n]==100){
				ledDir[n]=-1;
			}else if(ledLevel[n]==0){
				ledDir[n]=1;
			}
		}

		// Show the date/time and button states around every second
		if(++pTmr==100){
			uint32_t buts = GPIOC->IDR;
			HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);//Get time
			HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);//get date
			sprintf(
				s,
				"%02d/%02d/20%02d %02d:%02d:%02d : %s %s : ",
				rtcDate.Date,rtcDate.Month,rtcDate.Year,
				rtcTime.Hours, rtcTime.Minutes, rtcTime.Seconds,
				(buts&(1<<15)?"----":"BUT1"),
				(buts&(1<<14)?"----":"BUT2")
			);
			debugPrint(&huart1, s);
			pTmr=0;
			uint8_t ret=getHumidityTemp();
			if(ret){
				sprintf(s,"i2C returned: %d\r\n",ret);
			}else{
				// The default compiler settings disable conversion of floating point
				// to text, and enabling it took too much flash memory!
				int32_t t=(temperature*100);
				int32_t h=(humidity*100);
				uint8_t tm=t%100;
				uint8_t hm=h%100;
				int16_t td=(int16_t)t/100;
				int16_t hd=(int16_t)h/100;
				t/=100;
				h/=100;
				sprintf(s,"Temp: %d.%dC, Relative Humidity: %d.%d%%\r\n",td,tm,hd,hm);
			}
			debugPrintln(&huart1, s);
		}

	}

}

// Read the temperature and humidity from the on-board SHT30 sensor
uint8_t getHumidityTemp(void){
	uint8_t pData[6]={0x2C,0x06,0,0,0,0};
	uint8_t ret=HAL_I2C_Master_Transmit (&hi2c2, HUMIDITY_SENSOR_ADDR<<1, pData, 2, 10000);
	HAL_Delay(50);
	ret=HAL_I2C_Master_Receive (&hi2c2, HUMIDITY_SENSOR_ADDR<<1, pData, 6, 10000);

	if(ret){
		return ret;
	}

	temperature = ((((pData[0] * 256.0) + pData[1]) * 175) / 65535.0) - 45;
	humidity = ((((pData[3] * 256.0) + pData[4]) * 100) / 65535.0);

	return ret;
}

// Scan for i2C devices
void scanBus(void){
	HAL_Delay(1000);
	char s[16];
	debugPrint(&huart1, "Scanning for i2C devices...\r\n\r\n---- ");
	for(uint8_t i=1; i<128; i++){
		uint8_t ret = HAL_I2C_IsDeviceReady(&hi2c2, (uint16_t)(i<<1), 3, 5);
		if (ret != HAL_OK) /* No ACK Received At That Address */
		{
			sprintf(s, "---- ");
		}
		else if(ret == HAL_OK)
		{
			sprintf(s, "0x%02X ", i);
		}
		if((i%16)==15){
			debugPrintln(&huart1, s);
		}else{
			debugPrint(&huart1, s);
		}
	}
	debugPrintln(&huart1, "\r\nDone\r\n");
}
