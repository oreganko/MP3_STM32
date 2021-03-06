
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * This notice applies to any and all portions of this file
 * that are not between comment pairs USER CODE BEGIN and
 * USER CODE END. Other portions of this file, whether 
 * inserted by the user or by software development tools
 * are owned by their respective copyright owners.
 *
 * Copyright (c) 2018 STMicroelectronics International N.V. 
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of other 
 *    contributors to this software may be used to endorse or promote products 
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this 
 *    software, must execute solely and exclusively on microcontroller or
 *    microprocessor devices manufactured by or for STMicroelectronics.
 * 5. Redistribution and use of this software other than as permitted under 
 *    this license is void and will automatically terminate your rights under 
 *    this license. 
 *
 * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
 * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
        #include "stm32f7xx_hal.h"
        #include "cmsis_os.h"
        #include "fatfs.h"
        #include "lwip.h"
        #include "usb_host.h"
		#include "mp3dec.h"

        /* USER CODE BEGIN Includes */
        #include  <errno.h>
        #include  <sys/unistd.h>

        #include "stm32746g_discovery_lcd.h"
        #include "Utilities/Fonts/fonts.h"
        #include "stm32746g_discovery_ts.h"
        #include "stm32746g_discovery_audio.h"
        #include "term_io.h"
        #include "dbgu.h"
        #include "ansi.h"

        #include "FreeRTOS.h"
        #include "task.h"
        #include "queue.h"

        #include "lwip/opt.h"
        #include "lwip/api.h"
        #include "lwip/apps/fs.h"
        #include "lwip/dhcp.h"
        #include "lwip/tcpip.h"
        #include "lwip/netdb.h"
        #include "lwip/sockets.h"
		#include "ff.h"
        #include "lwip.h"

        #include "wm8994/wm8994.h"




        /* USER CODE END Includes */

        /* Private variables ---------------------------------------------------------*/
        ADC_HandleTypeDef hadc3;

        CRC_HandleTypeDef hcrc;

        DCMI_HandleTypeDef hdcmi;

        DMA2D_HandleTypeDef hdma2d;

        I2C_HandleTypeDef hi2c1;
        I2C_HandleTypeDef hi2c3;

        LTDC_HandleTypeDef hltdc;

        QSPI_HandleTypeDef hqspi;

        RTC_HandleTypeDef hrtc;

        SAI_HandleTypeDef hsai_BlockA2;
        SAI_HandleTypeDef hsai_BlockB2;

        SD_HandleTypeDef hsd1;

        SPDIFRX_HandleTypeDef hspdif;

        SPI_HandleTypeDef hspi2;

        TIM_HandleTypeDef htim1;
        TIM_HandleTypeDef htim2;
        TIM_HandleTypeDef htim3;
        TIM_HandleTypeDef htim5;
        TIM_HandleTypeDef htim7;
        TIM_HandleTypeDef htim8;
        TIM_HandleTypeDef htim12;

        UART_HandleTypeDef huart1;
        UART_HandleTypeDef huart6;

        SDRAM_HandleTypeDef hsdram1;

        osThreadId defaultTaskHandle;

        /* USER CODE BEGIN PV */
        /* Private variables ---------------------------------------------------------*/

        /* USER CODE END PV */

        /* Private function prototypes -----------------------------------------------*/
        void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC3_Init(void);
static void MX_CRC_Init(void);
static void MX_DCMI_Init(void);
static void MX_DMA2D_Init(void);
static void MX_FMC_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C3_Init(void);
static void MX_LTDC_Init(void);
static void MX_QUADSPI_Init(void);
static void MX_RTC_Init(void);
static void MX_SAI2_Init(void);
static void MX_SDMMC1_SD_Init(void);
static void MX_SPDIFRX_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM5_Init(void);
static void MX_TIM8_Init(void);
static void MX_TIM12_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_TIM7_Init(void);
        void StartDefaultTask(void const * argument);

        void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);





        /* USER CODE BEGIN PFP */
        /* Private function prototypes -----------------------------------------------*/

        /* USER CODE END PFP */

        /* USER CODE BEGIN 0 */
        void mainTask(void* p);

        osThreadId netconn_thread_handle;

        #define LCD_X_SIZE      RK043FN48H_WIDTH
        #define LCD_Y_SIZE      RK043FN48H_HEIGHT
static uint32_t lcd_image_fg[LCD_Y_SIZE][LCD_X_SIZE] __attribute__((section(".sdram"))) __attribute__((unused));
static uint32_t lcd_image_bg[LCD_Y_SIZE][LCD_X_SIZE] __attribute__((section(".sdram"))) __attribute__((unused));

        #define MASS_STORAGE_BUF_SIZE		( 1024 * 1024 )
static uint32_t mass_storage_buf[MASS_STORAGE_BUF_SIZE] __attribute__((section(".sdram"))) __attribute__((unused));

        #define PRINTF_USES_HAL_TX    0
        #define ENABLE_NETWORK        0

        int __io_putchar(int ch)
        {
        uint8_t data = ch;
        #if PRINTF_USES_HAL_TX
        HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, (uint8_t*)&data, len, 100);
        #else
        while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE) == RESET) { ; }
        huart1.Instance->TDR = (uint16_t)data;
        #endif
        return 0;
        }

        char inkey(void)
        {
        uint32_t flags = huart1.Instance->ISR;

        if((flags & UART_FLAG_RXNE) || (flags & UART_FLAG_ORE))
        {
        __HAL_UART_CLEAR_OREFLAG(&huart1);
        return (huart1.Instance->RDR);
        }
        else
        return 0;
        }

//partially based on available code examples
static void lcd_start(void)
        {
        /* LCD Initialization */
        BSP_LCD_Init();

        /* LCD Initialization */
        BSP_LCD_LayerDefaultInit(0, (unsigned int)lcd_image_bg);
        //BSP_LCD_LayerDefaultInit(1, (unsigned int)lcd_image_bg+(LCD_X_SIZE*LCD_Y_SIZE*4));
        BSP_LCD_LayerDefaultInit(1, (unsigned int)lcd_image_fg);

        /* Enable the LCD */
        BSP_LCD_DisplayOn();

        /* Select the LCD Background Layer  */
        BSP_LCD_SelectLayer(0);

        /* Clear the Background Layer */
        BSP_LCD_Clear(LCD_COLOR_WHITE);
        BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

        BSP_LCD_SetColorKeying(1,LCD_COLOR_WHITE);

        /* Select the LCD Foreground Layer  */
        BSP_LCD_SelectLayer(1);

        /* Clear the Foreground Layer */
        BSP_LCD_Clear(LCD_COLOR_WHITE);
        BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  
  /* Configure the transparency for foreground and background :
     Increase the transparency */
        BSP_LCD_SetTransparency(0, 255);
        BSP_LCD_SetTransparency(1, 255);
        }


        Point trianglePoints[3];
        Point rectanglePoints[4];

        void draw_triangle(int X0, int Y0, int X1, int Y1, int X2, int Y2) {
        trianglePoints[0].X = X0;
        trianglePoints[0].Y = Y0;
        trianglePoints[1].X = X1;
        trianglePoints[1].Y = Y1;
        trianglePoints[2].X = X2;
        trianglePoints[2].Y = Y2;
        BSP_LCD_FillPolygon(trianglePoints, 3);
        }

        void draw_rectangle(int X0, int Y0, int X1, int Y1, int X2, int Y2, int X3, int Y3) {
        rectanglePoints[0].X = X0;
        rectanglePoints[0].Y = Y0;
        rectanglePoints[1].X = X1;
        rectanglePoints[1].Y = Y1;
        rectanglePoints[3].X = X2;
        rectanglePoints[3].Y = Y2;
        rectanglePoints[2].X = X3;
        rectanglePoints[2].Y = Y3;
        BSP_LCD_FillPolygon(rectanglePoints, 4);
        }

        void draw_empty_rectangle(int X0, int Y0, int X1, int Y1, int X2, int Y2, int X3, int Y3) {
        rectanglePoints[0].X = X0;
        rectanglePoints[0].Y = Y0;
        rectanglePoints[1].X = X1;
        rectanglePoints[1].Y = Y1;
        rectanglePoints[3].X = X2;
        rectanglePoints[3].Y = Y2;
        rectanglePoints[2].X = X3;
        rectanglePoints[2].Y = Y3;
        BSP_LCD_DrawPolygon(rectanglePoints, 4);
        }


        void draw_central_circle(){
        uint16_t circleXpos = 0.5 * LCD_X_SIZE;
        uint16_t circleYpos = 0.5 * LCD_Y_SIZE;
        int R = 70;
        BSP_LCD_FillCircle(circleXpos, circleYpos, R);
        }

        void draw_start_pause(int background_color, int foreground_color){
        /* Select the LCD Background Layer  */
        BSP_LCD_SelectLayer(0);
        BSP_LCD_SetTextColor(background_color);
        draw_central_circle();

        //select Foreground Layer
        BSP_LCD_SelectLayer(1);
        BSP_LCD_SetTextColor(foreground_color);
        draw_triangle(240, 136, 200, 111, 200, 161);
        draw_rectangle(256, 111, 262, 111, 256, 161, 262, 161);
        draw_rectangle(270, 111, 276, 111, 270, 161, 276, 161);

        }

        void draw_current_volume(int volume){
        BSP_LCD_SelectLayer(1);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		draw_rectangle(440, 36, 460, 36, 440, 236, 460, 236);
        BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
        draw_rectangle(440, 236 - (volume*2), 460, 236 - (volume*2), 440, 236, 460, 236);
        draw_empty_rectangle(440, 36, 460, 36, 440, 236, 460, 236);
        }

static TS_StateTypeDef  TS_State;


        int initialize_touchscreen(void)
        {
        uint8_t  status = 0;
        status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
        if(status != TS_OK) return -1;
        return 0;
        }

//funkcja sprawdzajaca czy x jest w podanym zakresie //jak nie dziala to zmienic int na bool (dodac biblioteke?)
int inRange(unsigned low, unsigned high, unsigned x)
{
return (low <= x && x <= high);
}

 //globalne "zakresy"
int x0 = 40, x1 = 82, x2 = 120, x3 = 150, x4 = 0.5 * LCD_X_SIZE - 70,
x5 = 0.5 * LCD_X_SIZE + 70, x6 = 330, x7 = 360, x8 = 400, x9 = 425;
int y0 = 36, y1 = 0.5 * LCD_Y_SIZE - 70, y2 = 56, y3 = 115,
y4 = 157, y5 = 0.5 * LCD_Y_SIZE + 70, y6 = 216, y7 = 236;


//zmieniona na globalne funkcja rysujaca
void draw_play_background(int first_color, int second_color){
    //draw_start_pause(LCD_COLOR_DARKGREEN, LCD_COLOR_GREEN);
	draw_start_pause(first_color, second_color);
    //drawing forward/backward buttons
    BSP_LCD_SelectLayer(1);
    BSP_LCD_SetTextColor(first_color);
    draw_triangle(x6, y3, x7, 136, x6, y4);
    draw_triangle(x3, y3, x2, 136, x3, y4);
    //draw volume status
    draw_current_volume(70);
    //draw volume buttons
    draw_triangle(x9, y6, x8, y6, 412, y7);
    draw_triangle(x9, y2, x8, y2, 412, y0);
    //draw stop button
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    draw_rectangle(x0, y3, x1, y3, x0, y4, x1, y4);
}

void draw_playing_background() {
	draw_play_background(LCD_COLOR_DARKGREEN, LCD_COLOR_GREEN);
	}
	
void draw_stopped_background() {
	draw_play_background(LCD_COLOR_ORANGE, LCD_COLOR_DARKYELLOW);
	}

        /* USER CODE END 0 */

/**
 * @brief  The application entry point.
 *
 * @retval None
 */
        int main(void)
        {
        /* USER CODE BEGIN 1 */

        /* USER CODE END 1 */

        /* MCU Configuration----------------------------------------------------------*/

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
        MX_ADC3_Init();
        MX_CRC_Init();
        MX_DCMI_Init();
        MX_DMA2D_Init();
        MX_FMC_Init();
        MX_I2C1_Init();
        MX_I2C3_Init();
        MX_LTDC_Init();
        MX_QUADSPI_Init();
        MX_RTC_Init();
        MX_SAI2_Init();
        MX_SDMMC1_SD_Init();
        MX_SPDIFRX_Init();
        MX_SPI2_Init();
        MX_TIM1_Init();
        MX_TIM2_Init();
        MX_TIM3_Init();
        MX_TIM5_Init();
        MX_TIM8_Init();
        MX_TIM12_Init();
        MX_USART1_UART_Init();
        MX_USART6_UART_Init();
        MX_TIM7_Init();
        /* USER CODE BEGIN 2 */
        debug_init(&huart1);

        xprintf(ANSI_FG_GREEN "STM32F746 Discovery Project" ANSI_FG_DEFAULT "\n");

        printf("Regular printf\n");

        lcd_start();
        draw_stopped_background();
        initialize_touchscreen();

        /* USER CODE END 2 */

        /* USER CODE BEGIN RTOS_MUTEX */
        /* add mutexes, ... */
        /* USER CODE END RTOS_MUTEX */

        /* USER CODE BEGIN RTOS_SEMAPHORES */
        /* add semaphores, ... */
        /* USER CODE END RTOS_SEMAPHORES */

        /* USER CODE BEGIN RTOS_TIMERS */
        /* start timers, add new ones, ... */
        /* USER CODE END RTOS_TIMERS */

        /* Create the thread(s) */
        /* definition and creation of defaultTask */
        osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 4096);
        defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

        /* USER CODE BEGIN RTOS_THREADS */
        /* add threads, ... */
        /* USER CODE END RTOS_THREADS */

        /* USER CODE BEGIN RTOS_QUEUES */
        /* add queues, ... */
        /* USER CODE END RTOS_QUEUES */


        /* Start scheduler */
        osKernelStart();

        /* We should never get here as control is now taken by the scheduler */

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

        RCC_OscInitTypeDef RCC_OscInitStruct;
        RCC_ClkInitTypeDef RCC_ClkInitStruct;
        RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

        /**Configure the main internal regulator output voltage 
         */
        __HAL_RCC_PWR_CLK_ENABLE();

        __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

        /**Initializes the CPU, AHB and APB busses clocks 
         */
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
        RCC_OscInitStruct.HSEState = RCC_HSE_ON;
        RCC_OscInitStruct.LSIState = RCC_LSI_ON;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
        RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
        RCC_OscInitStruct.PLL.PLLM = 25;
        RCC_OscInitStruct.PLL.PLLN = 400;
        RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
        RCC_OscInitStruct.PLL.PLLQ = 8;
        if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        /**Activate the Over-Drive mode 
         */
        if (HAL_PWREx_EnableOverDrive() != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        /**Initializes the CPU, AHB and APB busses clocks 
         */
        RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
        |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
        RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
        RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
        RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
        RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

        if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPDIFRX|RCC_PERIPHCLK_LTDC
        |RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART1
        |RCC_PERIPHCLK_USART6|RCC_PERIPHCLK_SAI2
        |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_I2C3
        |RCC_PERIPHCLK_SDMMC1|RCC_PERIPHCLK_CLK48;
        PeriphClkInitStruct.PLLI2S.PLLI2SN = 100;
        PeriphClkInitStruct.PLLI2S.PLLI2SP = RCC_PLLP_DIV2;
        PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
        PeriphClkInitStruct.PLLI2S.PLLI2SQ = 2;
        PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
        PeriphClkInitStruct.PLLSAI.PLLSAIR = 4;
        PeriphClkInitStruct.PLLSAI.PLLSAIQ = 4;
        PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV4;
        PeriphClkInitStruct.PLLI2SDivQ = 1;
        PeriphClkInitStruct.PLLSAIDivQ = 1;
        PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
        PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
        PeriphClkInitStruct.Sai2ClockSelection = RCC_SAI2CLKSOURCE_PLLSAI;
        PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
        PeriphClkInitStruct.Usart6ClockSelection = RCC_USART6CLKSOURCE_PCLK2;
        PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
        PeriphClkInitStruct.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;
        PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLLSAIP;
        PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        /**Configure the Systick interrupt time 
         */
        HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

        /**Configure the Systick 
         */
        HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

        /* SysTick_IRQn interrupt configuration */
        HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
        }

/* ADC3 init function */
static void MX_ADC3_Init(void)
        {

        ADC_ChannelConfTypeDef sConfig;

        /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
         */
        hadc3.Instance = ADC3;
        hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
        hadc3.Init.Resolution = ADC_RESOLUTION_12B;
        hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
        hadc3.Init.ContinuousConvMode = DISABLE;
        hadc3.Init.DiscontinuousConvMode = DISABLE;
        hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
        hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
        hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
        hadc3.Init.NbrOfConversion = 1;
        hadc3.Init.DMAContinuousRequests = DISABLE;
        hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
        if (HAL_ADC_Init(&hadc3) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
         */
        sConfig.Channel = ADC_CHANNEL_4;
        sConfig.Rank = ADC_REGULAR_RANK_1;
        sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
        if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        }

/* CRC init function */
static void MX_CRC_Init(void)
        {

        hcrc.Instance = CRC;
        hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
        hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
        hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
        hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
        hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
        if (HAL_CRC_Init(&hcrc) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        }

/* DCMI init function */
static void MX_DCMI_Init(void)
        {

        hdcmi.Instance = DCMI;
        hdcmi.Init.SynchroMode = DCMI_SYNCHRO_HARDWARE;
        hdcmi.Init.PCKPolarity = DCMI_PCKPOLARITY_FALLING;
        hdcmi.Init.VSPolarity = DCMI_VSPOLARITY_LOW;
        hdcmi.Init.HSPolarity = DCMI_HSPOLARITY_LOW;
        hdcmi.Init.CaptureRate = DCMI_CR_ALL_FRAME;
        hdcmi.Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;
        hdcmi.Init.JPEGMode = DCMI_JPEG_DISABLE;
        hdcmi.Init.ByteSelectMode = DCMI_BSM_ALL;
        hdcmi.Init.ByteSelectStart = DCMI_OEBS_ODD;
        hdcmi.Init.LineSelectMode = DCMI_LSM_ALL;
        hdcmi.Init.LineSelectStart = DCMI_OELS_ODD;
        if (HAL_DCMI_Init(&hdcmi) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        }

/* DMA2D init function */
static void MX_DMA2D_Init(void)
        {

        hdma2d.Instance = DMA2D;
        hdma2d.Init.Mode = DMA2D_M2M;
        hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
        hdma2d.Init.OutputOffset = 0;
        hdma2d.LayerCfg[1].InputOffset = 0;
        hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
        hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
        hdma2d.LayerCfg[1].InputAlpha = 0;
        if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        }

/* I2C1 init function */
static void MX_I2C1_Init(void)
        {

        hi2c1.Instance = I2C1;
        hi2c1.Init.Timing = 0x00C0EAFF;
        hi2c1.Init.OwnAddress1 = 0;
        hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
        hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
        hi2c1.Init.OwnAddress2 = 0;
        hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
        hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
        hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
        if (HAL_I2C_Init(&hi2c1) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        /**Configure Analogue filter 
         */
        if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        /**Configure Digital filter 
         */
        if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        }

/* I2C3 init function */
static void MX_I2C3_Init(void)
        {

        hi2c3.Instance = I2C3;
        hi2c3.Init.Timing = 0x00C0EAFF;
        hi2c3.Init.OwnAddress1 = 0;
        hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
        hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
        hi2c3.Init.OwnAddress2 = 0;
        hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
        hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
        hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
        if (HAL_I2C_Init(&hi2c3) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        /**Configure Analogue filter 
         */
        if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        /**Configure Digital filter 
         */
        if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        }

/* LTDC init function */
static void MX_LTDC_Init(void)
        {

        LTDC_LayerCfgTypeDef pLayerCfg;

        hltdc.Instance = LTDC;
        hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
        hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
        hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
        hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
        hltdc.Init.HorizontalSync = 40;
        hltdc.Init.VerticalSync = 9;
        hltdc.Init.AccumulatedHBP = 53;
        hltdc.Init.AccumulatedVBP = 11;
        hltdc.Init.AccumulatedActiveW = 533;
        hltdc.Init.AccumulatedActiveH = 283;
        hltdc.Init.TotalWidth = 565;
        hltdc.Init.TotalHeigh = 285;
        hltdc.Init.Backcolor.Blue = 0;
        hltdc.Init.Backcolor.Green = 0;
        hltdc.Init.Backcolor.Red = 0;
        if (HAL_LTDC_Init(&hltdc) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        pLayerCfg.WindowX0 = 0;
        pLayerCfg.WindowX1 = 480;
        pLayerCfg.WindowY0 = 0;
        pLayerCfg.WindowY1 = 272;
        pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
        pLayerCfg.Alpha = 255;
        pLayerCfg.Alpha0 = 0;
        pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
        pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
        pLayerCfg.FBStartAdress = 0xC0000000;
        pLayerCfg.ImageWidth = 480;
        pLayerCfg.ImageHeight = 272;
        pLayerCfg.Backcolor.Blue = 0;
        pLayerCfg.Backcolor.Green = 0;
        pLayerCfg.Backcolor.Red = 0;
        if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        }

/* QUADSPI init function */
static void MX_QUADSPI_Init(void)
        {

        /* QUADSPI parameter configuration*/
        hqspi.Instance = QUADSPI;
        hqspi.Init.ClockPrescaler = 255;
        hqspi.Init.FifoThreshold = 1;
        hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
        hqspi.Init.FlashSize = 1;
        hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
        hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
        hqspi.Init.FlashID = QSPI_FLASH_ID_1;
        hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
        if (HAL_QSPI_Init(&hqspi) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        }

/* RTC init function */
static void MX_RTC_Init(void)
        {

        /* USER CODE BEGIN RTC_Init 0 */

        /* USER CODE END RTC_Init 0 */

        RTC_TimeTypeDef sTime;
        RTC_DateTypeDef sDate;
        RTC_AlarmTypeDef sAlarm;

        /* USER CODE BEGIN RTC_Init 1 */

        /* USER CODE END RTC_Init 1 */

        /**Initialize RTC Only 
         */
        hrtc.Instance = RTC;
        hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
        hrtc.Init.AsynchPrediv = 127;
        hrtc.Init.SynchPrediv = 255;
        hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
        hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
        hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
        if (HAL_RTC_Init(&hrtc) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        /**Initialize RTC and set the Time and Date 
         */
        sTime.Hours = 0x0;
        sTime.Minutes = 0x0;
        sTime.Seconds = 0x0;
        sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
        sTime.StoreOperation = RTC_STOREOPERATION_RESET;
        if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        sDate.WeekDay = RTC_WEEKDAY_MONDAY;
        sDate.Month = RTC_MONTH_JANUARY;
        sDate.Date = 0x1;
        sDate.Year = 0x0;

        if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        /**Enable the Alarm A 
         */
        sAlarm.AlarmTime.Hours = 0x0;
        sAlarm.AlarmTime.Minutes = 0x0;
        sAlarm.AlarmTime.Seconds = 0x0;
        sAlarm.AlarmTime.SubSeconds = 0x0;
        sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
        sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
        sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
        sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
        sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
        sAlarm.AlarmDateWeekDay = 0x1;
        sAlarm.Alarm = RTC_ALARM_A;
        if (HAL_RTC_SetAlarm(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        /**Enable the Alarm B 
         */
        sAlarm.AlarmDateWeekDay = 0x1;
        sAlarm.Alarm = RTC_ALARM_B;
        if (HAL_RTC_SetAlarm(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        /**Enable the TimeStamp 
         */
        if (HAL_RTCEx_SetTimeStamp(&hrtc, RTC_TIMESTAMPEDGE_RISING, RTC_TIMESTAMPPIN_POS1) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        }

/* SAI2 init function */
static void MX_SAI2_Init(void)
        {

        hsai_BlockA2.Instance = SAI2_Block_A;
        hsai_BlockA2.Init.Protocol = SAI_FREE_PROTOCOL;
        hsai_BlockA2.Init.AudioMode = SAI_MODEMASTER_TX;
        hsai_BlockA2.Init.DataSize = SAI_DATASIZE_24;
        hsai_BlockA2.Init.FirstBit = SAI_FIRSTBIT_MSB;
        hsai_BlockA2.Init.ClockStrobing = SAI_CLOCKSTROBING_FALLINGEDGE;
        hsai_BlockA2.Init.Synchro = SAI_ASYNCHRONOUS;
        hsai_BlockA2.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
        hsai_BlockA2.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;
        hsai_BlockA2.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
        hsai_BlockA2.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_192K;
        hsai_BlockA2.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
        hsai_BlockA2.Init.MonoStereoMode = SAI_STEREOMODE;
        hsai_BlockA2.Init.CompandingMode = SAI_NOCOMPANDING;
        hsai_BlockA2.Init.TriState = SAI_OUTPUT_NOTRELEASED;
        hsai_BlockA2.FrameInit.FrameLength = 8;
        hsai_BlockA2.FrameInit.ActiveFrameLength = 1;
        hsai_BlockA2.FrameInit.FSDefinition = SAI_FS_STARTFRAME;
        hsai_BlockA2.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
        hsai_BlockA2.FrameInit.FSOffset = SAI_FS_FIRSTBIT;
        hsai_BlockA2.SlotInit.FirstBitOffset = 0;
        hsai_BlockA2.SlotInit.SlotSize = SAI_SLOTSIZE_DATASIZE;
        hsai_BlockA2.SlotInit.SlotNumber = 1;
        hsai_BlockA2.SlotInit.SlotActive = 0x00000000;
        if (HAL_SAI_Init(&hsai_BlockA2) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        hsai_BlockB2.Instance = SAI2_Block_B;
        hsai_BlockB2.Init.Protocol = SAI_FREE_PROTOCOL;
        hsai_BlockB2.Init.AudioMode = SAI_MODESLAVE_RX;
        hsai_BlockB2.Init.DataSize = SAI_DATASIZE_24;
        hsai_BlockB2.Init.FirstBit = SAI_FIRSTBIT_MSB;
        hsai_BlockB2.Init.ClockStrobing = SAI_CLOCKSTROBING_FALLINGEDGE;
        hsai_BlockB2.Init.Synchro = SAI_SYNCHRONOUS;
        hsai_BlockB2.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
        hsai_BlockB2.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
        hsai_BlockB2.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
        hsai_BlockB2.Init.MonoStereoMode = SAI_STEREOMODE;
        hsai_BlockB2.Init.CompandingMode = SAI_NOCOMPANDING;
        hsai_BlockB2.Init.TriState = SAI_OUTPUT_NOTRELEASED;
        hsai_BlockB2.FrameInit.FrameLength = 8;
        hsai_BlockB2.FrameInit.ActiveFrameLength = 1;
        hsai_BlockB2.FrameInit.FSDefinition = SAI_FS_STARTFRAME;
        hsai_BlockB2.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
        hsai_BlockB2.FrameInit.FSOffset = SAI_FS_FIRSTBIT;
        hsai_BlockB2.SlotInit.FirstBitOffset = 0;
        hsai_BlockB2.SlotInit.SlotSize = SAI_SLOTSIZE_DATASIZE;
        hsai_BlockB2.SlotInit.SlotNumber = 1;
        hsai_BlockB2.SlotInit.SlotActive = 0x00000000;
        if (HAL_SAI_Init(&hsai_BlockB2) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        }

/* SDMMC1 init function */
static void MX_SDMMC1_SD_Init(void)
        {

        hsd1.Instance = SDMMC1;
        hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
        hsd1.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
        hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
        hsd1.Init.BusWide = SDMMC_BUS_WIDE_1B;
        hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
        hsd1.Init.ClockDiv = 0;

        }

/* SPDIFRX init function */
static void MX_SPDIFRX_Init(void)
        {

        hspdif.Instance = SPDIFRX;
        hspdif.Init.InputSelection = SPDIFRX_INPUT_IN0;
        hspdif.Init.Retries = SPDIFRX_MAXRETRIES_NONE;
        hspdif.Init.WaitForActivity = SPDIFRX_WAITFORACTIVITY_OFF;
        hspdif.Init.ChannelSelection = SPDIFRX_CHANNEL_A;
        hspdif.Init.DataFormat = SPDIFRX_DATAFORMAT_LSB;
        hspdif.Init.StereoMode = SPDIFRX_STEREOMODE_DISABLE;
        hspdif.Init.PreambleTypeMask = SPDIFRX_PREAMBLETYPEMASK_OFF;
        hspdif.Init.ChannelStatusMask = SPDIFRX_CHANNELSTATUS_OFF;
        hspdif.Init.ValidityBitMask = SPDIFRX_VALIDITYMASK_OFF;
        hspdif.Init.ParityErrorMask = SPDIFRX_PARITYERRORMASK_OFF;
        if (HAL_SPDIFRX_Init(&hspdif) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        }

/* SPI2 init function */
static void MX_SPI2_Init(void)
        {

        /* SPI2 parameter configuration*/
        hspi2.Instance = SPI2;
        hspi2.Init.Mode = SPI_MODE_MASTER;
        hspi2.Init.Direction = SPI_DIRECTION_2LINES;
        hspi2.Init.DataSize = SPI_DATASIZE_4BIT;
        hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
        hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
        hspi2.Init.NSS = SPI_NSS_SOFT;
        hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
        hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
        hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
        hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
        hspi2.Init.CRCPolynomial = 7;
        hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
        hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
        if (HAL_SPI_Init(&hspi2) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        }

/* TIM1 init function */
static void MX_TIM1_Init(void)
        {

        TIM_ClockConfigTypeDef sClockSourceConfig;
        TIM_MasterConfigTypeDef sMasterConfig;
        TIM_OC_InitTypeDef sConfigOC;
        TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

        htim1.Instance = TIM1;
        htim1.Init.Prescaler = 0;
        htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim1.Init.Period = 0;
        htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        htim1.Init.RepetitionCounter = 0;
        htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = 0;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
        sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
        if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
        sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
        sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
        sBreakDeadTimeConfig.DeadTime = 0;
        sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
        sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
        sBreakDeadTimeConfig.BreakFilter = 0;
        sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
        sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
        sBreakDeadTimeConfig.Break2Filter = 0;
        sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
        if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        HAL_TIM_MspPostInit(&htim1);

        }

/* TIM2 init function */
static void MX_TIM2_Init(void)
        {

        TIM_ClockConfigTypeDef sClockSourceConfig;
        TIM_MasterConfigTypeDef sMasterConfig;
        TIM_OC_InitTypeDef sConfigOC;

        htim2.Instance = TIM2;
        htim2.Init.Prescaler = 0;
        htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim2.Init.Period = 0;
        htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = 0;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        HAL_TIM_MspPostInit(&htim2);

        }

/* TIM3 init function */
static void MX_TIM3_Init(void)
        {

        TIM_ClockConfigTypeDef sClockSourceConfig;
        TIM_MasterConfigTypeDef sMasterConfig;
        TIM_OC_InitTypeDef sConfigOC;

        htim3.Instance = TIM3;
        htim3.Init.Prescaler = 0;
        htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim3.Init.Period = 0;
        htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = 0;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        HAL_TIM_MspPostInit(&htim3);

        }

/* TIM5 init function */
static void MX_TIM5_Init(void)
        {

        TIM_ClockConfigTypeDef sClockSourceConfig;
        TIM_MasterConfigTypeDef sMasterConfig;
        TIM_OC_InitTypeDef sConfigOC;

        htim5.Instance = TIM5;
        htim5.Init.Prescaler = 0;
        htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim5.Init.Period = 0;
        htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        if (HAL_TIM_PWM_Init(&htim5) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = 0;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        HAL_TIM_MspPostInit(&htim5);

        }

/* TIM7 init function */
static void MX_TIM7_Init(void)
        {

        TIM_MasterConfigTypeDef sMasterConfig;

        htim7.Instance = TIM7;
        htim7.Init.Prescaler = 0;
        htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim7.Init.Period = 0;
        htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        }

/* TIM8 init function */
static void MX_TIM8_Init(void)
        {

        TIM_ClockConfigTypeDef sClockSourceConfig;
        TIM_MasterConfigTypeDef sMasterConfig;

        htim8.Instance = TIM8;
        htim8.Init.Prescaler = 0;
        htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim8.Init.Period = 0;
        htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        htim8.Init.RepetitionCounter = 0;
        htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        }

/* TIM12 init function */
static void MX_TIM12_Init(void)
        {

        TIM_OC_InitTypeDef sConfigOC;

        htim12.Instance = TIM12;
        htim12.Init.Prescaler = 0;
        htim12.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim12.Init.Period = 0;
        htim12.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        htim12.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if (HAL_TIM_PWM_Init(&htim12) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = 0;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        if (HAL_TIM_PWM_ConfigChannel(&htim12, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        HAL_TIM_MspPostInit(&htim12);

        }

/* USART1 init function */
static void MX_USART1_UART_Init(void)
        {

        huart1.Instance = USART1;
        huart1.Init.BaudRate = 115200;
        huart1.Init.WordLength = UART_WORDLENGTH_8B;
        huart1.Init.StopBits = UART_STOPBITS_1;
        huart1.Init.Parity = UART_PARITY_NONE;
        huart1.Init.Mode = UART_MODE_TX_RX;
        huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart1.Init.OverSampling = UART_OVERSAMPLING_16;
        huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
        huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
        if (HAL_UART_Init(&huart1) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        }

/* USART6 init function */
static void MX_USART6_UART_Init(void)
        {

        huart6.Instance = USART6;
        huart6.Init.BaudRate = 115200;
        huart6.Init.WordLength = UART_WORDLENGTH_8B;
        huart6.Init.StopBits = UART_STOPBITS_1;
        huart6.Init.Parity = UART_PARITY_NONE;
        huart6.Init.Mode = UART_MODE_TX_RX;
        huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart6.Init.OverSampling = UART_OVERSAMPLING_16;
        huart6.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
        huart6.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
        if (HAL_UART_Init(&huart6) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        }

/* FMC initialization function */
static void MX_FMC_Init(void)
        {
        FMC_SDRAM_TimingTypeDef SdramTiming;

        /** Perform the SDRAM1 memory initialization sequence
         */
        hsdram1.Instance = FMC_SDRAM_DEVICE;
        /* hsdram1.Init */
        hsdram1.Init.SDBank = FMC_SDRAM_BANK1;
        hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
        hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
        hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
        hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
        hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
        hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
        hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
        hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
        hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
        /* SdramTiming */
        SdramTiming.LoadToActiveDelay = 2;
        SdramTiming.ExitSelfRefreshDelay = 7;
        SdramTiming.SelfRefreshTime = 4;
        SdramTiming.RowCycleDelay = 7;
        SdramTiming.WriteRecoveryTime = 3;
        SdramTiming.RPDelay = 2;
        SdramTiming.RCDDelay = 2;

        if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
        {
        _Error_Handler(__FILE__, __LINE__);
        }

        }

/** Configure pins as 
 * Analog 
 * Input 
 * Output
 * EVENT_OUT
 * EXTI
 PB5   ------> USB_OTG_HS_ULPI_D7
 PH4   ------> USB_OTG_HS_ULPI_NXT
 PB13   ------> USB_OTG_HS_ULPI_D6
 PB12   ------> USB_OTG_HS_ULPI_D5
 PC0   ------> USB_OTG_HS_ULPI_STP
 PC2   ------> USB_OTG_HS_ULPI_DIR
 PA5   ------> USB_OTG_HS_ULPI_CK
 PB10   ------> USB_OTG_HS_ULPI_D3
 PA3   ------> USB_OTG_HS_ULPI_D0
 PB1   ------> USB_OTG_HS_ULPI_D2
 PB0   ------> USB_OTG_HS_ULPI_D1
 PB11   ------> USB_OTG_HS_ULPI_D4
 */
static void MX_GPIO_Init(void)
        {

        GPIO_InitTypeDef GPIO_InitStruct;

        /* GPIO Ports Clock Enable */
        __HAL_RCC_GPIOE_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOJ_CLK_ENABLE();
        __HAL_RCC_GPIOI_CLK_ENABLE();
        __HAL_RCC_GPIOK_CLK_ENABLE();
        __HAL_RCC_GPIOF_CLK_ENABLE();
        __HAL_RCC_GPIOH_CLK_ENABLE();

        /*Configure GPIO pin Output Level */
        HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

        /*Configure GPIO pin Output Level */
        HAL_GPIO_WritePin(GPIOI, ARDUINO_D7_Pin|ARDUINO_D8_Pin, GPIO_PIN_RESET);

        /*Configure GPIO pin Output Level */
        HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_Port, LCD_BL_CTRL_Pin, GPIO_PIN_SET);

        /*Configure GPIO pin Output Level */
        HAL_GPIO_WritePin(LCD_DISP_GPIO_Port, LCD_DISP_Pin, GPIO_PIN_SET);

        /*Configure GPIO pin Output Level */
        HAL_GPIO_WritePin(DCMI_PWR_EN_GPIO_Port, DCMI_PWR_EN_Pin, GPIO_PIN_RESET);

        /*Configure GPIO pin Output Level */
        HAL_GPIO_WritePin(GPIOG, ARDUINO_D4_Pin|ARDUINO_D2_Pin|EXT_RST_Pin, GPIO_PIN_RESET);

        /*Configure GPIO pin : OTG_HS_OverCurrent_Pin */
        GPIO_InitStruct.Pin = OTG_HS_OverCurrent_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(OTG_HS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ULPI_D7_Pin ULPI_D6_Pin ULPI_D5_Pin ULPI_D3_Pin 
                           ULPI_D2_Pin ULPI_D1_Pin ULPI_D4_Pin */
        GPIO_InitStruct.Pin = ULPI_D7_Pin|ULPI_D6_Pin|ULPI_D5_Pin|ULPI_D3_Pin
        |ULPI_D2_Pin|ULPI_D1_Pin|ULPI_D4_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /*Configure GPIO pin : OTG_FS_VBUS_Pin */
        GPIO_InitStruct.Pin = OTG_FS_VBUS_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(OTG_FS_VBUS_GPIO_Port, &GPIO_InitStruct);

        /*Configure GPIO pin : Audio_INT_Pin */
        GPIO_InitStruct.Pin = Audio_INT_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(Audio_INT_GPIO_Port, &GPIO_InitStruct);

        /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
        GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

        /*Configure GPIO pins : ARDUINO_D7_Pin ARDUINO_D8_Pin LCD_DISP_Pin */
        GPIO_InitStruct.Pin = ARDUINO_D7_Pin|ARDUINO_D8_Pin|LCD_DISP_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

        /*Configure GPIO pin : uSD_Detect_Pin */
        GPIO_InitStruct.Pin = uSD_Detect_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(uSD_Detect_GPIO_Port, &GPIO_InitStruct);

        /*Configure GPIO pin : LCD_BL_CTRL_Pin */
        GPIO_InitStruct.Pin = LCD_BL_CTRL_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(LCD_BL_CTRL_GPIO_Port, &GPIO_InitStruct);

        /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
        GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

        /*Configure GPIO pins : TP3_Pin NC2_Pin */
        GPIO_InitStruct.Pin = TP3_Pin|NC2_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

        /*Configure GPIO pin : DCMI_PWR_EN_Pin */
        GPIO_InitStruct.Pin = DCMI_PWR_EN_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(DCMI_PWR_EN_GPIO_Port, &GPIO_InitStruct);

        /*Configure GPIO pin : LCD_INT_Pin */
        GPIO_InitStruct.Pin = LCD_INT_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(LCD_INT_GPIO_Port, &GPIO_InitStruct);

        /*Configure GPIO pin : ULPI_NXT_Pin */
        GPIO_InitStruct.Pin = ULPI_NXT_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
        HAL_GPIO_Init(ULPI_NXT_GPIO_Port, &GPIO_InitStruct);

        /*Configure GPIO pins : ARDUINO_D4_Pin ARDUINO_D2_Pin EXT_RST_Pin */
        GPIO_InitStruct.Pin = ARDUINO_D4_Pin|ARDUINO_D2_Pin|EXT_RST_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

        /*Configure GPIO pins : ULPI_STP_Pin ULPI_DIR_Pin */
        GPIO_InitStruct.Pin = ULPI_STP_Pin|ULPI_DIR_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /*Configure GPIO pin : RMII_RXER_Pin */
        GPIO_InitStruct.Pin = RMII_RXER_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(RMII_RXER_GPIO_Port, &GPIO_InitStruct);

        /*Configure GPIO pins : ULPI_CLK_Pin ULPI_D0_Pin */
        GPIO_InitStruct.Pin = ULPI_CLK_Pin|ULPI_D0_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        }

        /* USER CODE BEGIN 4 */
        #if ENABLE_NETWORK

static char response[500];

//based on available code examples
static void http_server_serve(struct netconn *conn)
        {
        struct netbuf *inbuf;
        err_t recv_err;
        char* buf;
        u16_t buflen;
  
  /* Read the data from the port, blocking if nothing yet there. 
   We assume the request (the part we care about) is in one netbuf */
        recv_err = netconn_recv(conn, &inbuf);

        if (recv_err == ERR_OK)
        {
        if (netconn_err(conn) == ERR_OK)
        {
        netbuf_data(inbuf, (void**)&buf, &buflen);
    
      /* Is this an HTTP GET command? (only check the first 5 chars, since
      there are other formats for GET, and we're keeping it very simple )*/
        if ((buflen >=5) && (strncmp(buf, "GET /", 5) == 0))
        {
        response[0] = 0;

        strcpy(response, "HTTP/1.1 200 OK\r\n\
        Content-Type: text/html\r\n\
        Connnection: close\r\n\r\n\
<!DOCTYPE HTML>\r\n");

        #if 0
        strcat(response,"<html>\r\n\
<meta http-equiv=\"refresh\" content=\"10\">");
        #endif

        strcat(response,"<title>Prosta strona WWW</title>");
        strcat(response,"<h1>H1 Header</h1>");

        strcat(response,"A to jest tekst na stronie");
        netconn_write(conn, response, sizeof(response), NETCONN_NOCOPY);
        }
        }
        }
        /* Close the connection (server closes in HTTP) */
        netconn_close(conn);
  
  /* Delete the buffer (netconn_recv gives us ownership,
   so we have to make sure to deallocate the buffer) */
        netbuf_delete(inbuf);
        }


//based on available code examples
static void http_server_netconn_thread(void const *arg)
        {
        struct netconn *conn, *newconn;
        err_t err, accept_err;

        xprintf("http_server_netconn_thread\n");

        /* Create a new TCP connection handle */
        conn = netconn_new(NETCONN_TCP);

        if (conn!= NULL)
        {
        /* Bind to port 80 (HTTP) with default IP address */
        err = netconn_bind(conn, NULL, 80);

        if (err == ERR_OK)
        {
        /* Put the connection into LISTEN state */
        netconn_listen(conn);

        while(1)
        {
        /* accept any icoming connection */
        accept_err = netconn_accept(conn, &newconn);
        if(accept_err == ERR_OK)
        {
        /* serve connection */
        http_server_serve(newconn);

        /* delete connection */
        netconn_delete(newconn);
        }
        }
        }
        }
        }

        #endif //ENABLE_NETWORK


//defining
#define SONGS 20
#define DISK "1:"


static uint8_t player_state1 = 2;
int volume = 70;
int current_song = 0;
int songs = 0;
char titles[SONGS][100];
FRESULT res;

 #define AUDIO_OUT_BUFFER_SIZE                      8192
enum {
        BUFFER_OFFSET_NONE = 0,
        BUFFER_OFFSET_HALF,
        BUFFER_OFFSET_FULL,
        };

        uint8_t buff[AUDIO_OUT_BUFFER_SIZE];
static FIL file;
        extern ApplicationTypeDef Appli_state;
static uint8_t buf_offs = BUFFER_OFFSET_NONE;
static uint32_t fpos = 0;

        char play_buffer[AUDIO_OUT_BUFFER_SIZE];
        unsigned char input_buffer[AUDIO_OUT_BUFFER_SIZE];
        unsigned char *input_buffer_pointer;
        short spare_buffer[AUDIO_OUT_BUFFER_SIZE/2];
        short *spare_buffer_pointer;
        int spare_buffer_offset = 0;
        int bytes_in_input_buffer = 0;
        MP3FrameInfo mp3_frame_info;
		HMP3Decoder decoder;
		int title_offset = 0;

        void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
        {
        buf_offs = BUFFER_OFFSET_FULL;
        }

        void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
        {
        buf_offs = BUFFER_OFFSET_HALF;
        }

       void print_title() {
		if(strlen(titles[current_song])>=22){
			uint8_t title[22];
			memcpy(title, titles[current_song] + title_offset, 22);
			BSP_LCD_SelectLayer(1);
			BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
			BSP_LCD_DisplayStringAt(0, LCD_Y_SIZE - 30, title, LEFT_MODE);
			title_offset = (title_offset + 1)%(strlen(titles[current_song])-21);
			}
			else{
				uint8_t title[22] = "                      ";
				memcpy(title, titles[current_song], strlen(titles[current_song]));
				BSP_LCD_SelectLayer(1);
				BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
				BSP_LCD_DisplayStringAt(0, LCD_Y_SIZE - 30, title, LEFT_MODE);
				
				xprintf("%s    %d\n", title, strlen(titles[current_song]));
				}
		}
		
	void decode(int play_offset){
		uint32_t bytes_read = 0;
		
		if (f_read(&file, input_buffer_pointer, AUDIO_OUT_BUFFER_SIZE - bytes_in_input_buffer, (void *) &bytes_read) != FR_OK) {
			BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
			xprintf("file reading error\n");
		}
		bytes_in_input_buffer += bytes_read;
		input_buffer_pointer = input_buffer;

		while(spare_buffer_offset < AUDIO_OUT_BUFFER_SIZE / 4){
			int offset = MP3FindSyncWord((unsigned char *) input_buffer_pointer, bytes_in_input_buffer);
			if(offset == -1){
			xprintf("Offset is -1 :(");
			}
			//We move pointer to find first frame
			bytes_in_input_buffer -= (offset);
			input_buffer_pointer += (offset);
			//int err = MP3GetNextFrameInfo(decoder, &mp3_frame_info, (unsigned char*) &input_buffer_pointer);
			//xprintf("err is %d\n", err);
			//We decode first frame - use input buffer for this. Decoded data goes to spare_buffer
			int is_error = MP3Decode(decoder, (unsigned char**) &input_buffer_pointer, (int*) &bytes_in_input_buffer, spare_buffer_pointer,0);
			if(is_error){
				xprintf("Error while decoding %d.\n", is_error);
				break;
			}
			//Need to know how many bits were processed
			
			MP3GetLastFrameInfo(decoder, &mp3_frame_info);
			spare_buffer_offset += mp3_frame_info.outputSamps;
			spare_buffer_pointer = spare_buffer + spare_buffer_offset;

		}
		//move data in input buffer to its beginning
		memcpy(input_buffer, input_buffer_pointer, bytes_in_input_buffer);
		//input_buffer_pointer has to show place where new data can be put
		input_buffer_pointer = input_buffer + bytes_in_input_buffer;

		//We want to play music so we fullfill half of play_buffer and argument play_offset shows which one half
		memcpy(play_buffer + play_offset, spare_buffer, AUDIO_OUT_BUFFER_SIZE/2);
		//We have copied audio_out_buffer_size/4 of spare_buffer to play_buffer so we delete it
		memcpy(spare_buffer, &spare_buffer[AUDIO_OUT_BUFFER_SIZE/4], (spare_buffer_offset - AUDIO_OUT_BUFFER_SIZE/4) * 2);
		spare_buffer_offset -= AUDIO_OUT_BUFFER_SIZE/4;
		spare_buffer_pointer = spare_buffer + spare_buffer_offset;
		buf_offs = BUFFER_OFFSET_NONE;
}

void play_new() {
		xprintf("Song %s of current song: %d", titles[current_song], current_song);
		title_offset = 0;
        res = f_open(&file,titles[current_song],FA_READ);
        if(res==FR_OK)
        {
        xprintf("mp3 file open OK\n");
        }
        else
        {
        xprintf("wave file open ERROR, res = %d\n",res);
        //f_disp_res(res);
        while(1);
        }
        spare_buffer_pointer = spare_buffer;
        input_buffer_pointer = input_buffer;
		
		BSP_AUDIO_OUT_Play((uint16_t*)&play_buffer[0],AUDIO_OUT_BUFFER_SIZE);
        decode(0);
        decode(AUDIO_OUT_BUFFER_SIZE/2);
        player_state1 = 1;
        
        fpos = 0;
        buf_offs = BUFFER_OFFSET_NONE;
	}

void end_song() {
	BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
	f_close(&file);
    printf("EndOfSong");
    player_state1 = 0;
	spare_buffer_offset = 0;
    bytes_in_input_buffer = 0;
	buf_offs = BUFFER_OFFSET_NONE;
}

int volume_up(){
    if (BSP_AUDIO_OUT_SetVolume(volume < 91 ? volume += 10 : volume) != AUDIO_OK)
        {
            xprintf("ERROR: Failed to set audio volume\n");
			
            return -1;
        }
		xprintf("v: %d", volume);
		draw_current_volume(volume);
        return 0;
}

int volume_down(){
    if (BSP_AUDIO_OUT_SetVolume(volume > 9 ? volume -= 10 : volume) != AUDIO_OK)
        {
            xprintf("ERROR: Failed to set audio volume\n");
       
			return -1;
        }
		xprintf("v: %d", volume);
		draw_current_volume(volume);
        return 0;
}


///funkcja wykrywajaca dotyk
void detect_touch(TS_StateTypeDef touch){
    unsigned x = touch.touchX[0];
    unsigned y = touch.touchY[0];
    if(inRange(x0, x1, x) && inRange(y3, y4, y)){
        //stop_song();
        xprintf("song stopped\n");
		draw_stopped_background();
		end_song();
		current_song = 0;
		player_state1 = 2;
    }
    else if(inRange(x2, x3, x) && inRange(y3, y4, y)){
        //play_prev_song();
		current_song = current_song > 0 ? current_song - 1 : songs - 1;
		end_song();
		play_new();
        xprintf("prev song\n");
    }
    else if(inRange(x4, x5, x) && inRange(y1, y5, y)){
		if(player_state1 == 2){
			play_new();
			draw_playing_background();
			xprintf("song started\n");
			}
        else if(player_state1){ //pause_song(); 
			BSP_AUDIO_OUT_Pause();
			draw_stopped_background();
			player_state1 = 0;
			xprintf("song paused\n"); }
        else { //continue_song(); 
			BSP_AUDIO_OUT_Resume();
			draw_playing_background();
            player_state1 = 1;
			xprintf("song continued\n"); }
    }
    else if(inRange(x6, x7, x) && inRange(y3, y4, y)){
        //play_next_song();
		current_song = (current_song + 1) % songs;
		end_song();
		play_new();
        xprintf("next song\n");
    }
    else if(inRange(x8, x9, x)){
        if(inRange(y0, y2, y)){
            volume_up();
            xprintf("volume up\n");
        }
        else if(inRange(y6, y7, y)){
            volume_down();
            xprintf("volume down\n");
        }
    }
}



        /* USER CODE END 4 */

        /* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used 
 * @retval None
 */
        /* USER CODE END Header_StartDefaultTask */
        void StartDefaultTask(void const * argument)
        {
        /* init code for FATFS */
        MX_FATFS_Init();

        /* init code for USB_HOST */
        MX_USB_HOST_Init();

        /* init code for LWIP */
        #if ENABLE_NETWORK
        MX_LWIP_Init();
        #endif

        /* USER CODE BEGIN 5 */
        #if ENABLE_NETWORK
        osThreadDef(netconn_thread, http_server_netconn_thread, osPriorityNormal, 0, 1024);
        netconn_thread_handle = osThreadCreate(osThread(netconn_thread), NULL);
        #endif

        vTaskDelay(1000);

        lcd_start();


        xprintf("waiting for USB mass storage\n");

        do
        {
        xprintf(".");
        vTaskDelay(250);
        }while(Appli_state != APPLICATION_READY);

        xprintf("initializing audio codec...\n");

        
        FRESULT f_result;
        DIR directory;
        FILINFO file_info;

//finding mp3 file
        f_result = f_findfirst(&directory, &file_info, DISK, "*.mp3");
        while(f_result== FR_OK && file_info.fname[0] && songs < SONGS) {
        char path[50];
        strcpy(path, DISK);
        strcpy(path, "/");
        strcpy(path, file_info.fname);
        strcpy(titles[songs], path);
        f_result = f_findnext(&directory, &file_info);
        songs++;
        }
        f_closedir(&directory);
        draw_stopped_background();
		xprintf("SONGS %d\n", songs);
		for(int i = 0; i<songs; i++) {
			xprintf("SONG no %d: %s\n", i, titles[i]);
			}

        if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE1, volume, AUDIO_FREQUENCY_44K) == 0)
        {
        xprintf("audio init OK\n");
        }
        else
        {
        xprintf("audio init ERROR\n");
        }
        BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
		
		
        decoder = MP3InitDecoder();
		int i = 0;
        /* Infinite loop */
        for(;;)
        {
		i = (i + 1) % 200;
		if(i == 0)
			{print_title();}
		BSP_TS_GetState(&TS_State);
		if(TS_State.touchDetected){
			detect_touch(TS_State);
			vTaskDelay(100);
		}

        if(player_state1 == 1)
        {
        if (buf_offs == BUFFER_OFFSET_HALF) {
        decode(0);
        }
        if (buf_offs == BUFFER_OFFSET_FULL) {
        decode(AUDIO_OUT_BUFFER_SIZE / 2);
        }
        if (bytes_in_input_buffer == 0) {
		end_song();
        current_song = (current_song + 1)%songs;
		play_new();
        }

        }


        vTaskDelay(1);

        }
        /* USER CODE END 5 */
        }

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
        void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
        {
        /* USER CODE BEGIN Callback 0 */

        /* USER CODE END Callback 0 */
        if (htim->Instance == TIM6) {
        HAL_IncTick();
        }
        /* USER CODE BEGIN Callback 1 */

        /* USER CODE END Callback 1 */
        }

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  file: The file name as string.
 * @param  line: The line in file as a number.
 * @retval None
 */
        void _Error_Handler(char *file, int line)
        {
        /* USER CODE BEGIN Error_Handler_Debug */
        /* User can add his own implementation to report the HAL error return state */
        while(1)
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
        void assert_failed(uint8_t* file, uint32_t line)
        {
        /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
        /* USER CODE END 6 */
        }
        #endif /* USE_FULL_ASSERT */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
