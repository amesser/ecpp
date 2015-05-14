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
from waflib import Build
from waflib import Utils
import os.path

@conf
def ecpp_getgcclibpath(conf):
    dir = conf.bldnode.abspath()
    bld = Build.BuildContext(top_dir=dir, out_dir='/tmp')
    bld.progress_bar = 0
    bld.targets = 'dummy'
    bld.logger = conf.logger
    bld.all_envs.update(conf.all_envs)
    bld.env = conf.env

    bld(features='cprogram', source='blub.c', target='dummy')
    bld.compile()

@conf  
def ecpp_setupbuild(conf, id, board = None, device = None, platform = None, arch = None):
    kw = zip('board device platform arch'.split(),(board,device,platform,arch))

    for k,v in kw:
        module = 'ecpp_%s_%s' % (k,v)
         
        if os.path.exists(os.path.join(conf.env['ECPP_DIR'], 'waf' , module + '.py')):
            conf.load(module)

    conf.setenv("")
    envname    = id

    if envname not in conf.all_envs:
        for k,v in kw:
            func = getattr(conf,'ecpp_setupbuild_%s_%s' % (k,v),None)
            if func:
                func(**dict(kw))
                break
        conf.setenv(envname,conf.env)
    else:
        conf.setenv(envname)

    # override build flag
    conf.env['ECPP_BUILDLIB'] = False

@conf
def ecpp_build(bld, id, **kw):
    env = bld.all_envs[id]
    
    features = Utils.to_list(kw.get('features',[]))[:] 
    features.append('ecpp')
    features.extend(env['ECPP_FEATURES'])

    use = Utils.to_list(kw.get('use',[]))[:] 
    use.extend(env['ECPP_USE'])

    if kw['target'] in use:
        use.remove(kw['target'])

    kw['features'] = features
    kw['use']      = use

    bld(env=env,**kw)

def configure(conf):
    conf.load('ecpp_common')
