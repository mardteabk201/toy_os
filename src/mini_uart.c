#include "utils.h"
#include "peripherals/mini_uart.h"
#include "peripherals/gpio.h"

#define UART_BASE    0xffff000029000000
void uart_send(char c)
{
	*((unsigned int *)UART_BASE) = c;
}

char uart_recv(void)
{
	while (1) {
		if (get32(AUX_MU_LSR_REG) & 0x01)
			break;
	}
	return (get32(AUX_MU_IO_REG) & 0xFF);
}

void uart_init(void)
{
	/* init UART (38400 8N1) */
	/*
	ldr	x4, =0x09000000		// UART base
	mov	w5, #0x10		// IBRD
	str	w5, [x4, #0x24]
	*/
	*(unsigned int *)0x09000024 = 0x10;
	/*
	mov	w5, #0xc300
	orr	w5, w5, #0x0001		// CR
	str	w5, [x4, #0x30]
	*/
	*(unsigned int *)0x09000030 = 0xc301;
}

void putc(void *p, char c)
{
	uart_send(c);
}
