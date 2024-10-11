/* 
 * File:   application.c
 * Author: AyaAli
 * Created on September 22, 2023, 10:24 AM
 */
#include "application.h"

void EUSART_TX_Interrupt_Hundeler(void);
//void EUSART_RX_Interrupt_Hundeler(void);
//void EUSART_Framming_Error_Interrupt_Hundeler(void);
//void EUSART_Overrun_Error_Interrupt_Hundeler(void);
//volatile uint_8 virtual_Tx_counter, virtual_Rx_counter;
usart_t usart_obj;
void usart_int_init(){
    Std_ReturnType ret = E_OK;    
    usart_obj.baudrate = 9600;
    usart_obj.baudrate_gen_cfg = BAUDRATE_ASYNC_8BIT_LOW_SPEED;
    usart_obj.tx_cfg.usart_tx_enable = EUSART_ASYNCHRONOUSE_TX_ENABLE;
    usart_obj.tx_cfg.usart_tx_interrupt_enable = EUSART_ASYNCHRONOUSE_TX_INTERRUPT_DISABLE ;
    usart_obj.tx_cfg.usart_tx_9bit_enable = EUSART_ASYNCHRONOUSE_TX_9Bit_DISABLE ;
    usart_obj.rx_cfg.usart_rx_enable = EUSART_ASYNCHRONOUSE_RX_ENABLE;
    usart_obj.rx_cfg.usart_rx_interrupt_enable = EUSART_ASYNCHRONOUSE_RX_INTERRUPT_DISABLE ;
    usart_obj.rx_cfg.usart_rx_9bit_enable = EUSART_ASYNCHRONOUSE_RX_9Bit_DISABLE ;
    
    usart_obj.EUSART_Framming_Error_HUNDELER = NULL;
    usart_obj.EUSART_Overrun_Error_HUNDELER = NULL;
    usart_obj.EUSART_RX_DEFUALTINTERRUPT_HUNDELER = NULL;
    usart_obj.EUSART_TX_DEFUALTINTERRUPT_HUNDELER = NULL;
    ret = USART_ASYNC_Init(&usart_obj);
};
led_t led1 ={
.Port_Name = PORTD_INDEX,
.pin = PIN0,
.led_statues = LED_OFF
};

//led_t led2 ={
//.Port_Name = PORTD_INDEX,
//.pin = PIN1,
//.led_statues = LED_OFF
//};

uint_8 rec_usart = 0;
int main() {
Std_ReturnType ret = E_NOT_OK;
usart_int_init();
ret = led_initialize(&led1);
//ret = led_initialize(&led2);
while(1){
    ret = USART_ASYNC_ReadByte_Blocking(&rec_usart);
//    __delay_ms(200);
    if(rec_usart == 'a'){
    led_toggle(&led1);
    __delay_ms(200);
    }
    else{}
//    ret = USART_ASYNC_ReadByte_Blocking(&rec_usart);
//    __delay_ms(200);
    
}
 return (EXIT_SUCCESS);
}
void application_initialize(void){
   Std_ReturnType ret = E_NOT_OK;

}
//void EUSART_TX_Interrupt_Hundeler(void){
//    virtual_Tx_counter++;
//    led_toggle(&led1);
//}
//void EUSART_RX_Interrupt_Hundeler(void){
//    Std_ReturnType ret = E_NOT_OK;  
//    virtual_Rx_counter++;
//rec_usart = USART_ASYNC_ReadByte_Non_Blocking(&usart_obj);
//switch(rec_usart){
//    case 'a':
//        ret = led_turn_on(&led1);
//        ret = USART_ASYNC_WriteString_Blocking("led1 on\r",8);
//        break;
//    case 'b':
//       ret = led_turn_off(&led1);
//       ret = USART_ASYNC_WriteString_Blocking("led1 off\r",9);
//        break;
//    case 'c':
//       ret = led_turn_on(&led2); 
//       ret = USART_ASYNC_WriteString_Blocking("led2 on\r",8);
//        break;
//    case 'd':
//       ret = led_turn_off(&led2);
//       ret = USART_ASYNC_WriteString_Blocking("led2 off\r",9);
//        break;
//    default: 
//         ret = led_turn_off(&led1);
//         ret = led_turn_off(&led2);
//         break;
//}
//}
//
//void EUSART_Framming_Error_Interrupt_Hundeler(void){
//    uint_8 data = 0;
//    USART_ASYNC_ReadByte_Non_Blocking(&data);
//
//    
//}
//void EUSART_Overrun_Error_Interrupt_Hundeler(void){
//USART_ASYNC_Rx_Reset();
//}
