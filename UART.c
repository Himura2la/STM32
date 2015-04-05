//*
#include <inttypes.h>
#include <stdbool.h>
#include <stm32f0xx_gpio.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_usart.h>
#include <stm32f0xx_misc.h>
#include <string.h>

void delay(volatile uint32_t number){ while(number--); }

void USARTInit(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    USART_InitTypeDef USART_InitStructure;
		USART_InitStructure.USART_BaudRate = 9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);	//TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);	//RX

    USART_Cmd(USART1, ENABLE);

    //Прерывание
	NVIC_InitTypeDef nvic;
		nvic.NVIC_IRQChannel = USART1_IRQn;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		nvic.NVIC_IRQChannelPriority = 0;
	NVIC_Init(&nvic);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

char USARTData[80];
uint8_t USARTCount = 0;
bool USARTGot = false;

void USART1_IRQHandler(){
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		uint8_t USARTByte = USART_ReceiveData(USART1);

		if (USARTByte != '\r'){
			USARTData[USARTCount++] = USARTByte;
			USART_SendData(USART1, USARTByte);
		}
		else
			USARTGot = true;
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

void sendChar(char d){
	USART_SendData(USART1, (uint8_t)d);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
void send(char *text){	while(*text) sendChar(*text++); }
void sendN(char *text, uint8_t N){
	uint8_t count = N;
	while(count--) sendChar(*text++);
}

void CRLF(){sendChar(13);sendChar(10);}
void USARTReset(){
	USARTGot = false;
	USARTCount = 0;
}


int main(void) {
	SystemInit();
	USARTInit();

	CRLF();send("=== STM32F030F4 is initialized and ready to accept instructions ===");
	CRLF();send("<-");
    while(1) {
    	if (USARTGot){
    		CRLF();send("->");
    		if (!memcmp(USARTData, "hello", USARTCount)){
    			send("Hello! How are you?");
    		}
    		else if (!memcmp(USARTData, "fine", USARTCount) || !memcmp(USARTData, "good", USARTCount)){
        		send("Glad to hear, me too!");
    		}
    		else {
        		send("I don't know what '");
        		sendN(USARTData, USARTCount);
        		send("' means... Try to say 'hello'.");
    		}
    		USARTReset();
    		CRLF();send("<-");
    	}
    }
}
//*/
