/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
 * All rights reserved.
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "ptr.h"

#ifdef RUN_SELF_TESTS

#include "test.h"
#include "callback.h"

namespace ns3 {

class NoCount
{
public:
  NoCount (Callback<void> cb);
  ~NoCount ();
private:
  Callback<void> m_cb;
};
NoCount::NoCount (Callback<void> cb)
  : m_cb (cb)
{}
NoCount::~NoCount ()
{
  m_cb ();
}

class PtrTest : Test
{
public:
  PtrTest ();
  virtual ~PtrTest ();
  virtual bool RunTests (void);
private:
  void DestroyNotify (void);
  Ptr<NoCount> CallTest (Ptr<NoCount> p);
  uint32_t m_nDestroyed;
};

PtrTest::PtrTest ()
  : Test ("Ptr")
{}

PtrTest::~PtrTest ()
{}

void 
PtrTest::DestroyNotify (void)
{
  m_nDestroyed++;
}
Ptr<NoCount> 
PtrTest::CallTest (Ptr<NoCount> p)
{
  return p;
}

bool
PtrTest::RunTests (void)
{
  bool ok = true;

  Callback<void> cb = MakeCallback (&PtrTest::DestroyNotify, this);
  m_nDestroyed = false;
  {
    Ptr<NoCount> p = new NoCount (cb);
  }
  if (m_nDestroyed != 1)
    {
      ok = false;
    }

  m_nDestroyed = 0;
  {
    Ptr<NoCount> p;
    p = new NoCount (cb);
  }
  if (m_nDestroyed != 1)
    {
      ok = false;
    }

  m_nDestroyed = 0;
  {
    Ptr<NoCount> p1;
    p1 = new NoCount (cb);
    Ptr<NoCount> p2 = p1;
  }
  if (m_nDestroyed != 1)
    {
      ok = false;
    }

  m_nDestroyed = 0;
  {
    Ptr<NoCount> p1;
    p1 = new NoCount (cb);
    Ptr<NoCount> p2;
    p2 = p1;
  }
  if (m_nDestroyed != 1)
    {
      ok = false;
    }

  m_nDestroyed = 0;
  {
    Ptr<NoCount> p1;
    p1 = new NoCount (cb);
    Ptr<NoCount> p2 = new NoCount (cb);
    p2 = p1;
  }
  if (m_nDestroyed != 2)
    {
      ok = false;
    }

  m_nDestroyed = 0;
  {
    Ptr<NoCount> p1;
    p1 = new NoCount (cb);
    Ptr<NoCount> p2;
    p2 = new NoCount (cb);
    p2 = p1;
  }
  if (m_nDestroyed != 2)
    {
      ok = false;
    }

  m_nDestroyed = 0;
  {
    Ptr<NoCount> p1;
    p1 = new NoCount (cb);
    p1 = new NoCount (cb);
  }
  if (m_nDestroyed != 2)
    {
      ok = false;
    }

  m_nDestroyed = 0;
  {
    Ptr<NoCount> p1;
    {
      Ptr<NoCount> p2;
      p1 = new NoCount (cb);
      p2 = new NoCount (cb);
      p2 = p1;
    }
    if (m_nDestroyed != 1)
      {
        ok = false;
      }
  }
  if (m_nDestroyed != 2)
    {
      ok = false;
    }

  m_nDestroyed = 0;
  {
    Ptr<NoCount> p1;
    {
      Ptr<NoCount> p2;
      p1 = new NoCount (cb);
      p2 = new NoCount (cb);
      p2 = CallTest (p1);
    }
    if (m_nDestroyed != 1)
      {
        ok = false;
      }
  }
  if (m_nDestroyed != 2)
    {
      ok = false;
    }
  

  return ok;
}

PtrTest g_ptr_test;

}; // namespace ns3

#endif /* RUN_SELF_TESTS */
