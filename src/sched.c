#include "sched.h"
#include "irq.h"
#include "printf.h"
#include "fork.h"
#include "utils.h"
#include "mm.h"

static struct task_struct init_task = INIT_TASK;
struct task_struct *current = &(init_task);
struct task_struct * task[NR_TASKS] = {&(init_task), };
int nr_tasks = 1;

void preempt_disable(void)
{
	current->preempt_count++;
}

void preempt_enable(void)
{
	current->preempt_count--;
}

void _schedule(void)
{
	int next, c;
	struct task_struct * p;
	char d;

	preempt_disable();

	while (1) {
		c = -1;
		next = 0;
		for (int i = 0; i < NR_TASKS; i++) {
			p = task[i];
			if (p && p->state == TASK_RUNNING && p->counter > c) {
				c = p->counter;
				next = i;
			}
		}

		if (c)
			break;

		for (int i = 0; i < NR_TASKS; i++) {
			p = task[i];
			if (p)
				p->counter = (p->counter >> 1) + p->priority;
		}
	}

	switch_to(task[next], next);
	d = *(char *)(current->cpu_context.x20);
	if ('a' <= d && d <= 'e')
		printf("Out timer tick %s\n", (char *)(current->cpu_context.x20));
	else
		printf("Out timer tick scheduler, gonna schedule away\n");
	preempt_enable();
}

void schedule(void)
{
	current->counter = 0;
	_schedule();
}

void switch_to(struct task_struct * next, int index)
{
	struct task_struct *prev;

	if (current == next)
		return;

	prev = current;
	current = next;

	/* This will be the return entry */
	cpu_switch_to(prev, next);
}

void schedule_tail(void)
{
	/* 进程在主动调度后处于关闭抢占的状态，如果这里不开启，便再也不调度了 */
	preempt_enable();
}

void timer_tick()
{
	printf("\ntimer tick\n");
	//printf("In  timer tick %s", (char *)(current->cpu_context.x20));
	printf("In  timer tick scheduler\n");

	current->counter--;

	if (current->counter > 0 || current->preempt_count > 0) {
		printf(" Not change\n");
		return;
	}

	printf("\n");

	//printf("current preempt_count: %d\n", current->preempt_count);

	//printf("current counter: %d\n", current->counter);
	current->counter = 0; /* Why set 0 here */

	/* 如果不开中断，被调度走就不会返回了，timer中断上下文就不会恢复，就再也没有timer中断了 */
	enable_irq();
	_schedule();
	/* 就像一个特工一样，换好衣服他就出去了，出去的第一件事就是，关中断 */
	/* 因为你至少得让他运行一会儿，不能立马又来下一个中断 */
	disable_irq();
}

void exit_process()
{
	preempt_disable();
	for (int i = 0; i < NR_TASKS; i++) {
		if (task[i] == current) {
			task[i]->state = TASK_ZOMBIE;
			break;
		}
	}
	if (current->stack) {
		free_page(current->stack);
	}
	preempt_enable();
	schedule();
}
