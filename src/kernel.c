#include "printf.h"
#include "utils.h"
#include "mini_uart.h"
#include "timer.h"
#include "irq.h"
#include "gic_v3.h"

void kernel_main(void)
{
	//uart_init();
	init_printf(0, putc);
	printf("Hell we come!\n");

	irq_vector_init();
	gic_v3_initialize();
	timer_init();
	enable_irq();

	while (1);
}
