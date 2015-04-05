/*
#include <inttypes.h>
#include <stdbool.h>
#include <system_stm32f0xx.h>
#include <stm32f0xx_gpio.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_tim.h>
#include <stm32f0xx_misc.h>

//void delay(volatile uint32_t number){ while(number--); }


#define PERIOD 3000

void PWMInit(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitTypeDef PWMInit;
	TIM_TimeBaseStructInit(&PWMInit);	//Load Default
		PWMInit.TIM_Period = PERIOD;
	TIM_TimeBaseInit(TIM3, &PWMInit);

	//Output Compare
	TIM_OCInitTypeDef PWMOCInit;
	TIM_OCStructInit(&PWMOCInit);	//Load Default
		PWMOCInit.TIM_OCMode = TIM_OCMode_PWM1;
		PWMOCInit.TIM_OutputState = TIM_OutputState_Enable;
		PWMOCInit.TIM_Pulse = 0;
	TIM_OC1Init(TIM3, &PWMOCInit);

	//Port:	PA6 == TIM3_CH1 (AF1) p34
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitTypeDef PAPWMInit;
		PAPWMInit.GPIO_Pin = GPIO_Pin_6;
		PAPWMInit.GPIO_Mode = GPIO_Mode_AF;
		PAPWMInit.GPIO_Speed = GPIO_Speed_Level_2;   //Побыстрее
		PAPWMInit.GPIO_OType = GPIO_OType_PP;
		PAPWMInit.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &PAPWMInit);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);

	TIM_Cmd(TIM3, ENABLE);
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
}


void TimerInit(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef TInit;
	TIM_TimeBaseStructInit(&TInit);
	TInit.TIM_Period = PERIOD;
	TIM_TimeBaseInit(TIM2, &TInit);

	TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // Включить прерывание при обновлении таймера TIM2

	//Прерывание
	NVIC_InitTypeDef nvic;
		nvic.NVIC_IRQChannel = TIM2_IRQn;
		nvic.NVIC_IRQChannelPriority = 0;
		nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

bool fade_up = true;
void TIM2_IRQHandler() {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){ //Обновление
		uint16_t pulse = TIM3->CCR1;
		if (fade_up) {
			if (pulse < PERIOD) TIM3->CCR1 = pulse + 1;
			else fade_up = false;
		}
		else {
			if (pulse > 0) TIM3->CCR1 = pulse - 1;
			else fade_up = true;
		}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}


int main(void) {
	SystemInit();
	TimerInit();
	PWMInit();

    while(1) {
    }
}
//*/
