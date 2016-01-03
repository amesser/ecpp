/* This linkerscript is to be used with newlibs crt0 */

ENTRY(_start)

MEMORY
{
  INTRAM0(rwx)      : ORIGIN = 0x00000000,  LENGTH = 32K
  INTRAM1(rwx)      : ORIGIN = 0x00008000,  LENGTH = 32K
  INTRAM2(rwx)      : ORIGIN = 0x00010000,  LENGTH = 32K
  INTRAM4(rwx)      : ORIGIN = 0x00018000,  LENGTH = 32K
  DTCM(rw)          : ORIGIN = 0x10000000,  LENGTH = 8K
  SDRAM(rwx)        : ORIGIN = 0x80000000,  LENGTH = 8M
}

SECTIONS
{
  /* name of dynamic loader */
  .interp        : { *(.interp) 	}
  /* minimal relocation table */
  .hash          : { *(.hash)		}
  .dynsym        : { *(.dynsym)		}
  .dynstr        : { *(.dynstr)		}
  /* gnu version information */
  .gnu.version   : { *(.gnu.version)	}
  .gnu.version_d   : { *(.gnu.version_d)	}
  .gnu.version_r   : { *(.gnu.version_r)	}
  /* reloaction information */
  .rel.init      : { *(.rel.init)	}
  .rela.init     : { *(.rela.init)	}
  .rel.text      :
    {
      *(.rel.text)
      *(.rel.text.*)
      *(.rel.gnu.linkonce.t*)
    }
  .rela.text     :
    {
      *(.rela.text)
      *(.rela.text.*)
      *(.rela.gnu.linkonce.t*)
    }
  .rel.fini      : { *(.rel.fini)	}
  .rela.fini     : { *(.rela.fini)	}
  .rel.rodata    :
    {
      *(.rel.rodata)
      *(.rel.rodata.*)
      *(.rel.gnu.linkonce.r*)
    }
  .rela.rodata   :
    {
      *(.rela.rodata)
      *(.rela.rodata.*)
      *(.rela.gnu.linkonce.r*)
    }
  .rel.data      :
    {
      *(.rel.data)
      *(.rel.data.*)
      *(.rel.gnu.linkonce.d*)
    }
  .rela.data     :
    {
      *(.rela.data)
      *(.rela.data.*)
      *(.rela.gnu.linkonce.d*)
    }
  .rel.ctors     : { *(.rel.ctors)	}
  .rela.ctors    : { *(.rela.ctors)	}
  .rel.dtors     : { *(.rel.dtors)	}
  .rela.dtors    : { *(.rela.dtors)	}
  .rel.got       : { *(.rel.got)	}
  .rela.got      : { *(.rela.got)	}
  .rel.sdata     :
    {
      *(.rel.sdata)
      *(.rel.sdata.*)
      *(.rel.gnu.linkonce.s*)
    }
  .rela.sdata     :
    {
      *(.rela.sdata)
      *(.rela.sdata.*)
      *(.rela.gnu.linkonce.s*)
    }
  .rel.sbss      : { *(.rel.sbss)	}
  .rela.sbss     : { *(.rela.sbss)	}
  .rel.bss       : { *(.rel.bss)	}
  .rela.bss      : { *(.rela.bss)	}
  .rel.plt       : { *(.rel.plt)	}
  .rela.plt      : { *(.rela.plt)	}
  /* jump table for shared library */
  .plt      : { *(.plt)	}

  /* text segment */
  .text      :
  {
    *(.text)
    *(.text.*)
    *(.stub)
    /* .gnu.warning sections are handled specially by elf32.em.  */
    *(.gnu.warning)
    *(.gnu.linkonce.t*)
    *(.glue_7t) *(.glue_7)
  } >SDRAM
  
  .init          : 
  { 
    KEEP (*(.init))
  } >SDRAM
  
  .ARM.exidx : 
  {
    __exidx_start = .;
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    __exidx_end = .;
  } > SDRAM
  _etext = .;
  PROVIDE (etext = .);
  .fini      :
  {
    KEEP (*(.fini))
  } >SDRAM = 0
  .rodata   : { *(.rodata) *(.rodata.*) *(.gnu.linkonce.r*) } >SDRAM
  .rodata1   : { *(.rodata1) } >SDRAM
  .eh_frame_hdr : { *(.eh_frame_hdr) } >SDRAM

  /* data segment *>
  /* Adjust the address for the data segment.  We want to adjust up to
     the same address within the page on the next page up.  */
  . = ALIGN(256) + (. & (256 - 1));
  .data    :
  {
    *(.data)
    *(.data.*)
    *(.gnu.linkonce.d*)
    SORT(CONSTRUCTORS)
  } >SDRAM
  .data1   : { *(.data1) } >SDRAM
  .eh_frame : { KEEP (*(.eh_frame)) } >SDRAM
  .gcc_except_table : { *(.gcc_except_table) } >SDRAM
  .ctors   : 
  {
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  } >SDRAM
   .dtors         :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } >SDRAM
  .jcr            : { KEEP (*(.jcr)) } >SDRAM
  .got           : { *(.got.plt) *(.got) } >SDRAM
  .dynamic       : { *(.dynamic) } >SDRAM
  /* We want the small data sections together, so single-instruction offsets
     can access them all, and initialized data all before uninitialized, so
     we can shorten the on-disk segment size.  */
  .sdata     : 
  {
    *(.sdata) 
    *(.sdata.*)
    *(.gnu.linkonce.s.*)
  } >SDRAM
  _edata = .;
  PROVIDE (edata = .);

  /* bss segment */
  __bss_start = .;
  __bss_start__ = .;
  .sbss (NOLOAD):
  {
    *(.dynsbss)
    *(.sbss)
    *(.sbss.*)
    *(.scommon)
  } >SDRAM
  .bss (NOLOAD):
  {
   *(.dynbss)
   *(.bss)
   *(.bss.*)
   *(COMMON)
   /* Align here to ensure that the .bss section occupies space up to
      _end.  Align after .bss to ensure correct alignment even if the
      .bss section disappears because there are no input sections.  */
   . = ALIGN(32 / 8);
  } >SDRAM
  . = ALIGN(32 / 8);
  _end = .;
  _bss_end__ = . ; __bss_end__ = . ; __end__ = . ;
  PROVIDE (end = .);

  /* stacks */
  .stack ALIGN(0x4000) (NOLOAD): { 
    *(.stack)
    LONG(0);
    . = ALIGN(0x4000);
    __stack = .;
  } >SDRAM


  /* These must appear regardless of  .  */
  /* Stabs debugging sections.  */
  .stab 0 : { *(.stab) }
  .stabstr 0 : { *(.stabstr) }
  .stab.excl 0 : { *(.stab.excl) }
  .stab.exclstr 0 : { *(.stab.exclstr) }
  .stab.index 0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment 0 : { *(.comment) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  .debug_ranges   0 : { *(.debug_ranges) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }

}

