#include "kmem.h"

// Globals
mach_port_t gKernelTask = MACH_PORT_NULL;
kptr_t gKernelBase = 0;

/**
 * @function init_tfpzero
 * @param base Slid kernel base address
 */
void init_tfpzero(kptr_t base, task_t tfpzero) {
    gKernelTask = tfpzero;
    gKernelBase = base;
}

/**
 * @function copyin
 * @brief Copy data to userland from the kernel, works like a memcpy
 * @param to Userland address to copy data to
 * @param from Kernel address to read data from
 * @param size Number of bytes to copy
*/
void copyin(void* to, kptr_t from, size_t size) {
    mach_vm_size_t outsize = size;
    size_t szt = size;
    if (size > 0x1000) {
        size = 0x1000;
    }
    size_t off = 0;
    while (1) {
        mach_vm_read_overwrite(gKernelTask, off+from, size, (mach_vm_offset_t)(off+to), &outsize);
        szt -= size;
        off += size;
        if (szt == 0) {
            break;
        }
        size = szt;
        if (size > 0x1000) {
            size = 0x1000;
        }
        
    }
}

/**
 * @function copyout
 * @brief Copy data to the kernel from userland
 * @param to Kernel address to copy data to
 * @param from Userland address to read data from
 * @param size Number of bytes to copy
*/
void copyout(kptr_t to, void* from, size_t size) {
    mach_vm_write(gKernelTask, to, (vm_offset_t)from, (mach_msg_type_number_t)size);
}


/**
 * @function ReadAnywhere64
 * @brief Read 64-bits at a given kernel address
 * @param addr (Slid) kernel address to read from
 * @return 64-bit value
*/
uint64_t ReadAnywhere64(kptr_t addr) {
    uint64_t val = 0;
    copyin(&val, addr, sizeof(uint64_t));
    return val;
}

/**
 * @function WriteAnywhere64
 * @brief Write 64-bits at a given kernel address
 * @param addr (Slid) kernel address to write to
 * @return The written value
 */
uint64_t WriteAnywhere64(kptr_t addr, uint64_t val) {
    copyout(addr, &val, sizeof(uint64_t));
    return val;
}

kptr_t ReadKernelPointer(kptr_t addr) {
    kptr_t val = 0;
    copyin(&val, addr, sizeof(kptr_t));
    return val;
}

kptr_t WriteKernelPointer(kptr_t addr, kptr_t val) {
    copyout(addr, &val, sizeof(kptr_t));
    return val;
}

/**
 * @function ReadAnywhere32
 * @brief Read 32-bits at a given kernel address
 * @param addr (Slid) kernel address to read from
 * @return 32-bit value
*/
uint32_t ReadAnywhere32(kptr_t addr) {
    uint32_t val = 0;
    copyin(&val, addr, sizeof(uint32_t));
    return val;
}

/**
 * @function WriteAnywhere32
 * @brief Write 32-bits at a given kernel address
 * @param addr (Slid) kernel address to write to
 * @return The written value
 */
uint32_t WriteAnywhere32(kptr_t addr, uint32_t val) {
    copyout(addr, &val, sizeof(uint32_t));
    return val;
}

/**
 * @function ReadAnywhere16
 * @brief Read 16-bits at a given kernel address
 * @return 16-bit value
 */
uint16_t ReadAnywhere16(uint32_t addr) {
    uint16_t val = 0;
    copyin(&val, addr, sizeof(uint16_t));
    return val;
}

/**
 * @function WriteAnywhere16
 * @brief Write 16-bits at a given kernel address
 * @return The written value
 */
uint16_t WriteAnywhere16(kptr_t addr, uint16_t val) {
    copyout(addr, &val, sizeof(uint16_t));
    return val;
}

/**
 * @function ReadAnywhere8
 * @brief Read a byte at a given kernel address
 * @return byte value
 */
uint8_t ReadAnywhere8(kptr_t addr) {
    uint8_t val = 0;
    copyin(&val, addr, sizeof(uint8_t));
    return val;
}

/**
 * @function WriteAnywhere8
 * @brief Write a byte at a given kernel address
 * @return The written value
 */
uint8_t WriteAnywhere8(kptr_t addr, uint8_t val) {
    copyout(addr, &val, sizeof(uint8_t));
    return val;
}
