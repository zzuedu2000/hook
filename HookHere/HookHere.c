#include <linux/kallsyms.h>
#include <linux/cpu.h>
#include <linux/kprobes.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/device.h>

#define OPTSIZE 5

char saved_op[OPTSIZE] = {0};
char jump_op[OPTSIZE] = {0};

int (*ptr_tmp_hello_read)(struct file *file, char __user *userbuf, size_t bytes, loff_t *off);
int (*ptr_orig_hello_read)(struct file *file, char __user *userbuf, size_t bytes, loff_t *off);

int stub_hello_conntrack_in(struct file *file, char __user *userbuf, size_t bytes, loff_t *off)
{
    printk("hook stub conntrack\n");
    return 0;
}
int hook_hello_read(struct file *file, char __user *userbuf, size_t bytes, loff_t *off)
{
    printk(KERN_EMERG "hook conntrack herok\n");
    return ptr_tmp_hello_read(file, userbuf, bytes, off);
}

static void *(*ptr_poke_smp)(void *addr, const void *opcode, size_t len);

static __init int replace_function__init(void)
{
    s32 hook_offset, orig_offset;

    printk("hello replace_function__init\n");
	
    // 这个poke函数完成的就是重映射，写text段
    //ptr_poke_smp = kallsyms_lookup_name("text_poke_smp");
	ptr_poke_smp = kallsyms_lookup_name("text_poke");
    if (!ptr_poke_smp)
    {
        printk(KERN_INFO "err");
        return -1;
    }

    //找到需要hook的函数
    ptr_orig_hello_read = kallsyms_lookup_name("hello_read");
    printk(KERN_EMERG "ptr_orig_hello_read=%#x\n", ptr_orig_hello_read);
    if (!ptr_orig_hello_read)
    {
        printk("err");
        return -1;
    }

    jump_op[0] = 0xe9; //jmp指令
    // 计算目标hook函数到当前位置的相对偏移
    hook_offset = (s32)((long)hook_hello_read - (long)ptr_orig_hello_read - OPTSIZE);
    // 后面4个字节为一个相对偏移
    (*(s32 *)(&jump_op[1])) = hook_offset;

    saved_op[0] = 0xe9;
    //计算目标原始函数将要执行的位置到当前位置的偏移
    orig_offset = (s32)((long)ptr_orig_hello_read + OPTSIZE - ((long)stub_hello_conntrack_in + OPTSIZE));
    (*(s32 *)(&saved_op[1])) = orig_offset;

    get_online_cpus();

    // 替换操作！
    ptr_poke_smp(stub_hello_conntrack_in, saved_op, OPTSIZE);
    ptr_tmp_hello_read = stub_hello_conntrack_in;
    printk(KERN_EMERG "ptr_tmp_hello_read=%#x\n", ptr_tmp_hello_read);

    barrier();
    ptr_poke_smp(ptr_orig_hello_read, jump_op, OPTSIZE);

    put_online_cpus();

    return 0;
}

static __exit void replace_function_exit(void)
{
    get_online_cpus();
    ptr_poke_smp(ptr_orig_hello_read, saved_op, OPTSIZE);
    ptr_poke_smp(stub_hello_conntrack_in, jump_op, OPTSIZE);
    barrier();
    put_online_cpus();
}
module_init(replace_function__init);
module_exit(replace_function_exit);

MODULE_DESCRIPTION("hook test");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.1");

