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

atsam_vars = {
  'atsam4s8b' : {
     'CFLAGS'    : ['-mcpu=cortex-m4', '-mthumb'],
     'CXXFLAGS'  : ['-mcpu=cortex-m4', '-mthumb'],
     'LINKFLAGS' : ['-mcpu=cortex-m4', '-mthumb'],
     'DEFINES'   : ['SAM4S8B=1', '__SAM4S8B__=1', 'BOARD=USER_BOARD'],
  },
}

@conf
def ecpp_setupbuild_platform_atsam(conf, device, board, platform, arch):
    global atsam_vars

    vars     = atsam_vars[device]

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

      # On embedded systems global statics are never destroyed in almost any case. However, C++ generates
      # corresponding destructor calls by default. Since we're not using default libs, cxa_atexit might
      # be unavailable.
      #
      # Lets avoid at least a linker "__dso_handle undefined" error by using standard atexit mechanism
      conf.env.append_value('CXXFLAGS', ['-fno-use-cxa-atexit'])

      conf.env.append_value('LINKFLAGS', ['-nodefaultlibs', '--static', '-Wl,--gc-sections'])

      for x in 'ram flash'.split():
          ldscript = 'device_%s_%s.ld' % (device,x)
          ldscript = conf.root.find_node(os.path.join(conf.env['ECPP_DIR'],'linkerscripts',ldscript ))

          if ldscript:
              conf.env['LINKERSCRIPT_%s' % x] = ldscript.abspath()
          else:
              conf.env['LINKERSCRIPT_%s' % x] = u'Error: Please define a linkerscript in wscript'

      conf.env['DEVICE'] = device

      # new libc needs ecpp library for support code!
      conf.env['STLIB_c']   = ['c', 'gcc']
      conf.env['STLIB_gcc'] = []

      conf.env.append_value('ECPP_FEATURES',['firmware-hex'])
    else:
      conf.setenv(envname)

atmel_atsam_spl_vars = {
  'atsam4s8' : {
     'DEFINES' : '',
  }
}

atmel_atsam_spl_startup = {
  'atsam4s8' : 'startup_atsam4s8.s',
}

@conf
def ecpp_3rdpartybuild_st_spl(bld, id, path, **kw):
    env = bld.all_envs[id]

    vars     = atmel_atsam_spl_vars[env['DEVICE']]
    startup  = atmel_atsam_spl_startup[env['DEVICE']]

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
