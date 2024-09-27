#pragma once

	 
#define ANSWER_OK "OKr/n/"
#define ANSWER_ERR "ERRORr/n/"
#define ANSWER_BUSY "busy p...r/n/"
class Commands
{
public:
	Commands() {};
	~Commands() {};
	
	enum commands
	{
		cmdGetOK,
		cmdGetRst,
		cmdGetVer, 
		cmdGetMode,
		cmdGetAPlist,
		cmdGetBaud,
		cmdGetUartCur,
		
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

	const char* answer[50] = { 
		ANSWER_OK,
		//	cmdGetOK,	"AT"
		"\r\nready\r\n",
		//	cmdGetRst,	"AT+RST" 
		ANSWER_OK,
		//	cmdGetVer,	"AT+GMR"
		ANSWER_OK,
		//	cmdGetMode,	"AT+CWMODE=?"
		ANSWER_OK,
		//	cmdGetAPlist,	"AT+CWLAP"
		ANSWER_OK,
		//	cmdGetBaud,	"AT+CIOBAUD?"
		ANSWER_OK,
		//	cmdGetUartCur,	"AT+UART_CUR?"
		   
		"\r\nready\r\n",
		//	cmdSetFact,	"AT+RST" 
		ANSWER_OK,
		//	cmdSetModeSta, "AT+CWMODE=1" 
		ANSWER_OK,
		//	cmdSetModeSta, "AT+CWMODE=2"
		ANSWER_OK,
		//	cmdSetModeSta, "AT+CWMODE=3"
		ANSWER_OK,
		//	cmdSetUartCur, "AT+UART_CUR="
	 
	};
	
	const char* enumCmd[50] = {
		"AT",
		"AT+RST",
		"AT+GMR",
		"AT+CWMODE?",
		"AT+CWLAP",
		"AT+CIOBAUD?",
		//ERROR
		
		"AT+UART_CUR?",
		//+UART_CUR:<baudrate>,<databits>,<stopbits>,<parity>,<flow control>
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
		"AT+UART_CUR=230400,8,1,0,1",
		//+UART_CUR:<baudrate>,<databits>,<stopbits>,<parity>,<flow control>
		
	};
private:
};