#pragma once
#include <hardware/uart.h>
 
#include "hardware/gpio.h"
#include "Commands.hpp"
#include <string.h>
#include <stdio.h>
#include <pico/time.h>
	  
class Uart :public Cmds
{
public:
	
	Uart(uart_inst_t* uart = uart0,  // Используем uart0 по умолчанию
		uint uart_baud = 115200, 
		uint uart_data_bits = 8, 
		uint uart_stop_bits = 1, 
		uart_parity_t uart_parity = UART_PARITY_NONE,
		uint uart_rx_pin = 1, 
		uint uart_tx_pin = 0)
		: Cmds()
	{			
		_uart = uart;
		_baud = uart_baud;
		_data_bits = uart_data_bits;
		_stop_bits = uart_stop_bits;
		_parity = uart_parity;
		_rx_pin = uart_rx_pin;
		_tx_pin = uart_tx_pin;
		_status = _status_td::OK;
		
		uart_init(uart, uart_baud);		
		gpio_set_function(uart_tx_pin, GPIO_FUNC_UART);
		gpio_set_function(uart_rx_pin, GPIO_FUNC_UART); 
		uart_set_format(uart, uart_data_bits, uart_stop_bits, uart_parity);
		//uart_set_hw_flow(uart, false, false); 
		//uart_set_fifo_enabled(uart, true);
		//uart_puts(_uart, "\r\n");
		memset(_answer, 0x00, _answerBufferSize);
		_status = _status_td::OK;
		//uart_puts(uart, "\r\n");
		busy_wait_ms(200); 
	} 
	~Uart() {} 
	
	bool SendCommand(int commandNum)
	{		
		const auto __command = static_cast<commands>(commandNum);
		const auto __commandPrt = Cmds::GetCmdStr(__command);
		const auto __answerPrt =  Cmds::GetErrStr(__command);
		const uint __timeout = Cmds::GetTimeout(__command);
		uint __answerLen = strlen(__answerPrt);
		const char*  __errorStr = ANSWER_ERR;
		const char* __busyStr = ANSWER_BUSY;
		bool __result = false;
		bool __gotResult = 0;
		_status = _status_td::WORK;
		uint idx = 0;
		_answerEnd = 0;
		
		memset(_answer, 0xFF, _answerBufferSize);
		uart_puts(_uart, __commandPrt);
		uart_puts(_uart, "\r\n");

		auto __startTime = get_absolute_time();			
		while (!__gotResult && uart_is_readable_within_us(_uart, 1000000))			
		{						  
			_answer[idx++] = uart_getc(_uart);
			_answerEnd = idx;
			//__startTime = get_absolute_time();
			if (idx >= ANSWER_ERR_LEN && !memcmp(_answer + _answerEnd - ANSWER_ERR_LEN, __errorStr, ANSWER_ERR_LEN))
			{ 
				_status = _status_td::ERROR;
				__result = false;
				__gotResult = true;
				__answerLen = ANSWER_ERR_LEN;
			}			
			else if (idx >= ANSWER_BUSY_LEN && !memcmp(_answer + _answerEnd - ANSWER_BUSY_LEN, __busyStr, ANSWER_BUSY_LEN))
			{
				_status = _status_td::BUSY;
				__result = false;
				__gotResult = true;
				__answerLen = ANSWER_BUSY_LEN;
			} 
			else if (idx >= __answerLen && !memcmp(_answer + _answerEnd - __answerLen, __answerPrt, __answerLen))
			{
				_status = _status_td::OK;
				__result = true;
				__gotResult = true;
			}			
		}  
		memset(_answer + _answerEnd - __answerLen, 0x00, __answerLen);
		_status = (_status == _status_td::WORK) ? _status_td::UNKNOWN : _status; 		
		return __result;
	}
	
private:
	uart_inst_t* _uart;
	uint _baud;
	uint _data_bits;
	uint _stop_bits;
	uart_parity_t _parity;
	uint _rx_pin;
	uint _tx_pin;
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
	
	void _ClrAnswerOK()
	{
		memset(_answer + _answerEnd - 5, 0x00, 5);
	}
	
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