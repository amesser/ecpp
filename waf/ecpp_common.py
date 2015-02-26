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

class strip(Task.Task):
    color   = 'GREEN'
    run_str = '${STRIP} -g ${SRC}'

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
            o.write(i.read("rb"),"wb")
        
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
@after_method('apply_link')
def ecpp_linkerscript(self):
    if 'cprogram' in self.features or 'cxxprogram' in self.features:
        t = self.link_task
        linkerscript = t.env['LINKERSCRIPT']

        if linkerscript:
            t.env.append_value('LINKFLAGS', '-T%s' % linkerscript)
