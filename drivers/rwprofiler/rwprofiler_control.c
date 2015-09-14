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


void rwprofiler_start(void)
{
	pr_info("@rwprofiler start");
}

void rwprofiler_stop(void)
{
	pr_info("@rwprofiler stop");
{
