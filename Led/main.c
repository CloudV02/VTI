#include <stm32f10x.h>
#define PIN13					(13u)
#define PIN13_MASK 		(~((0xF)<<20))
#define PIN13_INIT		(3u<<20u)
#define ON_PIN13			(1u<<PIN13)
#define OFF_PIN13			(~(1u<<PIN13))	
void RCC_Init(void);
void GPIO_Init(void);
void Blynk_Led(void);
void delay(int time);

int main(){
	RCC_Init();
	GPIO_Init();
	while(1){
		Blynk_Led();
	}
}

void RCC_Init(){
	/*Enable Clock Port C*/
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	
	/*Enable Clock TIM Base*/
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
}

void GPIO_Init(){
	/* bit 0011 pos 20*/
	GPIOC->CRH &= PIN13_MASK;
	GPIOC->CRH |= PIN13_INIT;
}
void Blynk_Led(){
	GPIOC->ODR |= ON_PIN13;
	delay(5000000);
	GPIOC->ODR &= OFF_PIN13;
	delay(5000000);
}

void delay(int time){
	while(time--);
}
