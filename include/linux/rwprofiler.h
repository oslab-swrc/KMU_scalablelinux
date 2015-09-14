#ifndef __LINUX_RWPROFILER
#define __LINUX_RWPROFILER

#include <linux/spinlock.h>
#include <linux/sched.h>
#include <linux/time.h>

#define MM_LOCK_RW_FOR_MM_STAT(__x)

#ifdef MM_LOCK_RW_FOR_MM_STAT

#define DECLARE_MM_STAT(stat)                          \
   DECLARE_PER_CPU(unsigned long long, mm_stat_##stat);

MM_LOCK_RW_FOR_MM_STAT(DECLARE_MM_STAT)
#undef DECLARE_MM_STAT

#define MM_LOCK_RW_MM_STAT_ADD(stat, val)                \
	do {
		percpu_add(mm_stat_##stat, val);
	} while (0)

#else

#define MM_LOCK_RW_MM_STAT_ADD(stat, val) do { } while (0)

#endif

#define MM_LOCK_RW_MM_STAT_INC(stat) MM_LOCK_RW_MM_STAT_ADD(stat, 1)

struct mm_lock_rw_log {
	int id;
	pid_t tgid;
	int cpu_id;
	long timestamp_sec;
	long timestamp_nsec;
	long value1;
	long value2;
};


#define MAX_RECORD_COUNT 10
#define MAX_RECORD_INFO 4096 * 100

#define KERNEL_BASE_ID 1500
#define I_MMAP_READER (KERNEL_BASE_ID + 0)
#define I_MMAP_WRITER (KERNEL_BASE_ID + 1)
#define MMAP_SEM_READER (KERNEL_BASE_ID + 2)
#define MMAP_SEM_WRITER (KERNEL_BASE_ID + 3)
#define ANON_VMA_READER (KERNEL_BASE_ID + 4)
#define ANON_VMA_WRITER (KERNEL_BASE_ID + 5)

typedef struct {
	int id;
	pid_t tgid;
	int cpu_id;
	long timestamp_sec;
	long timestamp_nsec;
	long value1;
	long value2;
} kernel_record_task_info_t;

typedef struct {
	struct list_head list;
	pid_t pid;
	pid_t tgid;
	char comm[TASK_COMM_LEN];
} kernel_task_info_t;

void rwprofiler_stop(void);
void rwprofiler_start(void);
int rwprofiler_dump(void);
kernel_record_task_info_t *rwprofiler_get_dump_info(void);
int rwprofiler_get_dump_count(void);
void clover_tarce_get_lock(void);
int clover_tarce_get_sequence_number(void);
kernel_task_info_t *rwprofiler_get_task_info_list(void);
int rwprofiler_get_thread_count(void);

/* Global variables */
extern kernel_record_task_info_t *kernel_record_info;
extern int kernel_record_count;
extern int is_record;
extern spinlock_t rwprofiler_lock;


#define RWPROFILER_TIMESTAMP(ID, V1, V2) 				\
	do { 													\
		unsigned long flags;								\
		struct timespec kernel_tp; 							\
		kernel_record_task_info_t *ktp;						\
		if (!is_record)										\
			goto no_record##ID##V1##V2;						\
															\
		if (kernel_record_count >= MAX_RECORD_INFO)			\
			goto no_record##ID##V1##V2;						\
															\
		spin_lock_irqsave(&rwprofiler_lock, flags);		\
		ktp = kernel_record_info + kernel_record_count++;	\
		spin_unlock_irqrestore(&rwprofiler_lock, flags);	\
															\
		getnstimeofday(&kernel_tp);							\
		ktp->id = ID;										\
		ktp->tgid = current->pid;							\
		ktp->cpu_id = task_thread_info(current)->cpu;		\
		ktp->timestamp_sec = kernel_tp.tv_sec;				\
		ktp->timestamp_nsec = kernel_tp.tv_nsec;			\
		ktp->value1 = V1;									\
		ktp->value2 = V2;									\
															\
	no_record##ID##V1##V2:									\
		;													\
	} while (0)


#endif /* __LINUX_RWPROFILER */
