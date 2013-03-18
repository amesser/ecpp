#! /usr/bin/env python
# encoding: utf-8
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

from waflib.TaskGen import feature, after_method

@feature('firmware')
@after_method('apply_link')
def generate_firmware(self):
  elf_node = self.link_task.outputs[0]
  self.strip_task = self.create_task('strip',elf_node,None)

  tsk = self.create_task('listing', elf_node, elf_node.change_ext('.lst'))
  tsk.set_run_after(self.strip_task)

  tsk = self.create_task('ihex', elf_node, elf_node.change_ext('.hex'))
  tsk.set_run_after(self.strip_task)
  tsk.env.OBJCOPY_FLAGS = '-R .eeprom'.split()

  tsk = self.create_task('ihex', elf_node, elf_node.change_ext('.eep'))
  tsk.set_run_after(self.strip_task)
  tsk.env.OBJCOPY_FLAGS = '-j .eeprom --change-section-lma .eeprom=0'.split()

def configure(conf):
    env = conf.env

    for x in 'CFLAGS CXXFLAGS LINKFLAGS'.split():
        env.append_value(x, '-mmcu=%s' % env.DEVICE.lower())

    for x in 'CFLAGS CXXFLAGS'.split():
      env.append_value(x, ['-Os', '-funsigned-bitfields', '-fshort-enums'])

    env.append_values['LINKFLAGS', ['--static', '-Wl,--gc-sections']
