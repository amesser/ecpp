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
from waflib import Context
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
            break
    else:
        conf.fatal(u'No build support file found for build id %s' % id)

    conf.setenv("")
    envname    = id

    if envname in conf.all_envs:
        conf.fatal("Doubly defined build id")

    # setup a new build environment for this particular target
    for k,v in kw:
        func = getattr(conf,'ecpp_setupbuild_%s_%s' % (k,v),None)
        if func:
            func(**dict(kw))
            break
    else:
        conf.fatal(u'No builder found for build id %s' % id)

    # derive a new build environment from target environment for this
    # build id
    conf.setenv(envname,conf.env)
    conf.env['ECPP_ENVNAME'] = envname
    conf.env['ECPP_BUILDID'] = id

    # build ecpp library for this environment
    conf.env['ECPP_BUILDLIB'] =  True

    ecpp_libname = '%s/ecpp' % id.lower()

    # new libc needs ecpp library for support code!
    conf.env.append_value('STLIB_c', 'ecpp')

@conf
def ecpp_selectbuild(bld, id):
    if not isinstance(bld,EcppBuildContext):
        bld.fatal(u'Unexpected context %r' % bld)

    bld.variant = id

@conf
def ecpp_build(bld, **kw):
    # when no name is given to the target, we assume the target
    # lives entirely in the domain of a build id. To separate
    # targets from each other we prepend the build id as namespace
    explicit_names = 'name' in kw

    if 'id' in kw:
        env = bld.all_envs[kw['id']].derive()
    else:
        env = bld.env.derive()

    id = env['ECPP_BUILDID']

    if not explicit_names and not id:
        bld.fatal(u'Must be using build id when not using explicit names when building %r' % (kw.get('name', kw['target'])))

    features = Utils.to_list(kw.get('features',[]))[:]
    features.append('ecpp')
    features.extend(env['ECPP_FEATURES'])

    use = Utils.to_list(kw.get('use',[]))[:]

    if not explicit_names:
        use = list('%s/%s' % (id, x) for x in use)

    kw['features'] = features
    kw['use']      = use

    tg = bld(env=env,**kw)

    # adjust the name of the task generator
    if not explicit_names:
        tg.name = '%s/%s' % (id, kw['target'])

    return tg

@conf
def ecpp_load_package(self, package, **kw):
    path = os.path.dirname(__file__) + os.sep + 'ecpp_packages'

    module = Context.load_tool(package, [path], with_sys_path=True)
    fun = getattr(module, self.fun, None)
    fun(self, **kw)

def configure(conf):
    conf.load('ecpp_common')

# Replace default waf build command with our own version
# We dont want to use different variants in subdirs. Instead we want to use variants
# to identify the current target
class EcppBuildContext(object):
    def get_variant_dir(self):
        return self.out_dir

    variant_dir = property(get_variant_dir, None)

class EcppBuild(EcppBuildContext, Build.BuildContext):
    pass

class EcppInstall(EcppBuildContext, Build.InstallContext):
    pass

class EcppUninstall(EcppBuildContext, Build.UninstallContext):
    pass

class EcppClean(EcppBuildContext, Build.CleanContext):
    pass
