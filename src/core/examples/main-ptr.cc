/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
 */

#include "ns3/ptr.h"
#include "ns3/object.h"
#include <iostream>

using namespace ns3;

class PtrExample : public Object
{
public:
  PtrExample ();
  ~PtrExample ();
  void Method (void);
};
PtrExample::PtrExample ()
{
  std::cout << "PtrExample constructor" << std::endl;
}
PtrExample::~PtrExample()
{
  std::cout << "PtrExample destructor" << std::endl;
}
void
PtrExample::Method (void)
{
  std::cout << "PtrExample method" << std::endl;
}

static Ptr<PtrExample> g_ptr = 0;

static Ptr<PtrExample>
StorePtr (Ptr<PtrExample> p)
{
  Ptr<PtrExample> prev = g_ptr;
  g_ptr = p;
  return prev;
}

static void
ClearPtr (void)
{
  g_ptr = 0;
}



int main (int argc, char *argv[])
{
  {
    // Create a new object of type PtrExample, store it in global 
    // variable g_ptr
    Ptr<PtrExample> p = CreateObject<PtrExample> ();
    p->Method ();
    Ptr<PtrExample> prev = StorePtr (p);
    NS_ASSERT (prev == 0);
  }

  {
    // Create a new object of type PtrExample, store it in global 
    // variable g_ptr, get a hold on the previous PtrExample object.
    Ptr<PtrExample> p = CreateObject<PtrExample> ();
    Ptr<PtrExample> prev = StorePtr (p);
    // call method on object
    prev->Method ();
    // Clear the currently-stored object
    ClearPtr ();
    // get the raw pointer and release it.
    PtrExample *raw = GetPointer (prev);
    prev = 0;
    raw->Method ();
    raw->Unref ();
  }


  return 0;
}
