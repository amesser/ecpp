GCC ARM Toolchain
=================

The following section will guide you in the creation of a gcc based toolchain for
ARM architecture. First of all get the required software packages from the
following locations. You might want to use the most recent versions

- binutils http://www.gnu.org/software/binutils
- gcc http://gcc.gnu.org
- newlib http://sourceware.org/newlib/
- gdb http://www.gnu.org/software/gdb
- openocd http://sourceforge.net/projects/openocd


Preliminary
-----------

While it is possible to create a toolchain for exactly one arm instruction set, 
this howto concentrates on the creation of a toolchain suitable for different arm
processors. This is called `multilib`. In the default multilib setup, the gcc will 
only build support libraries for `arm` and `thumb` instruction set while gcc itself
will support any arm architecture. This will likely cause problems when compiling code
for newer arm cpus which use `thumb2`. In that case gcc will create thumb2 for your 
own source code but will link it against thumb mode libgcc and libc. As a result the 
object code contains invalid instructions and the programm will not run. Thus it is required
to modify the file ``gcc/config/arm/t-arm-elf`` in order to enable additional
libraries for newer targets. 

You might try to use this :download:`t-arm-elf <t-arm-elf>` as starting point.

Preparation
-----------

Start by firing up your favorite console. Now choose a location
for your toolchain. Depending on your needs you have different
possibilities:

- somewhere in your home directory
- under /usr/local
- somewhere else (I use the `GNU stow`_ tool to make tools available 
  from /usr/local while keeping every tool itself in a separate
  directory like ``arm-eabi-binutils-x.x.x``. In that case you
  have to choose the prefix for each tool individually like 
  ``/usr/local/stow/arm-eabi-binutils-x.x.x`` in order to avoid
  conflicts when multiple versions are available )

Lets setup the console for compiling::

  $ export PREFIX=/usr/local
  $ export TARGET=arm-none-eabi
  $ # Adapt to your machines core number
  $ export MAKEFLAGS="-j2"

binutils
--------

The next step is to compile the gnu binutils package which provides
assembler, linker and other important tools for maintaining binary
file for our target::

  $ tar -xjf binutils-x.x.x.tar.bz2
  $ cd binutils-x.x.x
  $ ./configure --prefix=${PREFIX} --target=${TARGET} \
                --enable-multilib \
                --enable-interwork
  $ make 
  $ make install
  $ cd ..

gcc (compiler)
--------------

This step generates the gcc c and c++ cross compiler for your target.
For the c++ compiler the c++ standard library will be created 
after the c standard library has been generated in the next step.::

  $ tar -xjf gcc-x.x.x.tar.bz2
  $ cp t-arm-elf gcc-x.x.x/gcc/config/arm/t-arm-elf
  $ mkdir gcc_build
  $ cd gcc_build
  $ ../gcc-x.x.x/configure --prefix=${PREFIX} --target=${TARGET} \
                           --enable-languages=c,c++ \
                           --without-headers \
                           --with-newlib \
                           --enable-shared \
                           --enable-softfloat \
                           --enable-multilib \
                           --enable-interwork \
                           --with-system-zlib \
                           --with-ld=${PREFIX}/bin/${TARGET}-ld \
                           --with-as=${PREFIX}/bin/${TARGET}-as
  $ make all-gcc all-target-libgcc
  $ make install-gcc install-target-libgcc
  $ cd ..

newlib
------

Usually you want to have the c library in order to use standard c 
functions in yor programs. It is also needed for compiling c++ 
standard library. For embedded systems the newlib c library is a
good choice. Newlib offers a lot of configuration options. I have 
choosen the following settings

--disable-newlib-register-fini      In embedded systems there is usually no end of 
                                    the program. So we wont need this.
--disable-newlib-atexit-alloc       Dont use malloc for creating atexit entries (we
                                    wont need this either)
--enable-newlib-multithread         On large systems we probably want to use multiple
                                    threads.
--disable-newlib-supplied-syscalls  Newlib comes with an implementation for its
                                    syscalls. I prefer to replace these by my 
                                    implementations. *Omit this if you dont want to provide
                                    these functions by your own*


::

  $ tar -xzf newlib-x.x.x.tar.gz
  $ mkdir newlib_build
  $ cd newlib_build
  $ ../newlib-x.x.x/configure --prefix=${PREFIX} --target=${TARGET} \
                              --disable-newlib-register-fini \
                              --disable-newlib-atexit-alloc \
                              --enable-newlib-multithread \
                              --disable-newlib-supplied-syscalls \
                              --enable-multilib \
                              --enable-interwork
  $ make all
  $ make install
  $ cd ..

gcc (libstdc++)
---------------

Now the c++ standard library can be generated. It requires the header
files from the standard library::

  $ cd gcc_build
  $ make all-target-libstdc++-v3
  $ make install-target-libstdc++-v3
  $ cd ..

gdb
---

If desired gdb in conjunction with openoce can be used to debug
your software on the target.::

  $ tar -xjf gdb-x.x.x.tar.bz2
  $ cd gdb-x.x.x
  $ ./configure --prefix=${PREFIX} --target=${TARGET}
  $ make
  $ make install

openocd
-------

With the help of openocd you can use most of the available JTAG debuggers
to debug your software. It connects the gdb with the target via JTAG.::

  $ tar -xjf openocd-x.x.x
  $ cd openocd-x.x.x
  $ ./configure --prefix=${PREFIX}
  $ make
  $ make install
  $ cd ..

.. _GNU stow: http://www.gnu.org/software/stow/

