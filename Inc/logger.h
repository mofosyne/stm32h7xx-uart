/*
  Brian Khuu 2019
  Written for stm32h7

  stm32h7 tip: https://www.st.com/content/st_com/en/support/learning/stm32-education/stm32-online-training/stm32h7-online-training.html
*/
#include "stm32h7xx_hal.h"
/***********************
  LOGGER
************************/
typedef enum log_type_t
{
  LOGGER_LOG_PANIC  = -2 ,
  LOGGER_LOG_FATAL  = -1 ,
  LOGGER_LOG_ERROR  =  0 ,
  LOGGER_LOG_WARN   =  1 ,
  LOGGER_LOG_INFO   =  2 ,
  LOGGER_LOG_DEBUG  =  3 ,
  LOGGER_LOG_TRACE  =  4 ,
  LOGGER_LOG_RAW    =  5
} log_type_t;

void log_record(const int type, const char* format, ...);
void log_hex_dump(const int type, const char* annotate, const void *addr, const uint32_t nBytes);

#if 1
/* log_record */
#define log_raw(   MSG, ...)  log_record(LOGGER_LOG_RAW,   MSG     , ##__VA_ARGS__)
#define log_trace( MSG, ...)  log_record(LOGGER_LOG_TRACE, MSG "\n", ##__VA_ARGS__)
#define log_debug( MSG, ...)  log_record(LOGGER_LOG_DEBUG, MSG "\n", ##__VA_ARGS__)
#define log_info(  MSG, ...)  log_record(LOGGER_LOG_INFO,  MSG "\n", ##__VA_ARGS__)
#define log_warn(  MSG, ...)  log_record(LOGGER_LOG_WARN,  MSG "\n", ##__VA_ARGS__)
#define log_err(   MSG, ...)  log_record(LOGGER_LOG_ERROR, MSG "\n", ##__VA_ARGS__)

/* log_hex_dump */
#define log_hex_dump_raw(   MSG, addr, nBytes)  log_hex_dump(LOGGER_LOG_RAW,   MSG, addr, nBytes)
#define log_hex_dump_trace( MSG, addr, nBytes)  log_hex_dump(LOGGER_LOG_TRACE, MSG, addr, nBytes)
#define log_hex_dump_debug( MSG, addr, nBytes)  log_hex_dump(LOGGER_LOG_DEBUG, MSG, addr, nBytes)
#define log_hex_dump_info(  MSG, addr, nBytes)  log_hex_dump(LOGGER_LOG_INFO,  MSG, addr, nBytes)
#define log_hex_dump_warn(  MSG, addr, nBytes)  log_hex_dump(LOGGER_LOG_WARN,  MSG, addr, nBytes)
#define log_hex_dump_err(   MSG, addr, nBytes)  log_hex_dump(LOGGER_LOG_ERROR, MSG, addr, nBytes)

#else // Log Disabled
#define log_raw(...)
#define log_trace(...)
#define log_debug(...)
#define log_info(...)
#define log_err(...)
#define log_warn(...)
#define log_hex_dump_raw(...)
#define log_hex_dump_trace(...)
#define log_hex_dump_debug(...)
#define log_hex_dump_info(...)
#define log_hex_dump_warn(...)
#define log_hex_dump_err(...)
#endif