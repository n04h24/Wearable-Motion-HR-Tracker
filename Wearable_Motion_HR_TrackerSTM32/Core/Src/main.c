/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <string.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define MPU6050 0x68
#define ACCEL_CONFIG 0x1C
#define PWR_MGMT_1 0x6B
#define VAR_NAME(var) #var

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// Stage increment 2

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
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */

  	  // UART Initialisation

  //Checking Refactor
  char buffer[32];
  // Creating buffer of size 16 (bytes)
  const char messageA[] = "UART connection OK!\n";
  // Immutable string of <16 char including \0

  strcpy(buffer, messageA);
  //Copy into source, char

  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), HAL_MAX_DELAY);
  //Confirm UART is OK

  	  //MPU6050 I2C Connection


  HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(&hi2c1, MPU6050 << 1, 1, 100);
  //Retrieve status of HAL function

  if (status == HAL_OK) {
	  strcpy(buffer, "MPU6050 connected\n");
	  //Copy success message to buffer;
	  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), HAL_MAX_DELAY);

  }
  else {
	  strcpy(buffer, "MPU6050 failed to connect...\n");
	  //Copy error message to buffer;
	  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), HAL_MAX_DELAY);
	  //Transmit over UART
  }

  	  //MPU6050 I2C Configuration


  uint8_t write_memory = 0x00;
  //Set all bits to 0
  uint8_t read_memory;
  status = HAL_I2C_Mem_Write(&hi2c1, MPU6050 << 1, PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &write_memory, 1, 100);
  //Wake device

  if (status == HAL_OK) {
	  strcpy(buffer, "Waking device...\n");
	  //Success message (PWR_MGMT_1)
	  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), HAL_MAX_DELAY);
	  //Transmit over UART
	  status = HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &read_memory, 1, 100);
	  	  //Read memory and store in 'address of' 8-bit variable
	  snprintf(buffer, sizeof(buffer), "PWR_MGMT_1 = 0x%02X\n", read_memory);
		  //Copy a formatted string containing hex value of config_mem (stored I2C memory read)
	  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), HAL_MAX_DELAY);

  }
  else {
	  strcpy(buffer, "Cannot wake device.\n");
	  //Error message (PWR_MGMT_1)
	  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), HAL_MAX_DELAY);
	  //Transmit over UART
  }


  write_memory = 0xF0;
  //Set to 0b11110000

  snprintf(buffer, sizeof(buffer), "Unpacking %s...\n", VAR_NAME(ACCEL_CONFIG));

  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), HAL_MAX_DELAY);

  status = HAL_I2C_Mem_Write(&hi2c1, MPU6050 << 1, ACCEL_CONFIG, I2C_MEMADD_SIZE_8BIT, &write_memory, 1, 100);
  //Configure XYZ accelerometer self-test (±8g recommended range)

  if (status == HAL_OK) {
	  strcpy(buffer, "Self-testing accelerometer...\n");
	  //Copy success message to buffer;
	  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), HAL_MAX_DELAY);
	  //Transmit over UART

	  status = HAL_I2C_Mem_Read(&hi2c1, MPU6050 << 1, ACCEL_CONFIG, I2C_MEMADD_SIZE_8BIT, &read_memory, 1, 100);
	  //Read memory and store in 'address of' 8-bit variable
	  if (status == HAL_OK){
		  snprintf(buffer, sizeof(buffer), "ACCEL_CONFIG = 0x%02X\n", read_memory);
		  	  //Copy a formatted string containing hex value of config_mem (stored I2C memory read)
		  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), HAL_MAX_DELAY);
		  //Compare current memory hex to intended 0xF0
	  }
	  else {
		  strcpy(buffer, "Cannot read self-test memory\n");
		  //Copy failed to read message
		  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), HAL_MAX_DELAY);
		 //Transmit over UART
	  }

  }
  else {
	  strcpy(buffer, "Cannot self-test accelerometer\n");
	  //Copy error message to buffer
	  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen((char*)buffer), HAL_MAX_DELAY);
	  //Transmit over UART
  }

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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00503D58;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
