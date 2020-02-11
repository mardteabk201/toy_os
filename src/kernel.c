#include "printf.h"
#include "utils.h"
#include "mini_uart.h"
#include "timer.h"
#include "irq.h"
#include "gic_v3.h"
#include "fork.h"
#include "sched.h"

void process(char *array)
{
	while (1) {
		for (int i = 0; i < 5; i++) {
			printf("%c.", array[0]);
			delay(40000000);
		}
	}
}

void kernel_main(void)
{
	//uart_init();
	init_printf(0, putc);
	printf("Hell we come!\n");

	irq_vector_init();
	gic_v3_initialize();
	timer_init();
	enable_irq();

	copy_process((unsigned long)&process, (unsigned long)"aaaaa");
	copy_process((unsigned long)&process, (unsigned long)"bbbbb");
	copy_process((unsigned long)&process, (unsigned long)"ccccc");
	copy_process((unsigned long)&process, (unsigned long)"ddddd");
	copy_process((unsigned long)&process, (unsigned long)"eeeee");

	while (1)
		schedule();
}
