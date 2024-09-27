#pragma once
#include <hardware/uart.h>
 
#include "hardware/gpio.h"
#include "Commands.hpp"
#include <string.h>
#include <stdio.h>
#include <pico/time.h>
	  
class Uart :public ATCmds
{
public:
	
	Uart(uart_inst_t* uart = uart0,  // Используем uart0 по умолчанию
		uint uart_baud = 115200, 
		uint uart_data_bits = 8, 
		uint uart_stop_bits = 1, 
		uart_parity_t uart_parity = UART_PARITY_NONE,
		uint uart_rx_pin = 1, 
		uint uart_tx_pin = 0,
		uint flow_ctrl_ctc = 0, 
		uint flow_ctrl_rtc = 0)
		: ATCmds()
	{			
		_uart = uart;
		_baud = uart_baud;
		_data_bits = uart_data_bits;
		_stop_bits = uart_stop_bits;
		_parity = uart_parity;
		_rx_pin = uart_rx_pin;
		_tx_pin = uart_tx_pin;
		_flow_ctrl_ctc = flow_ctrl_ctc;
		_flow_ctrl_rtc = flow_ctrl_rtc;
		
		_flow_ctrl = (_flow_ctrl_ctc << 1) | (_flow_ctrl_rtc << 0);
		_status = _status_td::OK;
		
		uart_init(uart, uart_baud);		
		gpio_set_function(uart_tx_pin, GPIO_FUNC_UART);
		gpio_set_function(uart_rx_pin, GPIO_FUNC_UART); 
		uart_set_format(uart, uart_data_bits, uart_stop_bits, uart_parity);
		uart_set_hw_flow(_uart, _flow_ctrl_ctc, _flow_ctrl_rtc); 
 
		//uart_set_fifo_enabled(uart, true);
		//uart_puts(_uart, "\r\n");
		memset(_answer, 0x00, _answerBufferSize);
		_status = _status_td::OK;
		//uart_puts(uart, "\r\n");
		busy_wait_ms(200); 
	} 
	~Uart() {} 
	
	bool SendGetCommand(int commandNum);
	
	bool SendSetCommand(const char* command);
	
	void SetBaud(uint baud)
	{
		_baud = baud;
		SetDevBaud();
		SetSelfBaud();
	}
	void SetSelfBaud()
	{ 
		auto __commandPrt = ATCmds::GetCmdStr(ATCmds::cmdSetUartCur);
		char result[100] = { 0,};
		//+UART_CUR=<baudrate>,<databits>,<stopbits>,<parity>,<flow control>
		sprintf(result, "%s%u,%u,%u,%u,%u", __commandPrt, _baud, _data_bits, _stop_bits, _parity, _flow_ctrl);
		SendSetCommand(result);
		
	}
	void SetDevBaud()
	{
		uart_deinit(_uart);
		uart_set_format(_uart, _data_bits, _stop_bits, _parity);
			uart_set_hw_flow(_uart, _flow_ctrl_ctc, _flow_ctrl_rtc); 
		uart_init(_uart, _baud);
		 
		 
		
	}
	
private:
	uart_inst_t* _uart;
	uint _baud;
	uint _data_bits;
	uint _stop_bits;
	uart_parity_t _parity;
	uint _rx_pin;
	uint _tx_pin;
	bool _flow_ctrl_rtc;
	bool _flow_ctrl_ctc;
	uint _flow_ctrl;
	uint _timeout;
	enum class _status_td
	{
		OK,
		ERROR,
		TIMEOUT,
		BUSY,
		WORK,
		UNKNOWN
	};
	_status_td _status;
			
	static const uint _answerBufferSize  = 1000;
	
	char _answer[_answerBufferSize];
	
	uint _answerEnd = 0;
	
	void _PrintAnswer(void)
	{
		size_t idx = 0;
		for (char ch : _answer)
		{
			//uart_putc(uart1, ch);
			printf("%c", ch);
			if (++idx == _answerEnd)
			{
				printf("\r\n");
				return;					
			}
		}
	}
};