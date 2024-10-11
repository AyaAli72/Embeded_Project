/* 
 * File:   hal_uart.c
 * Author: AyaAli
 *
 * Created on December 6, 2023, 7:34 PM
 */

#include "hal_uart.h"

#ifdef EUSART_TX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE 
    static void(*EUSART_TX_Interrupt_Hundeler)(void) = NULL;
#endif    
#ifdef EUSART_RX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE 
    static void(*EUSART_RX_Interrupt_Hundeler)(void) = NULL;
    static void(*EUSART_Framig_error_Interrupt_Hundeler)(void) = NULL;
    static void(*EUSART_Overrun_error_Interrupt_Hundeler)(void) = NULL;
#endif   
    
static void EUSART_BaudRate_Calculations(const usart_t *eusar);
static void EUSART_ASYNC_RX_Init(const usart_t *eusar);
static void EUSART_ASYNC_TX_Init(const usart_t *eusar);

Std_ReturnType USART_ASYNC_Init(const usart_t *eusar){
Std_ReturnType ret = E_NOT_OK;

if(eusar == NULL){
ret = E_NOT_OK;
}
else{
    RCSTAbits.SPEN = EUSART_MODULE_DISABLE;
    TRISCbits.RC7 = 1; 
    TRISCbits.RC6 = 1;
    EUSART_BaudRate_Calculations(eusar);
    EUSART_ASYNC_TX_Init(eusar);
    EUSART_ASYNC_RX_Init(eusar);
    RCSTAbits.SPEN = EUSART_MODULE_ENABLE;
    ret = E_OK;
}
return ret;
}

Std_ReturnType USART_ASYNC_DeInit(const usart_t *eusar){
Std_ReturnType ret = E_NOT_OK;

if(eusar == NULL){
ret = E_NOT_OK;
}
else{
     RCSTAbits.SPEN = EUSART_MODULE_DISABLE;
     ret = E_OK;
}
return ret;
}

Std_ReturnType USART_ASYNC_ReadByte_Blocking( uint_8 *data){
Std_ReturnType ret = E_OK;
 while(!PIR1bits.RCIF);
    *data = RCREG;
    return ret;
}

Std_ReturnType USART_ASYNC_ReadByte_Non_Blocking(uint_8 *data){
    Std_ReturnType ret = E_NOT_OK;
    if(PIR1bits.RCIF == 1){
        *data = RCREG;
        ret = E_OK;
    }
    else{
        ret = E_NOT_OK;
    }
    return ret;

}

Std_ReturnType USART_ASYNC_Rx_Reset(void){
Std_ReturnType ret = E_OK;
RCSTAbits.CREN = 0;
RCSTAbits.CREN = 1;
return ret;
}


Std_ReturnType USART_ASYNC_WriteByte_Blocking(uint_8 data){
Std_ReturnType ret = E_OK;
    while(!TXSTAbits.TRMT);
#ifdef EUSART_TX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE 
        EUSART_TX_INTERRUPT_ENABLE();
#endif        
    TXREG = data;
    return ret;
}
Std_ReturnType USART_ASYNC_WriteByte_Non_Blocking(uint_8 data){
Std_ReturnType ret = E_NOT_OK;
 if( TXSTAbits.TRMT == 1){
     TXREG = data;
     ret = E_OK;
 }
 else{
     ret = E_NOT_OK;
 }
return ret;
}

Std_ReturnType USART_ASYNC_WriteString_Non_Blocking(uint_8 *data, uint_16 str_len){
Std_ReturnType ret = E_OK;
uint_16 L_str_length = ZERO_INIT;
if(TXSTAbits.TRMT == 1){
for(L_str_length = ZERO_INIT ;  L_str_length < str_len ; L_str_length++){
USART_ASYNC_WriteByte_Blocking(data[L_str_length]);
}
}
else{ret = E_NOT_OK;}
return ret;
    
}

Std_ReturnType USART_ASYNC_WriteString_Blocking(uint_8 *data, uint_16 str_len){
    Std_ReturnType ret = E_OK;
    uint_16 L_str_length = ZERO_INIT;
        for(L_str_length = ZERO_INIT ;  L_str_length < str_len ; L_str_length++){
            ret = USART_ASYNC_WriteByte_Blocking(data[L_str_length]);
        }
        return ret;
}

static void EUSART_BaudRate_Calculations(const usart_t *eusar){
    float BaudRate_Temp = 0;
    switch(eusar->baudrate_gen_cfg){
        case BAUDRATE_ASYNC_8BIT_LOW_SPEED:
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUSE_MODE;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUSE_LOWSPEED;
            BAUDCONbits.BRG16 = EUSART_ASYNCHRONOUSE_8BIT_BUADRATE_GEN;
            BaudRate_Temp = ((_XTAL_FREQ /(float)eusar->baudrate)/ 64) - 1;
            break;
        case BAUDRATE_ASYNC_8BIT_HIGH_SPEED:
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUSE_MODE;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUSE_HIGHSPEED;
            BAUDCONbits.BRG16 = EUSART_ASYNCHRONOUSE_8BIT_BUADRATE_GEN;
            BaudRate_Temp = ((_XTAL_FREQ /(float)eusar->baudrate)/ 16) - 1;
            break;    
        case BAUDRATE_ASYNC_16BIT_LOW_SPEED:
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUSE_MODE;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUSE_LOWSPEED;
            BAUDCONbits.BRG16 = EUSART_ASYNCHRONOUSE_16BIT_BUADRATE_GEN;
            BaudRate_Temp = ((_XTAL_FREQ /(float)eusar->baudrate)/ 16) - 1;
            break;
        case BAUDRATE_ASYNC_16BIT_HIGH_SPEED:
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUSE_MODE;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUSE_HIGHSPEED;
            BAUDCONbits.BRG16 = EUSART_ASYNCHRONOUSE_16BIT_BUADRATE_GEN;
             BaudRate_Temp = ((_XTAL_FREQ /(float)eusar->baudrate)/ 4) - 1;
            break;
        case BAUDRATE_SYNC_8BIT:
            TXSTAbits.SYNC = EUSART_SYNCHRONOUSE_MODE;
            BAUDCONbits.BRG16 = EUSART_ASYNCHRONOUSE_8BIT_BUADRATE_GEN;
            BaudRate_Temp = ((_XTAL_FREQ /(float)eusar->baudrate)/ 4) - 1;
            break;
        case BAUDRATE_SYNC_16BIT:
            TXSTAbits.SYNC = EUSART_SYNCHRONOUSE_MODE;
            BAUDCONbits.BRG16 = EUSART_ASYNCHRONOUSE_16BIT_BUADRATE_GEN; 
            BaudRate_Temp = ((_XTAL_FREQ /(float)eusar->baudrate)/ 4) - 1;
            break;    
        default: ;    
        SPBRG = (uint_8)((uint_32)BaudRate_Temp);
        SPBRGH = (uint_8)(((uint_32)BaudRate_Temp) >> 8); 
    }
}

static void EUSART_ASYNC_TX_Init(const usart_t *eusar){
    if(eusar->tx_cfg.usart_tx_enable == EUSART_ASYNCHRONOUSE_TX_ENABLE){
        TXSTAbits.TXEN = EUSART_ASYNCHRONOUSE_TX_ENABLE;
        EUSART_TX_Interrupt_Hundeler = eusar->EUSART_TX_DEFUALTINTERRUPT_HUNDELER;            
    if(eusar->tx_cfg.usart_tx_interrupt_enable == EUSART_ASYNCHRONOUSE_TX_INTERRUPT_ENABLE){
        PIE1bits.TXIE = EUSART_ASYNCHRONOUSE_TX_INTERRUPT_ENABLE;
#ifdef EUSART_TX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE        
         EUSART_TX_INTERRUPT_ENABLE();      
#ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
  INTERRUPT_PRIORITYLEVEL_ENABLE();
  if(eusar->tx_cfg.usart_tx_priority == Interrupt_Periority_HIGH){
      INTERRUPT_GLOBALEINTERRUPTHIGH_ENABLE() ;
      EUSART_TX_HIGH_PRIORITY_SET() ;
  
  }
  else if(eusar->tx_cfg.usart_tx_priority == Interrupt_Periority_LOW){
      INTERRUPT_GLOBALEINTERRUPTLOW_ENABLE();
      EUSART_TX_LOW_PRIORITY_SET();
  } 
#else   
  INTERRUPT_GLOBALEINTERRUPT_ENABLE();
  INTERRUPT_PERIPHERALINTERRUPT_ENABLE();
#endif       
#endif 
    }
    else if(eusar->tx_cfg.usart_tx_interrupt_enable == EUSART_ASYNCHRONOUSE_TX_INTERRUPT_DISABLE){
       PIE1bits.TXIE = EUSART_ASYNCHRONOUSE_TX_INTERRUPT_DISABLE;
    }
    else{}
    if(eusar->tx_cfg.usart_tx_9bit_enable == EUSART_ASYNCHRONOUSE_TX_9Bit_ENABLE){
        TXSTAbits.TX9 = EUSART_ASYNCHRONOUSE_TX_9Bit_ENABLE;
    }
    else if(eusar->tx_cfg.usart_tx_9bit_enable == EUSART_ASYNCHRONOUSE_TX_9Bit_DISABLE){
        TXSTAbits.TX9 = EUSART_ASYNCHRONOUSE_TX_9Bit_DISABLE;
    }
    else{}
    }
    else{}
}

static void EUSART_ASYNC_RX_Init(const usart_t *eusar){
  if(eusar->rx_cfg.usart_rx_enable == EUSART_ASYNCHRONOUSE_RX_ENABLE){
      RCSTAbits.CREN = EUSART_ASYNCHRONOUSE_RX_ENABLE;
      EUSART_RX_Interrupt_Hundeler = eusar->EUSART_RX_DEFUALTINTERRUPT_HUNDELER;
      EUSART_Framig_error_Interrupt_Hundeler = eusar->EUSART_Framming_Error_HUNDELER;
      EUSART_Overrun_error_Interrupt_Hundeler = eusar->EUSART_Overrun_Error_HUNDELER;
    if(eusar->rx_cfg.usart_rx_interrupt_enable == EUSART_ASYNCHRONOUSE_RX_INTERRUPT_ENABLE){
        PIE1bits.RCIE = EUSART_ASYNCHRONOUSE_RX_INTERRUPT_ENABLE;
        
#ifdef EUSART_RX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE        
         EUSART_RX_INTERRUPT_ENABLE(); 
       
#ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
            INTERRUPT_PRIORITYLEVEL_ENABLE();
            if(eusar->rx_cfg.usart_rx_priority == Interrupt_Periority_HIGH){
                INTERRUPT_GLOBALEINTERRUPTHIGH_ENABLE() ;
                EUSART_RX_HIGH_PRIORITY_SET() ;
            
            }
            else if(eusar->rx_cfg.usart_rx_priority == Interrupt_Periority_LOW){
                INTERRUPT_GLOBALEINTERRUPTLOW_ENABLE();
                EUSART_RX_LOW_PRIORITY_SET();
            } 
#else   
  INTERRUPT_GLOBALEINTERRUPT_ENABLE();
  INTERRUPT_PERIPHERALINTERRUPT_ENABLE();
#endif   
#endif        
    }
    else if(eusar->rx_cfg.usart_rx_interrupt_enable == EUSART_ASYNCHRONOUSE_RX_INTERRUPT_DISABLE){
       PIE1bits.RCIE = EUSART_ASYNCHRONOUSE_RX_INTERRUPT_DISABLE;
    }
    else{}
    if(eusar->rx_cfg.usart_rx_9bit_enable == EUSART_ASYNCHRONOUSE_RX_9Bit_ENABLE){
        RCSTAbits.RX9 = EUSART_ASYNCHRONOUSE_RX_9Bit_ENABLE;
    }
    else if(eusar->rx_cfg.usart_rx_9bit_enable == EUSART_ASYNCHRONOUSE_RX_9Bit_DISABLE){
        RCSTAbits.RX9  = EUSART_ASYNCHRONOUSE_RX_9Bit_DISABLE;
    }
    else{}
    }
    else{}
} 

void EUSART_TX_ISR(void){
    EUSART_TX_INTERRUPT_DISABLE();
    if(EUSART_TX_Interrupt_Hundeler){
    EUSART_TX_Interrupt_Hundeler();
    }else{}

}
void EUSART_RX_ISR(void){ 
   if(EUSART_RX_Interrupt_Hundeler){
    EUSART_RX_Interrupt_Hundeler();
    }else{}
   if(EUSART_Framig_error_Interrupt_Hundeler){
    EUSART_Framig_error_Interrupt_Hundeler();
    }else{}
   if(EUSART_Overrun_error_Interrupt_Hundeler){
    EUSART_Overrun_error_Interrupt_Hundeler();
    }else{}


}