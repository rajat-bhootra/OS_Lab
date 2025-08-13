OUTPUT_FORMAT("elf32-littleriscv")
ENTRY(_start)

SECTIONS
{
  /* Place the program at 0x80000000 */
  . = 0x80000000;
  .text : { *(.text*) }
  .rodata : { *(.rodata*) }
  .data : { *(.data*) }
  .bss : { *(.bss*) *(COMMON) }
}
