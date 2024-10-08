#pragma once
//https://docs.espressif.com/projects/esp-at/en/release-v2.2.0.0_esp8266/AT_Command_Set/index.html
#include "hardware/uart.h"

#include "hardware/structs/io_bank0.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
 
 
#include <cstring>
#include <cstdio> 

#include "uart.hpp"
class ESP8285 :public Uart, public WFCmds {
private:
	// Приватные переменные и методы (доступны только внутри класса)
 
public:	
	
	// Публичные методы (доступны извне класса)
	ESP8285(uart_inst_t* uart = uart0,  // Используем uart0 по умолчанию
		uint uart_baud = 115200, 
		uint uart_data_bits = 8, 
		uint uart_stop_bits = 1, 
		uart_parity_t uart_parity = UART_PARITY_NONE,
		uint uart_rx_pin = 1, 
		uint uart_tx_pin = 0) 
		: Uart(uart, uart_baud, uart_data_bits, uart_stop_bits, uart_parity, uart_rx_pin, uart_tx_pin)
	{}
	 
	static ESP8285* instance;	 
	void UartChangeBaud(uint baud)		
	{
		/*
		uart_deinit(_uart);
		uart_init(_uart, baud);		
		*/
	}	
	bool TargetChangeBaud(uint baud)
	{
		//SendCommand()
		return false;
	}
	
	
	void SetIrq(bool val = false)
	{
		/*
		uint uart_irq_num = 0; 
		if (_uart == uart0)
		{
			uart_irq_num = UART0_IRQ;
		}
		else if (_uart == uart1)
		{				
				
			uart_irq_num = UART1_IRQ;
		}
		if (val)
		{ 
			irq_set_exclusive_handler(uart_irq_num,
				[]() {
					ESP8285::getInstance().irq();
				});
			irq_set_enabled(uart_irq_num, true);
			uart_set_irq_enables(_uart, true, false);
		}
		else
		{
			irq_set_enabled(uart_irq_num, true);
			uart_set_irq_enables(_uart, true, false);
		}*/
	}
 
	
	static void irqHandlerWrapper() {
		if (instance) {
			instance->irq();
		}
	}
	
	void irq() {
		/*
		printf("irq\n");
		static uint idx = 0;
		if (uart_is_readable(_uart))
		{
			_answer[idx++] = uart_getc(_uart);
			
		}
		idx = (idx == _answerEnd) ? 0 : idx;
		*/
	}
	
	static ESP8285& getInstance() {
		static ESP8285 instance;
		return instance;
	}
		
	
	
};

