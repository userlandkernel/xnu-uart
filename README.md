# XNU UART Interfacing with task_for_pid(0)
Feed me tfp0 and I give you UART.

## About
- Not depending on a kexecute strategy
- Implemented without controlling the PC, only memory access via mach_vm_read / mach_vm_write
- Should work on A12 and A13
- Never will write to kernel memory (thus never will trigger ktrr), only writes to UART registers mapped by ml_io_map

## Depends
- uart physical base address (can be found in device tree)
- gPEClockFrequencyInfo (a kernel structure offset)

## How to find gPEClockFrequencyInfo structure
- Find the string "memory-frequency"
- There should be one xref, that xref is PE_init_platform (includes the logic from PE_identify_machine)
- Go into that xref, and find the variable == 8 check
- The next lines should be ```gPEClockFrequencyInfo.fix_frequency_hz = *(unsigned long long *)value;```
- Use the structure from this repo to calculate the offset of this element ```offsetof(struct clock_frequency_info_t, fix_frequency_hz)```
- Subtract this element offset from the address and you'll have the address of the struct which is the offset

pe_identify_machine(boot_args * bootArgs)

## Future developments
- tfpzero-less uart interfacing (with user provided kernel r/w primitive functions)
- Creating UART devices in /dev/
