.section .text
.global isr_wrapper_33
.global isr_wrapper_32


# New keyboard interrupt wrapper (IRQ 1 = interrupt 33)
isr_wrapper_33:
    pusha                           # Save all registers
    call keyboard_interrupt_handler # Call our C keyboard function
    popa                            # Restore all registers
    iret                            # Return from interrupt

# New keyboard interrupt wrapper (IRQ 0 = interrupt 32)
isr_wrapper_32:
    pusha                           # Save all registers
    call timer_interrupt_handler # Call our C keyboard function
    popa                            # Restore all registers
    iret                            # Return from interrupt