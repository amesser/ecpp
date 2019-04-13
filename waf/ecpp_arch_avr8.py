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
from waflib.TaskGen   import feature, after_method
from waflib.Configure import conf
import os.path

@feature('avr-firmware')
@after_method('apply_link')
def ecpp_generate_avr8_firmware(self):
  if 'cprogram' in self.features or 'cxxprogram' in self.features:

      elf_node_orig = self.link_task.outputs[0]
      elf_node = elf_node_orig.change_ext('.stripped.elf')

      self.strip_task = self.create_task('strip',elf_node_orig,elf_node)

      tsk = self.create_task('listing', elf_node, elf_node_orig.change_ext('.lst'))

      tsk = self.create_task('ihex', elf_node, elf_node_orig.change_ext('.hex'))
      tsk.env.OBJCOPY_FLAGS = '-R .eeprom'.split()

      tsk = self.create_task('ihex', elf_node, elf_node_orig.change_ext('.eep'))
      tsk.env.OBJCOPY_FLAGS = '-j .eeprom --change-section-lma .eeprom=0'.split()

@conf
def ecpp_setupbuild_arch_avr8(conf,board,device,platform,arch):
    envname = 'device_%s' % device

    conf.load('ecpp_toolchain')
    conf.ecpp_setuptoolchain('avr8')

    create = envname not in conf.all_envs

    if create:
      conf.setenv(envname, conf.env)

      for x in 'ASFLAGS CFLAGS CXXFLAGS LINKFLAGS'.split():
        conf.env.append_value(x, ['-mmcu=%s' % device.lower()])

      for x in 'CFLAGS CXXFLAGS'.split():
        conf.env.append_value(x, ['-Os', '-funsigned-bitfields', '-fshort-enums', '-fomit-frame-pointer'])

      conf.env.append_value('LINKFLAGS', ['--static', '-Wl,--gc-sections'])

      n = conf.root.find_dir(os.path.join(conf.env['ECPP_DIR'],'src'))
      conf.env.append_value('INCLUDES', n.abspath())

      conf.env['DEVICE'] = device

      conf.env.append_value('ECPP_FEATURES',['avr-firmware'])
    else:
        conf.setenv(envname)
