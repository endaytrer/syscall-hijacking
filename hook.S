#define __NR_openat 257
.text
.extern syscall_table_save
.extern sayhi
.globl hooked_openat

hooked_openat:
    pushq %rdi
    pushq %rsi
    pushq %rdx
    pushq %r10
    movq %r10, %rcx

    callq sayhi

    popq %r10
    popq %rdx
    popq %rsi
    popq %rdi
    
    movq syscall_table_save + __NR_openat*8(%rip), %rax
    jmpq *%rax
