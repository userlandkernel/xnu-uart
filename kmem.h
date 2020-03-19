#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mach/mach.h>

#ifdef __arm__
typedef uint32_t long kptr_t; // Kernel pointer
#endif

#ifdef __aarch64__
typedef uint64_t kptr_t;
#endif

void init_tfpzero(kptr_t base, task_t tfpzero);

/* Missing from SDK */
kern_return_t mach_vm_read_overwrite(vm_map_t target_task, mach_vm_address_t address, mach_vm_size_t size, mach_vm_address_t data, mach_vm_size_t *outsize);
kern_return_t mach_vm_write(vm_map_t target_task, mach_vm_address_t address, vm_offset_t data, mach_msg_type_number_t dataCnt);
kern_return_t mach_vm_protect(vm_map_t target_task, mach_vm_address_t address, mach_vm_size_t size, boolean_t set_maximum, vm_prot_t new_protection);
kern_return_t mach_vm_allocate(vm_map_t target, mach_vm_address_t *address, mach_vm_size_t size, int flags);
kern_return_t mach_vm_deallocate(vm_map_t target, mach_vm_address_t address, mach_vm_size_t size);
kern_return_t mach_vm_region(vm_map_t target_task, mach_vm_address_t *address, mach_vm_size_t *size, vm_region_flavor_t flavor, vm_region_info_t info, mach_msg_type_number_t *infoCnt, mach_port_t *object_name);
kern_return_t mach_vm_read(vm_map_t target_task, mach_vm_address_t address, mach_vm_size_t size, vm_offset_t *data, mach_msg_type_number_t *dataCnt);

/* -- yalu102 by qwertyoruiop -- */
void copyin(void* to, kptr_t from, size_t size);
void copyout(kptr_t to, void* from, size_t size);
uint32_t ReadAnywhere32(kptr_t addr);
uint32_t WriteAnywhere32(kptr_t addr, uint32_t val);
uint16_t ReadAnywhere16(kptr_t addr);
uint16_t WriteAnywhere16(kptr_t addr, uint16_t val);
uint8_t ReadAnywhere8(uint8_t addr);
uint8_t WriteAnywhere8(kptr_t addr, uint8_t val);

/* userlandkernel functions */
uint32_t kalloc(vm_size_t size);
char* kread(kptr_t addr, size_t size);
void kfree(kptr_t addr, vm_size_t size);
int kstrcmp(kptr_t kstr, const char* str);

