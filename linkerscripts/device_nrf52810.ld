/* This linkerscript is to be used with newlibs crt0 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(_start)


MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 192K
  SRAM (rwx) : ORIGIN = 0x20000000, LENGTH =  24K
}

/* The stack size used by the application. NOTE: you need to adjust according to your application. */
_stack_size = DEFINED(_stack_size) ? _stack_size : 0x1000;

/* provide reasonable defaults linking without softdevice */
_nrf_mbr_flash_start         = DEFINED(_nrf_mbr_flash_start) ? _nrf_mbr_flash_start        : 0x00000000;
_nrf_mbr_flash_size          = DEFINED(_nrf_mbr_flash_size) ? _nrf_mbr_flash_size         : 0;

_nrf_softdevice_flash_start  = DEFINED(_nrf_softdevice_flash_start) ? _nrf_softdevice_flash_start : 0x00000000;
_nrf_softdevice_flash_size   = DEFINED(_nrf_softdevice_flash_size) ? _nrf_softdevice_flash_size  : 0;

_nrf_app_flash_start         = DEFINED(_nrf_app_flash_start) ? _nrf_app_flash_start : 0;

_nrf_mbr_ram_start           = DEFINED(_nrf_mbr_ram_start) ? _nrf_mbr_ram_start : 0x20000000;
_nrf_mbr_ram_size            = DEFINED(_nrf_mbr_ram_size) ? _nrf_mbr_ram_size  : 0;

_nrf_softdevice_ram_start    = DEFINED(_nrf_softdevice_ram_start) ? _nrf_softdevice_ram_start    : 0x20000000;
_nrf_softdevice_ram_min_size = DEFINED(_nrf_softdevice_ram_min_size) ? _nrf_softdevice_ram_min_size : 0;

_nrf_softdevice_stack_size   = DEFINED(_nrf_softdevice_stack_size) ? _nrf_softdevice_stack_size   : 0;


/* Section Definitions */
SECTIONS
{
    .mbr_flash _nrf_mbr_flash_start (NOLOAD) :
    {
        . = . + _nrf_mbr_flash_size;
    } >FLASH


    .softdevice_flash _nrf_softdevice_flash_start (NOLOAD) :
    {
        . = . + _nrf_softdevice_flash_size;
    } >FLASH


    .text _nrf_app_flash_start : ALIGN(128)
    {
        KEEP (*(.isr_vector))

        *(.text .text.* .gnu.linkonce.t.*)
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
    } >FLASH

    /* .ARM.exidx is sorted, so has to go in its own output section.  */
    PROVIDE_HIDDEN (__exidx_start = .);
    .ARM.exidx :
    {
      *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    PROVIDE_HIDDEN (__exidx_end = .);

    .mbr_ram _nrf_mbr_ram_start (NOLOAD) :
    {
        . = . + _nrf_mbr_ram_size;
    } > SRAM

    .softdevice_ram _nrf_softdevice_ram_start (NOLOAD) :
    {
        . = . + _nrf_softdevice_ram_min_size;
        KEEP( *(.ramsd .ramsd.*));
        __softdevice_ram_end = .;
    } > SRAM

    .data : ALIGN(4)
    {
        __data_start__ = .;
        *(.ramfunc .ramfunc.*);
        *(.data .data.*);

        . = ALIGN(4);
        __data_end__ = .;
    } > SRAM AT>FLASH

    __data_start_lma__ = LOADADDR(.data);

    /* .bss section which is used for uninitialized data */
    .bss (NOLOAD) : ALIGN(4)
    {
        __bss_start__ = . ;
        *(.bss .bss.*)
        *(COMMON)

        . = ALIGN(4);
        __bss_end__ = . ;
    } > SRAM


    __stack_start__ = ORIGIN(SRAM) + LENGTH(SRAM) - _nrf_softdevice_stack_size - _stack_size;

    /* stack section */
    .stack __stack_start__ (NOLOAD) : ALIGN(8)
    {
        . = . + _stack_size;
        . = . + _nrf_softdevice_stack_size;

        . = ALIGN(8);
        __stack_end__ = .;

        ASSERT( __stack_end__ <= ORIGIN(SRAM) + LENGTH(SRAM), "Required stack does not fit into RAM");
    } > SRAM
}
