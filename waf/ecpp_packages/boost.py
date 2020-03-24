#! /usr/bin/env python
# encoding: utf-8
#
# Boost library support
#
# Copyright 2020 Andreas Messer <andi@bastelmap.de>
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

from waflib import Utils
import sys

def configure(conf, packages = []):
    packages = Utils.to_list(packages or 'variant')


    include_paths = ['']

    if sys.platform != 'win32':
        # Try to find boot in system directories matching the target tripple

        target_triple = conf.env['TOOL_PREFIX'].strip('-')

        include_paths.extend([
          '/usr/local/%s/include' % target_triple,
          '/usr/%s/include' % target_triple,
        ])

    conf.start_msg('Checking for boost')
    for inc in include_paths:
        conf.env.stash()
        try:
            if inc:
                conf.env.append_value('INCLUDES',[inc])
            for p in packages:
                conf.check(header_name='boost/' + p + '.hpp', features='cxx cxxprogram')
        except conf.errors.ConfigurationError:
            conf.env.revert()
        else:
            conf.end_msg(inc)
            break
    else:
        conf.fatal("Package boost not found")


