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

#include "ns3/test.h"
#include "ns3/hash.h"

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
  : TestCase ("Check default Hash on a known string")
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
// Test FNV hash on fixed string

class HashFnv1aTestCase : public TestCase
{
public:
  HashFnv1aTestCase ();
  virtual ~HashFnv1aTestCase ();
private:
  virtual void DoRun (void);
};

HashFnv1aTestCase::HashFnv1aTestCase ()
  : TestCase ("Check Fnv1a Hash on a known string")
{
}

HashFnv1aTestCase::~HashFnv1aTestCase ()
{
}

void
HashFnv1aTestCase::DoRun (void)
{
  std::string key("The quick brown fnv1a.");
  Hasher hasher = Hasher ( Create<Hash::Function::Fnv1a> () );

  uint32_t h32r = 0x5735855b;  // Fnv1a(key)
  uint32_t h32 = hasher.clear ().GetHash32 (key);
  NS_TEST_ASSERT_MSG_EQ (h32, h32r,
                         "Hash32 produced " << std::hex << std::setw ( 8) << h32
                         << ", expected "   << std::hex << std::setw ( 8) << h32r
                         );

  uint64_t h64r = 0x6fb0aea4ad83c27b;
  uint64_t h64 = hasher.clear ().GetHash64 (key);
  NS_TEST_ASSERT_MSG_EQ (h64, h64r,
                         "Hash64 produced " << std::hex << std::setw (16) << h64
                         << ", expected "   << std::hex << std::setw (16) << h64r
                         );
}


//----------------------------
//
// Test Murmur3 hash on fixed string

class HashMurmur3TestCase : public TestCase
{
public:
  HashMurmur3TestCase ();
  virtual ~HashMurmur3TestCase ();
private:
  virtual void DoRun (void);
};

HashMurmur3TestCase::HashMurmur3TestCase ()
  : TestCase ("Check Murmur3 Hash on a known string")
{
}

HashMurmur3TestCase::~HashMurmur3TestCase ()
{
}

void
HashMurmur3TestCase::DoRun (void)
{
  std::string key("The quick brown murmur3.");
  Hasher hasher = Hasher ( Create<Hash::Function::Murmur3> () );

  uint32_t h32r = 0xe8a2d100;  // Murmur3(key)
  uint32_t h32 = hasher.clear ().GetHash32 (key);
  NS_TEST_ASSERT_MSG_EQ (h32, h32r,
                         "Hash32 produced " << std::hex << std::setw ( 8) << h32
                         << ", expected "   << std::hex << std::setw ( 8) << h32r
                         );

  uint64_t h64r = 0x95373d091a691071;
  uint64_t h64 = hasher.clear ().GetHash64 (key);
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
  AddTestCase (new HashFnv1aTestCase);
  AddTestCase (new HashMurmur3TestCase);
}

static HashTestSuite g_hashTestSuite;  
  
}  // namespace ns3
