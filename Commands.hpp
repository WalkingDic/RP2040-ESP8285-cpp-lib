#pragma once

#include <map>
#include <string>
	 
#define ANSWER_OK "\r\nOK\r\n"
#define ANSWER_OK_LEN 6U
#define ANSWER_ERR "\r\nERROR\r\n"
#define ANSWER_ERR_LEN 9U
#define ANSWER_BUSY "\r\nbusy p...\r\n"
#define ANSWER_BUSY_LEN 13U
#define ANSWER_READY "\r\nready\r\n"
class ATCmds
{
public:
	ATCmds() {}
	~ATCmds() {}
	
	enum commands
	{
		cmdGetOK,
		cmdGetRst,
		cmdGetVer, 
		cmdGetMode,
		cmdGetAPlist,
		cmdGetBaud,
		cmdGetUartCur,
		cmdGetUartDef,
		
		/*
		cmdSetEchoEn,
		cmdSetEchoDis, 
		cmdGetSysTimeStamp,
		cmdGetUartCur,
		cmdGetUartDef,
		cmdWConn,
		cmdWGetIp,
		cmdWGetUpd,
		cmdGetCMD,
		cmdGetRAM,
		cmdGetRFpower,
		cmdGetTime,
		cmdGetSysLog,
		cmdSetSysLogEna,
		cmdSetSysLogDis,*/
		
		cmdSetFact,
		cmdSetModeSta,
		cmdSetModeAp,
		cmdSetModeBoth,
		cmdSetUartCur,
		STR_COUNT
	};
	static const uint count = (int) static_cast<commands>(STR_COUNT);
	uint cmdNum = count + 1;
	const char* answers[count] = { 
		//	cmdGetOK,	"AT"
		ANSWER_OK,
		//	cmdGetRst,	"AT+RST" 
		ANSWER_READY,
		//	cmdGetVer,	"AT+GMR"
		ANSWER_OK,
		//	cmdGetMode,	"AT+CWMODE=?"
		ANSWER_OK,
		//	cmdGetAPlist,	"AT+CWLAP"
		ANSWER_OK,
		//	cmdGetBaud,	"AT+CIOBAUD?"
		ANSWER_OK,
		//	cmdGetUartCur,	"AT+UART_CUR?"
		ANSWER_OK,
		//	cmdGetUartDef,	"AT+UART_DEF?",
		ANSWER_OK,
		   
		//	cmdSetFact,	"AT+RST" 
		ANSWER_READY,
		//	cmdSetModeSta, "AT+CWMODE=1" 
		ANSWER_OK,
		//	cmdSetModeSta, "AT+CWMODE=2"
		ANSWER_OK,
		//	cmdSetModeSta, "AT+CWMODE=3"
		ANSWER_OK,
		//	cmdSetUartCur, "AT+UART_CUR="
		ANSWER_OK,	 
	};
	const uint timeout[count] = { 
		//	cmdGetOK,	"AT"
		1000,
		//	cmdGetRst,	"AT+RST" 
		1000000,
		//	cmdGetVer,	"AT+GMR"
		1000,
		//	cmdGetMode,	"AT+CWMODE=?"
		1000,
		//	cmdGetAPlist,	"AT+CWLAP"
		1000,
		//	cmdGetBaud,	"AT+CIOBAUD?"
		1000,
		//	cmdGetUartCur,	"AT+UART_CUR?"
		1000,
		//	cmdGetUartDef,	"AT+UART_DEF?",
		1000,
		   
		
		//	cmdSetFact,	"AT+RST" 
		1000000,
		//	cmdSetModeSta, "AT+CWMODE=1" 
		1000,
		//	cmdSetModeSta, "AT+CWMODE=2"
		1000,
		//	cmdSetModeSta, "AT+CWMODE=3"
		1000,
		//	cmdSetUartCur, "AT+UART_CUR="
		1000,	 
	};
	
	const char* cmdStrings[count] = {
		"AT",
		"AT+RST",
		"AT+GMR",
		"AT+CWMODE?",
		"AT+CWLAP",
		//ERROR
		"AT+CIOBAUD?",
		
		//+UART_CUR:<baudrate>,<databits>,<stopbits>,<parity>,<flow control>
		"AT+UART_CUR?",
		"AT+UART_DEF?",
		/*
		"ATE1",
		"ATE0",
		"AT+SYSTIMESTAMP",
		"AT+UART_CUR?",
		"AT+UART_DEF?",
		"AT+CWJAP=\"IEERAS\",\"4991351195\",\"48:8f:5a:70:28:63\"",
		"AT+CIFSR",
		"AT+CIUPDATE",
		"AT+CMD?",
		"AT+SYSRAM?",
		"AT+RFPOWER?",
		"AT+SYSTIMESTAMP?",
		"AT+SYSLOG?",
		"AT+SYSLOG=1",
		"AT+SYSLOG=0",*/
		
		"AT+RESTORE",
		"AT+CWMODE=1",
		"AT+CWMODE=2",
		"AT+CWMODE=3",
		//+UART_CUR=<baudrate>,<databits>,<stopbits>,<parity>,<flow control>
		"AT+UART_CUR=",
		
	};
	
	int GetCmdNo(commands value) {
		if (value >= 0 && value < STR_COUNT) { 
			return static_cast<int>(value);
		}
		return INT16_MAX;
	}  
	const char* GetCmdStr(commands value) {
		if (value >= 0 && value < STR_COUNT) {
			return cmdStrings[value];
		}
		return "Unknown";
	}  
	const char* GetErrStr(commands value) {
		if (value >= 0 && value < STR_COUNT) {
			return answers[value];
		}
		return "Unknown";
	}  
	uint GetTimeout(commands value) {
		if (value >= 0 && value < STR_COUNT) {
			return timeout[value];
		}
		return UINT32_MAX;
	}  

private:
};
 
class WFCmds
{
public:
	WFCmds() {}
	~WFCmds() {}
	struct CommandData {
		std::string command;
		int timeout;
		std::string expectedResponse;
	};
	
private:
};