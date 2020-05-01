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

void kernel_process()
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

	copy_process(PF_KTHREAD, (unsigned long)&kernel_process, 0);

	while (1)
		schedule();
}
