/*
  Brian Khuu 2019
  Written for stm32h7

  stm32h7 tip: https://www.st.com/content/st_com/en/support/learning/stm32-education/stm32-online-training/stm32h7-online-training.html
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h> //isprint

#include "logger.h"
#include "stm32h7xx_hal.h"

/***********************
  LOGGER
************************/

log_type_t log_level = LOGGER_LOG_RAW;

#include <stdarg.h>
#include <ctype.h> //isprint

static void log_tx(const char* str)
{
#if 0
  /* Check if debugger is attached */
  if (!((CoreDebug->DHCSR & 1) == 1 ))
    return;
  /* Calculate String Length */
  int len = 0;
  for (len = 0; str[len]; (len)++);

  /* Call Semihost */
  int args[3] = {2 /*stderr*/, (int) str, (int)len};
  asm volatile (
    " mov r0, %[reason]  \n"
    " mov r1, %[arg]  \n"
    " bkpt %[swi] \n"
    : /* Output */
    : [reason] "r" (0x05), [arg] "r" (args), [swi] "i" (0xAB) /* Inputs */
    : "r0", "r1", "r2", "r3", "ip", "lr", "memory", "cc"
  );
#else
  for (int i = 0 ; str[i] ; i++)
  {
    ITM_SendChar(str[i]);
  }
#endif
}

void log_record(const int type, const char* format, ...)
{
  char buff[400] = {0};
  size_t buff_size = sizeof(buff);

  if (log_level < type)
    return;

  /* Check if debugger is attached */
  if (!((CoreDebug->DHCSR & 1) == 1 ))
  {
    return;
  }

  /* Format String */
  va_list argptr;
  va_start(argptr, format);
  vasnprintf(buff, &buff_size, format, argptr);
  va_end(argptr);

  /* Print Log Type */
  switch (type)
  {
    case LOGGER_LOG_PANIC :
      log_tx("P: ");
      break;
    case LOGGER_LOG_FATAL :
      log_tx("F: ");
      break;
    case LOGGER_LOG_ERROR :
      log_tx("E: ");
      break;
    case LOGGER_LOG_WARN  :
      log_tx("W: ");
      break;
    case LOGGER_LOG_INFO  :
      log_tx("I: ");
      break;
    case LOGGER_LOG_DEBUG :
      log_tx("D: ");
      break;
    case LOGGER_LOG_TRACE :
      log_tx("T: ");
      break;
  }

  log_tx(buff);
}

void log_hex_dump(const int type, const char* annotate, const void *addr, const uint32_t nBytes)
{
  const uint8_t byte_per_row = 16;
  const uint8_t *addr_ptr = addr;

  if (log_level < type)
    return;

  /* Print Log Type */
  switch (type)
  {
    case LOGGER_LOG_PANIC :
      log_tx("P: ");
      break;
    case LOGGER_LOG_FATAL :
      log_tx("F: ");
      break;
    case LOGGER_LOG_ERROR :
      log_tx("E: ");
      break;
    case LOGGER_LOG_WARN  :
      log_tx("W: ");
      break;
    case LOGGER_LOG_INFO  :
      log_tx("I: ");
      break;
    case LOGGER_LOG_DEBUG :
      log_tx("D: ");
      break;
    case LOGGER_LOG_TRACE :
      log_tx("T: ");
      break;
  }

  log_tx(annotate);
  log_tx(" :\n");

  /* All zero check */
  bool allzero = true;
  for (uint32_t i = 0 ; i < nBytes ; i++)
  {
    const uint8_t *addr8 = addr;

    if (addr8[i] != 0)
    {
      allzero = false;
      break;
    }
  }

  if (allzero)
  {
    char buff[400] = {0}; // Line Buff
    char *buff_ptr = buff;
    const char *buff_end = buff + sizeof(buff) - 1;
    buff_ptr += snprintf(buff_ptr, buff_end - buff_ptr, " 0 | 00 ... 00 (%u Bytes)", (unsigned) nBytes);
    log_tx(buff);
    log_tx("\n");
    return;
  }

  /* Print Hex */
  uint32_t offset = 0;
  while (offset < nBytes)
  {
    char buff[400] = {0}; // Line Buff
    char *buff_ptr = buff;
    const char *buff_end = buff + sizeof(buff) - 1;

#if 0
    bool zerorowDetected = true;
    // Zero Row Check
    for (uint8_t i = 0 ; i < byte_per_row ; i++)
    {
      if ((offset + i) >= nBytes)
        break;
      if (addr_ptr[offset + i] != 0)
        zerorowDetected = false;
    }

    if (zerorowDetected)
    {
      buff_ptr += snprintf(buff_ptr, buff_end - buff_ptr, " %4u | 00 ... 00", (unsigned) offset);
      log_tx(buff);
      log_tx("\n");
      offset += byte_per_row;
      continue;
    }
#endif

    // HEX
    buff_ptr += snprintf(buff_ptr, buff_end - buff_ptr, " %4u | ", (unsigned) offset);
    for (uint8_t i = 0 ; i < byte_per_row ; i++)
    {
      if ((offset + i) >= nBytes)
        break;
      buff_ptr += snprintf(buff_ptr, buff_end - buff_ptr, " %02x", addr_ptr[offset + i]);
    }

    // PRINTABLE
    buff_ptr += snprintf(buff_ptr, buff_end - buff_ptr, "  :  ");
    for (uint8_t i = 0 ; i < byte_per_row ; i++)
    {
      if ((offset + i) >= nBytes)
        break;
      char c = isprint(addr_ptr[offset + i]) ? addr_ptr[offset + i] : '.';
      buff_ptr += snprintf(buff_ptr, buff_end - buff_ptr, "%c", c);
    }

    log_tx(buff);
    log_tx("\n");

    // INCREMENT
    offset += byte_per_row;
  }

  log_tx("\n");
}
