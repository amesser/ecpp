#! /usr/bin/env python
# encoding: utf-8
#
# STM32 microcontroller build support
#
# Copyright 2014 Andreas Messer <andi@bastelmap.de>
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

from waflib.Configure import conf
import os.path

stm32_cpu = {
  'stm32f405' :   ('-mcpu=cortex-m4', '-mthumb'),
}

@conf
def ecpp_setupbuild_platform_stm32(conf, device, board, platform, arch):
    global stm32_cpu

    ldscript = 'device_%s.ld' % device
    
    flags = stm32_cpu[device]

    envname = 'device_%s' % device

    conf.load('ecpp_toolchain')
    conf.ecpp_setuptoolchain('arm')

    create = envname not in conf.all_envs
    conf.setenv(envname, conf.env)

    if create:      
      for x in 'CFLAGS CXXFLAGS LINKFLAGS'.split():
        conf.env.append_value(x, flags)

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
      conf.env['STLIB_gcc'] = ['gcc']
      
      