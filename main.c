#include <stdint.h>
#include "main.h"



//OVER8 and PS is not set as oversampling needs to be 0 and not parity selected respectively.

int main(void)
{
    
    volatile uint32_t *USART2_clock_enable_reg = (volatile uint32_t *)(RCC_BASE_ADDRESS + RCC_APB1ENR_OFFSET);
    *USART2_clock_enable_reg |= (1 << 17);     //17th bit is USART2 clock enable bit

    volatile uint32_t *rcc_ahb1enr = (volatile uint32_t *)(RCC_BASE_ADDRESS + 0x30);

    volatile uint32_t *moder_gpio_a = (volatile uint32_t *)GPIOA_BASE_ADDRESS;
    uint32_t mask;

    volatile uint32_t *gpio_a_AFRL = (volatile uint32_t*)(GPIOA_BASE_ADDRESS + AFRL_OFFSET);

    volatile uint32_t *usart_brr = (volatile uint32_t*)(USART2_BASE_ADDRESS + USART_BRR_OFFSET);

    volatile uint32_t *uart_cr1 = (volatile uint32_t*)(USART2_BASE_ADDRESS + USART_CR1_OFFSET);

    *rcc_ahb1enr |= (1 << 0);   // enable GPIOA clock

    // Configure the moder pins to AF mode
    //0x02 -> 10 is the AF mode

    mask = 0x03 << (TX_PIN * 2);
    *moder_gpio_a &= ~mask;                 //clear respective bits before setting
    mask = 0x03 << (RX_PIN * 2);
    *moder_gpio_a &= ~mask;
    mask = 0x02 << (TX_PIN * 2);            // Mask for TX
    *moder_gpio_a |= mask;
    mask = 0x02 << (RX_PIN * 2);            // Mask for RX
    *moder_gpio_a |= mask;

    // Configure AFR bit
    // 0x07 -> 0111 is for AF7 AF-mode

    mask = 0x07 << (TX_PIN * 4);            // Mask for TX
    *gpio_a_AFRL &= ~mask;                  //clear respective bits before setting
    *gpio_a_AFRL |= mask;
    mask = 0x07 << (RX_PIN * 4);            // Mask for RX
    *gpio_a_AFRL &= ~mask;
    *gpio_a_AFRL |= mask;

    // Configure the BRR
    // BRR = (mantissa << 4) | fraction
    *usart_brr = (104 << 4) | 3;            //calculation in README

    // Enable Transmiter, Reciever and USART

    *uart_cr1 |= (1 << TE_BIT);
    *uart_cr1 |= (1 << RE_BIT);
    *uart_cr1 |= (1 << UE_BIT);

    // Set word length and parity choice

    *uart_cr1 &= ~(1 << UART_FRAME_BIT);        // no need to clear actually
    *uart_cr1 &= ~(1 << PARITY_BIT);            // no need to clear actually

    
    while(1)
    {
        for(volatile uint32_t i = 0; i < 100000; i++);
        char ch = uart_recieve_char();
        uart_send_char(ch);
    }
    
    return 0;
}