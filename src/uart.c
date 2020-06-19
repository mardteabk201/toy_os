#include "peripherals/mini_uart.h"
#include "peripherals/gpio.h"

#define UART_BASE    0xffff000029000000
void uart_send(void *p, char c)
{
	*((unsigned int *)UART_BASE) = c;
}
