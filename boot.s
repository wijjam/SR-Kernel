# Multiboot header constants
.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

# Multiboot header - GRUB looks for this
.section .multiboot
.align 4
.long MAGIC
.long FLAGS  
.long CHECKSUM

# Stack setup
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB stack
stack_top:

# Entry point
.section .text
.global _start
.type _start, @function
_start:
    mov $stack_top, %esp
    call kernel_main
    
    # Halt if kernel returns
    cli
1:  hlt
    jmp 1b

.size _start, . - _start