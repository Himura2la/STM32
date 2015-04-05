#include <inttypes.h>
#include <system_stm32f0xx.h>
#include <stm32f0xx_gpio.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_tim.h>

//void delay(volatile uint32_t number){ while(number--); }

void PortsInit(){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitTypeDef PAInit;
	PAInit.GPIO_Pin = GPIO_Pin_0;
	PAInit.GPIO_Mode = GPIO_Mode_OUT;
	PAInit.GPIO_Speed = GPIO_Speed_Level_1;
	PAInit.GPIO_OType = GPIO_OType_PP;
	PAInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &PAInit);
}

void TimerInit(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_TimeBaseInitTypeDef TInit;
	TInit.TIM_Prescaler = 8000;	//1мс
	TInit.TIM_CounterMode = TIM_CounterMode_Up;
	TInit.TIM_Period = 2000; 	//2c
	TInit.TIM_ClockDivision = TIM_CKD_DIV1;
	TInit.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TInit);
	TIM_Cmd(TIM1, ENABLE);
}

int main(void) {
	SystemInit();
	PortsInit();
	TimerInit();

    while(1) {
    	int timerValue = TIM_GetCounter(TIM1);
    	if (timerValue == 0)
    		GPIO_SetBits(GPIOA, GPIO_Pin_0);
    	else if (timerValue == 1000) //1c
    		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
    }
}
