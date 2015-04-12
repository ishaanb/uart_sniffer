// #includes go here

#include <linux/kobject.h>
#include <linux/string.h>
#include <stdbool.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/time.h>
//#include "led_interrupt.h"

#include<linux/kernel.h>
#include<mach/gpio.h>
#include<linux/interrupt.h>
#include<mach/platform.h>
#include<asm/io.h>
//static int time,sec,min,hour,days;

struct kobject *wakeline_kobject;
static struct timeval *ktime_wake_ts;
struct timeval wake_timestamp_tmp[4];
#define GPIO_PIN 23 
#define START_GPIO_PIN 25 
#define GPIO_PIN_RISING 24 
#define GPIO_DESC  "WAKE_N Interrupts"
#define GPIO_DESC_START  "Test Start"
#define GPIO_DESC_RISE  "RISING WAKE_N"
#define GPIO_DEVICE_DESC "ISHAAN"
static const int INT_GPIO_PIN = 18 ;
// Interrupt variables block 
unsigned int irq_gpio = 0;
unsigned int irq_gpio_start = 0;
unsigned int irq_gpio_rising = 0;
extern bool response ;
bool start_test = false;
EXPORT_SYMBOL(start_test);

/*
 * The "wakeline_enable, wakeline_disable" file where a static variable is read from and written to.
 */
ssize_t ts_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf)
{
	return sprintf(buf, "%lu.%lu", wake_timestamp_tmp[attr->index].tv_sec, wake_timestamp_tmp[attr->index].tv_usec);
}

ssize_t ts_store(struct kobject *kobj, struct kobj_attribute *attr,
                         const char *buf, size_t count)
{
	ktime_wake_ts = (struct timeval *) buf;
	memcpy(&wake_timestamp_tmp[attr->index], ktime_wake_ts, sizeof(struct timeval));
        return count;
}

struct kobj_attribute wake_timestamp[] = {
	__ATTR_SERIAL(wakeline_enable, 0, 0666, ts_show, ts_store),
	__ATTR_SERIAL(wakeline_disable, 1, 0666, ts_show, ts_store),
	__ATTR_SERIAL(wakeline_ack_enable, 2,  0666, ts_show, ts_store),
	__ATTR_SERIAL(wakeline_ack_disable, 3, 0666, ts_show, ts_store),
};

struct attribute *attrs_wake[] = {
	&wake_timestamp[0].attr,
	&wake_timestamp[1].attr,
	&wake_timestamp[2].attr,
	&wake_timestamp[3].attr,
	NULL,
};


/*
 * An unnamed attribute group will put all of the attributes directly in
 * the kobject directory.  If we specify a name, a subdirectory will be
 * created for the attributes with the directory being the name of the
 * attribute group.
 */



struct attribute_group attr_wake_group = {
	.attrs = attrs_wake,
};


//static struct kobject *wakeline_kobject;

struct GpioRegisters {
	uint32_t GPFSEL[6];
	uint32_t Reserved1;
	uint32_t GPSET[2];
	uint32_t Reserved2;
	uint32_t GPCLR[2];
};


struct GpioRegisters *s_pGpioRegisters;
static void SetGPIOFunction (int GPIO, int functionCode) 
{
	int registerIndex = GPIO/10 ; // To access GPFSEL for that row of GPIOs
	int bit = (GPIO%10) *3 ;  //  to go to the bit of that bpio. Each gpio has 3 bits for the function code

	unsigned oldValue = s_pGpioRegisters->GPFSEL[registerIndex];
	unsigned mask = 0b111 << bit;
	printk(KERN_INFO "Changing function of GPIO%d from %x to %x \n",
		GPIO, (oldValue >> bit) & 0b111, functionCode);
	s_pGpioRegisters->GPFSEL[registerIndex] = 
		(oldValue & ~mask) | ((functionCode << bit ) & mask) ;
}

static void SetGPIOOutputValue (int GPIO, bool outputValue)
{
	if(outputValue) 
		s_pGpioRegisters->GPSET[GPIO/32] = (1 << (GPIO % 32)) ;
	else 
		s_pGpioRegisters->GPCLR[GPIO/32] = (1 << (GPIO %32)) ;
}

irqreturn_t irq_handler_test_start (int irq, void *dev_id , struct pt_regs *regs) 
{
	if (start_test) {
		start_test = false ;
	} else { 
		start_test = true ;
	}
//	printk(KERN_INFO "Alright the test can start now! \n" ) ;
	return IRQ_HANDLED;
}

irqreturn_t irq_handler (int irq, void *dev_id , struct pt_regs *regs) 
{ 	
	// you can also use ktime_get() function which gives nano sec time resolution if required
	// connect the wake line after module has been loaded into kernel.
	// So the first transition from 0->1 is a junk value and it initialises 'wake' to true.
        static bool wake = false ;  
        static bool ack = false ;  
	struct timeval kernel_time;
	char *buf;
	static bool init = false;
	//static int count = 0;
	do_gettimeofday(&kernel_time);
	buf = (char *)&kernel_time;

	if(response == true) {
		ack =false ;
	}
	if (unlikely (!init) ) {
		init = true;
        	wake = true;
		ack = false;  
		printk (KERN_INFO "First junk wake value! Lets start the real deal !\n");
	} else if (start_test){
		if (wake ) {
			wake = false;
		} else {
			wake = true ;
		}

		if (ack == false) {
			if(false == wake) {  
			// Falling edge (Wake enabled)
			      ts_store(wakeline_kobject, &wake_timestamp[0],buf , sizeof(buf)); //enable
			      printk(KERN_INFO "Storing wake_enable value at ");
 			      printk(KERN_INFO " %lu.%lu \n",  kernel_time.tv_sec, kernel_time.tv_usec);
			} else {
			//Rising edge (wake disabled)
			      ts_store(wakeline_kobject, &wake_timestamp[1], buf , sizeof(buf)); //disable
			      printk(KERN_INFO "Storing wake_disable value at ");
 			      printk(KERN_INFO " %lu.%lu \n",  kernel_time.tv_sec, kernel_time.tv_usec);
			      ack = true;
			}
		} else { // Storing acknowledge wake timestamps
			if(false == wake) {
        	        // Falling edge (Wake enabled)
        	              ts_store(wakeline_kobject, &wake_timestamp[2], buf , sizeof(buf));
			      printk(KERN_INFO "Storing wake_ack_enable value at");
 			      printk(KERN_INFO " %lu.%lu \n",  kernel_time.tv_sec, kernel_time.tv_usec);
        	        } else {
        	        //Rising edge (wake disabled)
        	              ts_store(wakeline_kobject, &wake_timestamp[3],buf , sizeof(buf));
			      printk(KERN_INFO "Storing wake_ack_disable value at");
 			      printk(KERN_INFO " %lu.%lu \n",  kernel_time.tv_sec, kernel_time.tv_usec);
        		      ack = false;
		        }
        	}
	}
	return IRQ_HANDLED;
}

void config_interrupt(void) 
{
	s_pGpioRegisters = (struct GpioRegisters *)__io_address(GPIO_BASE);
        SetGPIOFunction(INT_GPIO_PIN, 0b001); // Setting Output function to ledgpiopin
        SetGPIOOutputValue(INT_GPIO_PIN, false);

// Configure Pin 23 to receive wake interrupts
	if ((irq_gpio  = gpio_to_irq(GPIO_PIN)) < 0) {
		printk("GPIO to IRQ mapping failed %s \n ", GPIO_DESC);
		return;
	}
	if ((irq_gpio_start  = gpio_to_irq(START_GPIO_PIN)) < 0) {
		printk("GPIO to Test Start IRQ mapping failed %s \n ", GPIO_DESC_START);
		return;
	}
	
	
	printk(KERN_INFO "Mapped GPIO Pin : %d , and irq_gpio is  %d\n", GPIO_PIN, irq_gpio);
	printk(KERN_INFO "Mapped GPIO Pin : %d , and irq_gpio is  %d\n", START_GPIO_PIN, irq_gpio_start);
	
	if (request_irq(irq_gpio,(irq_handler_t) irq_handler ,
		        IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING, GPIO_DESC, GPIO_DEVICE_DESC)) {
	   printk("IRQ Request failed \n");
	   return;
	}

	if (request_irq(irq_gpio_start,(irq_handler_t) irq_handler_test_start ,
		        IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING, GPIO_DESC_START, GPIO_DEVICE_DESC)) {
	   printk("IRQ Test Start Request failed \n");
	   return;
	}
	return;
}

// Function to release the interrupts
void int_release(void) 
{
	free_irq(irq_gpio, GPIO_DEVICE_DESC);
	return;

}

 
static int __init wake_init(void)
{
	int retval;
	printk(KERN_INFO "Starting the Wake Line kernel Module");
	
	wakeline_kobject = kobject_create_and_add("wakeline", kernel_kobj);
	if(!wakeline_kobject)
		return -ENOMEM;
      
      // Create the files (for attributes) that are associated with this kobject
	retval = sysfs_create_group(wakeline_kobject, &attr_wake_group);
	if(retval)
		kobject_put(wakeline_kobject);
	
	config_interrupt();	
	return retval;
}
static void __exit wake_exit(void)
{

	kobject_put(wakeline_kobject);
	int_release();
}

module_init(wake_init);
module_exit(wake_exit);
MODULE_AUTHOR("Ishaan Biswas");
MODULE_LICENSE("GPL");

