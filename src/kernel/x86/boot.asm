bits 32

section .multiboot
    align 4
    dd 0x1BADB002
    dd 0x00000001
    dd -(0x1BADB002 + 0x00000001)

section .text

global start
extern kernel_init

start:
    cli
    push ebx
    call kernel_init
    hlt
