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
  bool m_destroyed;
};

PtrTest::PtrTest ()
  : Test ("Ptr")
{}

PtrTest::~PtrTest ()
{}

void 
PtrTest::DestroyNotify (void)
{
  m_destroyed = true;
}


bool
PtrTest::RunTests (void)
{
  bool ok = true;

  Callback<void> cb = MakeCallback (&PtrTest::DestroyNotify, this);
  m_destroyed = false;
  {
    Ptr<NoCount> p = new NoCount (cb);
  }
  if (!m_destroyed)
    {
      ok = false;
    }
  m_destroyed = false;
  {
    Ptr<NoCount> p;
    p = new NoCount (cb);
  }
  if (!m_destroyed)
    {
      ok = false;
    }
  

  return ok;
}

PtrTest g_ptr_test;

}; // namespace ns3

#endif /* RUN_SELF_TESTS */
