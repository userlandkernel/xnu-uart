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
