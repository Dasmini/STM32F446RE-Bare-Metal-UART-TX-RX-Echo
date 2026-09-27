#include <stdint.h>
#include "main.h"

char uart_recieve_char(void)
{
    volatile uint32_t *usart_sr = (volatile uint32_t*)USART2_BASE_ADDRESS;
    volatile uint32_t *usart_dr = (volatile uint32_t*)(USART2_BASE_ADDRESS + USART_DR_OFFSET);

    while(!(*usart_sr & (1 << RXNE_BIT)))
    {
        //wait
    }
    return *usart_dr;
}