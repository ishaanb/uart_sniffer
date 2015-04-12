#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/time.h>
#include<linux/kernel.h>
#include<mach/platform.h>
#include<asm/io.h>

extern void log_to_sysfs(u16 ch, struct timeval kernel_time)  ;
extern ssize_t message_error_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf) ;
extern ssize_t message_error_store(struct kobject *kobj, struct kobj_attribute *attr,
                         const char *buf, size_t count);
extern ssize_t mess_len_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf) ;
extern ssize_t mess_len_store(struct kobject *kobj, struct kobj_attribute *attr,
                         const char *buf, size_t count);
extern ssize_t message_end_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf);
extern ssize_t message_end_store(struct kobject *kobj, struct kobj_attribute *attr,
                         const char *buf, size_t count);
extern ssize_t char_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf);
extern ssize_t char_store(struct kobject *kobj, struct kobj_attribute *attr,
                         const char *buf, size_t count);
extern ssize_t char_ts_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf);
extern ssize_t char_ts_store(struct kobject *kobj, struct kobj_attribute *attr,
                         const char *buf, size_t count);

