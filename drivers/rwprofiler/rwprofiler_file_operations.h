#ifndef _RWPROFILER_OPERATIONS_H
#define _RWPROFILER_OPERATIONS_H

#include <linux/fs.h>

#define RWPROFILER_IOCTL_MAGIC 0x1ed2e55d

#define RWPROFILER_IOCTL_COMMAND RWPROFILER_IOCTL_MAGIC

#define RWPROFILER_GET_DATA 1
#define RWPROFILER_THREAD_COUNT 2
#define RWPROFILER_THREAD_INFO 3

#define RWPROFILER_OPERATION_MIN RWPROFILER_GET_DATA

#define RWPROFILER_OPERATION_MAX RWPROFILER_THREAD_INFO

int rwprofiler_file_operations_init(void);
void rwprofiler_file_operations_cleanup(void);
int rwprofiler_file_operations_open(struct inode *inode, struct file *file);
int rwprofiler_file_operations_release(struct inode *inode, struct file *file);
long rwprofiler_file_operations_ioctl(struct file *file,
				       unsigned int operation,
				       unsigned long argument);
ssize_t rwprofiler_file_operations_read(struct file *file, char *data,
					 size_t count, loff_t *file_position);


int rwprofiler_ioctl_core(unsigned int operation, unsigned long argument);
int rwprofiler_ioctl_individual(rwprofiler_device_operation_t *operation);
void rwprofiler_set_return_value(rwprofiler_device_operation_t *operation,
				  long return_value);

int rwprofiler_ioctl_get_data_core(rwprofiler_device_operation_t *operation);
int rwprofiler_ioctl_get_thread_count_core(rwprofiler_device_operation_t *
					    operation);
int rwprofiler_ioctl_get_thread_info_core(rwprofiler_device_operation_t *
					   operation);

#endif /* _RWPROFILER_OPERATIONS_H */
