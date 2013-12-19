Embedded C++ Platform Project
=============================

The embedded C++ Platform Project has just started as
base for tiny Atmel AVR firmware. It tries to help
the hobby developers with templates, drivers and build 
scripts to aid and ease development of applications 
running on embedded systems.

Features
--------

Build Scripts
~~~~~~~~~~~~~

As base for the build scripts, the `waf meta build system`_
was used. Currently the following targets are supported:

- Atmel AVR8
  All Targets supported by GCC compiler toolchain

- ARM
  - Hilscher netX50 (Very Very Preliminary)

.. _`waf meta build system`: http://code.google.com/p/waf/

Devices
~~~~~~~

Important part of embedded softwares are drivers to external
components. Currently implemented are:

- SDCard Driver for SPI Mode
- MCP3204 ADC Driver (SPI Mode) 

Protocols
~~~~~~~~~

Some initial work has been done to support the following protocols:

- USB, USB HID (Descriptors)

Authors
-------

Currently only me: Andreas Messer <andi@bastelmap.de>

References
----------

.. target-notes::
