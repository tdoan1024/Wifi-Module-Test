#include <project.h>
#include <stdio.h>   				/* sprintf() */

//************* usually you put here a #include "Data_and_Func.h" ******
//************* but this will not work because you declared variables in the .h

volatile uint8 HaveAnswer = 0;		//	Indicator for a complete answer
char ATanswer[80];

void Reset1(void)
{ 
	RST_Write(0); CyDelay(5); RST_Write(1);
}

CY_ISR(GetChar)
{
static uint8 Index = 0;
uint8 Chr;
	UART_ReadRxStatus();			//	Clear interrupts
	Chr = UART_GetByte();
	ATanswer[Index] = 0;			//	String terminator
	if((Chr == '\n') || (Chr == '\r'))	//	End of string?
	{
		Index = 0;					//	Start anew
		HaveAnswer = 1;				///	Set flag
	}
	else 
	{
		ATanswer[Index++] = Chr;	//	Put char into buffer
	}
}

uint16  WaitText(char *target, int time) 	/******************************************************/			 
 {
uint8 targetLen = strlen(target); 								// Длина искомого фрагмента
uint8 index = 0;  												// maximum target string length is 255 bytes
char c;															// текущий символ
while(UART_GetRxBufferSize())
//do
{ 							// читаем UART  
	  c = UART_ReadRxData();										// это элемент[index] искомой строки?  
      if( c == target[index]){  if(++index >= targetLen){return 1; }}	// return true если все элементы строки найдены    if all chars in the target match
      else  index = 0;   //..........................     				// не тот фрагмент - обнуляемся 
	//  if( !UART_GetRxBufferSize()){CyDelay(1); time--;}	// при проверке с==0 - вычитывает несколько раз, поэтому ReadRxData и GetRxBufferSize
	} //while(time);
return 0;	// буфер данных пуст
} 

//***********************************************************************
int connectWiFi(char *SSID, char *PASS)
{
char sendString[32];
UART_ClearRxBuffer(); 
sprintf(sendString,"AT+CWJAP=\"%s\",\"%s\"\r\n",SSID, PASS);	// формируем строку "AT+CWJAP="SSID","PASS"\r\n 
UART_PutString(sendString); 									// отправляем ее в модуль
if (!WaitText("OK", 100))return 0;	//0-для  прошивки 00160901	// команда получена, "OK"  но это не подтверждает подключение для прошивки   00160901	
CyDelay(5000);		 // 	для  прошивки 00160901			   	   даем время на подключение
UART_PutString("AT+CIFSR\r\n"); 	//для  прошивки 00160901	// Получить IP через 5 сек это будет подтверждением подключения
if(WaitText("ERROR", 500))return 0;	// для  прошивки 00160901	// попытка не удалась.....
return 1;  //1-для  прошивки 00160901														// признак успешного подключения к WIFI
}

//***********************  создание  сервра  *****************************************
void SetServer(int port)
{
char sendString[32];
UART_PutString("AT+CIPMUX=1\r\n"); 								// Количество соединений  (1 — мультисоединение (до 4-х))
//Test_Write(WaitText("OK", 500));  Test_Write(0);				// ждем ответа модуля
sprintf(sendString,"AT+CIPSERVER=1,%d\r\n",port);				// формируем строку "AT+CWJAP="SSID","PASS"\r\n 
UART_PutString(sendString); 									// Поднять сервер 0 — скрыт 1 — открытый <port> — порт
//Test_Write(WaitText("OK", 500));  Test_Write(0);
UART_PutString("AT+CIPSTO=20\r\n"); 							// сколько секунд клиент может ждать ответ
//Test_Write(WaitText("OK", 500));  Test_Write(0);
}

//***********************  отправка  ответа  ******************************************
void http(char *output) 		 
{
 char sendString[32];
 uint16 len=0;
 len=strlen(output)	;	
 sprintf(sendString,"AT+CIPSEND=0,%d\r\n",len);		//  
 UART_PutString(sendString); CyDelay(10);			// Для простоты: вместо ожидания символа '>' просто выдержим паузу
 UART_PutString(output);  
}

void webserver(void) {/************************************************************************************************/
 static uint16 test;		// счетчик автообновлений странички     // HTML-Шпаргалка:        	http://ruseller.com/htmlshpora.php?id=52
 char sendString[1000];	// не менее длины сообщения					// таблица основных тегов 	http://ru.html.net/tutorials/html/lesson8.php
																	// Основы HTML             	http://html-exp.narod.ru/base.htm 
 sprintf(sendString,	// Собираем ответ на запрос:				  
	"HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n\r\n"	// согласно стандарту отвечаем  HTTP заголовком  HTTP /1.0 200 OK  Content-Type: text/html  И пустой строкой
 	"<html><body>"													// затем открываем документ,  и не забыть закрыть:   </body></html>   
	"<title>ESP8266 Webserver</title>"								// заголовок 
	"<meta http-equiv='refresh' content='5' >" 						// To make the page reload or refresh itself (sec)
	"<body bgcolor=PaleGoldenRod>"									// зададим цвет фона документа именем   	http://www.w3schools.com/tags/ref_colornames.asp
	"<H3>Micro Site Web Server</H3>"								// заголовок
	"<span style='color:#0000FF'>\r\n"								// задаем цвет текста кодом	  	http://www.w3schools.com/tags/ref_colorpicker.asp
 	"<p>reloads = %d</p>\r\n"                                       // счетчик автообновлений
	"</body></html>" 	,test); 									// CLOSE PAGE  
								// 
	test++;															// 
http(sendString);													// страница готова, отправляем
CyDelay(1000);
UART_PutString("AT+CIPCLOSE=0\r\n");   	
}


/*********************************************************************************************************/
void IP_to_USB(void)
{
char8 ch;       					// Data received from the Serial port  
char buff[20];						// buffer of RX	
uint8 count = 0u;					//__________________
//UART_PutString("AT+CWLAP\r\n");
//UART_PutString("AT+CIFSR\r\n"); 	// Получить IP еще раз для отображения на дисплее
CyDelay(2);							// модулю на ответ нужно время	
WaitText("AT+CIFSR\r\r\n", 500);	// фрагмент ответа с IP адресом останется в буфере UART для функции GetIP() 
WaitText("AT+CWLAP\r\r\n", 500);
do{ 	// в буфере остался IP адрес
	  	ch= UART_GetChar();				// вычитываем символ
	  	buff[count]= ch;	count++;	// добавляем в буфер
	}while(ch); 
USB_UART_UartPutString("IP:");
USB_UART_UartPutString(buff); ;	// выведем IP на дисплей
}
