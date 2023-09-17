#include <linux/module.h>

#include "mysyscall.h"


int sys_net_modify(int flag) {
    if (flag) {
        current->flags |= PF_NET_HOOKED;
    } else {
        current->flags &= ~PF_NET_HOOKED;
    }
    printk(KERN_INFO "successfully hooked\n");
    return 0;
}

bool hooked_socket_c(void) {
    printk(KERN_INFO "Is hooked = %d\n", ((current->flags & PF_NET_HOOKED) != 0));
    return ((current->flags & PF_NET_HOOKED) != 0);
}