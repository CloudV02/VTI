#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_i2c.h"
#include "misc.h"

void UART_Config(void);
void SPI1_Slave_Init(void);
void I2C_Config(void);

volatile uint8_t uart_data = 0;
volatile uint8_t uart_send_flag = 0;
volatile uint8_t uart_counter = 1;
volatile uint8_t even_value = 0;
volatile uint8_t i2c_counter = 0;
uint8_t txIndex = 0;
volatile uint8_t tx_data = 1;  
volatile uint8_t rx_byte = 0;
char uart_char = 0;

void TIM_Config(){
	TIM_TimeBaseInitTypeDef Tim_InitStruct;
	Tim_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	Tim_InitStruct.TIM_Prescaler = 36000 - 1;
	Tim_InitStruct.TIM_Period = 0xFFFF;
	Tim_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &Tim_InitStruct);
	TIM_Cmd(TIM2, ENABLE);
	
}

void USART_SendChar(USART_TypeDef *USARTx, uint8_t data){
	USARTx->DR = 0x00;
	USART_SendData(USARTx,data);
	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) != 1);
}

void delays(uint32_t time){
	TIM_SetCounter(TIM2,0);
	while(TIM_GetCounter(TIM2)<time * 1000);
}

int main(void) {
    UART_Config();
    SPI1_Slave_Init();
    I2C_Config();
		volatile uint32_t i = 0;
    while (1) {
			
    }
}


void UART_Config(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // PA9 - TX, PA10 - RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    USART_Cmd(USART1, ENABLE);


    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	}



void SPI1_Slave_Init(void) {
    // Clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);

    // GPIO config
    GPIO_InitTypeDef GPIO_InitStructure;

    // SCK (PA5) & MOSI (PA7) ? input floating
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // MISO (PA6) ? Alternate Function Push-Pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // NSS (PA4) ? input floating
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // SPI config
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;  
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI1, &SPI_InitStructure);

   
    SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);

    // B?t SPI
    SPI_Cmd(SPI1, ENABLE);

    
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    SPI_I2S_SendData(SPI1, tx_data);

    // Enable NVIC
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = SPI1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

void I2C_Config(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 100000;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_OwnAddress1 = 0x08 << 1;

    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF, ENABLE);
    I2C_Cmd(I2C1, ENABLE);
    I2C_AcknowledgeConfig(I2C1, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// ----- UART TX Handler -----
/*void USART1_IRQHandler(void) {
    // Khi UART s?n sàng truy?n (TXE = Transmit Data Register Empty)
    if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) {
        USART_SendData(USART1, uart_data++);
        if (uart_data > 98) uart_data = 0;

        // T?t TXE interrupt n?u ch? mu?n g?i 1 byte
        USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    }

  
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        uint8_t received = USART_ReceiveData(USART1);
       
    }
}*/

void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_TXE)) {
        USART_SendData(USART1, uart_data++);
        if (uart_data > 100) uart_data = 1;
        USART_ClearITPendingBit(USART1, USART_IT_TXE);
    }
    if (USART_GetITStatus(USART1, USART_IT_RXNE)) {
        volatile uint8_t temp = USART_ReceiveData(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}


void SPI1_IRQHandler(void) {
    if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE)) {
        uint8_t dummy = SPI_I2S_ReceiveData(SPI1);  
        tx_data += 2;                               
        SPI_I2S_SendData(SPI1, tx_data);            
    }
}

void I2C1_EV_IRQHandler(void) {
     if (I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED)) {
        txIndex = 0;
    }

    if (I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_BYTE_TRANSMITTING)) {
        I2C_SendData(I2C1, i2c_counter);
        i2c_counter += 1;
        if (i2c_counter > 98) i2c_counter = 0;
    }
}
