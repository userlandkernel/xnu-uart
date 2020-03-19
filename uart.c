#include "uart.h"

uart_offsets_t UART_SYMBOLS = {};

void uart_symbols_init(kptr_t UART_BASE_PHYS, kptr_t UART_BASE_KERNEL){
  UART_SYMBOLS.UART_BASE_PHYS = UART_BASE_PHYS;
  UART_SYMBOLS.UART_BASE_VIRT = ReadKernelPointer(UART_BASE_KERNEL); // Maybe use TTE lookup in the future
}

int uart_get_reg(uart_reg_t reg) {
  if(reg == rUTXH) {
    return ReadAnywhere8(UART_SYMBOLS.UART_BASE_VIRT+rUTXH);
  }
  return ReadAnywhere32(UART_SYMBOLS.UART_BASE_VIRT+reg);
}

void uart_set_reg(uart_reg_t reg, int value) {
  if(reg == rUTXH) {
    WriteAnywhere32(UART_SYMBOLS.UART_BASE_VIRT+rUTXH, (uint8_t)(value & 0xff));
    return;
  }
  WriteAnywhere32(UART_SYMBOLS.UART_BASE_VIRT+reg, (uint8_t)(value & 0xff));
}

static void ln2410_uart_set_baud_rate(uint32_t baud_rate);

static void
ln2410_uart_init(void)
{
    uint32_t ucon = 0x405; /* NCLK, No interrupts, No DMA - just polled */
    uart_set_reg(rULCON, 0x03); /* 81N, not IR */
    
    // Override with pclk dt entry
    // if (dt_pclk != -1) {
    //    ucon = ucon & ~0x400;
    // }
    
    uart_set_reg(rUCON, ucon);
    uart_set_reg(rUMCON, 0x00); /* Clear Flow Control */
    
    ln2410_uart_set_baud_rate(115200);
    
    uart_set_reg(rUFCON, 0x03); /* Clear & Enable FIFOs */
    uart_set_reg(rUMCON, 0x01); /* Assert RTS on UART1 */
}
