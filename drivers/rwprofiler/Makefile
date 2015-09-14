ifeq ($(CLOVER_MODULE),1)

ccflags-y := -DCLOVER_LKM
rwprofiler-objs += rwprofiler_control.o rwprofiler_core.o rwprofiler_file_operations.o rwprofiler_ioctl_core.o rwprofiler_module.o 

obj-m += rwprofiler.o

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=$(PWD) modules 

clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=$(PWD) clean 

else
ifneq ($(KERNELRELEASE),)
	
obj-$(CONFIG_RWPROFILER) += rwprofiler.o

rwprofiler-objs += rwprofiler_control.o rwprofiler_core.o rwprofiler_file_operations.o rwprofiler_ioctl_core.o rwprofiler_module.o  

endif
endif
