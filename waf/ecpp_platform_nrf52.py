#! /usr/bin/env python
# encoding: utf-8
#
# Nordic NRF52 microcontroller build support
#
# Copyright 2019 Andreas Messer <andi@bastelmap.de>
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
from waflib import Utils
import os.path

nrf52_vars = {
  'nrf52810' : {
     'CFLAGS'    : ['-mcpu=cortex-m4', '-mthumb', '-mfloat-abi=soft'],
     'CXXFLAGS'  : ['-mcpu=cortex-m4', '-mthumb', '-mfloat-abi=soft'],
     'LINKFLAGS' : ['-mcpu=cortex-m4', '-mthumb'],
  },
}

@conf
def ecpp_setupbuild_platform_nrf52(conf, device, board, platform, arch):
    global nrf52_vars

    ldscript = 'device_%s.ld' % device
    vars     = nrf52_vars[device]

    envname = 'device_%s' % device

    conf.load('ecpp_toolchain ecpp_platform_arm')
    conf.ecpp_setuptoolchain('arm')

    create = envname not in conf.all_envs

    if create:
      conf.setenv(envname, conf.env)

      conf.env['ECPP_ENVNAME'] = envname

      for k,v in vars.items():
        conf.env.append_value(k, Utils.to_list(v))

      for x in 'CFLAGS CXXFLAGS LINKFLAGS'.split():
        conf.env.append_value(x + "_release", ['-Os'])
        conf.env.append_value(x + "_debug",   ['-O0'])

      conf.env.append_value('LINKFLAGS', ['-nodefaultlibs', '--static', '-Wl,--gc-sections'])

      ldscript = conf.root.find_node(os.path.join(conf.env['ECPP_DIR'],'linkerscripts',ldscript))

      if ldscript:
        conf.env['LINKERSCRIPT_ecpp'] = ldscript.abspath()

      conf.env['DEVICE'] = device

      conf.env['STLIB_gcc'] = ['gcc', 'c']

      conf.env.append_value('ECPP_FEATURES',['firmware-hex'])
    else:
      conf.setenv(envname)
