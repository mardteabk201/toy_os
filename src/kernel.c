#include "printf.h"
#include "utils.h"
#include "mini_uart.h"

void kernel_main(void)
{
	//uart_init();
	init_printf(0, putc);
	int el = get_el();
	putc((void *)0, 'E');
	putc((void *)0, '\n');
	printf("Exception level: %d \r\n", el);

	while (1);
}
