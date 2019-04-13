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

from waflib.Configure import conf
from waflib.TaskGen import feature,after_method, before_method
from waflib import Task
from waflib.Tools import c
import shutil

class strip(Task.Task):
    color   = 'GREEN'

    run_str_strip = '${STRIP} -g ${TGT}'

    (run_strip, strip_vars) = Task.compile_fun(run_str_strip)

    vars = strip_vars

    def run(self):
        i = self.inputs[0]
        o = self.outputs[0]

        shutil.copyfile(i.abspath(), o.abspath())
        self.run_strip()

class ihex(Task.Task):
    color   = 'GREEN'
    run_str = '${OBJCOPY} -O ihex ${OBJCOPY_FLAGS} ${SRC} ${TGT}'
    vars    = ['OBJDUMP_FLAGS']

class binary(Task.Task):
    color   = 'GREEN'
    run_str = '${OBJCOPY} -O binary ${OBJCOPY_FLAGS} ${SRC} ${TGT}'
    vars    = ['OBJDUMP_FLAGS']

class listing(Task.Task):
    color   = 'GREEN'
    run_str = '${OBJDUMP} -D ${SRC} > ${TGT}'

class compilelisting(c.c):
        "generate a compiler assembler listing"
        run_str = '${CXX} -Wa,-adhln -g ${ARCH_ST:ARCH} ${CXXFLAGS} ${CPPFLAGS} ${FRAMEWORKPATH_ST:FRAMEWORKPATH} ${CPPPATH_ST:INCPATHS} ${DEFINES_ST:DEFINES} ${CXX_SRC_F}${SRC} -o/dev/null >${TGT}'

class copy(Task.Task):
    color   = 'GREEN'
    def run(self):
        for i,o in zip(self.inputs,self.outputs):
            shutil.copyfile(i.abspath(), o.abspath())

@feature('listings')
@after_method('apply_link')
def generate_listings(self):
    for t in getattr(self, 'compiled_tasks', []):
        input  = t.inputs[0]
        output = t.outputs[0]

        self.create_task('compilelisting', input, output.parent.find_or_declare(output.name + '.lst'),)


@feature('ecpp')
@after_method('apply_link')
def ecpp_generate_map(self):
    if 'cprogram' in self.features or 'cxxprogram' in self.features:
        t = self.link_task
        t.set_outputs(t.outputs[0].change_ext('.map'))
        t.env.append_value('LINKFLAGS', '-Wl,-Map,%s' % t.outputs[-1].abspath())

@feature('ecpp')
@before_method('propagate_uselib_vars')
def ecpp_set_default_optimization(self):
    if 'debug' not in self.features and 'release' not in self.features:
        self.features.append('release')


@feature('ecpp')
@after_method('apply_link', 'propagate_uselib_vars')
def ecpp_linkerscript(self):
    from waflib.Tools.ccroot import USELIB_VARS

    if 'cprogram' in self.features or 'cxxprogram' in self.features:
        t = self.link_task

        # get list of declared linkerscripts
        linkerscripts = t.env['LINKERSCRIPTS'] or []

        # this is a top level linkerscript, add it
        linkerscript = t.env.get_flat('LINKERSCRIPT').strip()

        if linkerscript:
            linkerscripts.append(linkerscript)

        if linkerscripts:
            t.env.append_value('LINKFLAGS', ['-T%s' % s for s in linkerscripts])

            for x in linkerscripts:
                n = self.bld.root.find_resource(x)
                t.dep_nodes.append(n)

@feature('ecpp')
@after_method('process_source')
@before_method('apply_link')
def ecpp_updatecompiledtask(self):
    """Create object files in a subdirectory in order to allow using same source file
       for different build ids"""

    for t in getattr(self,'compiled_tasks',[]):
        node = t.inputs[0]

        if not t.env['ECPP_ENVNAME']:
            self.bld.fatal('ECPP Environment name for "%s" not set' % node.name)

        out = '%s_objects/%s.%d.o' % (t.env['ECPP_ENVNAME'],node.name, self.idx)
        t.outputs[0] = node.parent.find_or_declare(out)

@feature('firmware-hex')
@after_method('apply_link')
def ecpp_generate_firmware_hex(self):
  if 'cprogram' in self.features or 'cxxprogram' in self.features:
      elf_node_orig  = self.link_task.outputs[0]
      elf_node       = elf_node_orig.change_ext(".stripped.elf")

      self.strip_task = self.create_task('strip',elf_node_orig, elf_node)

      tsk = self.create_task('ihex', [elf_node], [elf_node_orig.change_ext('.hex')])