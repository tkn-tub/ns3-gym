/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 NICTA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Quincy Tse <quincy.tse@nicta.com.au>
 */
#include "fatal-impl.h"

#include <iostream>
#include <list>

#include <cstdlib>
#include <cstdio>

#include <csignal>

namespace ns3 {
namespace FatalImpl {

/* File-scope */
namespace {
  std::list<std::ostream*> *GetStreamList (void)
    {
      static std::list<std::ostream*> streams;
      return &streams;
    }
}

void
RegisterStream (std::ostream* stream)
{
  GetStreamList ()->push_back (stream);
}

void
UnregisterStream (std::ostream* stream)
{
  GetStreamList ()->remove (stream);
}


namespace {
  /* Overrides normal SIGSEGV handler once the
   * HandleTerminate function is run. */
  void sigHandler(int sig) 
  {
    FlushStreams (); 
    std::abort ();
  }
}

void 
FlushStreams (void)
{
  struct sigaction hdl;

  /* Override default SIGSEGV handler - will flush subsequent
   * streams even if one of the stream pointers is bad.
   * The SIGSEGV override should only be active for the
   * duration of this function. */
  hdl.sa_handler=sigHandler;
  sigaction (SIGSEGV, &hdl, 0);

  std::list<std::ostream*> *l = GetStreamList ();

  /* Need to do it this way in case any of the ostream* causes SIGSEGV */
  while (!l->empty ())
    {
      std::ostream* s (l->front ());
      l->pop_front ();
      s->flush ();
    }

  /* Restore default SIGSEGV handler (Not that it matters anyway) */
  hdl.sa_handler=SIG_DFL;
  sigaction (SIGSEGV, &hdl, 0);

  /* Flush all opened FILE* */
  std::fflush (0);

  /* Flush stdandard streams - shouldn't be required (except for clog) */
  std::cout.flush ();
  std::cerr.flush ();
  std::clog.flush ();
}

} //FatalImpl
} //ns3
