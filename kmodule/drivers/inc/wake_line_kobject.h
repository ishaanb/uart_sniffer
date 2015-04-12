#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/time.h>
#include<linux/kernel.h>
#include<mach/platform.h>
#include<asm/io.h>

extern void config_interrupt(void);
extern void int_release(void);

extern ssize_t ts_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf);
extern ssize_t ts_store(struct kobject *kobj, struct kobj_attribute *attr,
                         const char *buf, size_t count);
//extern irq_handler_test_start (int irq, void *dev_id , struct pt_regs *regs);
