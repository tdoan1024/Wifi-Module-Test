#include <project.h>
#include <stdio.h>   /* sprintf() */

//********************* DANGER ****************************
//*** Do not define anything in a .h file, ONLY declare data and functions *****
uint8 Com_Data;         	// data in RX
uint8 recon;
char8 ch;       			//Data received from the Serial port  
uint8 count_RX=0;			// counter bytes of RX
//*************************************************************
#ifndef Data_and_Func_H				//	This is the typical .h protection scheme
#define Data_and_Func_H
	
extern uint8 HaveAnswer;			//	Indicator for complete answer to an AT-command
extern char ATanswer[80];	//	Receive string


//-----------------------------------------------------------------------------------
void Reset1(void);	
int connectWiFi(char *SSID, char *PASS);	
void http(char *output); 	
void webserver(void);	
uint16  WaitText(char *target, int time);
void IP_to_USB(void);
void SetServer(int port);
CY_ISR_PROTO(GetChar);		//	Interrupt handler to get character(s) from UART

#endif


