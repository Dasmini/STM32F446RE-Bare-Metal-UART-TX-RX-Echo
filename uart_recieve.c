#include <stdint.h>
#include "main.h"

void USART2_IRQHandler(void)
{
    volatile uint32_t *gpio_a_odr = (volatile uint32_t *)GPIOA_ODR;
    *gpio_a_odr ^= (1 << 5);                // toggle LED to confirm ISR is firing
    volatile uint32_t *usart_dr = (volatile uint32_t*)USART_DR;
    volatile uint32_t *usart_sr = (volatile uint32_t*)USART_SR;

    if (*usart_sr & ( 1 << RXNE_BIT))
    {
        rx_data = *usart_dr;
        rx_flag = 1;
    } 
    
}


// Polling approach for Reference

char uart_recieve_char(void)
{
    volatile uint32_t *gpio_a_odr = (volatile uint32_t *)GPIOA_ODR;
    volatile uint32_t *usart_sr = (volatile uint32_t*)USART2_BASE_ADDRESS;
    volatile uint32_t *usart_dr = (volatile uint32_t*)(USART2_BASE_ADDRESS + USART_DR_OFFSET);

    while(!(*usart_sr & (1 << RXNE_BIT)))
    {
        //wait
    }
    *gpio_a_odr ^= (1 << 5);
    return *usart_dr;
}
