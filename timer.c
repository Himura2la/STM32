/*

#include <inttypes.h>
#include <system_stm32f0xx.h>
#include <stm32f0xx_gpio.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_tim.h>
#include <stm32f0xx_misc.h>

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

void TIntInit(void);

void TimerInit(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef TInit;
	TInit.TIM_Prescaler = 8000;	//1мс
	TInit.TIM_CounterMode = TIM_CounterMode_Up;
	TInit.TIM_Period = 1000; 	//1c
	TInit.TIM_ClockDivision = TIM_CKD_DIV1;
	TInit.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TInit);
	TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // Включить прерывание при обновлении таймера TIM2
	TIntInit();

	//Output Compare 1
	TIM_OCInitTypeDef OC1Init;
	OC1Init.TIM_OCMode = TIM_OCMode_Timing;
	OC1Init.TIM_OutputState = TIM_OutputState_Enable;
	OC1Init.TIM_OCNPolarity = TIM_OCPolarity_High;
	OC1Init.TIM_Pulse = 200;	//Регистр совпадения
	TIM_OC1Init(TIM2, &OC1Init);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);	// Включить прерывание при совпадении

}

void TIntInit(){
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = TIM2_IRQn;
	nvic.NVIC_IRQChannelPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}


void TIM2_IRQHandler() {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){ //Обновление
		GPIO_SetBits(GPIOA, GPIO_Pin_0);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET){	//Совпадение
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	}
}

int main(void) {
	SystemInit();
	PortsInit();
	TimerInit();

    while(1) {
    }
}
//*/