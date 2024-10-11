/* 
 * File:   hal_uart.h
 * Author: AyaAli
 *
 * Created on December 6, 2023, 7:34 PM
 */

#ifndef HAL_UART_H
#define	HAL_UART_H

/*Section : Includes*/
#include "hal_uart_cfg.h"
#include "../../../../Embeded-Compiler/pic/include/proc/pic18f4620.h"
#include "../device_config.h"
#include "../GPIO/hal_gpio.h"
#include "../interrupt/mcal_internal_interrupt.h"
#include "../mcal_std_type.h"

/*Section : Macro Declaration*/
#define EUSART_MODULE_ENABLE        1
#define EUSART_MODULE_DISABLE       0

#define EUSART_SYNCHRONOUSE_MODE     1
#define EUSART_ASYNCHRONOUSE_MODE    0

#define EUSART_ASYNCHRONOUSE_HIGHSPEED   1
#define EUSART_ASYNCHRONOUSE_LOWSPEED    0

#define EUSART_ASYNCHRONOUSE_16BIT_BUADRATE_GEN  1
#define EUSART_ASYNCHRONOUSE_8BIT_BUADRATE_GEN  0

#define EUSART_ASYNCHRONOUSE_TX_ENABLE    1
#define EUSART_ASYNCHRONOUSE_TX_DISABLE   0

#define EUSART_ASYNCHRONOUSE_TX_INTERRUPT_ENABLE    1
#define EUSART_ASYNCHRONOUSE_TX_INTERRUPT_DISABLE   0

#define EUSART_ASYNCHRONOUSE_TX_9Bit_ENABLE    1
#define EUSART_ASYNCHRONOUSE_TX_9Bit_DISABLE   0

#define EUSART_ASYNCHRONOUSE_RX_ENABLE    1
#define EUSART_ASYNCHRONOUSE_RX_DISABLE   0

#define EUSART_ASYNCHRONOUSE_RX_INTERRUPT_ENABLE    1
#define EUSART_ASYNCHRONOUSE_RX_INTERRUPT_DISABLE   0

#define EUSART_ASYNCHRONOUSE_RX_9Bit_ENABLE    1
#define EUSART_ASYNCHRONOUSE_RX_9Bit_DISABLE   0

#define EUSART_FRAMING_ERROR_DETECTED       1
#define EUSART_FRAMING_ERROR_CLEARED        0

#define EUSART_OVERRUN_ERROR_DETECTED       1
#define EUSART_OVERRUN_ERROR_CLEARED        0

/*Section : Macro Function Declaration*/
/*Section : Data Type Declaration*/
typedef enum{
    BAUDRATE_ASYNC_8BIT_LOW_SPEED,
    BAUDRATE_ASYNC_8BIT_HIGH_SPEED,
    BAUDRATE_ASYNC_16BIT_LOW_SPEED,
    BAUDRATE_ASYNC_16BIT_HIGH_SPEED,
    BAUDRATE_SYNC_8BIT,
    BAUDRATE_SYNC_16BIT        
}baudrate_gen_t;

typedef struct {
    uint_8 usart_tx_reserved:5;
    uint_8 usart_tx_enable:1;
    uint_8 usart_tx_interrupt_enable:1;
    uint_8 usart_tx_9bit_enable:1;
    Interrupt_Periority_cfg  usart_tx_priority;
}usart_tx_cfg_t;

typedef struct {
    uint_8 usart_rx_reserved:5;
    uint_8 usart_rx_enable:1;
    uint_8 usart_rx_interrupt_enable:1;
    uint_8 usart_rx_9bit_enable:1;
    Interrupt_Periority_cfg  usart_rx_priority;
}usart_rx_cfg_t;

typedef union{
    struct{
        uint_8 usart_tx_reserved:6;
        uint_8 usart_ferr:1;
        uint_8 usart_oerr:1;
    };
    uint_8 status;
}usart_error_status_t;


typedef struct{
    uint_32 baudrate;
    baudrate_gen_t baudrate_gen_cfg;
    usart_tx_cfg_t tx_cfg;
    usart_rx_cfg_t rx_cfg;
    usart_error_status_t error_status;
    void(*EUSART_TX_DEFUALTINTERRUPT_HUNDELER)(void);
    void(*EUSART_RX_DEFUALTINTERRUPT_HUNDELER)(void);    
    void(*EUSART_Framming_Error_HUNDELER)(void);
    void(*EUSART_Overrun_Error_HUNDELER)(void);
}usart_t;
/*Section : Function Declaration*/
Std_ReturnType USART_ASYNC_Init(const usart_t *eusar);
Std_ReturnType USART_ASYNC_DeInit(const usart_t *eusar);


Std_ReturnType USART_ASYNC_ReadByte_Blocking(uint_8 *data);
Std_ReturnType USART_ASYNC_ReadByte_Non_Blocking(uint_8 *data);
Std_ReturnType USART_ASYNC_Rx_Reset(void);

Std_ReturnType USART_ASYNC_WriteByte_Blocking(uint_8 data);
Std_ReturnType USART_ASYNC_WriteByte_Non_Blocking(uint_8 data);
Std_ReturnType USART_ASYNC_WriteString_Blocking(uint_8 *data, uint_16 str_len);
Std_ReturnType USART_ASYNC_WriteString_Non_Blocking(uint_8 *data, uint_16 str_len);
#endif	/* HAL_UART_H */

