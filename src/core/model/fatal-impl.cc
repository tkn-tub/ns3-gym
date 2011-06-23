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

/**
 * Note on implementation: the singleton pattern we use here is tricky because
 * it has to deal with:
 *   - make sure that whoever calls Register (potentially before main enters and 
 *     before any constructor run in this file) succeeds
 *   - make sure that whoever calls Unregister (potentially before FlushStream runs
 *     but also after it runs) succeeds
 *   - make sure that the memory allocated with new is deallocated with delete before
 *     the program exits so that valgrind reports no leaks
 *
 * This is why we go through all the painful hoops below.
 */

/* File-scope */
namespace {
std::list<std::ostream*> **PeekStreamList (void)
{
  static std::list<std::ostream*> *streams = 0;
  return &streams;
}
std::list<std::ostream*> *GetStreamList (void)
{
  std::list<std::ostream*> **pstreams = PeekStreamList ();
  if (*pstreams == 0)
    {
      *pstreams = new std::list<std::ostream*> ();
    }
  return *pstreams;
}
struct destructor
{
  ~destructor ()
  {
    std::list<std::ostream*> **pstreams = PeekStreamList ();
    delete *pstreams;
    *pstreams = 0;
  }
};
}

void
RegisterStream (std::ostream* stream)
{
  GetStreamList ()->push_back (stream);
}

void
UnregisterStream (std::ostream* stream)
{
  std::list<std::ostream*> **pl = PeekStreamList ();
  if (*pl == 0)
    {
      return;
    }
  (*pl)->remove (stream);
  if ((*pl)->empty ())
    {
      delete *pl;
      *pl = 0;
    }
}


namespace {
/* Overrides normal SIGSEGV handler once the
 * HandleTerminate function is run. */
void sigHandler (int sig)
{
  FlushStreams ();
  std::abort ();
}
}

void 
FlushStreams (void)
{
  std::list<std::ostream*> **pl = PeekStreamList ();
  if (pl == 0)
    {
      return;
    }


  /* Override default SIGSEGV handler - will flush subsequent
   * streams even if one of the stream pointers is bad.
   * The SIGSEGV override should only be active for the
   * duration of this function. */
  struct sigaction hdl;
  hdl.sa_handler=sigHandler;
  sigaction (SIGSEGV, &hdl, 0);

  std::list<std::ostream*> *l = *pl;

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

  delete l;
  *pl = 0;
}

} //FatalImpl
} //ns3
