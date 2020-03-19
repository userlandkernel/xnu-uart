#include "kmem.h"

/*
 * Taken from XNU pe_serial.c
*/

// UART register offsets

#ifndef USE_SAMSUNG_UART

typedef uint8_t uart_reg_t;

  #define rULCON    0x00    // UART Line control
  #define rUCON     0x04    // UART Control
  #define rUFCON    0x08    // UART FIFO control
  #define rUMCON    0x0c    // UART Modem control
  #define rUTRSTAT  0x10    // UART Tx/Rx status
  #define rUERSTAT  0x14    // UART Rx error status
  #define rUFSTAT   0x18    // UART FIFO status
  #define rUMSTAT   0x1c    // UART Modem status
  #define rUTXH     0x20    // UART Transmission Hold
  #define rURXH     0x24    // UART Receive buffer
  #define rUBRDIV   0x28    // UART Baud rate divisor
  
#endif

struct pe_serial_functions {
    void            (*uart_init) (void);
    void            (*uart_set_baud_rate) (int unit, uint32_t baud_rate);
    int             (*tr0) (void);
    void            (*td0) (int c);
    int             (*rr0) (void);
    int             (*rd0) (void);
    struct pe_serial_functions *next;
};

typedef struct uart_kernel_symbols {
    kptr_t UART_BASE_PHYS;
    kptr_t UART_BASE_VIRT;
    kptr_t PE_CLOCK_FREQINFO;
} uart_offsets_t;

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

