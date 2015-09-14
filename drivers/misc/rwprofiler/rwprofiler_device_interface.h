#ifndef _RWPROFILER_DEVICE_INTERFACE_H
#define _RWPROFILER_DEVICE_INTERFACE_H

typedef struct {
	char *data;
	int count;
} rwprofiler_device_operation_get_rwdata_t;

typedef struct {
	int count;
} rwprofiler_device_operation_get_thread_count_t;

typedef struct {
	char *data;
} rwprofiler_device_operation_get_thread_info_t;

typedef union {
	rwprofiler_device_operation_get_rwdata_t operation_get_data;
	rwprofiler_device_operation_get_thread_count_t operation_get_thread_count;
	rwprofiler_device_operation_get_thread_info_t operation_get_thread_info;
} rwprofiler_device_operation_data_t;

typedef struct {
	int operation_type;
	unsigned long version;
	rwprofiler_device_operation_data_t data;
	long return_value;
} rwprofiler_device_operation_t;

typedef struct {
	long operations_count;
	rwprofiler_device_operation_t *operations;
} rwprofiler_device_interface_t;

#endif /* _RWPROFILER_DEVICE_INTERFACE_H */
