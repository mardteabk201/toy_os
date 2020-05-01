#include "arm/mmu.h"
#include "arm/sysregs.h"
#include "mm.h"

.text

.globl _start
_start:
	mrs	x0, mpidr_el1		
	and	x0, x0,#0xFF		// Check processor id
	cbz	x0, master		// Hang for all non-primary CPU
	b	proc_hang

proc_hang: 
	b 	proc_hang

master:
	ldr x0, =SCTLR_VALUE_MMU_DISABLED
	msr sctlr_el1, x0

	mrs	x0, CurrentEL
	lsr	x0, x0, #2
	cmp x0, #3
	beq	el3
	cmp x0, #2
	beq	el2
	cmp x0, #1
	b el1_entry

el2:
	ldr	x0, =HCR_VALUE
	msr	hcr_el2, x0

	mov	x0, #SPSR_VALUE
	msr	spsr_el2, x0

	adr x0, el1_entry
	msr	elr_el2, x0
	eret

el3:
	ldr x0, =HCR_VALUE
	msr scr_el3, x0

	ldr x0, =SCR_VALUE
	msr scr_el3, x0

	adr x0, el1_entry
	msr elr_el3, x0
	eret

el1_entry:
	adr	x0, bss_begin
	adr	x1, bss_end
	sub	x1, x1, x0
	bl 	memzero

	bl 	__create_page_tables

	mov	x0, #VA_START			
	ldr x1, =(0x10000000) //0x70000000 ---> 0xffff000010000000
	add	sp, x0, x1

	adrp	x0, idmap_pg_dir
	msr	ttbr0_el1, x0

	adrp	x0, pg_dir				
	msr	ttbr1_el1, x0

	ldr	x0, =(TCR_VALUE)		
	msr	tcr_el1, x0

	ldr	x0, =(MAIR_VALUE)
	msr	mair_el1, x0

	ldr	x2, =kernel_main

	mov	x0, #SCTLR_MMU_ENABLED				
	msr	sctlr_el1, x0

	br 	x2

	.macro	create_pgd_entry, tbl, virt, tmp1, tmp2
	create_table_entry \tbl, \virt, PGD_SHIFT, \tmp1, \tmp2
	create_table_entry \tbl, \virt, PUD_SHIFT, \tmp1, \tmp2
	.endm

	.macro	create_table_entry, tbl, virt, shift, tmp1, tmp2
	lsr	\tmp1, \virt, #\shift
	and	\tmp1, \tmp1, #PTRS_PER_TABLE - 1			// table index
	add	\tmp2, \tbl, #PAGE_SIZE
	orr	\tmp2, \tmp2, #MM_TYPE_PAGE_TABLE	
	str	\tmp2, [\tbl, \tmp1, lsl #3] /* 将pud的地址填入pgd的那个spot中 */
	add	\tbl, \tbl, #PAGE_SIZE					// next level table page
	.endm

	.macro	create_block_map, tbl, phys, start, end, flags, tmp1
	lsr	\start, \start, #SECTION_SHIFT
	and	\start, \start, #PTRS_PER_TABLE - 1                 // table index
	lsr	\end, \end, #SECTION_SHIFT
	and	\end, \end, #PTRS_PER_TABLE - 1                     // table index
	lsr	\phys, \phys, #SECTION_SHIFT
	mov	\tmp1, #\flags
	orr	\phys, \tmp1, \phys, lsl #SECTION_SHIFT			// table entry
1:
	str	\phys, [\tbl, \start, lsl #3]				// store the entry
	add	\start, \start, #1					// next entry
	add	\phys, \phys, #SECTION_SIZE				// next block
	cmp	\start, \end
	b.ls	1b
	.endm

__create_page_tables:
	mov	x29, x30						// save return address

	adrp	x0, idmap_pg_dir
	mov	x1, #PG_DIR_SIZE
	bl 	memzero

	/* Mapping kernel identically */
	adrp	x0, idmap_pg_dir
	ldr	x1, =(PA_START)
	create_pgd_entry x0, x1, x2, x3

	ldr	x1, =(PA_START)
	ldr	x2, =(PA_START)		// first virtual address
	ldr	x3, =(PA_START + 0x20000000 - 1)	// last virtual address /* 1GB 0x4000 0000 ~ 0x7fff ffff*/
	create_block_map x0, x1, x2, x3, MMU_FLAGS, x4

	/* Mapping kernel */
	adrp	x0, pg_dir
	mov	x1, #PG_DIR_SIZE
	bl 	memzero

	adrp	x0, pg_dir
	mov	x1, #VA_START 
	create_pgd_entry x0, x1, x2, x3

	ldr	x1, =(PA_START)
	mov 	x2, #VA_START		// first virtual address
	ldr	x3, =(VA_START + 0x20000000 - 1)	// last virtual address /* 1GB 0x4000 0000 ~ 0x7fff ffff*/
	create_block_map x0, x1, x2, x3, MMU_FLAGS, x4

	/* Mapping io */
	adrp	x0, pg_dir
	ldr	x1, =(VA_START + 0x28000000)
	create_pgd_entry x0, x1, x2, x3

	ldr	x1, =(0x08000000)
	ldr	x2, =(VA_START + 0x28000000)  /* 0x08000000 ---> 0xffff000028000000 */
	ldr	x3, =(VA_START + 0x2a000000 - 1)
	create_block_map x0, x1, x2, x3, MMU_DEVICE_FLAGS, x4

	mov	x30, x29		// restore return address
	ret
