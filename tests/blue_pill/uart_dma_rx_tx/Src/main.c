/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <microsila_ll/core/utils.h>
#include <microsila_ll/core/byte_buf.h>
#include <microsila_ll/periph/uarts.h>
#include "user/leds.h"
#include "test/uart_test.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
// One TIM2 tick is set to 25 microseconds, that is 40 ticks per millisecond
#define MILLIS_TO_SYSTICKS(_TICKS) (_TICKS * 40)

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint32_t systicks = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
static void _main_routine(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void _enable_timer(void) {
      LL_TIM_EnableCounter(TIM2);
      LL_TIM_EnableIT_UPDATE(TIM2);
    }
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/
  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),10, 0));

  /** NOJTAG: JTAG-DP Disabled and SW-DP Enabled 
  */
  LL_GPIO_AF_Remap_SWJ_NOJTAG();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();

  /* USER CODE BEGIN 2 */
	uart1_init(9600, UartTest_OnUart1DataReceived, 3, 3, 3);
	uart2_init(9600, UartTest_OnUart2DataReceived, 3, 3, 3);
	uart3_init(9600, UartTest_OnUart3DataReceived, 3, 3, 3);
	_enable_timer();

	_main_routine();
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

   if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
  {
    Error_Handler();  
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {
    
  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {
    
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  
  }
  LL_Init1msTick(72000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(72000000);
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  /* TIM2 interrupt Init */
  NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(TIM2_IRQn);

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  TIM_InitStruct.Prescaler = 35;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 49;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM2, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM2);
  LL_TIM_SetClockSource(TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM2);
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
}


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_SetOutputPin(INFO_LED_GPIO_Port, INFO_LED_Pin);

  /**/
  GPIO_InitStruct.Pin = INFO_LED_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(INFO_LED_GPIO_Port, &GPIO_InitStruct);

}


/* USER CODE BEGIN 4 */

void delay(const uint32_t millis) {
	uint32_t target_ts = systicks + MILLIS_TO_SYSTICKS(millis);
	while (systicks < target_ts) { }
}


static void visualize_result(OP_RESULT result) {
	switch (result) {
		case OPR_OK:
			led_signal_success();
		  break;
		case OPR_TIMEOUT:
			led_signal_timeout();
		  break;
		default:
			led_signal_error();
	}
}

static void halt_if_error(OP_RESULT result) {
	if (result == OPR_OK) { return; }
	visualize_result(result);
	while (TRUE) { }
}

// If uarts are interconnected with each other, 
// they must be disabled together, and then together re-enabled in order to reset properly
static void reset_uarts(void) {
	/* The preferred way */
	uart1_disable();
	uart2_disable();
	uart3_disable();

	uart1_enable();
	uart2_enable();
	uart3_enable();
}

static void set_baud(uint32_t baud) {
	
	uart1_set_baud(baud);
	uart2_set_baud(baud);
	uart3_set_baud(baud);
	delay(4);
	reset_uarts();
}


static void _main_routine(void) {
	
	led_signal_disable();
	
	uart1_enable();
	uart2_enable();
	uart3_enable();
	
	halt_if_error(UartTest_Init());
	
	visualize_result(OPR_OK);
	delay(2000);
	led_signal_disable();

	
	uart1_tx_str("1Ftest junk 1 dfgdgdsfgdfgh df ...\n");
	uart2_tx_str("2Stest junk 2 erer eryh erhy erh ...\n");
	uart3_tx_str("3Ttest junk 3 serh ehj dfgh dfhdzfhdfh...\n");
	
	// Reset function stops ongoing transfers and clears all buffers,
	// so that previous operation does not affect next one;
	
	set_baud(9600);
	visualize_result(UartTest_RunOne(UART_TEST_TRIPLE_TX_RX));
	delay(2000);
	led_signal_disable();
	delay(1000);
	
	// test at different baud rates
	set_baud(115200);
	visualize_result(UartTest_RunOne(UART_TEST_TRIPLE_TX_RX));
	delay(2000);
	led_signal_disable();
	delay(1000);
	

	set_baud(921600);
	visualize_result(UartTest_RunOne(UART_TEST_TRIPLE_TX_RX));
	delay(2000);
	led_signal_disable();
	delay(1000);
	
	set_baud(2100000);
	visualize_result(UartTest_RunOne(UART_TEST_TRIPLE_TX_RX));

	while (TRUE) { }
	
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


