#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/uaccess.h>

#include <asm-generic/errno-base.h>

#include "rwprofiler_device_interface.h"
#include "rwprofiler_file_operations.h"
#include "rwprofiler_module.h"

#include <linux/rwprofiler.h>


static const struct file_operations rwprofiler_file_operations = {
	.owner = THIS_MODULE,
	.open = rwprofiler_file_operations_open,
	.release = rwprofiler_file_operations_release,
	.read = rwprofiler_file_operations_read,
	.unlocked_ioctl = rwprofiler_file_operations_ioctl,
	.compat_ioctl = rwprofiler_file_operations_ioctl,
};

static struct miscdevice rwprofiler_miscdev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "rwprofiler",
	.fops = &rwprofiler_file_operations
};

int rwprofiler_file_operations_init(void)
{
	int retval_registration;

	/* register a character device */
	retval_registration = misc_register(&rwprofiler_miscdev);
	if (retval_registration < 0)
		return retval_registration;

	return 0;
}

void rwprofiler_file_operations_cleanup(void)
{
	misc_deregister(&rwprofiler_miscdev);
}

int rwprofiler_file_operations_open(struct inode *inode, struct file *file)
{
	return 0;
}

int rwprofiler_file_operations_release(struct inode *inode, struct file *file)
{
	return 0;
}

int rwprofiler_ioctl_core(unsigned int operation, unsigned long argument)
{
	rwprofiler_device_interface_t *device_data;
	long operations_count = 0;
	int retval = -EINVAL;

	device_data = (rwprofiler_device_interface_t *) argument;

	if (operation != RWPROFILER_IOCTL_COMMAND) {
		pr_err("%s, %d\n", __func__, __LINE__);
		return -EOPNOTSUPP;
	}

	if (device_data == NULL) {
		pr_err("%s, %d\n", __func__, __LINE__);
		return -EINVAL;
	}

	if (device_data->operations_count <= 0) {
		pr_err("%s, %d\n", __func__, __LINE__);
		return -EINVAL;
	}

	while (operations_count < device_data->operations_count) {
		retval = rwprofiler_ioctl_individual(
				&device_data->operations[operations_count]);
		operations_count++;
	}
	return retval;
}

int rwprofiler_ioctl_get_data_core(rwprofiler_device_operation_t *operation)
{
	int retval = 0;

	if (retval != 0)
		return -EFAULT;
	else
		return 0;
}

int rwprofiler_ioctl_individual(rwprofiler_device_operation_t *operation)
{
	if (operation->operation_type < RWPROFILER_OPERATION_MIN ||
	    operation->operation_type > RWPROFILER_OPERATION_MAX) {
		rwprofiler_set_return_value(operation, -101);
		pr_err("%s, %d\n", __func__, __LINE__);
		return -EOPNOTSUPP;
	}

	if (operation->operation_type == RWPROFILER_GET_DATA) {
		return rwprofiler_ioctl_get_data_core(operation);
	} else {
		rwprofiler_set_return_value(operation, -109);
		return -EINVAL;
	}

	return 0;
}

void rwprofiler_set_return_value(rwprofiler_device_operation_t *operation,
				  long return_value)
{
	int retval;

	retval = (int)copy_to_user(&operation->return_value,
				   &return_value,
				   sizeof(operation->return_value));
}

long rwprofiler_file_operations_ioctl(struct file *file,
				       unsigned int operation,
				       unsigned long argument)
{
	pr_debug("rwprofiler_file_operations_ioctl : operation : %x\n",
			operation);
	return rwprofiler_ioctl_core(operation, argument);
}

ssize_t rwprofiler_file_operations_read(struct file *file, char *data,
					 size_t count, loff_t *file_position)
{
	int ret = 0;

	return ret;
}
