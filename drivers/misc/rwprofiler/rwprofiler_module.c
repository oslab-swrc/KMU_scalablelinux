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

#include <linux/rwprofiler.h>
#include "rwprofiler_module.h"
#include "rwprofiler_file_operations.h"

#define RWPROFILER_BUFFER_FLAGS	0

DEFINE_SPINLOCK(rwprofiler_lock);
EXPORT_SYMBOL(rwprofiler_lock);
static struct ring_buffer *rwprofiler_ring_buffer;

void free_buffers(void)
{
	if (rwprofiler_ring_buffer)
		ring_buffer_free(rwprofiler_ring_buffer);
	rwprofiler_ring_buffer = NULL;
}

int alloc_buffers(void)
{

	return 0;

fail:
	return -ENOMEM;
}


static int __init rwprofiler_init(void)
{
	int retval = 0;
	int error;

	retval |= rwprofiler_file_operations_init();
	pr_info("rwprofiler : finished initialization\n");
	if (retval != 0)
		return -ENOMEM;

	/* Step 1: create ring buffer */

	rwprofiler_start();
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
