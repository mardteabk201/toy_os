#include "utils.h"
#include "printf.h"
#include "peripherals/timer.h"
#include <stdint.h>
#include "aarch64.h"
#include "gic_v3.h"
#include "board.h"

const unsigned int interval = 200000;
unsigned int curVal = 0;
uint32_t cntfrq;

void get_cnt_freq()
{
	cntfrq = raw_read_cntfrq_el0();
	printf("cntfrq: %d\n", cntfrq);
}

void set_next_event()
{
	uint64_t current_cnt;

	current_cnt = raw_read_cntvct_el0();
	printf("current_cnt: %d\n", current_cnt);
	raw_write_cntv_cval_el0(current_cnt + cntfrq);
	printf("rewrite: %d\n", current_cnt + cntfrq);
	printf("read: %d\n", raw_read_cntv_cval_el0());
}

void timer_init(void)
{
	disable_cntv();
	get_cnt_freq();
	set_next_event();
	enable_cntv();
}

void timer_next(void)
{
	//disable_cntv();
	gicd_clear_pending(TIMER_IRQ);
	set_next_event();
	//enable_cntv();
}

void handle_timer_irq(void)
{
	printf("Timer interrupt received\n\r");
	timer_next();
}
