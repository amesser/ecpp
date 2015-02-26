#! /usr/bin/env python
# encoding: utf-8
#
# Hilscher netx processor build support
#
# Copyright 2013 Andreas Messer <andi@bastelmap.de>
#
# This file is part of the Embedded C++ Platform Project.
#
# Embedded C++ Platform Project (ECPP) is free software: you can
# redistribute it and/or modify it under the terms of the GNU General
# Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any later
# version.
#
# Embedded C++ Platform Project is distributed in the hope that it
# will be useful, but WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with ECPP.  If not, see <http://www.gnu.org/licenses/>.
#
# As a special exception, the copyright holders of ECPP give you
# permission to link ECPP with independent modules to produce an
# executable, regardless of the license terms of these independent
# modules, and to copy and distribute the resulting executable under
# terms of your choice, provided that you also meet, for each linked
# independent module, the terms and conditions of the license of that
# module.  An independent module is a module which is not derived from
# or based on ECPP.  If you modify ECPP, you may extend this exception
# to your version of ECPP, but you are not obligated to do so.  If you
# do not wish to do so, delete this exception statement from your
# version.

from waflib.TaskGen   import feature, after_method
from waflib.Task      import Task, compile_fun
from waflib.Configure import conf
from waflib.Context   import STDOUT, BOTH
import os.path
import struct

netx_cpu = {
  'netx50'  :   'arm966e-s',
  'netx100' :   'arm926ej-s',
  'netx500' :   'arm926ej-s',
}

netx_boards = {
  'netstick' : 'netx50',
  'nxsb100'  : 'netx500',
}

@conf
def ecpp_setupbuild_platform_netx(conf, device, board, platform, arch):
    global netx_cpu

    if not device:
      device, ldscript = netx_boards[board], 'board_hilscher_%s.ld' % board
    else:
      ldscript = 'device_hilscher_%s.ld' % device
    
    cpu = netx_cpu[device]

    envname = 'device_%s' % device

    conf.load('ecpp_toolchain')
    conf.ecpp_setuptoolchain('arm')

    if envname not in conf.all_envs:
      conf.setenv(envname, conf.env)
      
      for x in 'CFLAGS CXXFLAGS LINKFLAGS'.split():
        conf.env.append_value(x, ['-mthumb-interwork','-mcpu=%s' % cpu])

      for x in 'CFLAGS CXXFLAGS LINKFLAGS'.split():
        conf.env.append_value(x + "_compile_thumb", ['-mthumb'])

      for x in 'CFLAGS CXXFLAGS LINKFLAGS'.split():
        conf.env.append_value(x + "_compile_arm", ['-marm'])

      conf.env.append_value('LINKFLAGS', ['-nodefaultlibs', '--static', '-Wl,--gc-sections'])

      ldscript = conf.root.find_node(os.path.join(conf.env['ECPP_DIR'],'linkerscripts',ldscript))

      if ldscript:
        conf.env['LINKERSCRIPT'] = ldscript.abspath()

      conf.env['DEVICE'] = device

      # Mark this env to build a ecpp library for
      conf.env['ECPP_BUILDLIB'] = True
      conf.env.append_value('ECPP_LIBNAME', 'ecpp_%s' % conf.env['DEVICE'].lower()) 

      # new libc needs ecpp library for support code!
      conf.env['STLIB_c']   = ['c', 'ecpp_%s' % conf.env['DEVICE'].lower()]
      # lib gcc needs memcpy from libc
      conf.env['STLIB_gcc'] = ['gcc', 'c']
      
      conf.env.append_value('ECPP_FEATURES',['netx-firmware'])
    else:
      conf.setenv(envname)

class netx_bootimage(object):
    __slots__ = """
      magiccookie
      memcontrol
      applentrypoint
      applchecksum
      applsize
      applloadaddress
      signature
      control0
      control1
      control2
      control3
      control4
      asicctrl
      param
      source
      bootchecksum
      binary
    """.split()

    def encode(self):
        # enforce some values
        self.signature = 0x5854454E
        self.asicctrl  = 1

        fields = [getattr(self,x,0) for x in self.__slots__[0:-1]]
        # compute boot block checksum
        fields[-1] = (sum(fields[0:-1]) * -1) 
        fields = [x & 0xFFFFFFFF for x in fields]
        
        bootblock = struct.pack("<16L", *fields)

        return bootblock + self.binary

    SPI_SPEED = dict((v,i << 1) for i,v in enumerate([
         50000,
        100000,
        200000,
        500000,
       1000000,
       1250000,
       2000000,
       2500000,
       3333300,
       5000000,
      10000000,
      12500000,
      16666600,
      25000000,
    ]))

    SPIFLASH = {
        "magiccookie" : 0xF8BEAF00,
        "memcontrol"  : SPI_SPEED[5000000],
        "source"      : 2,
    }

    def setBootSource(self,source, **kw):
      for k,v in source.items():
        setattr(self,k,v)

      for k,v in kw.items():
        setattr(self,k,v)

    # some default timing values for typical sdram 
    # device 
    SDRAM = {
        "control0" : 0x030D0001,
        "control1" : 0x03C23251,
    }

    setBootDestination = setBootSource

    def setApplication(self,binary, loadaddress, entrypoint):
        padding = [ "", "\x00\x00\x00", "\x00\x00", "\x00"][len(binary) % 4]

        binary = binary + padding
        self.binary = binary
        self.applsize = len(binary) / 4

        offset   = 0
        checksum = 0
        while offset < len(binary):
            value, = struct.unpack_from("<L", binary,offset)
            checksum += value
            offset += 4

        self.applchecksum = checksum
        self.applloadaddress  = loadaddress
        self.applentrypoint   = entrypoint

class netx_rom(Task):
    def run(self):
        elfnode, binnode = self.inputs
        fwnode, = self.outputs

        bld = self.generator.bld
        env = {"LANG" : "C"}
        
        symbols = bld.cmd_and_log([self.env['NM'], elfnode.abspath()], 
          env=env,  output=STDOUT, quiet=BOTH)

        symbols = [line.split(None,2) for line in symbols.splitlines()]
        symbols = dict((x[2],int(x[0],16)) for x in symbols if len(x) == 3)

        sections = bld.cmd_and_log([self.env['OBJDUMP'], "-w", "-h", elfnode.abspath()], 
          env=env, output=STDOUT, quiet=BOTH)

        sections = sections.splitlines()

        while sections:
            l = sections.pop(0)
            if l.lower().startswith("idx"):
                break
          
        lma = None

        for line in sections:
          cols = line.strip().split(None,7)
          flags = cols[7].lower().translate(None," \t").split(",")

          if "load" in flags:
              load = int(cols[4], 16)
              lma = min(lma or load, load)

        bootimg = netx_bootimage()
        bootimg.setBootSource(bootimg.SPIFLASH)
        bootimg.setBootDestination(bootimg.SDRAM)
        bootimg.setApplication(binnode.read('rb'), 
          lma,symbols["_start"])

        fwnode.write(bootimg.encode(),'wb')


@feature('netx-firmware')
@after_method('apply_link')
def generate_netx_firmware(self):
  if 'cprogram' in self.features or 'cxxprogram' in self.features:
      elf_node      = self.link_task.outputs[0]
      elf_node_copy = elf_node.change_ext("-cpy.elf")
      
      self.create_task('copy',elf_node,elf_node_copy)
      self.strip_task = self.create_task('strip',elf_node_copy,None)
    
      tsk = self.create_task('binary', [elf_node_copy], [elf_node_copy.change_ext('.bin')])
      tsk.set_run_after(self.strip_task)

      rom_tsk = self.create_task('netx_rom', [elf_node_copy, tsk.outputs[0]], [elf_node_copy.change_ext('.rom')])
