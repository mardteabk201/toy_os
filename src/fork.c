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
	task[pid] = p;

	preempt_enable();

	return pid;
}

/* 这个函数的实现有点技巧 */
/* 这个进程刚开始创建时，在ret_from_fork里，sp是0x50000ef0 */
/* 它知道，自己最终回回到ret_from_fork，并且sp会回到0x50000ef0 */
/* 所以，pt_regs就是0x50000ef0，然后填充好pt_regs */
/* 到时候直接执行ret_to_usr里的kernel_exit */
/* 就可以配置好，sp_el0和esr_el1 */
int move_to_user_mode(unsigned long start, unsigned long size, unsigned long pc)
{
	struct pt_regs *regs = task_pt_regs(current);

	regs->pstate = PSR_MODE_EL0t;
	regs->pc = pc;
	regs->sp = 2 * PAGE_SIZE;

	unsigned long code_page = allocate_user_page(current, 0);

	memcpy(code_page, start, size);
	set_pgd(current->mm.pgd);

	return 0;
}

struct pt_regs * task_pt_regs(struct task_struct *tsk)
{
	unsigned long p = (unsigned long)tsk + THREAD_SIZE - sizeof(struct pt_regs);

	return (struct pt_regs *)p;
}
