#ifndef _MYSYSCALL_H
#define _MYSYSCALL_H

#include <linux/unistd.h>
/* Using linux's struct task_struct.flag to store if the process has been traced  */
/* int sys_net_modify(int flags)*/
#define __NR_net_modify (__NR_statx + 1)
#define PF_NET_HOOKED 0x00010000

/* flags for sys_net_modify*/
#define SYSHOOK_ADD 1
#define SYSHOOK_REMOVE 0

#endif