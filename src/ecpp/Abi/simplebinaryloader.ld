/* This linkerscript is to be used with simple binary loader */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(header)

MEMORY
{
  UPDATERMEM    (rwx)    : ORIGIN = 0x00000000, LENGTH = 32K
  UPDATERUNUSED (rw!A)   : ORIGIN = 0x00010000, LENGTH = 1M
}

/* Section Definitions */
SECTIONS
{
  .updater_header 0x00000000 :
  {
    KEEP(*(.updater_header))
  } > UPDATERMEM

  .plt  :
  {
    *(.plt)
  } > UPDATERMEM

  .text : ALIGN(4)
  {

    *(.text .text.* .gnu.linkonce.t.*)
    *(.ramfunc .ramfunc.*);
    *(.glue_7t) *(.glue_7)
    *(.rodata .rodata* .gnu.linkonce.r.*)
    *(.ARM.extab* .gnu.linkonce.armextab.*)

    /* Support C constructors, and C destructors in both user code
        and the C library. This also provides support for C++ code. */
    . = ALIGN(4);
    KEEP(*(.init))
    . = ALIGN(4);
    __preinit_array_start = .;
    KEEP (*(.preinit_array))
    __preinit_array_end = .;

    . = ALIGN(4);
    __init_array_start = .;
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array))
    __init_array_end = .;

    . = ALIGN(0x4);
    KEEP (*crtbegin.o(.ctors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*crtend.o(.ctors))

    . = ALIGN(4);
    KEEP(*(.fini))

    . = ALIGN(4);
    __fini_array_start = .;
    KEEP (*(.fini_array))
    KEEP (*(SORT(.fini_array.*)))
    __fini_array_end = .;

    KEEP (*crtbegin.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*crtend.o(.dtors))

    . = ALIGN(4);
  } >UPDATERMEM


  /* .ARM.exidx is sorted, so has to go in its own output section.  */
  PROVIDE_HIDDEN (__exidx_start = .);
  .ARM.exidx :
  {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > UPDATERMEM
  PROVIDE_HIDDEN (__exidx_end = .);

  /* collect all variables for relocation in one section to
   * be able to relocate them in plc firmware. Section must be called got to make strip happy */
  .got : { *(.got .got.*); } > UPDATERMEM

  .data : ALIGN(4)
  {
    *(.data.rel .data.rel.*);
    *(.data .data.*);
    . = ALIGN(4);
  } > UPDATERMEM

  .rel.dyn :
  {
    __rel_start__ = .;

    *(.rel.init)
    *(.rel.text .rel.text.* .rel.gnu.linkonce.t.*)
    *(.rel.fini)
    *(.rel.rodata .rel.rodata.* .rel.gnu.linkonce.r.*)
    *(.rel.data.rel.ro* .rel.gnu.linkonce.d.rel.ro.*)
    *(.rel.data .rel.data.* .rel.gnu.linkonce.d.*)
    *(.rel.tdata .rel.tdata.* .rel.gnu.linkonce.td.*)
    *(.rel.tbss .rel.tbss.* .rel.gnu.linkonce.tb.*)
    *(.rel.ctors)
    *(.rel.dtors)
    *(.rel.got)
    *(.rel.bss  .rel.bss.* .rel.gnu.linkonce.b.*)
    *(.rel.plt  .rel.plt.*)
    *(.rel.iplt .rel.iplt.*)
    *(.rel.*)

    __rel_end__ = .;
  } > UPDATERMEM

  /* .bss section which is used for uninitialized data */
  .bss (NOLOAD) : ALIGN(4)
  {
    __bss_start__ = . ;
    *(.bss .bss.*)
    *(COMMON)

    . = ALIGN(4);
    __bss_end__ = . ;
  } > UPDATERMEM

  __program_end__ = __bss_end__;


  /* __create_plc_program_offset__ = create_program - plc_program; */
  /* __plc_program_len__           = __bss_end__    - plc_program; */

  .dynamic : { *(.dynamic); } >UPDATERUNUSED
  .dynstr  : { *(.dynstr); }  >UPDATERUNUSED
  .dynsym  : { *(.dynsym); }  >UPDATERUNUSED
  .hash     : { *(.hash); }   >UPDATERUNUSED
}
