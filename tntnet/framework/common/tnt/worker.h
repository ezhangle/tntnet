/* tnt/worker.h
 * Copyright (C) 2003-2005 Tommi Maekitalo
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */


#ifndef TNT_WORKER_H
#define TNT_WORKER_H

#include <string>
#include <cxxtools/tcpstream.h>
#include <cxxtools/thread.h>
#include <cxxtools/mutex.h>
#include <tnt/comploader.h>
#include <tnt/tntnet.h>
#include <tnt/scope.h>

namespace tnt
{
  class HttpRequest;
  class HttpReply;

  class Worker : public cxxtools::DetachedThread
  {
      static cxxtools::Mutex mutex;

      Tntnet& application;

      static Comploader comploader;

      Scope threadScope;

      pthread_t threadId;
      const char* state;
      time_t lastWaitTime;

      typedef std::set<Worker*> workers_type;
      static workers_type workers;

      static unsigned maxRequestTime;
      static bool enableCompression;

      bool processRequest(HttpRequest& request, std::iostream& socket,
        unsigned keepAliveCount);
      void healthCheck(time_t currentTime);

    public:
      Worker(Tntnet& app);

      virtual void run();

      void dispatch(HttpRequest& request, HttpReply& reply);

      static void timer();

      /// Sets a hard limit for request-time.
      /// When the time is exceeded, this process exits.
      static void setMaxRequestTime(unsigned sec)  { maxRequestTime = sec; }
      static unsigned getMaxRequestTime()          { return maxRequestTime; }

      static workers_type::size_type getCountThreads();

      static void setEnableCompression(bool sw = true)  { enableCompression = sw; }
      static unsigned getEnableCompression()            { return enableCompression; }
  };
}

#endif // TNT_WORKER_H

