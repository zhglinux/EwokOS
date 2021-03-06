#ifndef MMU_H
#define MMU_H

#include <types.h>

#define KB 1024
#define MB (1024*KB)
#define GB (1024*MB)

#define PAGE_SIZE (4*KB)
#define PAGE_TABLE_SIZE (1*KB)

#define PAGE_DIR_NUM 4096
#define PAGE_DIR_SIZE (16*KB)

#define KERNEL_BASE 0x80000000 //=2G
#define MMIO_BASE (KERNEL_BASE + 1*GB)
#define ALLOCATABLE_MEMORY_START ((uint32_t)_kernelEnd +  2*MB)
#define INTERRUPT_VECTOR_BASE 0xffff0000

#define KERNEL_STACK_BOTTOM (KERNEL_BASE - 2 * PAGE_SIZE)
#define USER_STACK_BOTTOM (KERNEL_BASE - 3 * PAGE_SIZE)

#define INIT_MEMORY_SIZE (32*MB)

#define ALIGN_DOWN(x, alignment) ((x) & ~(alignment - 1))
#define ALIGN_UP(x, alignment) (((x) + alignment - 1) & ~(alignment - 1))

#define V2P(V) ((uint32_t)V - KERNEL_BASE)
#define P2V(P) ((uint32_t)P + KERNEL_BASE)

/* descriptor types */
#define PAGE_TYPE 2
#define PAGE_DIR_TYPE 1

/* access permissions */
#define AP_RW_D 0x55
#define AP_RW_R 0xaa
#define AP_RW_RW 0xff

#define PAGE_DIR_INDEX(x) ((uint32_t)x >> 20)
#define PAGE_INDEX(x) (((uint32_t)x >> 12) & 255)

#define PAGE_TABLE_TO_BASE(x) ((uint32_t)x >> 10)
#define BASE_TO_PAGE_TABLE(x) ((void *) ((uint32_t)x << 10))
#define PAGE_TO_BASE(x) ((uint32_t)x >> 12)

#ifndef __ASSEMBLER__

/* a 32-bit entry in hardware's PageDir table */
typedef struct {
	unsigned int type : 2;
	unsigned int : 3;
	unsigned int domain : 4;
	unsigned int : 1;
	unsigned int base : 22;
} PageDirEntryT;

/* a 32-bit entry in hardware's page table */
typedef struct {
	unsigned int type : 2;
	unsigned int bufferable : 1;
	unsigned int cacheable : 1;
	unsigned int permissions : 8;
	unsigned int base : 20;
} PageTableEntryT; 

/* to hold information about a mapping */
typedef struct {
	uint32_t vAddr;
	uint32_t pStart;
	uint32_t pEnd;
	int permissions;
} MemoryMapT ;

void mapPages(PageDirEntryT *vm, MemoryMapT mapping);
void mapPage(PageDirEntryT *vm, uint32_t physical,
		     uint32_t virtual_addr, int access_permissions);
void unmapPage(PageDirEntryT *vm, uint32_t virtual_addr);

#endif

#endif
