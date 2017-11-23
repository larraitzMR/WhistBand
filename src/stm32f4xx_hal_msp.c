/*
 * stm32f4xx_hal_msp.c
 *
 *  Created on: 14 nov. 2017
 *      Author: Larraitz
 */

#include "stm32f4xx_hal.h"

DMA_HandleTypeDef hdma_usart6_rx;
DMA_HandleTypeDef hdma_usart6_tx;


/**
 * Initializes the Global MSP.
 */

void HAL_MspInit(void) {
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	/* System interrupt init*/
	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart) {
	GPIO_InitTypeDef GPIO_InitStruct;
	  if(huart->Instance==USART2)
	  {
	    /* Peripheral clock enable */
	    __USART2_CLK_ENABLE();

	    /**USART2 GPIO Configuration
	    PA2     ------> USART2_TX
	    PA3     ------> USART2_RX
	    */
	    GPIO_InitStruct.Pin = GPIO_PIN_2;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull = GPIO_NOPULL;
	    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
	    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	    GPIO_InitStruct.Pin = GPIO_PIN_3;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  }
	  else if(huart->Instance==USART6) {
		/* Peripheral clock enable */
		__USART6_CLK_ENABLE();

		/**USART6 GPIO Configuration
		 PC6     ------> USART6_TX
		 PC7     ------> USART6_RX
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;//GPIO_PULLUP?
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	    /* Peripheral DMA init*/

	    hdma_usart6_rx.Instance = DMA2_Stream2;
	    hdma_usart6_rx.Init.Channel = DMA_CHANNEL_5;
	    hdma_usart6_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	    hdma_usart6_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	    hdma_usart6_rx.Init.MemInc = DMA_MINC_ENABLE;
	    hdma_usart6_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	    hdma_usart6_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	    hdma_usart6_rx.Init.Mode = DMA_CIRCULAR;
	    hdma_usart6_rx.Init.Priority = DMA_PRIORITY_LOW;
	    hdma_usart6_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	    HAL_DMA_Init(&hdma_usart6_rx);

	    __HAL_LINKDMA(huart,hdmarx,hdma_usart6_rx);

	    hdma_usart6_tx.Instance = DMA2_Stream6;
	    hdma_usart6_tx.Init.Channel = DMA_CHANNEL_5;
	    hdma_usart6_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	    hdma_usart6_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	    hdma_usart6_tx.Init.MemInc = DMA_MINC_ENABLE;
	    hdma_usart6_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	    hdma_usart6_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	    hdma_usart6_tx.Init.Mode = DMA_CIRCULAR;
	    hdma_usart6_tx.Init.Priority = DMA_PRIORITY_LOW;
	    hdma_usart6_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	    HAL_DMA_Init(&hdma_usart6_tx);

	    __HAL_LINKDMA(huart,hdmatx,hdma_usart6_tx);

//		/* Peripheral interrupt init*/
//		HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);
//		HAL_NVIC_EnableIRQ(USART6_IRQn);
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

  if(huart->Instance==USART6)
  {
    /* Peripheral clock disable */
    __USART6_CLK_DISABLE();

    /**USART6 GPIO Configuration
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6|GPIO_PIN_7);

    /* Peripheral DMA DeInit*/
    HAL_DMA_DeInit(huart->hdmarx);
    HAL_DMA_DeInit(huart->hdmatx);

    /* Peripheral interrupt DeInit*/
    HAL_NVIC_DisableIRQ(USART6_IRQn);

  }

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi) {
	GPIO_InitTypeDef GPIO_InitStruct;
	if (hspi->Instance == SPI1) {
		/* Peripheral clock enable */
		__SPI1_CLK_ENABLE();

		/**SPI1 GPIO Configuration
		 PA5     ------> SPI1_SCK
		 PA6     ------> SPI1_MISO
		 PA7     ------> SPI1_MOSI
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi) {
	if (hspi->Instance == SPI1) {
		/* Peripheral clock disable */
		__SPI1_CLK_DISABLE();

		/**SPI1 GPIO Configuration
		 PA5     ------> SPI1_SCK
		 PA6     ------> SPI1_MISO
		 PA7     ------> SPI1_MOSI
		 */
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	}
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c) {
	GPIO_InitTypeDef GPIO_InitStruct;
	if (hi2c->Instance == I2C1) {
		/**I2C1 GPIO Configuration
		 PB6     ------> I2C1_SCL
		 PB7     ------> I2C1_SDA
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* Peripheral clock enable */
		__I2C1_CLK_ENABLE();
	}
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c) {
	if (hi2c->Instance == I2C1) {
		/* Peripheral clock disable */
		__I2C1_CLK_DISABLE();

		/**I2C1 GPIO Configuration
		 PB6     ------> I2C1_SCL
		 PB7     ------> I2C1_SDA
		 */
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6 | GPIO_PIN_7);
	}
}

void HAL_WWDG_MspInit(WWDG_HandleTypeDef* hwwdg) {
	if (hwwdg->Instance == WWDG) {
		/* Peripheral clock enable */
		__WWDG_CLK_ENABLE();
	}

}

void HAL_WWDG_MspDeInit(WWDG_HandleTypeDef* hwwdg) {
	if (hwwdg->Instance == WWDG) {
		/* Peripheral clock disable */
		__WWDG_CLK_DISABLE();
	}
}
