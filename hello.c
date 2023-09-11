#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/unistd.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Daniel Gu");
MODULE_DESCRIPTION("A simple Linux kernel module");
MODULE_VERSION("1.0");

static struct kprobe kp = {
    .symbol_name = "kallsyms_lookup_name"
};
unsigned long syscall_table_save[1024];
unsigned long *syscall_table;
#define unprotect_memory() \
u64 dummy; \
asm volatile( \
    "movq %%cr0, %0\n" \
    "andq $0xfffffffffffeffff, %0\n" \
    "movq %0, %%cr0\n" \
    : "=r" (dummy)\
    : \
    : \
)

#define protect_memory() asm volatile( \
    "movq %0, %%cr0\n" \
    : \
    : "a" (dummy)\
    : \
)
asmlinkage int hooked_open(const char *filename, int flags, umode_t mode) {
    printk(KERN_INFO "Hello from hook!\n");
    return ((asmlinkage long (*)(const char *, int, umode_t))(syscall_table[__NR_open]))(filename, flags, mode);
}

void find_syscall_table(void) {
    register_kprobe(&kp);
    unsigned long (*kallsyms_lookup_name)(const char *) = (unsigned long (*)(const char *))kp.addr;
    unregister_kprobe(&kp);
    syscall_table = (unsigned long *)kallsyms_lookup_name("sys_call_table");
    // test if sys_close is at syscall_table[]
}
void hook_syscall(u32 syscall_num, void *entry) {
    syscall_table_save[syscall_num] = syscall_table[syscall_num];
    printk(KERN_INFO "Original entry is at    0x%lx\n", syscall_table_save[syscall_num]);
    unprotect_memory();
    printk(KERN_INFO "New entry is at         0x%lx\n", (unsigned long)entry);
    syscall_table[syscall_num] = (unsigned long)entry;
    protect_memory();
    printk(KERN_INFO "New entry injected at   0x%lx\n", syscall_table[syscall_num]);
}
void unhook_syscall(u32 syscall_num) {
    unprotect_memory();
    syscall_table[syscall_num] = syscall_table_save[syscall_num];
    protect_memory();
}
static int __init hello_init(void) {
    printk(KERN_INFO "Hello, world!\n");
    find_syscall_table();
    hook_syscall(__NR_open, hooked_open);
    return 0;
}

static void __exit hello_exit(void) {
    printk(KERN_INFO "Goodbye, world!\n");
    unhook_syscall(__NR_open);
}

module_init(hello_init);
module_exit(hello_exit);