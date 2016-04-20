#include <stm32f0xx.h>
#include <stm32f0xx_gpio.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_tim.h>

#define BLed GPIO_Pin_8
#define GLed GPIO_Pin_9
#define Button GPIO_Pin_0

volatile uint32_t licz = 0, v = 10000, tmp = 0;

void GPIO(){
	GPIO_InitTypeDef G;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	G.GPIO_Mode = GPIO_Mode_OUT;
	G.GPIO_OType = GPIO_OType_PP;
	G.GPIO_Pin = (GLed | BLed);
	G.GPIO_PuPd = GPIO_PuPd_NOPULL;
	G.GPIO_Speed = GPIO_Speed_Level_1;

	GPIO_Init(GPIOC, &G);

	G.GPIO_Mode = GPIO_Mode_IN;
	G.GPIO_OType = GPIO_OType_PP;
	G.GPIO_Pin = Button;
	G.GPIO_PuPd = GPIO_PuPd_DOWN;
	G.GPIO_Speed = GPIO_Speed_Level_1;

	GPIO_Init(GPIOA, &G);
}

void funkcja(){
	licz++;
	if(licz>=v){
		licz=0;
	}
	if(licz>=(v/2)){
		GPIO_SetBits(GPIOC, GLed);
	}
	else {
		GPIO_ResetBits(GPIOC, GLed);
	}
	if(GPIO_ReadInputDataBit(GPIOA, Button)){
		if(tmp==0) {
			v = v/2;
		}
		tmp++;
		if(tmp>=1000){
			v = 10000;
			tmp = 0;
		}
	}
	else {
		tmp=0;
	}
}

void SysTick_Handler(void){
	funkcja();
}

int main(void)
{
	GPIO();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
    while(1)
    {
    }
}
