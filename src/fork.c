#include "mm.h"
#include "sched.h"
#include "printf.h"
#include "fork.h"
#include "entry.h"
#include "utils.h"

int copy_process(unsigned long clone_flags, unsigned long fn, unsigned long arg)
{
	struct task_struct *p;
	int pid;
	struct pt_regs *childregs, *cur_regs;

	preempt_disable();

	p = (struct task_struct *)allocate_kernel_page();
	childregs = task_pt_regs(p);

	if (clone_flags & PF_KTHREAD) {
		p->cpu_context.x19 = fn;
		p->cpu_context.x20 = arg;
	} else {
		cur_regs = task_pt_regs(current);
		*childregs = *cur_regs;
		childregs->regs[0] = 0;
		copy_virt_memory(p);
	}

	p->flags = clone_flags;
	p->priority = current->priority;
	p->state = TASK_RUNNING;
	p->counter = p->priority;
	p->preempt_count = 1; //disable preemtion until schedule_tail

	p->cpu_context.pc = (unsigned long)ret_from_fork;
	p->cpu_context.sp = (unsigned long)childregs;
	pid = nr_tasks++;
	p->pid = pid;
	task[pid] = p;

	preempt_enable();

	return pid;
}

struct pt_regs * task_pt_regs(struct task_struct *tsk)
{
	unsigned long p = (unsigned long)tsk + THREAD_SIZE - sizeof(struct pt_regs);

	return (struct pt_regs *)p;
}
