/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
#include "tags.h"
#include <string.h>

namespace ns3 {

TagRegistry *
TagRegistry::GetInstance (void)
{
  static TagRegistry registry;
  return &registry;
}

TagRegistry::TagRegistry ()
  : m_sorted (false)
{}


void 
TagRegistry::Record (std::string uuid, PrettyPrinter prettyPrinter, Destructor destructor)
{
  NS_ASSERT (!m_sorted);
  struct TagInfoItem item;
  item.uuid = uuid;
  item.printer = prettyPrinter;
  item.destructor = destructor;
  m_registry.push_back (item);
}
bool 
TagRegistry::CompareItem (const struct TagInfoItem &a, const struct TagInfoItem &b)
{
  return a.uuid < b.uuid;
}
uint32_t 
TagRegistry::LookupUid (std::string uuid)
{
  if (!m_sorted) 
    {
      std::sort (m_registry.begin (), m_registry.end (), &TagRegistry::CompareItem);
      m_sorted = true;
    }
  NS_ASSERT (m_sorted);
  uint32_t uid = 1;
  for (TagsDataCI i = m_registry.begin (); i != m_registry.end (); i++) 
    {
      if (i->uuid == uuid) 
        {
          return uid;
        }
      uid++;
    }
  // someone asked for a uid for an unregistered uuid.
  NS_ASSERT (!"You tried to use unregistered tag: make sure you create an instance of type TagRegistration<YouTagType>.");
  // quiet compiler
  return 0;
}
void 
TagRegistry::PrettyPrint (uint32_t uid, uint8_t buf[Tags::SIZE], std::ostream &os)
{
  NS_ASSERT (uid > 0);
  uint32_t index = uid - 1;
  NS_ASSERT (m_registry.size () > index);
  PrettyPrinter prettyPrinter = m_registry[index].printer;
  if (prettyPrinter != 0) 
    {
      prettyPrinter (buf, os);
    }
}
void 
TagRegistry::Destruct (uint32_t uid, uint8_t buf[Tags::SIZE])
{
  NS_ASSERT (uid > 0);
  uint32_t index = uid - 1;
  NS_ASSERT (m_registry.size () > index);
  Destructor destructor = m_registry[index].destructor;
  NS_ASSERT (destructor != 0);
  destructor (buf);
}



#ifdef USE_FREE_LIST

struct Tags::TagData *Tags::gFree = 0;
uint32_t Tags::gN_free = 0;

struct Tags::TagData *
Tags::AllocData (void)
{
  struct Tags::TagData *retval;
  if (gFree != 0) 
    {
      retval = gFree;
      gFree = gFree->m_next;
      gN_free--;
    } 
  else 
    {
      retval = new struct Tags::TagData ();
    }
  return retval;
}

void
Tags::FreeData (struct TagData *data)
{
  if (gN_free > 1000) 
    {
      delete data;
      return;
    }
  gN_free++;
  data->m_next = gFree;
  data->m_id = 0;
  gFree = data;
}
#else
struct Tags::TagData *
Tags::AllocData (void)
{
  struct Tags::TagData *retval;
  retval = new struct Tags::TagData ();
  return retval;
}

void
Tags::FreeData (struct TagData *data)
{
  delete data;
}
#endif

bool
Tags::Remove (uint32_t id)
{
  bool found = false;
  for (struct TagData *cur = m_next; cur != 0; cur = cur->m_next) 
    {
      if (cur->m_id == id) 
        {
          found = true;
        }
    }
  if (!found) 
    {
      return false;
    }
  struct TagData *start = 0;
  struct TagData **prevNext = &start;
  for (struct TagData *cur = m_next; cur != 0; cur = cur->m_next) 
    {
      if (cur->m_id == id) 
        {
          /**
           * XXX
           * Note: I believe that we could optimize this to
           * avoid copying each TagData located after the target id
           * and just link the already-copied list to the next tag.
           */
          continue;
        }
      struct TagData *copy = AllocData ();
      copy->m_id = cur->m_id;
      copy->m_count = 1;
      copy->m_next = 0;
      memcpy (copy->m_data, cur->m_data, Tags::SIZE);
      *prevNext = copy;
      prevNext = &copy->m_next;
    }
  *prevNext = 0;
  RemoveAll ();
  m_next = start;
  return true;
}

void 
Tags::PrettyPrint (std::ostream &os)
{
  for (struct TagData *cur = m_next; cur != 0; cur = cur->m_next) 
    {
      TagRegistry::GetInstance ()->PrettyPrint (cur->m_id, cur->m_data, os);
    }
}


}; // namespace ns3

#ifdef RUN_SELF_TESTS

#include "ns3/test.h"
#include <iomanip>
#include <iostream>

namespace ns3 {

static bool g_a;
static bool g_b;
static bool g_c;
static bool g_z;

class TagsTest : Test {
public:
  TagsTest ();
  virtual ~TagsTest ();
  virtual bool RunTests (void);
};

struct myTagA {
  uint8_t a;
};
struct myTagB {
  uint32_t b;
};
struct myTagC {
  uint8_t c [Tags::SIZE];
};
struct myInvalidTag {
  uint8_t invalid [Tags::SIZE+1];
};
struct myTagZ {
  uint8_t z;
};

class MySmartTag 
{
public:
  MySmartTag ()
  {
    std::cout << "construct" << std::endl;
  }
  MySmartTag (const MySmartTag &o)
  {
    std::cout << "copy" << std::endl;
  }
  ~MySmartTag ()
  {
    std::cout << "destruct" << std::endl;
  }
  MySmartTag &operator = (const MySmartTag &o)
  {
    std::cout << "assign" << std::endl;
    return *this;
  }
  static void PrettyPrinterCb (const MySmartTag *a, std::ostream &os)
  {}
};

static void 
myTagAPrettyPrinterCb (struct myTagA const*a, std::ostream &os)
{
  //os << "struct myTagA, a="<<(uint32_t)a->a<<std::endl;
  g_a = true;
}
static void 
myTagBPrettyPrinterCb (struct myTagB const*b, std::ostream &os)
{
  //os << "struct myTagB, b="<<b->b<<std::endl;
  g_b = true;
}
static void 
myTagCPrettyPrinterCb (struct myTagC const*c, std::ostream &os)
{
  //os << "struct myTagC, c="<<(uint32_t)c->c[0]<<std::endl;
  g_c = true;
}
static void 
myTagZPrettyPrinterCb (struct myTagZ const*z, std::ostream &os)
{
  //os << "struct myTagZ" << std::endl;
  g_z = true;
}


static TagRegistration<struct myTagA> gMyTagARegistration ("A", &myTagAPrettyPrinterCb);
static TagRegistration<struct myTagB> gMyTagBRegistration ("B", &myTagBPrettyPrinterCb);
static TagRegistration<struct myTagC> gMyTagCRegistration ("C", &myTagCPrettyPrinterCb);
static TagRegistration<struct myTagZ> g_myTagZRegistration ("ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ", 
                                                            &myTagZPrettyPrinterCb);
static TagRegistration<MySmartTag> g_myTagSmartRegistration ("SmartTag", &MySmartTag::PrettyPrinterCb);


TagsTest::TagsTest ()
  : Test ("Tags")
{}
TagsTest::~TagsTest ()
{}

bool 
TagsTest::RunTests (void)
{
  bool ok = true;

  // build initial tag.
  Tags tags;
  struct myTagA a;
  a.a = 10;
  tags.Add (a);
  a.a = 0;
  tags.Peek (a);
  if (a.a != 10) 
    {
      ok = false;
    }
  g_a = false;
  tags.PrettyPrint (std::cout);
  if (!g_a)
    {
      ok = false;
    }
  struct myTagB b;
  b.b = 0xff;
  tags.Add (b);
  b.b = 0;
  tags.Peek (b);
  if (b.b != 0xff) 
    {
      ok = false;
    }
  g_b = false;
  g_a = false;
  tags.PrettyPrint (std::cout);
  if (!g_a || !g_b)
    {
      ok = false;
    }
  // make sure copy contains copy.
  Tags other = tags;
  g_b = false;
  g_a = false;
  other.PrettyPrint (std::cout);
  if (!g_a || !g_b)
    {
      ok = false;
    }
  g_b = false;
  g_a = false;
  tags.PrettyPrint (std::cout);
  if (!g_a || !g_b)
    {
      ok = false;
    }
  struct myTagA oA;
  oA.a = 0;
  other.Peek (oA);
  if (oA.a != 10) 
    {
      ok = false;
    }
  struct myTagB oB;
  oB.b = 1;
  other.Peek (oB);
  if (oB.b != 0xff) 
    {
      ok = false;
    }
  // remove data.
  other.Remove (oA);
  if (other.Peek (oA)) 
    {
      ok = false;
    }
  g_b = false;
  g_a = false;
  other.PrettyPrint (std::cout);
  if (g_a || !g_b)
    {
      ok = false;
    }
  if (!tags.Peek (oA)) 
    {
      ok = false;
    }
  other.Remove (oB);
  if (other.Peek (oB)) 
    {
      ok = false;
    }
  if (!tags.Peek (oB)) 
    {
      ok = false;
    }

  other = tags;
  Tags another = other;
  struct myTagC c;
  memset (c.c, 0x66, 16);
  another.Add (c);
  c.c[0] = 0;
  another.Peek (c);
  if (!another.Peek (c)) 
    {
      ok = false;
    }
  if (tags.Peek (c)) 
    {
      ok = false;
    }

  other = other;
  //other.prettyPrint (std::cout);

  //struct myInvalidTag invalid;
  //tags.add (&invalid);

  struct myTagZ tagZ;
  Tags testLastTag;
  tagZ.z = 0;
  testLastTag.Add (tagZ);
  g_z = false;
  testLastTag.PrettyPrint (std::cout);
  if (!g_z)
    {
      ok = false;
    }

  MySmartTag smartTag;
  {
    Tags tmp;
    tmp.Add (smartTag);
    tmp.Peek (smartTag);
    tmp.Remove (smartTag);
  }

  return ok;
}

static TagsTest gTagsTest;


}; // namespace ns3

#endif /* RUN_SELF_TESTS */

