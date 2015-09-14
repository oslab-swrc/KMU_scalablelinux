#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/kobject.h>
#include <linux/vmalloc.h>
#include <linux/kprobes.h>
#include <linux/thermal.h>
#include <linux/version.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
#endif

#include <linux/rwprofiler.h>
#include "rwprofiler_module.h"
#include "rwprofiler_file_operations.h"

DEFINE_SPINLOCK(rwprofiler_lock);
EXPORT_SYMBOL(rwprofiler_lock);

static int __init rwprofiler_init(void)
{
	int retval = 0;
	int error;

	/* Step 1: create ring buffer */

	retval |= rwprofiler_file_operations_init();
	pr_info("rwprofiler : finished initialization\n");
	if (retval != 0)
		return -ENOMEM;

	rwprofiler_work_init();

	return 0;
}

static void __exit rwprofiler_exit(void)
{
	rwprofiler_stop();
	rwprofiler_file_operations_cleanup();

	/* Step 1: clean-up ring buffer */
}

module_init(rwprofiler_init);
module_exit(rwprofiler_exit);
MODULE_LICENSE("GPL v2");
