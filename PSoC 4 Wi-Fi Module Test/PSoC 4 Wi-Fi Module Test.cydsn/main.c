/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <device.h>
#include <stdio.h>   				/* sprintf() */
#include "Data_and_Func.h"
int uart_test();
#define SSID "TD" 				        // ваш SSID
#define PASS "244466666" 			    // ваш пароль
#define Wait(x)							//	Just for better readability

int i=0;

CY_ISR_PROTO(WIFI_Reconnect);
CY_ISR(Reconnect)
	{ recon++; }
void InitializeSystem(void)
{
	CyGlobalIntEnable;
    UART_Start();
    USB_UART_Start();	
	Rx_Int_StartEx(GetChar);
}

//------------------------------------------------------------------------------------------------------
int main()
{ 
    //char Rxbyte[10]={0};
    //char data[32];
    InitializeSystem();


    
    //CyDelay(5000);
    UART_PutString("AT+CWMODE=1\r\n");   
	UART_PutString("AT+CIFSR\r\n");   
	while(!HaveAnswer)	Wait();			//	Wait for answer from device
	USB_UART_UartPutString("\n WELCOME\n\r");
	USB_UART_UartPutString(ATanswer);
	USB_UART_UartPutCRLF(' ');
	HaveAnswer = 0;		
	CyDelay(1000);
        


// ****************try to connect to wifi ********************************************
    for(i=0; i<3; i++)
    {
        USB_UART_UartPutString("Connecting...\n");
        if(connectWiFi(SSID, PASS))
        { 
            USB_UART_UartPutString("Wi-Fi connected!\n");
            break;
        } 
    }

    if (i==3) 
    {
        USB_UART_UartPutString("Wi-Fi not connected\n");
        CyDelay(3000); 
        CySoftwareReset();
    }	// сброс чипа
    //------------------------------------------------------------------------------------------------
    IP_to_USB();
    SetServer(8045);		// включаем сервер на порт  8046 ( или какой вам удобно)
    CyDelay(500);
    UART_ClearRxBuffer();	// начнем жизнь с нового листа ))

    for(;;)
    {    
    

    
    }    
    
   

//    while(1)
//    { 								
//        if (WaitText("GET", 500))						// ждем запрос GET от клиента
//    	{
//    	    CyDelay(50);								// "GET" получен, подождем пока закончится передача
//    	    webserver();								// отвечаем и закрываем соединение
//    	    recon=0;									// сброс счетчика профилактической перезагрузки
//    	}
//        if(recon==60) 	
//        {
//            recon=0; 
//            CySoftwareReset ();
//        }	// скучно ...  перезагрузимся для профилактики зависания ESP8266 )))
//    }

}
/* [] END OF FILE */
