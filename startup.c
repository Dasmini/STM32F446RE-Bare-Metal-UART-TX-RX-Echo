extern int main(void);
extern unsigned int _estack;

void Reset_Handler(void) {
    main();
    while (1);
}

__attribute__((section(".isr_vector")))
void (* const vector_table[])(void) = {
    (void (*)(void))&_estack,   // initial stack pointer
    Reset_Handler                // reset handler
};