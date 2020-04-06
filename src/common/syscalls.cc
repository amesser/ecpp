/*
 *  Copyright 2013 Andreas Messer <andi@bastelmap.de>
 *
 *  Newlib Syscall Implementation
 *
 *  This file is part of the Embedded C++ Platform Project.
 *
 *  Embedded C++ Platform Project (ECPP) is free software: you can
 *  redistribute it and/or modify it under the terms of the GNU General
 *  Public License as published by the Free Software Foundation,
 *  either version 3 of the License, or (at your option) any later
 *  version.
 *
 *  Embedded C++ Platform Project is distributed in the hope that it
 *  will be useful, but WITHOUT ANY WARRANTY; without even the implied
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ECPP.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  As a special exception, the copyright holders of ECPP give you
 *  permission to link ECPP with independent modules to produce an
 *  executable, regardless of the license terms of these independent
 *  modules, and to copy and distribute the resulting executable under
 *  terms of your choice, provided that you also meet, for each linked
 *  independent module, the terms and conditions of the license of that
 *  module.  An independent module is a module which is not derived from
 *  or based on ECPP.  If you modify ECPP, you may extend this exception
 *  to your version of ECPP, but you are not obligated to do so.  If you
 *  do not wish to do so, delete this exception statement from your
 *  version.
 *  */
#include "common/syscalls.h"
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

volatile uint8_t dummy = 1;

#if 0
void abort()
{

  /* loop forever */
  while (1) dummy = 1;
}
#endif

void _exit(int)
{
  while (1) dummy = 1;
}

void atexit()
{
}

void _kill(void)
{
  while (1) dummy = 1;
}

int _getpid(void)
{
  return 0;
}

void* _sbrk(void)
{
  return 0;
}

int _write(void)
{
  return -1;
}

void _close(void)
{
}

void _fstat(void)
{

}

void _isatty(void)
{

}

void _lseek(void)
{

}

int _read(void)
{
  return -1;
}

#ifdef __cplusplus
};
#endif
