ENTRY(_start)

SECTIONS
{
    . = 1M;

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }

    . = ALIGN(4096); /* align the paging to be 4KB for the 12 bit flags */
    _paging_dir = .;
    . += 4096;       /* gives 4KB to the dir */
    _paging_table1 = .;
    . += 4096;       /* gives 4KB to the paging table */
    _end = .;        /* start of the heap and end of kernel memory */

}