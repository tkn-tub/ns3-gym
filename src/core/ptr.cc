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
#include "object-base.h"

namespace ns3 {

class NoCount;

template <typename T>
void Foo (void) {}

class PtrTest : Test
{
public:
  PtrTest ();
  virtual ~PtrTest ();
  virtual bool RunTests (void);
  void DestroyNotify (void);
private:
  Ptr<NoCount> CallTest (Ptr<NoCount> p);
  Ptr<NoCount> const CallTestConst (Ptr<NoCount> const p);
  uint32_t m_nDestroyed;
};


class Base : public ObjectBase
{
public:
  Base ();
  virtual ~Base ();
  void Ref (void) const;
  void Unref (void) const;
private:
  mutable uint32_t m_count;
};

class NoCount : public Base
{
public:
  NoCount (PtrTest *test);
  ~NoCount ();
  void Nothing (void) const;
private:
  PtrTest *m_test;
};

Base::Base ()
  : m_count (1)
{}
Base::~Base ()
{}
void 
Base::Ref (void) const
{
  m_count++;
}
void 
Base::Unref (void) const
{
  m_count--;
  if (m_count == 0)
    {
      delete this;
    }
}

NoCount::NoCount (PtrTest *test)
  : m_test (test)
{}
NoCount::~NoCount ()
{
  m_test->DestroyNotify ();
}
void
NoCount::Nothing () const
{}

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

Ptr<NoCount> const 
PtrTest::CallTestConst (Ptr<NoCount> const p)
{
  return p;
}

bool
PtrTest::RunTests (void)
{
  bool ok = true;

  m_nDestroyed = false;
  {
    Ptr<NoCount> p = Create<NoCount> (this);
  }
  if (m_nDestroyed != 1)
    {
      ok = false;
    }

  m_nDestroyed = 0;
  {
    Ptr<NoCount> p;
    p = Create<NoCount> (this);
    p = p;
  }
  if (m_nDestroyed != 1)
    {
      ok = false;
    }

  m_nDestroyed = 0;
  {
    Ptr<NoCount> p1;
    p1 = Create<NoCount> (this);
    Ptr<NoCount> p2 = p1;
  }
  if (m_nDestroyed != 1)
    {
      ok = false;
    }

  m_nDestroyed = 0;
  {
    Ptr<NoCount> p1;
    p1 = Create<NoCount> (this);
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
    p1 = Create<NoCount> (this);
    Ptr<NoCount> p2 = Create<NoCount> (this);
    p2 = p1;
  }
  if (m_nDestroyed != 2)
    {
      ok = false;
    }

  m_nDestroyed = 0;
  {
    Ptr<NoCount> p1;
    p1 = Create<NoCount> (this);
    Ptr<NoCount> p2;
    p2 = Create<NoCount> (this);
    p2 = p1;
  }
  if (m_nDestroyed != 2)
    {
      ok = false;
    }

  m_nDestroyed = 0;
  {
    Ptr<NoCount> p1;
    p1 = Create<NoCount> (this);
    p1 = Create<NoCount> (this);
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
      p1 = Create<NoCount> (this);
      p2 = Create<NoCount> (this);
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
      p1 = Create<NoCount> (this);
      p2 = Create<NoCount> (this);
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

  {
    Ptr<NoCount> p1;
    Ptr<NoCount> const p2 = CallTest (p1);
    Ptr<NoCount> const p3 = CallTestConst (p1);
    Ptr<NoCount> p4 = CallTestConst (p1);
    Ptr<NoCount const> p5 = p4;
    //p4 = p5; You cannot make a const pointer be a non-const pointer.
    // but if you use const_pointer_cast, you can.
    p4 = const_pointer_cast<NoCount> (p5);
    p5 = p1;
    Ptr<NoCount> p;
    if (p == 0)
      {}
    if (p != 0)
      {}
    if (0 == p)
      {}
    if (0 != p)
      {}
    if (p)
      {}
    if (!p)
      {}
  }

  m_nDestroyed = 0;
  {
    NoCount *raw;
    {
      Ptr<NoCount> p = Create<NoCount> (this);
      {
        Ptr<NoCount const> p1 = p;
      }
      raw = GetPointer (p);
      p = 0;
    }
    if (m_nDestroyed != 0)
      {
        ok = false;
      }
    delete raw;
  }

  m_nDestroyed = 0;
  {
    Ptr<NoCount> p = Create<NoCount> (this);
    const NoCount *v1 = PeekPointer (p);
    NoCount *v2 = PeekPointer (p);
    v1->Nothing ();
    v2->Nothing ();
  }
  if (m_nDestroyed != 1)
    {
      ok = false;
    }

  {
    Ptr<Base> p0 = Create<NoCount> (this);
    Ptr<NoCount> p1 = Create<NoCount> (this);
    if (p0 == p1)
      {
        ok = false;
      }
    if (p0 != p1)
      {
      }
    else
      {
        ok = false;
      }
  }
#if 0
  {
    Ptr<NoCount> p = Create<NoCount> (cb);
    Callback<void> callback = MakeCallback (&NoCount::Nothing, p);
    callback ();
  }
  {
    Ptr<const NoCount> p = Create<NoCount> (cb);
    Callback<void> callback = MakeCallback (&NoCount::Nothing, p);
    callback ();
  }
#endif

#if 0
  // as expected, fails compilation.
  {
    Ptr<const Base> p = Create<NoCount> (cb);
    Callback<void> callback = MakeCallback (&NoCount::Nothing, p);
  }
  // local types are not allowed as arguments to a template.
  {
    class B
    {
    public:
      B () {}
    };
    Foo<B> ();
  }
#endif
  

  return ok;
}

PtrTest g_ptr_test;

}; // namespace ns3

#endif /* RUN_SELF_TESTS */
