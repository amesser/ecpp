#! /usr/bin/env python
# encoding: utf-8

from waflib.Configure import conf, find_program as find_program_orig 
from waflib.Tools import gxx
from waflib.Tools import ar
from waflib.Tools import c
from waflib.TaskGen import feature,after_method, before_method
from waflib import Task

@conf 
def find_program(self,filename,**kw):
    prefix = self.env['TOOL_PREFIX'] or ''
    
    if not isinstance(filename,list):
        filename = [filename]
        
    return find_program_orig(self,list(prefix + x for x in filename),**kw)     
    

class strip(Task.Task):
    color   = 'GREEN'
    run_str = '${STRIP} -g ${SRC}'

class ihex(Task.Task):
    color   = 'GREEN'
    run_str = '${OBJCOPY} -O ihex ${OBJCOPY_FLAGS} ${SRC} ${TGT}'
    vars    = ['OBJDUMP_FLAGS']

class listing(Task.Task):
    color   = 'GREEN'
    run_str = '${OBJDUMP} -D ${SRC} > ${TGT}'

class compilelisting(c.c):
        "generate a compiler assembler listing"
        run_str = '${CXX} -Wa,-adhln -g ${ARCH_ST:ARCH} ${CXXFLAGS} ${CPPFLAGS} ${FRAMEWORKPATH_ST:FRAMEWORKPATH} ${CPPPATH_ST:INCPATHS} ${DEFINES_ST:DEFINES} ${CXX_SRC_F}${SRC} -o/dev/null >${TGT}'

@feature('firmware')
@after_method('apply_link')
def generate_listings(self):
    for t in getattr(self, 'compiled_tasks', []):
        input  = t.inputs[0]
        output = t.outputs[0]
        
        self.create_task('compilelisting', input, output.parent.find_or_declare(output.name + '.lst'),)

     
@feature('firmware')
@after_method('apply_link')
def generate_firmware(self):
  elf_node = self.link_task.outputs[0]
  self.strip_task = self.create_task('strip',elf_node,None)

  tsk = self.create_task('listing', elf_node, elf_node.change_ext('.lst'))
  tsk.set_run_after(self.strip_task)
  
  tsk = self.create_task('ihex', elf_node, elf_node.change_ext('.flash'))
  tsk.set_run_after(self.strip_task)
  tsk.env.OBJCOPY_FLAGS = '-R .eeprom'.split()

  tsk = self.create_task('ihex', elf_node, elf_node.change_ext('.eeprom'))
  tsk.set_run_after(self.strip_task)
  tsk.env.OBJCOPY_FLAGS = '-j .eeprom --change-section-lma .eeprom=0'.split()


tool_prefixes = {
    'avr8' : 'avr-'
}

device2arch = {
  'attiny2313' : 'avr8',
}


#def configure(conf):
#  conf.env.append_value('CXXFLAGS', , '-std=c++11', '-Wall',])
#  conf.env.append_value('CXXFLAGS', ['-mmcu=attiny2313', '-DF_CPU=7680000UL'])
#  conf.env.append_value('CXXFLAGS', .split())
#  conf.env.append_value('LINKFLAGS', ['-static', '-mmcu=attiny2313', '-Wl,--gc-sections'])
#  conf.recurse('src')


device_cxxflags = {
   'attiny2313' : ['-mmcu=attiny2313']              
}

device_linkflags = {
   'attiny2313' : ['-mmcu=attiny2313']              
}


@feature('firmware')
@before_method('process_rule')
def prepare_environment(self):
    global tool_prefix, device2arch, device_cxxflags, device_linkflags
    
    self.use = ['PLATFORM'] + self.to_list(getattr(self,'use',''))
    
    device = self.device
    
    arch = device2arch[device];
    env  = self.bld.all_envs['ARCH_' + arch.upper()]
    env = env.derive()

    env.append_value('CXXFLAGS',   device_cxxflags[device])    
    env.append_value('LINKFLAGS', ['--static', '-Wl,--gc-sections'] + device_linkflags[device])

    env.append_value('CXXFLAGS',  ['-Os', '-funsigned-bitfields', '-fshort-enums'])    
    env.append_value('CXXFLAGS',  ['-Os', '-funsigned-bitfields', '-fshort-enums'])    
    
    self.env = env
    

@conf
def load_tools(conf,arch):
    global tool_prefix
    
    arch = arch.lower()    
    prefix = tool_prefixes[arch]
        
    env = conf.env
    
    conf.setenv('ARCH_' + arch.upper(), env)
    
    conf.env.TOOL_PREFIX = prefix
    
    conf.load('gxx')
    conf.load('ar')
    conf.find_program(['strip'],   var='STRIP')
    conf.find_program(['objcopy'], var='OBJCOPY')
    conf.find_program(['objdump'], var='OBJDUMP')

    conf.env.append_value('CXXFLAGS', ['-std=c++11', '-Wall',])
    
    conf.setenv('')
    
def options(opt):
    opt.load('gxx')
    opt.load('ar')
  
def configure(conf):
    conf.env.append_value('INCLUDES_PLATFORM', conf.path.find_dir('src').abspath())    


  
def build(ctx):
  pass
