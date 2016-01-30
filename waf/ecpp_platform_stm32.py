#! /usr/bin/env python
# encoding: utf-8
#
# STM32 microcontroller build support
#
# Copyright 2014-2016 Andreas Messer <andi@bastelmap.de>
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

stm32_vars = {
  'stm32f405' : {
     'CFLAGS'    : ['-mcpu=cortex-m4', '-mthumb'],
     'CXXFLAGS'  : ['-mcpu=cortex-m4', '-mthumb'],
     'LINKFLAGS' : ['-mcpu=cortex-m4', '-mthumb'],
  },
}

@feature('stm32-firmware')
@after_method('apply_link')
def ecpp_generate_stm32_firmware(self):
  if 'cprogram' in self.features or 'cxxprogram' in self.features:
      elf_node      = self.link_task.outputs[0]
      elf_node_copy = elf_node.change_ext("-cpy.elf")
      
      self.create_task('copy',elf_node,elf_node_copy)
      self.strip_task = self.create_task('strip',elf_node_copy,None)
    
      tsk = self.create_task('ihex', [elf_node_copy], [elf_node.change_ext('.hex')])
      tsk.set_run_after(self.strip_task)

@conf
def ecpp_setupbuild_platform_stm32(conf, device, board, platform, arch):
    global stm32_vars

    ldscript = 'device_%s.ld' % device
    vars     = stm32_vars[device]

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
        conf.env['LINKERSCRIPT'] = ldscript.abspath()

      conf.env['DEVICE'] = device

      # new libc needs ecpp library for support code!
      conf.env['STLIB_c']   = ['c', 'ecpp_%s' % conf.env['DEVICE'].lower()]
      conf.env['STLIB_gcc'] = ['gcc', 'c']

      # Mark this env to build a ecpp library for
      conf.env['ECPP_BUILDLIB'] = True
      conf.env.append_value('ECPP_LIBNAME', 'ecpp_%s' % conf.env['DEVICE'].lower()) 

      conf.env.append_value('ECPP_FEATURES',['stm32-firmware'])
    else:
      conf.setenv(envname)
      
st_stm32_spl_vars = {
  'stm32f405' : {
     'DEFINES' : 'STM32F405xx USE_HAL_DRIVER',
  }
}

st_stm32_spl_startup = {
  'stm32f405' : 'startup_stm32f405xx.s',
}

@conf
def ecpp_3rdpartybuild_st_spl(bld, id, path, **kw):
    global st_stm32_spl_defines
    env = bld.all_envs[id]
    
    vars     = st_stm32_spl_vars[env['DEVICE']]
    startup  = st_stm32_spl_startup[env['DEVICE']]

    base_path = bld.path.find_dir(path)
    
    if not base_path:
        bld.fatal("Path '%s' not found" % path)

    source = Utils.to_list(kw.get('source',[]))[:]
    source.extend(base_path.ant_glob(['*.c']))
    source.append(base_path.find_node(startup))
    
    includes = Utils.to_list(kw.get('includes',[]))[:]
    includes.extend([path])

    export_defines = Utils.to_list(vars['DEFINES'])
    
    defines = Utils.to_list(kw.get('defines',[]))[:]
    defines.extend(export_defines)

    features = Utils.to_list(kw.get('features',[]))[:]
    features.extend(Utils.to_list('c cstlib cxx cxxstlib'))

    kw['source']          = source
    kw['includes']        = includes
    kw['defines']         = defines
    kw['export_includes'] = [path]
    kw['export_defines']  = export_defines
    kw['features']        = features
    
    bld.ecpp_build(id = id, **kw)
