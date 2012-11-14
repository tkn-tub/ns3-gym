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

#include <iomanip>
#include <string>

#include "ns3/hash.h"
#include "ns3/test.h"

namespace ns3 {


//----------------------------
//
// Test default hash on fixed string

class HashFoxTestCase : public TestCase
{
public:
  HashFoxTestCase ();
  virtual ~HashFoxTestCase ();
private:
  virtual void DoRun (void);
};

HashFoxTestCase::HashFoxTestCase ()
  : TestCase ("Check Hash on a known string")
{
}

HashFoxTestCase::~HashFoxTestCase ()
{
}

void
HashFoxTestCase::DoRun (void)
{
  std::string key("The quick brown fox.");

  uint32_t h32r = 0xd138bc62;  // murmur3(key)
  uint32_t h32 = Hash32 (key);
  NS_TEST_ASSERT_MSG_EQ (h32, h32r,
                         "Hash32 produced " << std::hex << std::setw ( 8) << h32
                         << ", expected "   << std::hex << std::setw ( 8) << h32r
                         );

  uint64_t h64r = 0xfc30fb5d90d6e3a9;
  uint64_t h64 = Hash64 (key);
  NS_TEST_ASSERT_MSG_EQ (h64, h64r,
                         "Hash64 produced " << std::hex << std::setw (16) << h64
                         << ", expected "   << std::hex << std::setw (16) << h64r
                         );
}


//----------------------------
//
// Hash tester

class HashTestSuite : public TestSuite
{
public:
  HashTestSuite ();
};
  
HashTestSuite::HashTestSuite ()
  : TestSuite ("hash", UNIT)
{
  AddTestCase (new HashFoxTestCase);
}

static HashTestSuite g_hashTestSuite;  
  
}  // namespace ns3
