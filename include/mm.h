#ifndef	_MM_H
#define	_MM_H

#include "peripherals/base.h"

#define PAGE_SHIFT				12
#define TABLE_SHIFT 			9
#define SECTION_SHIFT			(PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE   			(1 << PAGE_SHIFT)
#define SECTION_SIZE			(1 << SECTION_SHIFT)

#define LOW_MEMORY              0x50000000
#define HIGH_MEMORY				0x60000000

#define PAGING_MEMORY           (HIGH_MEMORY - LOW_MEMORY)
#define PAGING_PAGES            (PAGING_MEMORY/PAGE_SIZE)

#ifndef __ASSEMBLER__

void memzero(unsigned long src, unsigned long n);

unsigned long get_free_page();
void free_page(unsigned long p);

#define UART_BASE   0x09000000

#endif

#endif  /*_MM_H */
