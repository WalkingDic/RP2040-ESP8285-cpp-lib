/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <stdio.h>
#include <string.h>

#include "ESP8285.hpp"
 

// Pico W devices use a GPIO on the WIFI chip for the LED,
// so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#define LED_DELAY_MS 2000

#define UART1_TX_PIN 4   // Пин для передачи данных (TX) для UART1
#define UART1_RX_PIN 5   // Пин для приема данных (RX) для UART1
#define MAX_STR_LEN 100          // Максимальная длина строки для чтения

// Perform initialisation
int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
	// A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
	// so we can use normal GPIO functionality to turn the led on and off
	gpio_init(PICO_DEFAULT_LED_PIN);
	gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
	return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
	// For Pico W devices we need to initialise the driver etc
	return cyw43_arch_init();
#endif
}

// Turn the led on or off
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
	// Just set the GPIO on or off
	gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
	// Ask the wifi "driver" to set the GPIO on or off
	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

int main() { ///19200, 38400, 57600, 115200, 230400, 460800, 921600
	ESP8285 ESP;      
	// Инициализация UART1 с заданной скоростью
	uart_init(uart1, 115200);
	// Настройка пинов для UART1 (TX на GPIO4 и RX на GPIO5)
	gpio_set_function(UART1_TX_PIN, GPIO_FUNC_UART);
	gpio_set_function(UART1_RX_PIN, GPIO_FUNC_UART); 
	
	// Перенаправляем стандартный вывод на UART1
	stdio_uart_init_full(uart1, 115200, UART1_TX_PIN, UART1_RX_PIN);

	int rc = pico_led_init();
	hard_assert(rc == PICO_OK);
//	auto ff = ESP.GetCmdNo(ESP8285::cmdGetOK);
	ESP.SendGetCommand(Uart::cmdGetOK);
	ESP.SendGetCommand(Uart::cmdGetRst);
//	ESP.SendGetCommand(Uart::cmdGetOK);
//	ESP.SendGetCommand(Uart::cmdGetUartDef);
//	ESP.SendGetCommand(Uart::cmdGetOK);
//	ESP.SendGetCommand(Uart::cmdSetFact);
//	ESP.SendGetCommand(Uart::cmdGetOK);
//	ESP.SendGetCommand(Uart::cmdGetRst);
//	ESP.SendGetCommand(Uart::cmdGetOK);
	ESP.SendGetCommand(Uart::cmdGetUartCur);
	ESP.SendGetCommand(Uart::cmdGetOK);
	ESP.SetBaud(115200);
	//ESP.SetBaud(230400);
	ESP.SendGetCommand(Uart::cmdGetOK);
	
	ESP.SendGetCommand(Uart::cmdGetUartCur);
	ESP.SendGetCommand(Uart::cmdGetOK);
	
	
	/*ESP.SendCommand(ESP.GetCmdNo(ESP8285::cmdGetOK));
	ESP.SendCommand(ESP.GetCmdNo(ESP8285::cmdGetUartCur));
	ESP.SendCommand(ESP.GetCmdNo(ESP8285::cmdGetOK));
	ESP.SendCommand(ESP.GetCmdNo(ESP8285::cmdSetUartCur));
	ESP.SendCommand(ESP.GetCmdNo(ESP8285::cmdGetOK));
	
	ESP.UartChangeBaud(230400);
	ESP.SendCommand(ESP.GetCmdNo(ESP8285::cmdGetOK));
	
	ESP.SendCommand(ESP.GetCmdNo(ESP8285::cmdGetRst));
	 
	ESP.SendCommand(ESP.GetCmdStr(ESP8285::cmdGetRst));	   
	ESP.SendCommand(ESP.GetCmdStr(ESP8285::cmdGetVer));	 
	ESP.SendCommand(ESP.GetCmdStr(ESP8285::cmdGetMode));	
	
	ESP.SendCommand(ESP.GetCmdStr(ESP8285::cmdSetModeSta));	
	ESP.SendCommand(ESP.GetCmdStr(ESP8285::cmdGetMode));	
	ESP.SendCommand(ESP.GetCmdStr(ESP8285::cmdGetAPlist));	 
	
	
	
	
	ESP.PrintAnswer();
	sleep_ms(1000);
	printf("ssss");
	ESP.PrintAnswer();
	
	auto time = get_absolute_time();
	ESP.SendCommand(ESP.GetCmdStr(ESP8285::cmdGetOK));
	
	
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetSysLog));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdSetSysLogEna));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetOK));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetTime));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetOK));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetRAM));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetOK));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetCMD));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetOK));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetUartDef));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetOK));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetUartCur));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetOK));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetVer));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetOK));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdSetModeSta));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetOK));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetRFpower));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetOK));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetAPlist));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetOK));
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdWConn));
	
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetOK));
	
		
	if (ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetAPlist))) {
			
		//uart_puts(uart1, ESP.GetAnswer());
		;
	}
		
	//while (!ESP.Ready())
	{
		
	}
	ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetOK));
	
	*/
	while (true) {
		pico_set_led(true); 
		sleep_ms(LED_DELAY_MS);
		printf("%d \r\n", to_ms_since_boot(get_absolute_time()));
		pico_set_led(false);
		sleep_ms(LED_DELAY_MS);
		/*
		ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetOK));
		ESP.SendCommand(ESP.enumToString(ESP8285::cmdWGetIp));
		if (ESP.SendCommand(ESP.enumToString(ESP8285::cmdGetAPlist ))) {
			
			uart_puts(uart1, ESP.GetAnswer());
			;
		}
		
		//;
		uart_puts(uart0, "AT+GMR\r\n"); 
		bool flag = false;
		bool flag2 = false;
		memset(buffer, 0x00, MAX_STR_LEN);
	    
		size_t index = 0;
	    
		while (uart_is_readable(uart1))
		{
			buffer[index++] = uart_getc(uart1);
			; flag = true;
		    
		}
		if (flag)
		{
		    
			asm("nop");
		    
			uart_puts(uart0, buffer);
		    
		}
		index = 0;
		while (uart_is_readable(uart0))
		{
			buffer2[index++] = uart_getc(uart0);
			; flag2 = true;
		    
		}
		if (flag2)
		{
		    
			asm("nop");
		     
		    
		}*/
	}
}

 
