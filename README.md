# xnu-uart
Feed me tfp0 and I give you UART.

## About
- Not depending on a kexecute strategy
- Implemented without controlling the PC, only memory access via mach_vm_read / mach_vm_write
- Should work on A12 and A13
- Never will write to kernel memory (thus never will trigger ktrr), only writes to UART registers mapped by ml_io_map

## Future developments
- tfpzero-less uart interfacing (with user provided kernel r/w primitive functions)
- Creating UART devices in /dev/
