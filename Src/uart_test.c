
#include <stdbool.h>
#include "stm32h7xx_hal.h"
#include "logger.h"

#define TIMEOUT_MS 5000
#define BUFFERSIZE 10

UART_HandleTypeDef *ghwuart_ptr = NULL;

/*** Status Checking ****************************/

char* HAL_StatusTypeDef_to_str(uint8_t val)
{
  switch (val)
  {
    case HAL_OK      : return "HAL_OK";
    case HAL_ERROR   : return "HAL_ERROR";
    case HAL_BUSY    : return "HAL_BUSY";
    case HAL_TIMEOUT : return "HAL_TIMEOUT";
    default: return "";
  }
}

char* uart_state_to_str(uint8_t val)
{
  switch (val)
  {
    case HAL_UART_STATE_RESET      : return "Reset";
    case HAL_UART_STATE_READY      : return "Ready";
    case HAL_UART_STATE_BUSY       : return "Busy";
    case HAL_UART_STATE_BUSY_TX    : return "Busy Tx";
    case HAL_UART_STATE_BUSY_RX    : return "Busy Rx";
    case HAL_UART_STATE_BUSY_TX_RX : return "Busy Tx and Rx";
    case HAL_UART_STATE_TIMEOUT    : return "Timeout";
    case HAL_UART_STATE_ERROR      : return "Error";
    default: return "";
  }
}

char* uart_error_to_str(uint8_t val)
{
  switch (val)
  {
    case HAL_UART_ERROR_NONE : return "No error";
    case HAL_UART_ERROR_PE   : return "Parity error";
    case HAL_UART_ERROR_NE   : return "Noise error";
    case HAL_UART_ERROR_FE   : return "Frame error";
    case HAL_UART_ERROR_ORE  : return "Overrun error";
    case HAL_UART_ERROR_DMA  : return "DMA transfer error";
    case HAL_UART_ERROR_RTO  : return "Receiver Timeout error";
    default: return "";
  }
}

void uart_debug_error()
{
#define checkBitChange(ccount, curr_reg, prev_reg, mask, msg) if ((curr_reg & mask) != (prev_reg & mask)) {log_info("(%d) %s bit %s : %s",ccount, #mask, (curr_reg & mask)?"set":"cleared", msg);}
  static int count = 0;
  static uint32_t prev_CR1 = 0;
  static uint32_t prev_CR2 = 0;
  static uint32_t prev_CR3 = 0;
  static uint32_t prev_ISR = 0;
  uint32_t curr_CR1 = ghwuart_ptr->Instance->CR1;
  uint32_t curr_CR2 = ghwuart_ptr->Instance->CR2;
  uint32_t curr_CR3 = ghwuart_ptr->Instance->CR3;
  uint32_t curr_ISR = ghwuart_ptr->Instance->ISR;

  if (count == 0)
  {
    /******************  Bit definition for USART_CR1 register  *******************/
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_UE             , "USART Enable");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_UESM           , "USART Enable in STOP Mode");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_RE             , "Receiver Enable");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_TE             , "Transmitter Enable");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_IDLEIE         , "IDLE Interrupt Enable");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_RXNEIE_RXFNEIE , "RXNE and RX FIFO Not Empty Interrupt Enable");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_TCIE           , "Transmission Complete Interrupt Enable");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_TXEIE_TXFNFIE  , "TXE and TX FIFO Not Full Interrupt Enable");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_PEIE           , "PE Interrupt Enable");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_PS             , "Parity Selection");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_PCE            , "Parity Control Enable");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_WAKE           , "Receiver Wakeup method");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_M0             , "Word length - Bit 0");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_MME            , "Mute Mode Enable");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_CMIE           , "Character match interrupt enable");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_OVER8          , "Oversampling by 8-bit or 16-bit mode");
//    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_DEDT           , "DEDT[4:0] bits (Driver Enable Deassertion Time)");
//    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_DEAT           , "DEAT[4:0] bits (Driver Enable Assertion Time)");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_RTOIE          , "Receive Time Out interrupt enable");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_EOBIE          , "End of Block interrupt enable");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_M1             , "Word length - Bit 1");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_FIFOEN         , "FIFO mode enable");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_TXFEIE         , "TXFIFO empty interrupt enable");
    checkBitChange(count, curr_CR1, prev_CR1, USART_CR1_RXFFIE         , "RXFIFO Full interrupt enable");
    /******************  Bit definition for USART_CR2 register  *******************/
    checkBitChange(count, curr_CR2, prev_CR2, USART_CR2_SLVEN    , "Synchronous Slave mode Enable");
    checkBitChange(count, curr_CR2, prev_CR2, USART_CR2_DIS_NSS  , "Negative Slave Select (NSS) pin management");
    checkBitChange(count, curr_CR2, prev_CR2, USART_CR2_ADDM7    , "7-bit or 4-bit Address Detection");
    checkBitChange(count, curr_CR2, prev_CR2, USART_CR2_LBDL     , "LIN Break Detection Length");
    checkBitChange(count, curr_CR2, prev_CR2, USART_CR2_LBDIE    , "LIN Break Detection Interrupt Enable");
    checkBitChange(count, curr_CR2, prev_CR2, USART_CR2_LBCL     , "Last Bit Clock pulse");
    checkBitChange(count, curr_CR2, prev_CR2, USART_CR2_CPHA     , "Clock Phase");
    checkBitChange(count, curr_CR2, prev_CR2, USART_CR2_CPOL     , "Clock Polarity");
    checkBitChange(count, curr_CR2, prev_CR2, USART_CR2_CLKEN    , "Clock Enable");
//    checkBitChange(count, curr_CR2, prev_CR2, USART_CR2_STOP     , "STOP[1:0] bits (STOP bits)");
    checkBitChange(count, curr_CR2, prev_CR2, USART_CR2_LINEN    , "LIN mode enable");
    checkBitChange(count, curr_CR2, prev_CR2, USART_CR2_SWAP     , "SWAP TX/RX pins");
    checkBitChange(count, curr_CR2, prev_CR2, USART_CR2_RXINV    , "RX pin active level inversion");
    checkBitChange(count, curr_CR2, prev_CR2, USART_CR2_TXINV    , "TX pin active level inversion");
    checkBitChange(count, curr_CR2, prev_CR2, USART_CR2_DATAINV  , "Binary data inversion");
    checkBitChange(count, curr_CR2, prev_CR2, USART_CR2_MSBFIRST , "Most Significant Bit First");
    checkBitChange(count, curr_CR2, prev_CR2, USART_CR2_ABREN    , "Auto Baud-Rate Enabled");
//    checkBitChange(count, curr_CR2, prev_CR2, USART_CR2_ABRMODE  , "ABRMOD[1:0] bits (Auto Baud-Rate Mode)");
    checkBitChange(count, curr_CR2, prev_CR2, USART_CR2_RTOEN    , "Receiver Time-Out enable");
    checkBitChange(count, curr_CR2, prev_CR2, USART_CR2_ADD      , "Address of the USART node");
    /******************  Bit definition for USART_CR3 register  *******************/
    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_EIE     , "Error Interrupt Enable");
    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_IREN    , "IrDA mode Enable");
    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_IRLP    , "IrDA Low-Power");
    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_HDSEL   , "Half-Duplex Selection");
    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_NACK    , "SmartCard NACK enable");
    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_SCEN    , "SmartCard mode enable");
    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_DMAR    , "DMA Enable Receiver");
    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_DMAT    , "DMA Enable Transmitter");
    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_RTSE    , "RTS Enable");
    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_CTSE    , "CTS Enable");
    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_CTSIE   , "CTS Interrupt Enable");
    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_ONEBIT  , "One sample bit method enable");
    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_OVRDIS  , "Overrun Disable");
    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_DDRE    , "DMA Disable on Reception Error");
    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_DEM     , "Driver Enable Mode");
    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_DEP     , "Driver Enable Polarity Selection");
//    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_SCARCNT , "SCARCNT[2:0] bits (SmartCard Auto-Retry Count)");
//    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_WUS     , "WUS[1:0] bits (Wake UP Interrupt Flag Selection)");
    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_WUFIE   , "Wake Up Interrupt Enable");
    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_TXFTIE  , "TXFIFO threshold interrupt enable");
    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_TCBGTIE , "Transmission Complete before guard time, interrupt enable");
//    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_RXFTCFG , "RXFTCFG [2:0]Receive FIFO threshold configuration");
    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_RXFTIE  , "RXFIFO threshold interrupt enable");
//    checkBitChange(count, curr_CR3, prev_CR3, USART_CR3_TXFTCFG , "TXFIFO [2:0] threshold configuration");
    count++;
  }
  else if (curr_ISR != prev_ISR)
  {
    //log_info("%4d: curr_ISR = (%08lX --> %08lX) changes detected:", (int) count, (long unsigned) prev_ISR, (long unsigned) curr_ISR);
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_PE         , "Parity Error");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_FE         , "Framing Error");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_NE         , "Noise detected Flag");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_ORE        , "OverRun Error");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_IDLE       , "IDLE line detected");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_RXNE_RXFNE , "Read Data Register or RX FIFO Not Empty");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_TC         , "Transmission Complete");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_TXE_TXFNF  , "Transmit Data Register Empty or TX FIFO Not Full Flag");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_LBDF       , "LIN Break Detection Flag");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_CTSIF      , "CTS interrupt flag");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_CTS        , "CTS flag");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_RTOF       , "Receiver Time Out");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_EOBF       , "End Of Block Flag");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_UDR        , "SPI slave underrun error flag");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_ABRE       , "Auto-Baud Rate Error");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_ABRF       , "Auto-Baud Rate Flag");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_BUSY       , "Busy Flag");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_CMF        , "Character Match Flag");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_SBKF       , "Send Break Flag");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_RWU        , "Receive Wake Up from mute mode Flag");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_WUF        , "Wake Up from stop mode Flag");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_TEACK      , "Transmit Enable Acknowledge Flag");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_REACK      , "Receive Enable Acknowledge Flag");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_TXFE       , "TXFIFO Empty");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_RXFF       , "RXFIFO Full Flag");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_TCBGT      , "Transmission complete before guard time Flag");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_RXFT       , "RXFIFO threshold Flag");
    checkBitChange(count, curr_ISR, prev_ISR, USART_ISR_TXFT       , "TXFIFO threshold Flag");
    count++;
  }

  prev_CR1 = curr_CR1;
  prev_CR2 = curr_CR2;
  prev_CR3 = curr_CR3;
  prev_ISR = curr_ISR;
}

void uart_debug_print()
{
#define BOOTLOADER_PRINTREG_MON(X, MSG) \
    {\
      static int count = 0;\
      static uint32_t prev = 0;\
      if (!count) \
        { log_info("%4d: %40s = %08lX : %s", (int) count, (char*) #X, (long unsigned) X, (char*) MSG); count++;}\
      else if (prev != X) \
        { log_info("%4d: %40s = (%08lX --> %08lX) : %s", (int) count, (char*) #X, (long unsigned) prev, (long unsigned) X, (char*) MSG); count++;}\
      prev = X;\
    }
#if 0
  BOOTLOADER_PRINTREG_MON( ghwuart_ptr->TxXferSize        , "UART Tx Transfer size");
  BOOTLOADER_PRINTREG_MON( ghwuart_ptr->TxXferCount       , "UART Tx Transfer Counter");
  BOOTLOADER_PRINTREG_MON( ghwuart_ptr->RxXferSize        , "UART Rx Transfer size");
  BOOTLOADER_PRINTREG_MON( ghwuart_ptr->RxXferCount       , "UART Rx Transfer Counter");
  BOOTLOADER_PRINTREG_MON( ghwuart_ptr->Mask              , "UART Rx RDR register mask");
  BOOTLOADER_PRINTREG_MON( ghwuart_ptr->FifoMode          , "Specifies if the FIFO mode is being used.");
  BOOTLOADER_PRINTREG_MON( ghwuart_ptr->NbRxDataToProcess , "Number of data to process during RX ISR execution");
  BOOTLOADER_PRINTREG_MON( ghwuart_ptr->NbTxDataToProcess , "Number of data to process during TX ISR execution");
  BOOTLOADER_PRINTREG_MON( ghwuart_ptr->Lock              , (ghwuart_ptr->Lock == 0x01) ? "Locked" : "Unlocked");
  BOOTLOADER_PRINTREG_MON( HAL_UART_GetState(ghwuart_ptr) , uart_state_to_str(HAL_UART_GetState(ghwuart_ptr)));
  BOOTLOADER_PRINTREG_MON( ghwuart_ptr->ErrorCode         , uart_error_to_str(ghwuart_ptr->ErrorCode));
#endif
  BOOTLOADER_PRINTREG_MON( ghwuart_ptr->Instance->CR1     , "USART Control register 1");
  BOOTLOADER_PRINTREG_MON( ghwuart_ptr->Instance->CR2     , "USART Control register 2");
  BOOTLOADER_PRINTREG_MON( ghwuart_ptr->Instance->CR3     , "USART Control register 3");
  BOOTLOADER_PRINTREG_MON( ghwuart_ptr->Instance->BRR     , "USART Baud rate register");
  BOOTLOADER_PRINTREG_MON( ghwuart_ptr->Instance->GTPR    , "USART Guard time and prescaler register");
  BOOTLOADER_PRINTREG_MON( ghwuart_ptr->Instance->RTOR    , "USART Receiver Time Out register");
  BOOTLOADER_PRINTREG_MON( ghwuart_ptr->Instance->RQR     , "USART Request register");
  BOOTLOADER_PRINTREG_MON( ghwuart_ptr->Instance->ISR     , "USART Interrupt and status register");
  //BOOTLOADER_PRINTREG_MON( ghwuart_ptr->Instance->ICR     , "USART Interrupt flag Clear register");
  //BOOTLOADER_PRINTREG_MON( ghwuart_ptr->Instance->RDR     , "USART Receive Data register");
  //BOOTLOADER_PRINTREG_MON( ghwuart_ptr->Instance->TDR     , "USART Transmit Data register");
  BOOTLOADER_PRINTREG_MON( ghwuart_ptr->Instance->PRESC   , "USART clock Prescaler register");
}

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

void uart_ISR_test(UART_HandleTypeDef *huart)
{
  HAL_StatusTypeDef halStatus;
  ghwuart_ptr = huart;

  log_info("Uart Test Start");

  while (1)
  {
    //uint8_t buffer[256] = {0};
    uint8_t buffer[256 - 12] = {0};

    HAL_UART_Abort(ghwuart_ptr);
    log_info("************************* Rx *********************************");

    halStatus = HAL_UART_Receive_IT(ghwuart_ptr, (uint8_t *)buffer, sizeof(buffer));
    if (!WaitForUartReadyRx())
    {
      log_err("Rx Timeout (%s)", HAL_StatusTypeDef_to_str(halStatus));
    }
    else
    {
      log_err("Rx Received (%s)", HAL_StatusTypeDef_to_str(halStatus));
    }
    for (uint32_t i = 0 ; i < sizeof(buffer) ; i++)
    {
      if (buffer[i] != ((i % 9) + 1))
      {
        log_err("Got only %u valid bytes out of %u", (unsigned) i, sizeof(buffer));
        break;
      }
      else if ((i + 1) >= sizeof(buffer))
      {
        log_info("Got %u valid bytes", sizeof(buffer));
      }
    }
    HAL_Delay(100);
    log_hex_dump_info("Rx", buffer, sizeof(buffer));
    HAL_Delay(500);

#if 1
    log_info("************************* Tx *********************************");
    uint8_t testmessage[] = "Testing!!\r\n";

    halStatus = HAL_UART_Transmit_IT(ghwuart_ptr, (uint8_t *)testmessage, sizeof(testmessage));
    if (!WaitForUartReadyTx())
    {
      log_err("Tx Fail (%s)", HAL_StatusTypeDef_to_str(halStatus));
    }

    HAL_Delay(100);
#endif

  }

  return;
}

























