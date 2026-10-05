//#include <stdint.h>
//#include "main.h"

extern int main(void);
extern unsigned int _estack;

//volatile uint32_t *gpio_a_odr = (volatile uint32_t *)GPIOA_ODR;

void Reset_Handler(void) {
    main();
    while (1);
}

void Default_Handler(void) {
    //*gpio_a_odr ^= (1 << 5);
    while (1);   // trap here if an unexpected interrupt fires
}

// declare your real handler so the compiler knows about it before the table
void USART2_IRQHandler(void);

__attribute__((section(".isr_vector")))
void (* const vector_table[])(void) = {
    (void (*)(void))&_estack,       // 0: initial stack pointer
    Reset_Handler,                  // 1: Reset

    // Core exceptions (2-15) — using Default_Handler as placeholder for all
    Default_Handler,                // 2: NMI
    Default_Handler,                // 3: HardFault
    Default_Handler,                // 4: MemManage
    Default_Handler,                // 5: BusFault
    Default_Handler,                // 6: UsageFault
    0, 0, 0, 0,                     // 7-10: Reserved
    Default_Handler,                // 11: SVCall
    Default_Handler,                // 12: Debug Monitor
    0,                               // 13: Reserved
    Default_Handler,                // 14: PendSV
    Default_Handler,                // 15: SysTick

    // Peripheral IRQs (16 onward = IRQ0 onward)
    [16 ... 53] = Default_Handler,  // IRQ 0-37, fill with default
    [54] = USART2_IRQHandler,       // IRQ 38 = USART2, index 16+38=54
};