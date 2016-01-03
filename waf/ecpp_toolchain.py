#! /usr/bin/env python
# encoding: utf-8
#
# Generic gcc toolchain suppport
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

from waflib.Configure import conf, find_program as find_program_orig 
from waflib.Tools.ccroot import USELIB_VARS

USELIB_VARS['ecpp'] = set(['LINKERSCRIPT'])

tool_prefixes = {
  'avr8'    : ['avr-'],
  'arm'     : ['arm-none-eabi-', 'arm-unknown-eabi-'],
  'native'  : [''],
}

@conf 
def find_program(self,filename,**kw):
    prefix = self.env['TOOL_PREFIX'] or ''
    
    if not isinstance(filename,list):
        filename = [filename]
        
    return find_program_orig(self,list(prefix + x for x in filename),**kw)     
    
def options(opt):
    opt.load('gcc')
    opt.load('gxx')


@conf
def ecpp_setuptoolchain(conf, arch):
    global tool_prefixes

    arch = arch.lower()
    envname = 'toolchain_%s' % arch

    if envname not in conf.all_envs:
      conf.setenv(envname, conf.env)
      
      for prefix in tool_prefixes[arch]:
        try:
          conf.env.stash()
          conf.env['TOOL_PREFIX'] = prefix
          
          conf.load('gcc')
          conf.load('gxx')
          conf.load('gas')

          conf.find_program(['strip'],   var='STRIP')
          conf.find_program(['objcopy'], var='OBJCOPY')
          conf.find_program(['objdump'], var='OBJDUMP')
          conf.find_program(['nm'],      var='NM')

          conf.env.append_value('ASFLAGS',   ['-g'])
          conf.env.append_value('CFLAGS',    ['-g', '-Wall'])
          conf.env.append_value('CXXFLAGS',  ['-g', '-std=c++11','-Wall', '-ftemplate-depth=10000'])
        except conf.errors.ConfigurationError:
          conf.env.revert()
        else:
          break
      else:
        conf.fatal('Could not find a valid toolchain for "%s".' % arch)
    else:
      conf.setenv(envname)

