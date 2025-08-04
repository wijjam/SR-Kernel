.section .text
.global isr_wrapper_80
.global isr_wrapper_33

isr_wrapper_80:
    pusha                           # Save all registers
    call test_interrupt_handler     # Call C function
    popa                            # Restore all registers
    iret                            # Return from interrupt


# New keyboard interrupt wrapper (IRQ 1 = interrupt 33)
isr_wrapper_33:
    pusha                           # Save all registers
    call keyboard_interrupt_handler # Call our C keyboard function
    popa                            # Restore all registers
    iret                            # Return from interrupt