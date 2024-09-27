#include "ESP8285.hpp"


#include <pico/time.h>
#include <string.h>
bool ESP8285::SendCommand(int commandNum)
{		 
	auto obj = this;
	auto __command = static_cast<commands>(commandNum);
	auto __commandPrt = GetCmdStr(__command);
	auto __answerPrt = GetErrStr(__command);
		
		
	memset(_command, 0x00, _answerBufferSize);
	memccpy(_command, __commandPrt, '\0', _answerBufferSize);
	_status = _status_td::Working;
	uart_puts(_uart, __commandPrt);
	uart_puts(_uart, "\r\n");
	uint idx = 0;
	//char cmd[100] = {};
	//sprintf(cmd, "waiting - %s\r\n", command);
	memset(_answer, 0x00, _answerBufferSize);
	int pause = 0;
	while (!uart_is_readable(_uart))
	{
		pause++;
	}
	//char cmd[100] = { };
	//sprintf(cmd, "waiting - %d\r\n", pause++);
	//uart_puts(uart1, cmd); && uart_is_readable(_uart)
	//while (uart_is_readable(_uart))&& uart_is_readable_within_us(_uart,_timeout)
	//while (uart_is_readable_within_us(_uart, _timeout))
		
	absolute_time_t __startTime = get_absolute_time();
		
	while (get_absolute_time() - __startTime < 400000)			
	{			
		while (uart_is_readable(_uart))
		//while (_status == ESP8285::_status_td::Working)
		//if (uart_is_readable_within_us(_uart, _timeout))
		{
			_answer[idx++] = uart_getc(_uart);
			_answerEnd = idx;
			//	
		}
	}
	/*
	PrintAnswer();
	if (idx == 0)
	{
		_status = _status_td::TIMEOUT;
		return false;
}
//_status = _status_td::OK;
*/
	GetStatus(__answerPrt);		
	return _status == _status_td::OK;	
}
bool ESP8285::GetStatus(const char* answerPrt)
		
{
	 
	//const char* answer = 3;
	auto __len = strlen(answerPrt);
	char tmp[12] = { 0, }; // "busy p...r/n/"
	memcpy(tmp, _answer + _answerEnd - 5, 5);
		
	char answerOk = 0;
	char answerErr = 0;
	char answerBusy = 0;
		
	//"\r\nbusy p...\r\n"
	for (uint idx = 0; idx < sizeof(tmp); idx++)
	{
		answerOk |= (char)(tmp[idx] == _sampleOk[idx]) << idx;
		answerErr |= (char)(tmp[idx] == _sampleErr[idx]) << idx;
		answerBusy |= (char)(tmp[idx] == _sampleBusy[idx]) << idx; 
			
	}
		 
	if (answerBusy == 31)
	{
		_status = _status_td::BUSY;
		return true;
	}
	else if (answerErr == 31)
	{
		_status = _status_td::ERROR;
		return false;
	}
	else if (answerOk == 31)
	{
		_status = _status_td::OK;
		//memset(_answer + _answerEnd  - 8, 0x00, 8);
		return false;
	}
	else
	{
		_status = _status_td::Working;
		return true;
	}
}	  
int ESP8285::GetCmdNo(commands value) {
	if (value >= 0 && value < STR_COUNT) { 
		return static_cast<int>(value);
	}
	return INT16_MAX;
}  
const char* ESP8285::GetCmdStr(commands value) {
	if (value >= 0 && value < STR_COUNT) {
		return enumCmd[value];
	}
	return "Unknown";
}  
const char* ESP8285::GetErrStr(commands value) {
	if (value >= 0 && value < STR_COUNT) {
		return answer[value];
	}
	return "Unknown";
}  
/*
ESP8285::ESP8285(uart_inst_t* uart = uart0,  // Используем uart0 по умолчанию
	uint uart_baud = 115200, 
	uint uart_data_bits = 8, 
	uint uart_stop_bits = 1, 
	uart_parity_t uart_parity = UART_PARITY_NONE,
	uint uart_rx_pin = 1, 
	uint uart_tx_pin = 0) 
	: _uart(uart)
	, _baud(uart_baud)
	, _data_bits(uart_data_bits)
	, _stop_bits(uart_stop_bits)
	, _parity(uart_parity)
	, _rx_pin(uart_rx_pin)
	, _tx_pin(uart_tx_pin)
{
	uart_init(uart, uart_baud);		
	gpio_set_function(uart_tx_pin, GPIO_FUNC_UART);
	gpio_set_function(uart_rx_pin, GPIO_FUNC_UART); 
	uart_set_format(uart, uart_data_bits, uart_stop_bits, uart_parity);
	//uart_set_hw_flow(uart, false, false); 
	//uart_set_fifo_enabled(uart, true);
	//uart_puts(_uart, "\r\n");
	memset(_answer, 0x00, _answerBufferSize);
	_status = _status_td::OK;
}*/