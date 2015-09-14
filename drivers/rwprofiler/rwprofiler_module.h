#ifndef _RWPROFILER_MODULE_H
#define _RWPROFILER_MODULE_H

#include <linux/rwprofiler.h>

#define MODULE_NAME "rwprofiler"

#define MODULE_NAME_STR "rwprofiler"

#define MODULE_DEV_NAME MODULE_NAME_STR

#define RWPROFILER_MAJOR 245

#define RWPROFILER_MINOR 0

extern void rwprofiler_work_init(void);

#endif /* _PSEUDO_INPUT_MODULE_H  */
