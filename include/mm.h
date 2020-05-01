#ifndef	_MM_H
#define	_MM_H

#include "peripherals/base.h"

#define VA_START				0xffff000000000000

#define PAGE_SHIFT				12
#define TABLE_SHIFT 			9
#define SECTION_SHIFT			(PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_MASK				0xfffffffffffff000
#define PAGE_SIZE   			(1 << PAGE_SHIFT)
#define SECTION_SIZE			(1 << SECTION_SHIFT)

#define PA_START                0x60000000
#define LOW_MEMORY              0x70000000
#define HIGH_MEMORY             0x80000000

#define PTRS_PER_TABLE			(1 << TABLE_SHIFT)

#define PGD_SHIFT				PAGE_SHIFT + 3 * TABLE_SHIFT
#define PUD_SHIFT				PAGE_SHIFT + 2 * TABLE_SHIFT
#define PMD_SHIFT				PAGE_SHIFT + 1 * TABLE_SHIFT
#define PG_DIR_SIZE				(3 * PAGE_SIZE)

#define PAGING_MEMORY           (HIGH_MEMORY - LOW_MEMORY)
#define PAGING_PAGES            (PAGING_MEMORY/PAGE_SIZE)

#ifndef __ASSEMBLER__

#include "sched.h"

unsigned long get_free_page();
void free_page(unsigned long p);
void map_page(struct task_struct *task, unsigned long va, unsigned long page);
void memzero(unsigned long src, unsigned long n);
void memcpy(unsigned long dst, unsigned long src, unsigned long n);

int copy_virt_memory(struct task_struct *dst);
unsigned long allocate_kernel_page();
unsigned long allocate_user_page(struct task_struct *task, unsigned long va);

extern unsigned long pg_dir;
extern unsigned long idmap_pg_dir;

#define UART_BASE   0x09000000

#endif

#endif  /*_MM_H */
