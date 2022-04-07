/****************************************
JinMing electronics
https://www.ebay.com/itm/223274591029
****************************************/

#include "USART.h"
#include "stm32f4xx_hal.h"
#include "MyType.h"


u8 UsartBuff[SEND_BUF_SIZE];//Uart transfer buffer

UART_HandleTypeDef UART2_Handler; //UART Handle
DMA_HandleTypeDef  UART2TxDMA_Handler;      //DMA Handle

//UART2 init
//bound:BaudRate
void uart_init(u32 bound)
{	


	UART2_Handler.Instance = USART2;
	UART2_Handler.Init.BaudRate = 115200;
	UART2_Handler.Init.WordLength = UART_WORDLENGTH_8B;
	UART2_Handler.Init.StopBits = UART_STOPBITS_1;
	UART2_Handler.Init.Parity = UART_PARITY_NONE;
	UART2_Handler.Init.Mode = UART_MODE_TX_RX;
	UART2_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART2_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	  if (HAL_UART_Init(&UART2_Handler) != HAL_OK)
	  {

	  }
	//MYDMA_Config(DMA2_Stream7,DMA_CHANNEL_4);//Init DMA
	//HAL_DMA_Start(&UART2TxDMA_Handler,(u32)UsartBuff,(u32)&USART2->DR,SEND_BUF_SIZE);
}


//DMA_Streamx:DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA channel choice,@ref DMA_channel DMA_CHANNEL_0~DMA_CHANNEL_7
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx)
{ 
    __HAL_RCC_DMA2_CLK_ENABLE();//DMA1 clock enable
  
    __HAL_LINKDMA(&UART2_Handler,hdmatx,UART2TxDMA_Handler);
    
    //Tx DMA
    UART2TxDMA_Handler.Instance=DMA_Streamx;
    UART2TxDMA_Handler.Init.Channel=chx;
    UART2TxDMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;
    UART2TxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;
    UART2TxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;
    UART2TxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;
    UART2TxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;
    UART2TxDMA_Handler.Init.Mode=DMA_CIRCULAR;
    UART2TxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;
    UART2TxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;
    UART2TxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;
    UART2TxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;
    UART2TxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;
    
    HAL_DMA_DeInit(&UART2TxDMA_Handler);
		
    HAL_DMA_Init(&UART2TxDMA_Handler);
} 

