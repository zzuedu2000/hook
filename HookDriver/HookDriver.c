#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/uaccess.h>
//#include <mach/regs-gpio.h>
//#include <mach/hardware.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>


#define DEVICE_NAME "hello"
static struct class *hello_class;

static int hello_open(struct tty_struct *tty, struct file *filp)
{
  printk("open is successd!\n");
  return 0;
}
static int hello_read(struct file *file, char __user *userbuf, size_t bytes, loff_t *off)
{
  unsigned char buf[4];
  buf[0] = 0x11;
  buf[1] = 0x33;
  buf[2] = 0x44;
  buf[3] = 0x55;
  copy_to_user(userbuf, buf, sizeof(buf));
  return (sizeof(buf));
}

static struct file_operations hello_fops = {
    .owner = THIS_MODULE,
    .read = hello_read,
    .open = hello_open,
};
static int major;
static int hello_init(void)
{
  major = register_chrdev(0, DEVICE_NAME, &hello_fops);
  hello_class = class_create(THIS_MODULE, DEVICE_NAME);
  device_create(hello_class, NULL, MKDEV(major, 0), NULL, "hello");
  printk(KERN_ALERT "init is scussed!\n");
  return 0;
}
static void hello_exit(void)
{
  unregister_chrdev(major, DEVICE_NAME);
  device_destroy(hello_class, MKDEV(major, 0));
  class_destroy(hello_class);
  printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Herok");
MODULE_DESCRIPTION("A simple hello world module");
MODULE_ALIAS("A simplest module");
