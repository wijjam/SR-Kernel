.section .text
.global isr_wrapper_129
.global isr_wrapper_33
.global isr_wrapper_32


# New keyboard interrupt wrapper (IRQ 1 = interrupt 33)
isr_wrapper_33:
    pusha                           # Save all registers
    call keyboard_interrupt_handler # Call our C keyboard function
    popa                            # Restore all registers
    iret                            # Return from interrupt

# New timer interrupt wrapper (IRQ 0 = interrupt 32)
isr_wrapper_32:
    pusha
    movl %esp, %ebx
    pushl %ebx
    call timer_interrupt_handler
    addl $4, %esp # realign stack
    
    # Compare current and next
    movl current_process, %eax
    movl next_process, %ebx
    cmpl %eax, %ebx
    je skip_switch              # Skip if same
    
    # SAVE current ESP
    movl %esp, (%eax)
    
    # LOAD next ESP
    movl (%ebx), %esp
    
    # Update current_process
    movl %ebx, current_process
    
skip_switch:
    popa
    iret


isr_wrapper_129:
    pusha   # saves the registers
    movl %esp, %ebx
    pushl %ebx
    call system_call_interrupt_handler  # calls system_call_interrupt_handler
    addl $4, %esp # realign stack
    
    movl current_process, %eax
    movl next_process, %ebx

    # SAVE current ESP
    movl %esp, (%eax)
    
    # LOAD next ESP
    movl (%ebx), %esp

    popa
    iret
