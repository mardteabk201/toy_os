#include <stddef.h>
#include <stdint.h>
#include "printf.h"
#include "utils.h"
#include "mini_uart.h"
#include "timer.h"
#include "irq.h"
#include "gic_v3.h"
#include "fork.h"
#include "sched.h"
#include "sys.h"
#include "user.h"
#include "mm.h"

/* 这个函数的实现有点技巧 */
/* 这个进程刚开始创建时，在ret_from_fork里，sp是0x50000ef0 */
/* 它知道，自己最终回回到ret_from_fork，并且sp会回到0x50000ef0 */
/* 所以，pt_regs就是0x50000ef0，然后填充好pt_regs */
/* 到时候直接执行ret_to_usr里的kernel_exit */
/* 就可以配置好，sp_el0和esr_el1 */
int move_to_user_mode(unsigned long start, unsigned long size, unsigned long pc)
{
	struct pt_regs *regs = task_pt_regs(current);

	regs->pstate = PSR_MODE_EL0t;
	regs->pc = pc;
	regs->sp = 2 * PAGE_SIZE;

	unsigned long code_page = allocate_user_page(current, 0);

	memcpy(code_page, start, size);
	set_pgd(current->mm.pgd);

	return 0;
}

void temporary_process()
{
	unsigned long begin = (unsigned long)&user_begin;
	unsigned long end = (unsigned long)&user_end;
	unsigned long process = (unsigned long)&user_process;

	printf("Kernel process started. EL %d\r\n", get_el());

	move_to_user_mode(begin, end - begin, process - begin); //Default user code starts from 0x0
	/* 这个执行完会到哪里去？哪里来的回哪里去 */
}

void kernel_main(void)
{
	//uart_init();
	init_printf(NULL, putc);
	printf("Hell we come!\n");

	irq_vector_init();
	gic_v3_initialize();
	timer_init();
	enable_irq();

	copy_process(PF_KTHREAD, (unsigned long)&temporary_process, 0);

	while (1)
		schedule();
}
