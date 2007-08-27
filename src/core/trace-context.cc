/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#include "trace-context.h"
#include "trace-context-element.h"
#include "assert.h"

namespace ns3 {

TraceContext::TraceContext ()
  : m_data (0)
{}
TraceContext::TraceContext (TraceContext const &o)
  : m_data (o.m_data)
{
  if (m_data != 0)
    {
      m_data->count++;
    }
}
TraceContext const & 
TraceContext::operator = (TraceContext const &o)
{
  if (m_data != 0)
    {
      m_data->count--;
      if (m_data->count == 0)
        {
          uint8_t *buffer = (uint8_t *)m_data;
          delete [] buffer;
        }
    }
  m_data = o.m_data;
  if (m_data != 0)
    {
      m_data->count++;
    }
  return *this;
}
TraceContext::~TraceContext ()
{
  if (m_data != 0)
    {
      m_data->count--;
      if (m_data->count == 0)
        {
          uint8_t *buffer = (uint8_t *)m_data;
          delete [] buffer;
        }
    }
}

void 
TraceContext::Union (TraceContext const &o)
{
  if (o.m_data == 0)
    {
      return;
    }
  uint16_t currentUid;
  uint16_t i = 0;
  while (i < o.m_data->size) 
    {
      currentUid = o.m_data->data[i];
      uint8_t size = ElementRegistry::GetSize (currentUid);
      uint8_t *selfBuffer = CheckPresent (currentUid);
      uint8_t *otherBuffer = &(o.m_data->data[i+1]);
      if (selfBuffer != 0)
        {
          if (memcmp (selfBuffer, otherBuffer, size) != 0)
            {
              NS_FATAL_ERROR ("You cannot add TraceContexts which "<<
                              "have different values stored in them.");
            }
        }
      else
        {
          DoAdd (currentUid, otherBuffer);
        }
      i += 1 + size;
    }
}

uint8_t *
TraceContext::CheckPresent (uint8_t uid) const
{
  if (m_data == 0)
    {
      return false;
    }
  uint8_t currentUid;
  uint16_t i = 0;
  do {
    currentUid = m_data->data[i];
    uint8_t size = ElementRegistry::GetSize (currentUid);
    if (currentUid == uid)
      {
        return &m_data->data[i+1];
      }
    i += 1 + size;
  } while (i < m_data->size && currentUid != 0);
  return 0;
}


bool
TraceContext::DoAdd (uint8_t uid, uint8_t const *buffer)
{
  NS_ASSERT (uid != 0);
  uint8_t size = ElementRegistry::GetSize (uid);
  uint8_t *present = CheckPresent (uid);
  if (present != 0) {
    if (memcmp (present, buffer, size) == 0)
      {
        return true;
      }
    else
      {
        return false;
      }
  }
  if (m_data == 0)
    {
      uint16_t newSize = 1 + size;
      uint16_t allocatedSize;
      if (newSize > 4)
        {
          allocatedSize = sizeof (struct Data) + newSize - 4;
        }
      else
        {
          allocatedSize = sizeof (struct Data);
        }
      struct Data *data = (struct Data *) (new uint8_t [allocatedSize] ());
      data->size = newSize;
      data->count = 1;
      data->data[0] = uid;
      memcpy (data->data + 1, buffer, size);
      m_data = data;
    }
  else
    {
      uint16_t newSize = m_data->size + 1 + size;
      uint16_t allocatedSize;
      if (newSize > 4)
        {
          allocatedSize = sizeof (struct Data) + newSize - 4;
        }
      else
        {
          allocatedSize = sizeof (struct Data);
        }
      struct Data *data = (struct Data *) (new uint8_t [allocatedSize] ());
      data->size = newSize;
      data->count = 1;
      memcpy (data->data, m_data->data, m_data->size);
      data->data[m_data->size] = uid;
      memcpy (data->data + m_data->size + 1, buffer, size);
      m_data->count--;
      if (m_data->count == 0)
        {
          uint8_t *buffer = (uint8_t *)m_data;
          delete [] buffer;
        }
      m_data = data;
    }
  return true;
}
bool
TraceContext::DoGet (uint8_t uid, uint8_t *buffer) const
{
  if (m_data == 0)
    {
      return false;
    }
  uint8_t currentUid;
  uint16_t i = 0;
  do {
    currentUid = m_data->data[i];
    uint8_t size = ElementRegistry::GetSize (currentUid);
    if (currentUid == uid)
      {
        memcpy (buffer, &m_data->data[i+1], size);
        return true;
      }
    i += 1 + size;
  } while (i < m_data->size && currentUid != 0);
  return false;
}

void 
TraceContext::Print (std::ostream &os) const
{
  if (m_data == 0)
    {
      return;
    }
  uint8_t currentUid;
  uint16_t i = 0;
  do {
    currentUid = m_data->data[i];
    uint8_t size = ElementRegistry::GetSize (currentUid);
    uint8_t *instance = &m_data->data[i+1];
    ElementRegistry::Print (currentUid, instance, os);
    i += 1 + size;
    if (i < m_data->size && currentUid != 0)
      {
        os << " ";
      }
    else
      {
        break;
      }
  } while (true);
}

std::ostream& operator<< (std::ostream& os, const TraceContext &context)
{
  context.Print (os);
  return os;
}

}//namespace ns3

#include "test.h"
#include <sstream>

namespace ns3 {

template <int N>
class Ctx : public TraceContextElement
{
public:
  static uint16_t GetUid (void) {static uint16_t uid = AllocateUid<Ctx<N> > (GetName ()); return uid;}
  static std::string GetName (void) {std::ostringstream oss; oss << "Ctx" << N; return oss.str ();}
  Ctx () : m_v (0) {}
  Ctx (int v) : m_v (v) {}
  void Print (std::ostream &os) {os << N;}
  int Get (void) const { return N;}
private:
  int m_v;
};

class TraceContextTest : public Test
{
public:
  TraceContextTest ();
  virtual bool RunTests (void);
};

TraceContextTest::TraceContextTest ()
  : Test ("TraceContext")
{}
bool 
TraceContextTest::RunTests (void)
{
  bool ok = true;

  TraceContext ctx;
  Ctx<0> v0;
  Ctx<0> v01 = Ctx<0> (1);
  Ctx<1> v1;
  Ctx<2> v2;
  Ctx<3> v3;

  if (ctx.SafeGet (v0))
    {
      ok = false;
    }
  ctx.AddElement (v0);
  ctx.AddElement (v0);
  if (ctx.SafeAdd (v01))
    {
      ok = false;
    }
  ctx.Get (v0);
  ctx.AddElement (v1);
  ctx.Get (v1);
  ctx.Get (v0);
  ctx.Get (v1);

  TraceContext copy = ctx;
  ctx.Get (v0);
  ctx.Get (v1);
  copy.Get (v0);
  copy.Get (v1);
  copy.AddElement (v2);
  copy.Get (v0);
  copy.Get (v1);
  copy.Get (v2);
  ctx.AddElement (v3);
  ctx.Get (v0);
  ctx.Get (v1);
  ctx.Get (v3);

  if (ctx.SafeGet (v2))
    {
      ok = false;
    }
  if (copy.SafeGet (v3))
    {
      ok = false;
    }
  ctx.Union (copy);
  ctx.Get (v2);
  if (copy.SafeGet (v3))
    {
      ok = false;
    }
  copy.Union (ctx);
  copy.Get (v3);  
  
  return ok;
}

static TraceContextTest g_traceContextTest;


}//namespace ns3
