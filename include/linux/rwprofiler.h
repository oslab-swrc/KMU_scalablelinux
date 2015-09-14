#ifndef __LINUX_RWPROFILER_
#define __LINUX_RWPROFILER_

#include <linux/spinlock.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/cpu.h>

#define RWPROFILER_FOR_RWLOCK(__x)       \
     __x(anon_vma_cycle)

#ifdef RWPROFILER_FOR_RWLOCK

#define DECLARE_RWPROFILER(stat)                          \
   DECLARE_PER_CPU(unsigned long long, rwprofiler_##stat);
RWPROFILER_FOR_RWLOCK(DECLARE_RWPROFILER)
#undef DECLARE_RWPROFILER
#endif

#define RWPROFILER_STAT_ADD(stat, val)             \
     do { this_cpu_add(rwprofiler_##stat, val); } while (0)


#define RWPROFILER_STAT(stat, cpu)             \
     do { per_cpu(rwprofiler_##stat, cpu); } while (0)

struct rwprofiler_rw_log {
	int id;
	long timestamp_sec;
	long timestamp_nsec;
	long value1;
	long value2;
	struct llist_node llnode;
};

#define KERNEL_BASE_ID 1500
#define I_MMAP_READER (KERNEL_BASE_ID + 0)
#define I_MMAP_WRITER (KERNEL_BASE_ID + 1)
#define MMAP_SEM_READER (KERNEL_BASE_ID + 2)
#define MMAP_SEM_WRITER (KERNEL_BASE_ID + 3)
#define ANON_VMA_READER (KERNEL_BASE_ID + 4)
#define ANON_VMA_WRITER (KERNEL_BASE_ID + 5)

void rwprofiler_stop(void);
void rwprofiler_start(void);
int rwprofiler_dump(void);
rwprofiler_rw_log_t *rwprofiler_get_dump_info(void);
int rwprofiler_get_dump_count(void);
rwprofiler_rw_log_t *rwprofiler_get_task_info_list(void);
int rwprofiler_get_thread_count(void);

#endif /* __LINUX_RWPROFILER */
