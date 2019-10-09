/*
 *  Copyright 2019 Andreas Messer <andi@bastelmap.de>
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
#ifndef ECPP_EXECUTION_JOBQUEUE_HPP_
#define ECPP_EXECUTION_JOBQUEUE_HPP_

#include <cstdint>

namespace ecpp::Execution {
  /* Implementation of a simple function pointer based task queue
    * mechanism. Intended to be used for serialize and asynchronous processing
    * of tasks of limited duration. */
  class JobQueue;

  class Job
  {
  public:
    typedef void (*HandlerType)(Job &);

    constexpr Job() {};
    constexpr Job(HandlerType handler) : Handler{ handler} {};

    void setHandler(HandlerType  handler)
    {
      Handler = handler;
    }

  protected:
    void run()
    {
      if (nullptr == Handler)
        return;


      Handler(*this);
    }

  private:
    Job         *NextJob  {nullptr};
    HandlerType  Handler  {nullptr};

    friend class JobQueue;
  };

  class JobQueue
  {
  public:
    void enqueue(Job* job);
    Job* next();

  private:
    Job*    Head;
    Job*    Tail;
  };
}


#endif /* ECPP_EXECUTION_JOBQUEUE_HPP_ */
