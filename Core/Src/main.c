/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "CAN_Main.h"
#include "CAN_C620.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DISABLE_CAN_WAIT_CONNECT 0
#define DISABLE_C620_WAIT_CONNECT 0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma location=0x2007c000
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
#pragma location=0x2007c0a0
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

#elif defined ( __CC_ARM )  /* MDK ARM Compiler */

__attribute__((at(0x2007c000))) ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
__attribute__((at(0x2007c0a0))) ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

#elif defined ( __GNUC__ ) /* GNU Compiler */
ETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT] __attribute__((section(".RxDecripSection"))); /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT] __attribute__((section(".TxDecripSection")));   /* Ethernet Tx DMA Descriptors */

#endif

ETH_TxPacketConfig TxConfig;

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

ETH_HandleTypeDef heth;

UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_tx;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */
NUM_OF_DEVICES num_of_devices;

MCMD_HandleTypedef mcmd4_struct;

MCMD_Feedback_Typedef mcmd_fb;

uint8_t num_of_c620 = 1;

C620_DeviceInfo c620_dev_info_global[8];

C620_FeedbackData c620_fb[8];


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_ETH_Init(void);
static void MX_CAN1_Init(void);
static void MX_CAN2_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(uint8_t ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE {
    HAL_UART_Transmit(&huart3, &ch, 1, 500);
    return ch;
}



void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan){
    C620_WhenTxMailboxCompleteCallbackCalled(hcan);
    CANLib_WhenTxMailbox0_1_2CompleteCallbackCalled(hcan);
}

void HAL_CAN_TxMailbox0AbortCallback(CAN_HandleTypeDef *hcan){
    C620_WhenTxMailboxAbortCallbackCalled(hcan);
    CANLib_WhenTxMailbox0_1_2AbortCallbackCalled(hcan);
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan){
    C620_WhenTxMailboxCompleteCallbackCalled(hcan);
    CANLib_WhenTxMailbox0_1_2CompleteCallbackCalled(hcan);
}

void HAL_CAN_TxMailbox1AbortCallback(CAN_HandleTypeDef *hcan){
    C620_WhenTxMailboxAbortCallbackCalled(hcan);
    CANLib_WhenTxMailbox0_1_2AbortCallbackCalled(hcan);
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan){
    C620_WhenTxMailboxCompleteCallbackCalled(hcan);
    CANLib_WhenTxMailbox0_1_2CompleteCallbackCalled(hcan);
}

void HAL_CAN_TxMailbox2AbortCallback(CAN_HandleTypeDef *hcan){
    C620_WhenTxMailboxAbortCallbackCalled(hcan);
    CANLib_WhenTxMailbox0_1_2AbortCallbackCalled(hcan);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    WhenCANRxFifo0MsgPending(hcan, &num_of_devices);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan){
    C620_WhenCANRxFifo1MsgPending(hcan);
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
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART3_UART_Init();
  MX_ETH_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_USB_OTG_FS_PCD_Init();
  /* USER CODE BEGIN 2 */


  printf("Start Initializing CAN System:Begin\n\r");
  HAL_Delay(100);

  CAN_SystemInit(&hcan2); // F7のCAN通信のinit

   // デバイス数の設定 (今回はmcmd4が1枚)
  num_of_devices.mcmd3 = 1;
  num_of_devices.mcmd4 = 0;
  num_of_devices.air = 0;
  num_of_devices.servo = 0;

  printf("Start Initializing CAN System:End\n\r");
  HAL_Delay(100);
  if(!DISABLE_CAN_WAIT_CONNECT)CAN_WaitConnect(&num_of_devices);  // 設定された全てのCANモジュール基板との接続が確認できるまで待機

	   // ここからはCANモジュール基板の設定
	 // 接続先のMCMDの設定
	 mcmd4_struct.device.node_type = NODE_MCMD4;  // nodeのタイプ (NODE_MCMD3など)
	 mcmd4_struct.device.node_id = 2;  // 基板の番号 (基板上の半固定抵抗を回す事で設定できる)
	 mcmd4_struct.device.device_num = 0;  // モーターの番号(MCMDなら0と1の2つが選べる)

	 // 制御パラメータの設定
	 mcmd4_struct.ctrl_param.ctrl_type = MCMD_CTRL_POS;  // 位置制御を行う
	 mcmd4_struct.ctrl_param.PID_param.kp = 0.10f;  // Pゲイン 0.10
	 mcmd4_struct.ctrl_param.PID_param.ki = 0.0f;  // Iゲイン 0.0
	 mcmd4_struct.ctrl_param.PID_param.kd = 0.0f;  // Dゲイン 0.0 (Dゲインは使いにくい)
	 mcmd4_struct.ctrl_param.accel_limit = ACCEL_LIMIT_ENABLE;  // PIDの偏差をclipするか
	 mcmd4_struct.ctrl_param.accel_limit_size = 2.0f;  // PIDの偏差をclipする場合の絶対値のmax値
	 mcmd4_struct.ctrl_param.feedback = MCMD_FB_ENABLE;  // MCMDからF7にフィードバックを送信するか否か
	 mcmd4_struct.ctrl_param.timup_monitor = TIMUP_MONITOR_DISABLE;  // timeupは未実装なのでDISABLE。
	 mcmd4_struct.enc_dir = MCMD_DIR_FW;  // Encoderの回転方向設定
	 mcmd4_struct.rot_dir = MCMD_DIR_FW;  // モーターの回転方向設定
	 mcmd4_struct.quant_per_unit = 0.00205f;  // エンコーダーの分解能に対する制御値の変化量の割合

	 // 原点サーチの設定
	 mcmd4_struct.limit_sw_type = LIMIT_SW_NC;  // 原点サーチにNomaly Closedのスイッチを用いる
	 mcmd4_struct.calib = CALIBRATION_DISABLE;  // 原点サーチを行う。
	 mcmd4_struct.calib_duty = -0.1f;  // 原点サーチ時のduty
	 mcmd4_struct.offset = 0.0f;  // 原点のオフセット
	 mcmd4_struct.fb_type = MCMD_FB_POS;  // 読み取った位置情報をF7にフィードバックする。

	 // パラメータなどの設定と動作命令をMCMDに送信する
	MCMD_init(&mcmd4_struct);
	HAL_Delay(10);
	MCMD_Calib(&mcmd4_struct);  // キャリブレーションを行う
	HAL_Delay(2000);  // キャリブレーションが終わるまで待つ
	MCMD_SetTarget(&mcmd4_struct, 0.0f);  // 目標値(0.0)を設定
	HAL_Delay(10);
	MCMD_Control_Enable(&mcmd4_struct);  // 制御開始
	HAL_Delay(10);

   printf("Start Initializing CAN System for C620:Begin\n\r");
   HAL_Delay(100);

   CAN_SystemInit(&hcan1);

   printf("Start Initializing CAN System for C620:End\n\r");
   HAL_Delay(100);
   C620_WaitForConnect(c620_dev_info_global, num_of_c620);  // C620の接続待ち

   c620_dev_info_global[0].device_id = 1;  // 1スタートな事に注意
   c620_dev_info_global[0].ctrl_param.accel_limit = C620_ACCEL_LIMIT_ENABLE;
   c620_dev_info_global[0].ctrl_param.use_internal_offset = C620_USE_OFFSET_POS_CALIB;
   c620_dev_info_global[0].ctrl_param.use_internal_offset = C620_USE_OFFSET_POS_INTERNAL;
   c620_dev_info_global[0].ctrl_param.ctrl_type = C620_CTRL_VEL;
   c620_dev_info_global[0].ctrl_param.accel_limit_size = 15.0f;
   c620_dev_info_global[0].ctrl_param.quant_per_rot = 1.0f/19.0f / 3.0f * 3.141592f * 2.0f;  //M3508は19:1
   c620_dev_info_global[0].ctrl_param.rotation = C620_ROT_ACW;  // 半時計周り

   c620_dev_info_global[0].ctrl_param.pid_vel.kp = 4.5f;  // 位置制御の場合はpid_velに速度制御用のgainを設定する
   c620_dev_info_global[0].ctrl_param.pid_vel.ki = 0.0f;
   c620_dev_info_global[0].ctrl_param.pid_vel.kd = 0.0f;
   c620_dev_info_global[0].ctrl_param.pid_vel.kff = 0.0f;

   c620_dev_info_global[0].ctrl_param.pid.kp = 10.0f;  // 位置制御用
   c620_dev_info_global[0].ctrl_param.pid.kp = 12.3f;  // 位置制御用
   c620_dev_info_global[0].ctrl_param.pid.ki = 0.16f;
   c620_dev_info_global[0].ctrl_param.pid.kd = 0.0f;
   c620_dev_info_global[0].ctrl_param.pid.kff = 0.0f;

// TODO:Init,Calibrationの必要性
//   C620_Init(&c620_dev_info_global[0],num_of_c620);
//   HAL_Delay(10);
//   C620_Calibration(&c620_dev_info_global[0], -2.0f, SWITCH_NO, GPIOG, GPIO_PIN_1, &hcan1);  //
//   HAL_Delay(1000);
//   C620_SetTarget(&c620_dev_info_global[0], 1.0f);  // 目標値を設定
//   HAL_Delay(10);
//   C620_ControlEnable(&c620_dev_info_global[0]);

   for(int i=0; i<num_of_c620; i++)C620_SetTarget(&c620_dev_info_global[i], 0.0f);
   if(!DISABLE_C620_WAIT_CONNECT)C620_WaitForConnect(c620_dev_info_global, num_of_c620);
//    for(int i=0; i<num_of_c620; i++){
//        C620_Calibration(&c620_dev_info_global[i], -2.0f, SWITCH_NO, GPIOG, GPIO_PIN_1, &hcan1);
//        C620_ControlEnable(&(c620_dev_info_global[i]));
//    }
   for(int i=0; i<num_of_c620; i++)C620_ControlEnable(&(c620_dev_info_global[i]));

   C620_SetTarget(&c620_dev_info_global[0],1.0f);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  mcmd_fb = Get_MCMD_Feedback(&(mcmd4_struct.device));
	  printf("value of mcmd3 %d\r\n",(int)(mcmd_fb.value));
	  HAL_Delay(5);

	  for(int i=0;i<num_of_c620;i++){
		  c620_fb[i] = Get_C620_FeedbackData(&c620_dev_info_global[i]);
		  printf("value of c620[%d] %d\r\n",i,(int)(c620_fb[i].current));
		  HAL_Delay(5);
	  }
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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 6;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_6TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = ENABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief CAN2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN2_Init(void)
{

  /* USER CODE BEGIN CAN2_Init 0 */

  /* USER CODE END CAN2_Init 0 */

  /* USER CODE BEGIN CAN2_Init 1 */

  /* USER CODE END CAN2_Init 1 */
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 6;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_6TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = DISABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = DISABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = ENABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN2_Init 2 */

  /* USER CODE END CAN2_Init 2 */

}

/**
  * @brief ETH Initialization Function
  * @param None
  * @retval None
  */
static void MX_ETH_Init(void)
{

  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

   static uint8_t MACAddr[6];

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  heth.Instance = ETH;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
  heth.Init.TxDesc = DMATxDscrTab;
  heth.Init.RxDesc = DMARxDscrTab;
  heth.Init.RxBuffLen = 1524;

  /* USER CODE BEGIN MACADDRESS */

  /* USER CODE END MACADDRESS */

  if (HAL_ETH_Init(&heth) != HAL_OK)
  {
    Error_Handler();
  }

  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
  /* USER CODE BEGIN ETH_Init 2 */

  /* USER CODE END ETH_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

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

#ifdef  USE_FULL_ASSERT
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
