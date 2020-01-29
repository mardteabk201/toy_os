#include "utils.h"
#include "peripherals/mini_uart.h"
#include "peripherals/gpio.h"

void uart_send(char c)
{
	while (1) {
		if (get32(AUX_MU_LSR_REG) & 0x20)
			break;
	}
	put32(AUX_MU_IO_REG, c);
}

char uart_recv(void)
{
	while (1) {
		if (get32(AUX_MU_LSR_REG) & 0x01)
			break;
	}
	return (get32(AUX_MU_IO_REG) & 0xFF);
}

#define UART_BASE    0x09000000
void uart_send_string(char* str)
{
	while (*str)
		*((unsigned int *)UART_BASE) = *str++;
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
