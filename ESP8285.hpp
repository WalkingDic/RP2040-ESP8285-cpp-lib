#pragma once
//https://docs.espressif.com/projects/esp-at/en/release-v2.2.0.0_esp8266/AT_Command_Set/index.html
#include "hardware/uart.h"

#include "hardware/structs/io_bank0.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
 
 
#include <cstring>
#include <cstdio> 

#include "Commands.hpp"
class ESP8285 :public Commands {
private:
	// Приватные переменные и методы (доступны только внутри класса)
	int privateVar;
	uart_inst_t* _uart;
	uint _baud;
	uint _data_bits;
	uint _stop_bits;
	uart_parity_t _parity;
	uint _rx_pin;
	uint _tx_pin;
	uint _timeout = 1000;
	enum class _status_td
	{
		OK,
		ERROR,
		TIMEOUT,
		BUSY,
		Working
	};
	_status_td _status;
	
	static const uint _answerBufferSize  = 1000;
	
	char _answer[_answerBufferSize];
	
	uint _answerEnd = 0;
	char _sample[6] = { "\nOK\r\n" };
	char _sampleOk[6] = { "\nOK\r\n" };
	char _sampleErr[6] = { "ROR\r\n" };
	char _sampleBusy[6] = { "usy\r\n" };
	char _command[_answerBufferSize] = { 0, };
	

	void _ClrAnswerOK()
	{
		memset(_answer + _answerEnd - 5, 0x00, 5);
	}
	
	
	 
public:	
	ESP8285(uart_inst_t* uart = uart0,  // Используем uart0 по умолчанию
		uint uart_baud = 115200, 
		uint uart_data_bits = 8, 
		uint uart_stop_bits = 1, 
		uart_parity_t uart_parity = UART_PARITY_NONE,
		uint uart_rx_pin = 1, 
		uint uart_tx_pin = 0) 
		: Commands()
		/*_uart(uart)
		, _baud(uart_baud)
		, _data_bits(uart_data_bits)
		, _stop_bits(uart_stop_bits)
		, _parity(uart_parity)
		, _rx_pin(uart_rx_pin)
		, _tx_pin(uart_tx_pin)*/
	{
		_uart = uart;
		_baud = uart_baud;
		_data_bits = uart_data_bits;
		_stop_bits = uart_stop_bits;
		_parity = uart_parity;
		_rx_pin = uart_rx_pin;
		_tx_pin = uart_tx_pin;
		uart_init(uart, uart_baud);		
		gpio_set_function(uart_tx_pin, GPIO_FUNC_UART);
		gpio_set_function(uart_rx_pin, GPIO_FUNC_UART); 
		uart_set_format(uart, uart_data_bits, uart_stop_bits, uart_parity);
		//uart_set_hw_flow(uart, false, false); 
		//uart_set_fifo_enabled(uart, true);
		//uart_puts(_uart, "\r\n");
		memset(_answer, 0x00, _answerBufferSize);
		_status = _status_td::OK;
	}
	
	
	bool Ready() {
		return _status == _status_td::OK;
	}
	bool GetStatus(const char* answerPrt);
	static ESP8285* instance;
	
	const char* answer[50];
	
	const char* enumCmd[50];
	  
	int GetCmdNo(commands value);
	const char* GetCmdStr(commands value);
	const char* GetErrStr(commands value);
	void UartChangeBaud(uint baud)
	{
		uart_deinit(_uart);
		uart_init(_uart, baud);		
	}
	
	bool TargetChangeBaud(uint baud)
	{
		//SendCommand()
		return false;
	}
	bool GetAT() {
		uart_puts(_uart, "AT\r\n");
		uint idx = 0;
		while (uart_is_readable_within_us(_uart, _timeout))
		//if (uart_is_readable_within_us(_uart, _timeout))
		{
			_answer[idx++] = uart_getc(_uart);
			
		}
		if (idx == 0)
		{
			_status = _status_td::TIMEOUT;
			return false;
		}
		_status = _status_td::OK;
		_answerEnd = idx;
		return false;			
	}
	
	
	bool SendCommand(int commandNum);
	void SetIrq(bool val = false)
	{
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
		}
	}
	char* GetAnswer()
	{
		return _answer;
	}
	
	uint GetAnswerSize()
	{
		return _answerEnd;
	}
	/*
	bool GetVer() {
		
		uart_puts(_uart, "AT+GMR\r\n");
		uint idx = 0;
		while (uart_is_readable_within_us(_uart, _timeout))
		//if (uart_is_readable_within_us(_uart, _timeout))
		{
			_answer[idx++] = uart_getc(_uart);
			
		}
		if (idx == 0)
		{
			_status = _status_td::TIMEOUT;
			return false;
		}
		_status = _status_td::OK;
		_answerEnd = idx;
		return _getOK();
	}
	;
	 */
	
	static void irqHandlerWrapper() {
		if (instance) {
			instance->irq();
		}
	}
	
	void irq() {
		printf("irq\n");
		static uint idx = 0;
		if (uart_is_readable(_uart))
		{
			_answer[idx++] = uart_getc(_uart);
			
		}
		idx = (idx == _answerEnd) ? 0 : idx;
	}
	
	static ESP8285& getInstance() {
		static ESP8285 instance;
		return instance;
	}
		
	void PrintAnswer(void)
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
	
	// Публичные методы (доступны извне класса)
	void setPrivateVar(int value) {
		privateVar = value;
	}

	int getPrivateVar() const {
		return privateVar;
	}

	// Другие методы
	void printValue() const {
		//		std::cout << "Private variable value: " << privateVar << std::endl;
	}
};

