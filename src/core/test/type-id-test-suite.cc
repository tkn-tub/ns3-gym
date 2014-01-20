/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Lawrence Livermore National Laboratory
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
 * Author: Peter D. Barnes, Jr. <pdbarnes@llnl.gov>
 */

#include <iostream>
#include <iomanip>
#include <ctime>

#include "ns3/type-id.h"
#include "ns3/test.h"
#include "ns3/log.h"

using namespace std;

using namespace ns3;


const std::string suite("type-id: ");
  
//----------------------------
//
// Test for uniqueness of all TypeIds

class UniqueTypeIdTestCase : public TestCase
{
public:
  UniqueTypeIdTestCase ();
  virtual ~UniqueTypeIdTestCase ();
private:
  virtual void DoRun (void);
  enum { HashChainFlag = 0x80000000};
};

UniqueTypeIdTestCase::UniqueTypeIdTestCase ()
  : TestCase ("Check uniqueness of all TypeIds")
{
}

UniqueTypeIdTestCase::~UniqueTypeIdTestCase ()
{
}

void
UniqueTypeIdTestCase::DoRun (void)
{
  cout << suite << endl;
  cout << suite << GetName () << endl;
  
  // Use same custom hasher as TypeId
  ns3::Hasher hasher = ns3::Hasher ( Create<Hash::Function::Murmur3> () );
  
  uint32_t nids = TypeId::GetRegisteredN ();

  cout << suite << "UniqueTypeIdTestCase: nids: " << nids << endl;
  cout << suite << "TypeId list:" << endl;
  cout << suite << "TypeId  Chain  hash          Name" << endl;
  
  for (uint32_t i = 0; i < nids; ++i)
    {
      const TypeId tid = TypeId::GetRegistered (i);
      cout << suite << "" << std::setw(6) << tid.GetUid ();
      if (tid.GetHash () & HashChainFlag)
        {
          cout << "  chain";
        }
      else
        {
          cout << "       ";
        }
      cout << "  0x"     << std::setfill ('0') << std::hex << std::setw (8)
           << tid.GetHash () << std::dec << std::setfill (' ')
           << "    "     << tid.GetName ()
           << endl;

      NS_TEST_ASSERT_MSG_EQ (tid.GetUid (),
                             TypeId::LookupByName (tid.GetName ()).GetUid (),
                             "LookupByName returned different TypeId for "
                             << tid.GetName ());

      // Mask off HashChainFlag in this test, since tid might have been chained
      NS_TEST_ASSERT_MSG_EQ ((tid.GetHash () & (~HashChainFlag)),
                             (hasher.clear ().GetHash32 (tid.GetName ()) & (~HashChainFlag)),
                             "TypeId .hash and Hash32 (.name) unequal for "
                             << tid.GetName ());

      NS_TEST_ASSERT_MSG_EQ (tid.GetUid (),
                             TypeId::LookupByHash (tid.GetHash ()).GetUid (),
                             "LookupByHash returned different TypeId for "
                             << tid.GetName ());
      
    }

  cout << suite << "<-- end TypeId list -->" << endl;
}


//----------------------------
//
// Collision test

class CollisionTestCase : public TestCase
{
public:
  CollisionTestCase ();
  virtual ~CollisionTestCase ();
private:
  virtual void DoRun (void);
  enum { HashChainFlag = 0x80000000};
};

CollisionTestCase::CollisionTestCase ()
  : TestCase ("Check behavour when type names collide")
{
}

CollisionTestCase::~CollisionTestCase ()
{
}

void
CollisionTestCase::DoRun (void)
{
  cout << suite << endl;
  cout << suite << GetName () << endl;
  
  // Register two types whose hashes collide, in alphabetical order
  // Murmur3 collision from /usr/share/dict/web2
  string t1Name = "daemon";
  string t2Name = "unerring";
  cout << suite << "creating colliding types "
       << "'" << t1Name << "', '" << t2Name << "'"
       << " in alphabetical order:"
       << endl;
  TypeId t1 (t1Name.c_str ());
  TypeId t2 (t2Name.c_str ());

  // Check that they are alphabetical: t1 name < t2 name
  NS_TEST_ASSERT_MSG_EQ ( (t1.GetHash () & HashChainFlag), 0,
                         "First and lesser TypeId has HashChainFlag set");
  cout << suite << "collision: first,lesser  not chained: OK" << endl;

  NS_TEST_ASSERT_MSG_NE ( (t2.GetHash () & HashChainFlag), 0,
                         "Second and greater TypeId does not have HashChainFlag set");
  cout << suite << "collision: second,greater    chained: OK" << endl;


  // Register colliding types in reverse alphabetical order
  // Murmur3 collision from /usr/share/dict/web2
  string t3Name = "trigonon";
  string t4Name = "seriation";
  cout << suite << "creating colliding types "
       << "'" << t3Name << "', '" << t4Name << "'"
       << " in reverse alphabetical order:"
       << endl;
  TypeId t3 (t3Name.c_str ());
  TypeId t4 (t4Name.c_str ());

  // Check that they are alphabetical: t3 name > t4 name
  NS_TEST_ASSERT_MSG_NE ( (t3.GetHash () & HashChainFlag), 0,
                          "First and greater TypeId does not have HashChainFlag set");
  cout << suite << "collision: first,greater     chained: OK" << endl;

  NS_TEST_ASSERT_MSG_EQ ( (t4.GetHash () & HashChainFlag), 0,
                          "Second and lesser TypeId has HashChainFlag set");
  cout << suite << "collision: second,lesser not chained: OK" << endl;

  /** TODO Extra credit:  register three types whose hashes collide
   *
   *  None found in /usr/share/dict/web2
   */
  
}
  
  
//----------------------------
//
// Performance test

class LookupTimeTestCase : public TestCase
{
public:
  LookupTimeTestCase ();
  virtual ~LookupTimeTestCase ();
private:
  void DoRun (void);
  void DoSetup (void);
  void Report (const std::string how, const uint32_t delta) const ;

  enum { REPETITIONS = 100000 };
};

LookupTimeTestCase::LookupTimeTestCase ()
  : TestCase ("Measure average lookup time")
{
}

LookupTimeTestCase::~LookupTimeTestCase ()
{
}

void
LookupTimeTestCase::DoRun (void)
{
  cout << suite << endl;
  cout << suite << GetName () << endl;
  
  uint32_t nids = TypeId::GetRegisteredN ();

  int start = clock ();
  for (uint32_t j = 0; j < REPETITIONS; ++j)
    {
      for (uint32_t i = 0; i < nids; ++i)
        {
          const TypeId tid = TypeId::GetRegistered (i);
          const TypeId sid = TypeId::LookupByName (tid.GetName ());
        }
  }
  int stop = clock ();
  Report ("name", stop - start);

  start = clock ();
  for (uint32_t j = 0; j < REPETITIONS; ++j)
    {
      for (uint32_t i = 0; i < nids; ++i)
        {
          const TypeId tid = TypeId::GetRegistered (i);
          const TypeId sid = TypeId::LookupByHash (tid.GetHash ());
        }
  }
  stop = clock ();
  Report ("hash", stop - start);
  
}

void
LookupTimeTestCase::DoSetup (void)
{
  uint32_t nids = TypeId::GetRegisteredN ();
  
  cout << suite << "Lookup time: reps: " << REPETITIONS
       << ", num TypeId's: " << nids
       << endl;

}

void
LookupTimeTestCase::Report (const std::string how,
                            const uint32_t    delta) const
{
  double nids = TypeId::GetRegisteredN ();
  double reps = nids * REPETITIONS;
  
  double per = 1E6 * double(delta) / (reps * double(CLOCKS_PER_SEC));
  
  cout << suite << "Lookup time: by " << how << ": "
       << "ticks: " << delta
       << "\tper: "   << per
       << " microsec/lookup"
       << endl;					
}


//----------------------------
//
// TypeId test suites

class TypeIdTestSuite : public TestSuite
{
public:
  TypeIdTestSuite ();
};
  
TypeIdTestSuite::TypeIdTestSuite ()
  : TestSuite ("type-id", UNIT)
{
  // Turn on logging, so we see the result of collisions
  LogComponentEnable ("TypeId", ns3::LogLevel(LOG_ERROR|LOG_PREFIX_FUNC));
  
  // If the CollisionTestCase is performed before the
  // UniqueIdTestCase, the artificial collisions added by
  // CollisionTestCase will show up in the list of TypeIds
  // as chained.
  AddTestCase (new UniqueTypeIdTestCase, QUICK);
  AddTestCase (new CollisionTestCase, QUICK);
}

static TypeIdTestSuite g_TypeIdTestSuite;  
  

class TypeIdPerformanceSuite : public TestSuite
{
public:
  TypeIdPerformanceSuite ();
};
  
TypeIdPerformanceSuite::TypeIdPerformanceSuite ()
  : TestSuite ("type-id-perf", PERFORMANCE)
{
  AddTestCase (new LookupTimeTestCase, QUICK);
}

static TypeIdPerformanceSuite g_TypeIdPerformanceSuite;
