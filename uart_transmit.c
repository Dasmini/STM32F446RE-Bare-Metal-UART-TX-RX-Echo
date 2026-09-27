#include <stdint.h>
#include "main.h"

void uart_send_char(char ch)
{
    volatile uint32_t *usart_sr = (volatile uint32_t*)USART2_BASE_ADDRESS;
    volatile uint32_t *usart_dr = (volatile uint32_t*)(USART2_BASE_ADDRESS + USART_DR_OFFSET);

    while(!(*usart_sr & (1 << TXE_BIT)))
    {
        //wait
    }
    *usart_dr = ch;

}
