#ifndef MAIN_H
#define MAIN_H

#define RCC_BASE_ADDRESS            0x40023800
#define GPIOA_BASE_ADDRESS          0x40020000
#define USART2_BASE_ADDRESS         0x40004400
#define RCC_AHB1ENR_OFFSET          0x30
#define RCC_APB1ENR_OFFSET          0x40
#define AFRL_OFFSET                 0x20
#define USART_BRR_OFFSET            0x08
#define USART_CR1_OFFSET            0x0c
#define USART_DR_OFFSET             0x04
#define TX_PIN                      2
#define RX_PIN                      3
#define TE_BIT                      3
#define RE_BIT                      2
#define UE_BIT                      13
#define UART_FRAME_BIT              12
#define PARITY_BIT                  10
#define RXNE_BIT                    5
#define TXE_BIT                     7

void uart_send_char(char ch);

char uart_recieve_char(void);

#endif