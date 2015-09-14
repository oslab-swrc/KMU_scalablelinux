#include <linux/cpu.h>
#include <linux/cpufreq.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/gfp.h>
#include <linux/kernel_stat.h>
#include <linux/notifier.h>
#include <linux/sched.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#include <linux/tick.h>
#include <linux/uaccess.h>

#include <linux/rwprofiler.h>
#include "rwprofiler_module.h"

#define DEFAULT_TIMER_EXPIRE

static DECLARE_DELAYED_WORK(rwprofiler_flush_work, rwprofiler_flush_work_func);

void rwprofiler_flush_work_func(struct work_struct *work)
{
	int cpu;
	unsigned long long total = 0;
	struct rwprofiler_rw_log *rwlog;

	pr_info("@rwprofiler flush work func\n");
	for_each_online_cpu(cpu)
		total += RWPROFILER_STAT(anon_vma_cycle, cpu);
	pr_info("@rwprofiler : anon_vma_cycle %llu\n", total);
	rwlog->value1 = total;

	if (work_enabled)
		schedule_delayed_work(&b->work, DEFAULT_TIMER_EXPIRE);
}

void rwprofiler_work_init(void)
{
	INIT_DELAYED_WORK(&rwprofiler_flush_work, rwprofiler_flush_work_func);
}

void rwprofiler_start(void)
{
	pr_info("@rwprofiler start");
	schedule_delayed_work(&rwprofiler_flush_work,
			msecs_to_jiffies(kernel_record_time_sec * 1000));
}

void rwprofiler_stop(void)
{
	pr_info("@rwprofiler stop");
}
