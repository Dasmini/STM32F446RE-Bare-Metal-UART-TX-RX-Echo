# STM32F446RE Bare-Metal UART TX + RX Echo

A bare-metal firmware project that implements UART transmit and receive on
an STM32 Nucleo-F446RE using **direct register writes only** — no HAL, no
CubeMX, no standard library.

## What it does

The STM32 receives characters typed into a PC serial terminal over
USART2, and echoes each one straight back — proving a full TX + RX round
trip implemented entirely at the register level.

```
PC types 'A' → USART2 RX (PA3) → STM32 reads it → USART2 TX (PA2) → PC terminal shows 'A'
```

## Hardware

- **Board:** STM32 Nucleo-F446RE
- **MCU:** STM32F446RE (ARM Cortex-M4)
- **UART peripheral:** USART2
- **Pins:** PA2 (TX), PA3 (RX) — routed through the ST-Link's virtual COM
  port, so no external wiring or USB-to-TTL adapter is needed

## What gets configured

1. **GPIOA clock** — enabled via `RCC_AHB1ENR` bit 0 (required before any
   GPIOA register, including `MODER`/`AFRL`, will take effect)
2. **USART2 clock** — enabled via `RCC_APB1ENR` bit 17
3. **PA2/PA3 mode** — set to alternate function (`10`) in `GPIOA_MODER`
4. **PA2/PA3 alternate function** — set to AF7 (USART2) in `GPIOA_AFRL`
5. **Baud rate** — 9600 baud, written to `USART2_BRR` (mantissa/fraction
   calculated from a 16 MHz HSI peripheral clock)
6. **USART2 control** — `UE`, `TE`, `RE` bits set in `USART2_CR1`; word
   length and parity left at reset default (8 data bits, no parity, 1
   stop bit — i.e. 9600 8N1)

## Registers used

| Register         | Address       | Purpose                              |
|-------------------|---------------|----------------------------------------|
| `RCC_AHB1ENR`      | `0x40023830`  | Enable GPIOA peripheral clock          |
| `RCC_APB1ENR`      | `0x40023840`  | Enable USART2 peripheral clock         |
| `GPIOA_MODER`      | `0x40020000`  | Set PA2/PA3 to alternate function mode |
| `GPIOA_AFRL`       | `0x40020020`  | Select AF7 (USART2) on PA2/PA3         |
| `USART2_BRR`       | `0x40004408`  | Baud rate divider                      |
| `USART2_CR1`       | `0x4000440C`  | Enable USART, TX, RX                   |
| `USART2_SR`        | `0x40004400`  | Status flags (`TXE`, `RXNE`)           |
| `USART2_DR`        | `0x40004404`  | Data register (write to send, read to receive) |

Addresses were found in the STM32F446 reference manual (RM0390):
`peripheral_base + register_offset`.

## File structure

```
.
├── main.c            # Register setup (RCC, GPIO, USART2) + main loop
├── main.h             # Base addresses, offsets, bit definitions
├── uart_transmit.c     # uart_send_char() — polls TXE, writes DR
├── uart_recieve.c      # uart_recieve_char() — polls RXNE, reads DR
├── startup.c            # Vector table + Reset_Handler (shared with LED project)
├── linker.ld              # Memory layout, _estack (shared with LED project)
├── makefile                # Build, flash, clean targets
└── README.md
```

## Toolchain

- `arm-none-eabi-gcc` / `arm-none-eabi-objcopy` — compile and convert to
  raw binary
- `st-flash` (from `stlink-tools`) — flash over ST-Link

```bash
sudo apt install gcc-arm-none-eabi stlink-tools
```

> **Note (WSL users):** WSL doesn't pass through USB by default. Use
> [usbipd-win](https://github.com/dorssel/usbipd-win) on the Windows host
> to attach the ST-Link device before flashing:
> ```powershell
> usbipd attach --wsl --busid <BUSID>
> ```
> This needs to be re-run each time the board is unplugged/replugged or
> the WSL session restarts.

## Build & flash

```bash
make          # compiles main.c, uart_transmit.c, uart_recieve.c, startup.c → firmware.bin
make flash    # writes firmware.bin to 0x08000000 via st-flash
make clean    # removes build artifacts
```

Press the board's **RESET** button after flashing to start the new
firmware.

## Testing

Connect a serial terminal at 9600 baud:

```bash
sudo screen /dev/ttyACM0 9600
```

Type a character — the STM32 receives it over RX and sends it back over
TX, so it appears once on screen (`screen` does not locally echo
keystrokes by default, so what you see is the board's echo, confirming
the full round trip).

To exit `screen`: `Ctrl+A` then `K`, then `Y` to confirm.

## Key concepts

- **Peripheral clocks are independent** — GPIOA and USART2 each need
  their own clock enabled via `RCC` before their registers do anything;
  missing either one causes silent failure (writes appear to succeed but
  have no effect).
- **Alternate function mode** — a GPIO pin only comes under a
  peripheral's control once its `MODER` bits are set to `10` *and* the
  correct AF number is selected in `AFRL`/`AFRH`.
- **Clear-then-set** for multi-bit fields (`MODER`, `AFRL`) to avoid
  leaving stale bits from the reset state.
- **Status-flag polling** — both TX (`TXE`) and RX (`RXNE`) are
  implemented as blocking polling loops: wait for the flag, then
  access the data register. This is the simplest form of UART I/O,
  with no interrupts or DMA involved.
- **Baud rate register (`BRR`)** — holds a mantissa/fraction pair
  derived from the peripheral clock and target baud rate, not the baud
  rate itself.