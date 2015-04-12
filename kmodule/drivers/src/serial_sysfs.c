#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/time.h>
#include<linux/kernel.h>
#include<mach/platform.h>
#include<asm/io.h>
//#include"serial_sysfs.h"
#define MAX_MESSAGE_SIZE 25
// this is based on the baud rate. a 32 bit period is the wait period
// This is roughly 32*26 = 830. Defining it to be 815 to take care of inconsistencies
#define MESSAGE_END_WAIT_PERIOD 815
#define	CHAR_TRANSMIT_TIME 260
/*
 * The uartstream attribute store and show functions, 
Store : should be called in uart_insert_char to store to the character
show : should spitout the stored value when cat'ed
 */
extern struct kobject *uartstream_kobject;
char char1_tmp[MAX_MESSAGE_SIZE];
struct timeval char_ts_tmp[MAX_MESSAGE_SIZE];
struct timeval *ktime_char1_ts;
int messlen;
int messend;
int messerror;

static ssize_t message_error_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf)
{
        return sprintf(buf, "%d\n", messerror);
}

static ssize_t message_error_store(struct kobject *kobj, struct kobj_attribute *attr,
                         const char *buf, size_t count)
{
        memcpy(&messerror,buf,4);
        //sscanf(buf, "%du", &messend  );
        return count;
}




static ssize_t mess_len_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf)
{
        return sprintf(buf, "%d\n", messlen);
}

static ssize_t mess_len_store(struct kobject *kobj, struct kobj_attribute *attr,
                         const char *buf, size_t count)
{
        //sscanf(buf, "%du", &messlen );
	memcpy(&messlen,buf,4);
        return count;
}


static ssize_t message_end_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf)
{
        return sprintf(buf, "%d\n", messend);
}

static ssize_t message_end_store(struct kobject *kobj, struct kobj_attribute *attr,
                         const char *buf, size_t count)
{
	memcpy(&messend,buf,4);
        //sscanf(buf, "%du", &messend  );
        return count;
}

static ssize_t char_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf)
{
        return sprintf(buf, "%#x\n", char1_tmp[attr->index]);
}

static ssize_t char_store(struct kobject *kobj, struct kobj_attribute *attr,
                         const char *buf, size_t count)
{
        //sscanf(buf, "%c", &char1_tmp[0]   );
	memcpy(&char1_tmp[attr->index],buf,1);
        return count;
}

static ssize_t char_ts_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf)
{
	 return sprintf(buf, "%lu.%lu\n", char_ts_tmp[attr->index].tv_sec, char_ts_tmp[attr->index].tv_usec);
}

static ssize_t char_ts_store(struct kobject *kobj, struct kobj_attribute *attr,
                         const char *buf, size_t count)
{
         ktime_char1_ts = (struct timeval *) buf;
 	 memcpy(&char_ts_tmp[attr->index],ktime_char1_ts,sizeof(struct timeval));
         return count;
}

struct kobj_attribute message_error = 
 __ATTR(message_error, 0666, message_error_show, message_error_store);

struct kobj_attribute message_len = 
 __ATTR(message_length, 0666, mess_len_show, mess_len_store);

struct kobj_attribute message_end = 
 __ATTR(message_end, 0666, message_end_show, message_end_store);

struct kobj_attribute charval[] = {
        __ATTR_SERIAL(char1, 1 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char2, 2, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char3, 3, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char4, 4 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char5, 5, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char6, 6, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char7, 7, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char8, 8, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char9, 9, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char10, 10 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char11, 11 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char12, 12, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char13, 13, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char14, 14 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char15, 15, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char16, 16, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char17, 17, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char18, 18, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char19, 19, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char20, 20 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char21, 21 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char22, 22, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char23, 23, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char24, 24 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char25, 25, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char26, 26, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char27, 27, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char28, 28, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char29, 29, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char30, 30 , 0666, char_show, char_store),
        __ATTR_SERIAL(char31, 31 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char32, 32, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char33, 33, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char34, 34 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char35, 35, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char36, 36, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char37, 37, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char38, 38, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char39, 39, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char40, 40 , 0666, char_show, char_store),
        __ATTR_SERIAL(char41, 41 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char42, 42, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char43, 43, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char44, 44 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char45, 45, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char46, 46, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char47, 47, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char48, 48, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char49, 49, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char50, 50 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char51, 51 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char52, 52, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char53, 53, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char54, 54 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char55, 55, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char56, 56, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char57, 57, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char58, 58, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char59, 59, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char60, 60 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char61, 61 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char62, 62, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char63, 63, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char64, 64 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char65, 65, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char66, 66, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char67, 67, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char68, 68, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char69, 69, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char70, 70 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char71, 71 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char72, 72, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char73, 73, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char74, 74 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char75, 75, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char76, 76, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char77, 77, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char78, 78, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char79, 79, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char80, 80 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char81, 81 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char82, 82, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char83, 83, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char84, 84 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char85, 85, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char86, 86, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char87, 87, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char88, 88, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char89, 89, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char90, 90 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char91, 91 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char92, 92, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char93, 93, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char94, 94 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char95, 95, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char96, 96, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char97, 97, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char98, 98, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char99, 99, 0666, char_show, char_store) ,
        __ATTR_SERIAL(char100, 100 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char101, 101 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char102, 102 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char103, 102 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char104, 104 , 0666, char_show, char_store) ,
        __ATTR_SERIAL(char105, 105 , 0666, char_show, char_store)
 } ;

struct kobj_attribute char_timestamp[] = {
        __ATTR_SERIAL(char1_timestamp, 1 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char2_timestamp, 2, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char3_timestamp, 3, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char4_timestamp, 4 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char5_timestamp, 5, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char6_timestamp, 6, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char7_timestamp, 7, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char8_timestamp, 8, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char9_timestamp, 9, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char10_timestamp, 10 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char11_timestamp, 11 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char12_timestamp, 12, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char13_timestamp, 13, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char14_timestamp, 14 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char15_timestamp, 15, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char16_timestamp, 16, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char17_timestamp, 17, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char18_timestamp, 18, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char19_timestamp, 19, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char20_timestamp, 20 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char21_timestamp, 21 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char22_timestamp, 22, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char23_timestamp, 23, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char24_timestamp, 24 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char25_timestamp, 25, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char26_timestamp, 26, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char27_timestamp, 27, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char28_timestamp, 28, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char29_timestamp, 29, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char30_timestamp, 30 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char31_timestamp, 31 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char32_timestamp, 32, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char33_timestamp, 33, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char34_timestamp, 34 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char35_timestamp, 35, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char36_timestamp, 36, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char37_timestamp, 37, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char38_timestamp, 38, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char39_timestamp, 39, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char40_timestamp, 40 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char41_timestamp, 41 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char42_timestamp, 42, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char43_timestamp, 43, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char44_timestamp, 44 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char45_timestamp, 45, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char46_timestamp, 46, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char47_timestamp, 47, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char48_timestamp, 48, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char49_timestamp, 49, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char50_timestamp, 50 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char51_timestamp, 51 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char52_timestamp, 52, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char53_timestamp, 53, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char54_timestamp, 54 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char55_timestamp, 55, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char56_timestamp, 56, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char57_timestamp, 57, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char58_timestamp, 58, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char59_timestamp, 59, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char60_timestamp, 60 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char61_timestamp, 61 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char62_timestamp, 62, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char63_timestamp, 63, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char64_timestamp, 64 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char65_timestamp, 65, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char66_timestamp, 66, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char67_timestamp, 67, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char68_timestamp, 68, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char69_timestamp, 69, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char70_timestamp, 70 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char71_timestamp, 71 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char72_timestamp, 72, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char73_timestamp, 73, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char74_timestamp, 74 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char75_timestamp, 75, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char76_timestamp, 76, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char77_timestamp, 77, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char78_timestamp, 78, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char79_timestamp, 79, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char80_timestamp, 80 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char81_timestamp, 81 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char82_timestamp, 82, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char83_timestamp, 83, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char84_timestamp, 84 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char85_timestamp, 85, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char86_timestamp, 86, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char87_timestamp, 87, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char88_timestamp, 88, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char89_timestamp, 89, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char90_timestamp, 90 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char91_timestamp, 91 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char92_timestamp, 92, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char93_timestamp, 93, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char94_timestamp, 94 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char95_timestamp, 95, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char96_timestamp, 96, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char97_timestamp, 97, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char98_timestamp, 98, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char99_timestamp, 99, 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char100_timestamp, 100 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char101_timestamp, 101 , 0666, char_ts_show, char_ts_store) , 
        __ATTR_SERIAL(char102_timestamp, 102 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char103_timestamp, 102 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char104_timestamp, 104 , 0666, char_ts_show, char_ts_store) ,
        __ATTR_SERIAL(char105_timestamp, 105 , 0666, char_ts_show, char_ts_store)
};


/*
Called from the point the character is transferred to the dma. This function is called for each character to enable very fine grained logging. Might need to change this later.
*/
log_to_sysfs(u16 ch, struct timeval kernel_time) 
{
        char *buf_ts;
	char *buf_len, *buf_end, *buf_error;
        static unsigned int count = 0;
	static int end = 0 ;
	static int error = 0 ;
        static char uart_data[MAX_MESSAGE_SIZE] ;
	static struct timeval uart_ts[MAX_MESSAGE_SIZE] ;
	char excess_uart_data;
	struct timeval excess_uart_ts;
	int diff = 0;
	static bool valid = false;
	static bool log = true;
        if(count == 0 ) {
	    error = 0;
	    buf_end = (char *)&end;
	    buf_error = (char *)&error;
       	    message_end_store(uartstream_kobject, &message_end, buf_end , sizeof(end) );
	    message_error_store(uartstream_kobject, &message_error, buf_error , sizeof(error) );
    	    printk(KERN_INFO "Kernel Dev Version 0.91");
        }

	if (count >= MAX_MESSAGE_SIZE) {
	      	     printk (KERN_INFO "Maximum message size exceeded, data will not be logged. \n");
		     error =1 ;
		     log = false ;
		     count = MAX_MESSAGE_SIZE-1 ;
	}

	if (error > 0) {
	    	buf_error = (char *)&error;
		message_error_store(uartstream_kobject, &message_error, buf_error , sizeof(error) );
	}

	if (unlikely(!valid )) {
	// This is needed to discard the initial 20 junk UART interrupts that happen for some reason on startup
	// Get around it with this logic
		if (count ==19) {
			valid = true ;
			count = 0 ;
    	        	printk(KERN_INFO "Initial junk UART over \n");
		} else {
			count++;
		} 
	} else {
		if ( likely (log) ) { 
	   	     uart_data[count] = ch ;
	   	     uart_ts[count] = kernel_time ; 
	   	     buf_ts = (char *)&uart_ts[count];

	   	     if (uart_ts[count].tv_usec < CHAR_TRANSMIT_TIME) {
	   	        uart_ts[count].tv_sec -= 1;
	   	     }
	   	     uart_ts[count].tv_usec -= CHAR_TRANSMIT_TIME;
	   	     
           	     char_store(uartstream_kobject, &charval[count], &uart_data[count] , 1 );
           	     char_ts_store(uartstream_kobject, &char_timestamp[count], buf_ts , sizeof(kernel_time) );
		     if( likely (count > 0) ) {
	   		diff =  (uart_ts[count].tv_sec + uart_ts[count].tv_usec)   - (uart_ts[count-1].tv_sec + uart_ts[count-1].tv_usec) ; 
		  	printk(KERN_INFO "time diff is : %d and count is %d \n", diff, (count+1));
		     }
	   	} else {
	   	     uart_data[count-1] = uart_data[count] ;
	   	     uart_ts[count-1] = uart_ts[count] ; 
	   	     uart_data[count] = ch ;
	   	     uart_ts[count] = kernel_time ; 

	   	     if (uart_ts[count].tv_usec < CHAR_TRANSMIT_TIME) {
	   	        uart_ts[count].tv_sec -= 1;
	   	     }
	   	     uart_ts[count].tv_usec -= CHAR_TRANSMIT_TIME;
	   	     diff =  (uart_ts[count].tv_sec + uart_ts[count].tv_usec)   - (uart_ts[count-1].tv_sec + uart_ts[count-1].tv_usec) ; 
	//   	    diff =  (excess_uart_ts.tv_sec + excess_uart_ts.tv_usec)   - (uart_ts[count].tv_sec + uart_ts[count].tv_usec) ; 
		     printk(KERN_INFO "time diff is %d and count is %d \n", diff,  count);
		     //printk(KERN_INFO "uart_ts[%d].tv_sec :=  %d  uart_ts[%d].tv_usec = %d  and count is %d \n", count, uart_ts[count].tv_sec, count, uart_ts[count].tv_usec,  count);
		     //printk(KERN_INFO "uart_ts[%d].tv_sec :=  %d  uart_ts[%d].tv_usec = %d  and count is %d \n", count-1, uart_ts[count-1].tv_sec, count-1, uart_ts[count-1].tv_usec,  count-1);
		     //printk(KERN_INFO "excess_uart_ts.tv_sec :=  %d  excess_uart_ts.tv_usec = %d  and count is %d \n", excess_uart_ts.tv_sec, excess_uart_ts.tv_usec,  count);
		}

		if (unlikely (diff  > MESSAGE_END_WAIT_PERIOD )) { // Looks for end of message
	   	 // This is needed to look for the last byte in teh message. 	
	   	 // For a 38400 baud rate this is the 32 bit wait period (~830 us)
	   	 // This can be made parametric based on the baud rate
			 if (uart_ts[count].tv_usec < (MESSAGE_END_WAIT_PERIOD-CHAR_TRANSMIT_TIME)) {
			    uart_ts[count].tv_sec -= 1;
			 }
			 uart_ts[count].tv_usec -= (MESSAGE_END_WAIT_PERIOD-CHAR_TRANSMIT_TIME);
	   	     	 buf_ts = (char *)&uart_ts[count];
			
		//	if (log) {
                	 char_ts_store(uartstream_kobject, &char_timestamp[count], buf_ts , sizeof(kernel_time) );
		//	 }

	   		 end = 1;
	   		 printk(KERN_INFO "Message ended : Message length is %d \n", ++count);
	   		 buf_len = (char *)&count;
	   		 buf_end = (char *)&end;
	   		 mess_len_store(uartstream_kobject, &message_len, buf_len , sizeof(count) );
	   		 message_end_store(uartstream_kobject, &message_end, buf_end , sizeof(end) );
	   		 count = 0;
	   		 end = 0;
			 log = true ;
	   	 } else if (log){
 		  	count ++;
		 }
	}
}






struct attribute *attrs[] = {
	&message_len.attr,
	&message_end.attr,
	&message_error.attr,
        &char_timestamp[0].attr, &charval[0].attr,
        &char_timestamp[1].attr, &charval[1].attr,
        &char_timestamp[2].attr, &charval[2].attr,
        &char_timestamp[3].attr, &charval[3].attr,
        &char_timestamp[4].attr, &charval[4].attr,
        &char_timestamp[5].attr, &charval[5].attr,
        &char_timestamp[6].attr, &charval[6].attr,
        &char_timestamp[7].attr, &charval[7].attr,
        &char_timestamp[8].attr, &charval[8].attr,
        &char_timestamp[9].attr, &charval[9].attr,
	&char_timestamp[10].attr, &charval[10].attr,
        &char_timestamp[11].attr, &charval[11].attr,
        &char_timestamp[12].attr, &charval[12].attr,
        &char_timestamp[13].attr, &charval[13].attr,
        &char_timestamp[14].attr, &charval[14].attr,
        &char_timestamp[15].attr, &charval[15].attr,
        &char_timestamp[16].attr, &charval[16].attr,
        &char_timestamp[17].attr, &charval[17].attr,
        &char_timestamp[18].attr, &charval[18].attr,
        &char_timestamp[19].attr, &charval[19].attr,
        &char_timestamp[20].attr, &charval[20].attr,
        &char_timestamp[21].attr, &charval[21].attr,
        &char_timestamp[22].attr, &charval[22].attr,
        &char_timestamp[23].attr, &charval[23].attr,
        &char_timestamp[24].attr, &charval[24].attr,
        &char_timestamp[25].attr, &charval[25].attr,
        &char_timestamp[26].attr, &charval[26].attr,
        &char_timestamp[27].attr, &charval[27].attr,
        &char_timestamp[28].attr, &charval[28].attr,
        &char_timestamp[29].attr, &charval[29].attr,
	&char_timestamp[30].attr, &charval[30].attr,
        &char_timestamp[31].attr, &charval[31].attr,
        &char_timestamp[32].attr, &charval[32].attr,
        &char_timestamp[33].attr, &charval[33].attr,
        &char_timestamp[34].attr, &charval[34].attr,
        &char_timestamp[35].attr, &charval[35].attr,
        &char_timestamp[36].attr, &charval[36].attr,
        &char_timestamp[37].attr, &charval[37].attr,
        &char_timestamp[38].attr, &charval[38].attr,
        &char_timestamp[39].attr, &charval[39].attr,
	&char_timestamp[40].attr, &charval[40].attr,
        &char_timestamp[41].attr, &charval[41].attr,
        &char_timestamp[42].attr, &charval[42].attr,
        &char_timestamp[43].attr, &charval[43].attr,
        &char_timestamp[44].attr, &charval[44].attr,
        &char_timestamp[45].attr, &charval[45].attr,
        &char_timestamp[46].attr, &charval[46].attr,
        &char_timestamp[47].attr, &charval[47].attr,
        &char_timestamp[48].attr, &charval[48].attr,
        &char_timestamp[49].attr, &charval[49].attr,
        &char_timestamp[50].attr, &charval[50].attr,
        &char_timestamp[51].attr, &charval[51].attr,
        &char_timestamp[52].attr, &charval[52].attr,
        &char_timestamp[53].attr, &charval[53].attr,
        &char_timestamp[54].attr, &charval[54].attr,
        &char_timestamp[55].attr, &charval[55].attr,
        &char_timestamp[56].attr, &charval[56].attr,
        &char_timestamp[57].attr, &charval[57].attr,
        &char_timestamp[58].attr, &charval[58].attr,
        &char_timestamp[59].attr, &charval[59].attr,
	&char_timestamp[60].attr, &charval[60].attr,
        &char_timestamp[61].attr, &charval[61].attr,
        &char_timestamp[62].attr, &charval[62].attr,
        &char_timestamp[63].attr, &charval[63].attr,
        &char_timestamp[64].attr, &charval[64].attr,
        &char_timestamp[65].attr, &charval[65].attr,
        &char_timestamp[66].attr, &charval[66].attr,
        &char_timestamp[67].attr, &charval[67].attr,
        &char_timestamp[68].attr, &charval[68].attr,
        &char_timestamp[69].attr, &charval[69].attr,
        &char_timestamp[70].attr, &charval[70].attr,
        &char_timestamp[71].attr, &charval[71].attr,
        &char_timestamp[72].attr, &charval[72].attr,
        &char_timestamp[73].attr, &charval[73].attr,
        &char_timestamp[74].attr, &charval[74].attr,
        &char_timestamp[75].attr, &charval[75].attr,
        &char_timestamp[76].attr, &charval[76].attr,
        &char_timestamp[77].attr, &charval[77].attr,
        &char_timestamp[78].attr, &charval[78].attr,
        &char_timestamp[79].attr, &charval[79].attr,
	&char_timestamp[80].attr, &charval[80].attr,
        &char_timestamp[81].attr, &charval[81].attr,
        &char_timestamp[82].attr, &charval[82].attr,
        &char_timestamp[83].attr, &charval[83].attr,
        &char_timestamp[84].attr, &charval[84].attr,
        &char_timestamp[85].attr, &charval[85].attr,
        &char_timestamp[86].attr, &charval[86].attr,
        &char_timestamp[87].attr, &charval[87].attr,
        &char_timestamp[88].attr, &charval[88].attr,
        &char_timestamp[89].attr, &charval[89].attr,
	&char_timestamp[90].attr, &charval[90].attr,
        &char_timestamp[91].attr, &charval[91].attr,
        &char_timestamp[92].attr, &charval[92].attr,
        &char_timestamp[93].attr, &charval[93].attr,
        &char_timestamp[94].attr, &charval[94].attr,
        &char_timestamp[95].attr, &charval[95].attr,
        &char_timestamp[96].attr, &charval[96].attr,
        &char_timestamp[97].attr, &charval[97].attr,
        &char_timestamp[98].attr, &charval[98].attr,
        &char_timestamp[99].attr, &charval[99].attr,
        &char_timestamp[100].attr, &charval[100].attr,
        &char_timestamp[101].attr, &charval[101].attr,
        &char_timestamp[102].attr, &charval[102].attr,
        &char_timestamp[103].attr, &charval[103].attr,
        &char_timestamp[104].attr, &charval[104].attr,
	NULL,
};

/*
 * An unnamed attribute group will put all of the attributes directly in
 * the kobject directory.  If we specify a name, a subdirectory will be
 * created for the attributes with the directory being the name of the
 * attribute group.
 */

struct attribute_group attr_group = {
        .attrs = attrs,
};



