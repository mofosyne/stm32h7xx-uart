
#include <stdbool.h>
#include "stm32h7xx_hal.h"
#include "logger.h"

#define TIMEOUT_MS 10000
#define BUFFERSIZE 10

/*** Callback ***/
bool UartReadyTx = false;
bool UartReadyRx = false;

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: transfer complete */
  UartReadyTx = true;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: transfer complete */
  UartReadyRx = true;
}

bool WaitForUartReadyTx()
{
  uint32_t start = HAL_GetTick();
  while (!UartReadyTx)
  {
    if ( (HAL_GetTick() - start) > TIMEOUT_MS)
    {
      return false;
    }
  }
  UartReadyTx = false;
  return true;
}

bool WaitForUartReadyRx()
{
  uint32_t start = HAL_GetTick();
  while (!UartReadyRx)
  {
    if ( (HAL_GetTick() - start) > TIMEOUT_MS)
    {
      return false;
    }
  }
  UartReadyRx = false;
  return true;
}

/*** test functions ***/

UART_HandleTypeDef *ghwuart_ptr = NULL;

void uart_ISR_test(UART_HandleTypeDef *huart)
{
  ghwuart_ptr = huart;

  log_info("Uart Test Start");

  while (1)
  {
    uint8_t buffer[BUFFERSIZE] = {0};

    log_info("Rx");
    HAL_UART_Receive_IT(ghwuart_ptr, (uint8_t *)buffer, sizeof(buffer));
    log_debug(WaitForUartReadyRx() ? "Rx Passed" : "Rx Fail");
    log_hex_dump_info("Rx", buffer, sizeof(buffer));

    log_info("Tx");
    HAL_UART_Transmit_IT(ghwuart_ptr, (uint8_t *)buffer, sizeof(buffer));
    log_debug(WaitForUartReadyTx() ? "Tx Passed" : "Tx Fail");
    log_hex_dump_info("Tx", buffer, sizeof(buffer));
  }

  return;
}