#include "main.h"

UART_HandleTypeDef huart1; // USART1 핸들러

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();

    GPIO_PinState prev_led_state = GPIO_PIN_SET; // 초기 LED 상태 (꺼짐)

    while (1)
    {
        // 센서 상태 읽기
        GPIO_PinState sensor1_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
        GPIO_PinState sensor2_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);

        GPIO_PinState current_led_state = GPIO_PIN_SET; // LED 기본값: OFF

        // LED 제어 논리
        if (sensor1_state == GPIO_PIN_SET && sensor2_state == GPIO_PIN_SET)
        {
            current_led_state = GPIO_PIN_RESET; // LED ON (반전)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, current_led_state);
        }
        else if (sensor1_state == GPIO_PIN_SET && sensor2_state == GPIO_PIN_RESET)
        {
            current_led_state = GPIO_PIN_SET; // LED OFF (반전)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, current_led_state);
        }
        else if (sensor1_state == GPIO_PIN_RESET && sensor2_state == GPIO_PIN_SET)
        {
            current_led_state = GPIO_PIN_RESET; // LED OFF (반전)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, current_led_state);
        }
        else if (sensor1_state == GPIO_PIN_RESET && sensor2_state == GPIO_PIN_RESET)
        {
            current_led_state = GPIO_PIN_RESET; // LED OFF (반전)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, current_led_state);
        }

        // LED가 켜질 때만 메시지 전송
        if (current_led_state == GPIO_PIN_RESET && prev_led_state != GPIO_PIN_RESET)
        {
            const char *message = "LED ON\r\n";
            HAL_StatusTypeDef result = HAL_UART_Transmit(&huart1, (uint8_t *)message,
             strlen(message), HAL_MAX_DELAY);

            if (result != HAL_OK)
            {
                const char *error_message = "UART Transmit Error\r\n";
                HAL_UART_Transmit(&huart1, (uint8_t *)error_message, strlen(error_message), HAL_MAX_DELAY);
                Error_Handler(); // 오류 처리
            }
        }

        prev_led_state = current_led_state; // 이전 LED 상태 업데이트
        HAL_Delay(100); // 100ms 대기
    }
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 7;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

static void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 9600; // HC-06 기본 Baud Rate
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    // PA0, PA1 설정 (센서 입력)
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // PA8 설정 (LED 출력)
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // PA9 (USART1_TX) 및 PA10 (USART1_RX) 설정
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1; // Alternate Function 설정
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}
