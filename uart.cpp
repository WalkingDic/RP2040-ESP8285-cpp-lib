
#include "uart.hpp"
bool Uart::SendCommand(int commandNum)
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