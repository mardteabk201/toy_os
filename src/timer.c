#include "sched.h"
#include "printf.h"
#include <stdint.h>
#include "aarch64.h"
#include "gic_v3.h"

const unsigned int interval = 200000;
unsigned int curVal = 0;
uint32_t cntfrq;

void get_cnt_freq()
{
	cntfrq = raw_read_cntfrq_el0();
}

void set_next_event()
{
	uint64_t current_cnt;

	current_cnt = raw_read_cntvct_el0();
	raw_write_cntv_cval_el0(current_cnt + cntfrq);
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
	timer_next();
	timer_tick();
}

void timer_init(void)
{
	disable_cntv();
	get_cnt_freq();
	set_next_event();
	enable_cntv();
}
