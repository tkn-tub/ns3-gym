/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Washington
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
 */

#include <list>
#include "ns3/abort.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulation-singleton.h"
#include "ipv4-address-generator.h"

NS_LOG_COMPONENT_DEFINE("Ipv4AddressGenerator");

namespace ns3 {

class Ipv4AddressGeneratorImpl
{
public:
  Ipv4AddressGeneratorImpl ();
  virtual ~Ipv4AddressGeneratorImpl ();

  void Init (const Ipv4Address net, const Ipv4Mask mask, 
    const Ipv4Address addr);

  Ipv4Address GetNetwork (const Ipv4Mask mask) const;
  Ipv4Address NextNetwork (const Ipv4Mask mask);

  void InitAddress (const Ipv4Address addr, const Ipv4Mask mask);
  Ipv4Address GetAddress (const Ipv4Mask mask) const;
  Ipv4Address NextAddress (const Ipv4Mask mask);

  void Reset (void);
  bool AddAllocated (const Ipv4Address addr);

  void TestMode (void);
private:
  static const uint32_t N_BITS = 32;
  static const uint32_t MOST_SIGNIFICANT_BIT = 0x80000000;

  uint32_t MaskToIndex (Ipv4Mask mask) const;

  class NetworkState
  {
  public:
    uint32_t mask;
    uint32_t shift;
    uint32_t network;
    uint32_t addr;
    uint32_t addrMax;
  };

  NetworkState m_netTable[N_BITS];

  class Entry
  {
  public:
    uint32_t addrLow;
    uint32_t addrHigh;
  };

  std::list<Entry> m_entries;
  bool m_test;
};

Ipv4AddressGeneratorImpl::Ipv4AddressGeneratorImpl () 
  : m_entries (), m_test (false)
{
  NS_LOG_FUNCTION_NOARGS ();
  Reset ();
}

  void
Ipv4AddressGeneratorImpl::Reset (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  uint32_t mask = 0;
//
// There are 32 possible masks in a 32-bit integer.  Two of these are illegal
// for a network mask (0x00000000 and 0xffffffff).  Valid network masks
// correspond to some nonzero number of high order bits set to one followed by 
// some nonzero number of lower order bits set to zero.
//
// We look at a network number as an n-bit number where n is defined as the 
// number of bits in each mask.  Allocating a new network number is simply 
// incrementing this number.
//
// In order to combine an allocated network number with an IP address, we have
// to shift the network into the correct alignment with respect to its mask.
// For example, a network mask of 0xff000000 admits the possibility of 256
// different network numbers since there are eight bits available.  To create
// IP addresses, we need to shift the network number counter left by 24 bits
// to put it in correct alignment.  This leaves 24 bits left for addresses.
// We make sure we don't overflow by saving a maximum address number which is
// just the inverse of the mask (~mask).
//
  for (uint32_t i = 0; i < N_BITS; ++i)
    {
      m_netTable[i].mask = mask;
      mask >>= 1;
      mask |= MOST_SIGNIFICANT_BIT;
      m_netTable[i].network = 1;
      m_netTable[i].addr = 1;
      m_netTable[i].addrMax = ~mask;
      m_netTable[i].shift = N_BITS - i;
    }
  m_entries.clear ();
  m_test = false;
}

Ipv4AddressGeneratorImpl::~Ipv4AddressGeneratorImpl ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

  void
Ipv4AddressGeneratorImpl::Init (
  const Ipv4Address net,
  const Ipv4Mask mask,
  const Ipv4Address addr)
{
  NS_LOG_FUNCTION_NOARGS ();
//
// We're going to be playing with the actual bits in the network and mask so
// pull them out into ints.
//
  uint32_t maskBits = mask.Get ();
  uint32_t netBits = net.Get ();
  uint32_t addrBits = addr.Get ();
//
// Some quick reasonableness testing.
//
  NS_ABORT_MSG_UNLESS ((netBits & ~maskBits) == 0, "Ipv4AddressGeneratorImpl::Init (): Inconsistent network and mask");
  NS_ABORT_MSG_UNLESS ((addrBits & maskBits) == 0, "Ipv4AddressGeneratorImpl::Init (): Inconsistent address and mask");

//
// Convert the network mask into an index into the network number table.
// The network number comes in to us properly aligned for the mask and so
// needs to be shifted right into the normalized position (lowest bit of the
// network number at bit zero of the int that holds it).
//
  uint32_t index = MaskToIndex (mask);

  m_netTable[index].network = netBits >> m_netTable[index].shift;
  NS_ABORT_MSG_UNLESS (addrBits <= m_netTable[index].addrMax, "Ipv4AddressGeneratorImpl::Init(): Address overflow");
  m_netTable[index].addr = addrBits;
  return;
}

  Ipv4Address
Ipv4AddressGeneratorImpl::GetNetwork (
  const Ipv4Mask mask) const
{
  NS_LOG_FUNCTION_NOARGS ();

  uint32_t index = MaskToIndex (mask);
  return Ipv4Address (m_netTable[index].network << m_netTable[index].shift);
}

  Ipv4Address
Ipv4AddressGeneratorImpl::NextNetwork (
  const Ipv4Mask mask)
{
  NS_LOG_FUNCTION_NOARGS ();
//
// The way this is expected to be used is that an address and network prefix
// are initialized, and then NextAddress() is called repeatedly to set the
// addresses on a given subnet.  The client will expect that the first 
// addresses will use the network prefix she used to initialize the generator
// with.  After a subnet is assigned, the client will call NextNetwork to 
// get the network number of the next subnet.  This implies that that this
// operation is a pre-increment.
//
  uint32_t index = MaskToIndex (mask);
  ++m_netTable[index].network;
  return Ipv4Address (m_netTable[index].network << m_netTable[index].shift);
}

  void
Ipv4AddressGeneratorImpl::InitAddress (
  const Ipv4Address addr,
  const Ipv4Mask mask)
{
  NS_LOG_FUNCTION_NOARGS ();

  uint32_t index = MaskToIndex (mask);
  uint32_t addrBits = addr.Get ();

  NS_ABORT_MSG_UNLESS (addrBits <= m_netTable[index].addrMax, "Ipv4AddressGeneratorImpl::InitAddress(): Address overflow");
  m_netTable[index].addr = addrBits;
}

  Ipv4Address
Ipv4AddressGeneratorImpl::GetAddress (
  const Ipv4Mask mask) const
{
  NS_LOG_FUNCTION_NOARGS ();

  uint32_t index = MaskToIndex (mask);

  return Ipv4Address (
    (m_netTable[index].network << m_netTable[index].shift) |
    m_netTable[index].addr);
}

  Ipv4Address
Ipv4AddressGeneratorImpl::NextAddress (const Ipv4Mask mask)
{
  NS_LOG_FUNCTION_NOARGS ();
//
// The way this is expected to be used is that an address and network prefix
// are initialized, and then NextAddress() is called repeatedly to set the
// addresses on a given subnet.  The client will expect that the first address
// she gets back is the one she used to initialize the generator with.  This
// implies that this operation is a post-increment.
//
  uint32_t index = MaskToIndex (mask);

  NS_ABORT_MSG_UNLESS (m_netTable[index].addr <= m_netTable[index].addrMax,
                       "Ipv4AddressGeneratorImpl::NextAddress(): Address overflow");

  Ipv4Address addr = Ipv4Address (
    (m_netTable[index].network << m_netTable[index].shift) |
    m_netTable[index].addr);

  ++m_netTable[index].addr;
//
// Make a note that we've allocated this address -- used for address collision
// detection.
//
  AddAllocated (addr);
  return addr;
}

  bool
Ipv4AddressGeneratorImpl::AddAllocated (const Ipv4Address address)
{
  NS_LOG_FUNCTION_NOARGS ();

  uint32_t addr = address.Get ();

  NS_ABORT_MSG_UNLESS (addr, "Ipv4AddressGeneratorImpl::Add(): Allocating the broadcast address is not a good idea"); 
 
  std::list<Entry>::iterator i;

  for (i = m_entries.begin (); i != m_entries.end (); ++i)
    {
      NS_LOG_LOGIC ("examine entry: " << Ipv4Address ((*i).addrLow) << 
        " to " << Ipv4Address ((*i).addrHigh));
//
// First things first.  Is there an address collision -- that is, does the
// new address fall in a previously allocated block of addresses.
//
      if (addr >= (*i).addrLow && addr <= (*i).addrHigh)
        {
          NS_LOG_LOGIC ("Ipv4AddressGeneratorImpl::Add(): Address Collision: " << Ipv4Address (addr)); 
          if (!m_test) 
            {
              NS_FATAL_ERROR ("Ipv4AddressGeneratorImpl::Add(): Address Collision: " << Ipv4Address (addr));
            }
          return false;
       }
//
// If the new address is less than the lowest address in the current block,
// and can't be merged into to the current block, then insert it as a new
// block before the current block.
//
      if (addr < (*i).addrLow - 1)
        {
          break;
        }
//
// If the new address fits at the end of the block, look ahead to the next 
// block and make sure it's not a collision there.  If we won't overlap, then
// just extend the current block by one address.  We expect that completely
// filled network ranges will be a fairly rare occurrence, so we don't worry
// about collapsing address range blocks.
// 
      if (addr == (*i).addrHigh + 1)
        {
          std::list<Entry>::iterator j = i;
          ++j;

          if (j != m_entries.end ())
            {
              if (addr == (*j).addrLow)
                {
                  NS_LOG_LOGIC ("Ipv4AddressGeneratorImpl::Add(): "
                    "Address Collision: " << Ipv4Address (addr)); 
                  if (!m_test)
                    {
                      NS_FATAL_ERROR ("Ipv4AddressGeneratorImpl::Add(): Address Collision: " << Ipv4Address (addr));
                    }
                  return false;
                }
            }
          
          NS_LOG_LOGIC ("New addrHigh = " << Ipv4Address (addr));
          (*i).addrHigh = addr;
          return true;
        }
//
// If we get here, we know that the next lower block of addresses couldn't 
// have been extended to include this new address since the code immediately 
// above would have been executed and that next lower block extended upward.
// So we know it's safe to extend the current block down to includ the new
// address.
//
      if (addr == (*i).addrLow - 1)
        {
          NS_LOG_LOGIC ("New addrLow = " << Ipv4Address (addr));
          (*i).addrLow = addr;
          return true;
        }
    }

  Entry entry;
  entry.addrLow = entry.addrHigh = addr;
  m_entries.insert(i, entry);
  return true;
}

  void
Ipv4AddressGeneratorImpl::TestMode (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_test = true;
}

  uint32_t
Ipv4AddressGeneratorImpl::MaskToIndex (Ipv4Mask mask) const
{
//
// We've been given a mask that has a higher order bit set for each bit of the
// network number.  In order to translate this mask into an index, we just need
// to count the number of zero bits in the mask.  We do this in a loop in which
// we shift the mask right until we find the first nonzero bit.  This tells us
// the number of zero bits, and from this we infer the number of nonzero bits
// which is the number of bits in the mask.
//
// We use the number of bits in the mask as the number of bits in the network
// number and as the index into the network number state table.
//
  uint32_t maskBits = mask.Get ();

  for (uint32_t i = 0; i < N_BITS; ++i)
    {
      if (maskBits & 1)
        {
          uint32_t index = N_BITS - i;
          NS_ABORT_MSG_UNLESS (index > 0 && index < N_BITS, "Ipv4AddressGenerator::MaskToIndex(): Illegal Mask");
          return index;
        }
      maskBits >>= 1;
    }
  NS_ASSERT_MSG(false, "Ipv4AddressGenerator::MaskToIndex(): Impossible");
  return 0;
}

  void
Ipv4AddressGenerator::Init (
  const Ipv4Address net,
  const Ipv4Mask mask,
  const Ipv4Address addr)
{
  NS_LOG_FUNCTION_NOARGS ();

  SimulationSingleton<Ipv4AddressGeneratorImpl>::Get ()
    ->Init (net, mask, addr);
}

  Ipv4Address
Ipv4AddressGenerator::NextNetwork (const Ipv4Mask mask)
{
  NS_LOG_FUNCTION_NOARGS ();

  return SimulationSingleton<Ipv4AddressGeneratorImpl>::Get ()
    ->NextNetwork (mask);
}

  Ipv4Address
Ipv4AddressGenerator::GetNetwork (const Ipv4Mask mask)
{
  NS_LOG_FUNCTION_NOARGS ();

  return SimulationSingleton<Ipv4AddressGeneratorImpl>::Get ()
    ->GetNetwork (mask);
}

  void
Ipv4AddressGenerator::InitAddress (
  const Ipv4Address addr,
  const Ipv4Mask mask)
{
  NS_LOG_FUNCTION_NOARGS ();

  SimulationSingleton<Ipv4AddressGeneratorImpl>::Get ()
    ->InitAddress (addr, mask);
}

  Ipv4Address
Ipv4AddressGenerator::GetAddress (const Ipv4Mask mask)
{
  NS_LOG_FUNCTION_NOARGS ();

  return SimulationSingleton<Ipv4AddressGeneratorImpl>::Get ()
    ->GetAddress (mask);
}

  Ipv4Address
Ipv4AddressGenerator::NextAddress (const Ipv4Mask mask)
{
  NS_LOG_FUNCTION_NOARGS ();

  return SimulationSingleton<Ipv4AddressGeneratorImpl>::Get ()
    ->NextAddress (mask);
}

  void
Ipv4AddressGenerator::Reset (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  return SimulationSingleton<Ipv4AddressGeneratorImpl>::Get ()
    ->Reset ();
}

  bool
Ipv4AddressGenerator::AddAllocated (const Ipv4Address addr)
{
  NS_LOG_FUNCTION_NOARGS ();

  return SimulationSingleton<Ipv4AddressGeneratorImpl>::Get ()
    ->AddAllocated (addr);
}

  void
  Ipv4AddressGenerator::TestMode (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  SimulationSingleton<Ipv4AddressGeneratorImpl>::Get ()
    ->TestMode ();
}

}; // namespace ns3

#ifdef RUN_SELF_TESTS

#include "ns3/test.h"

namespace ns3 {

class Ipv4AddressGeneratorTest : public Test
{
public:
  Ipv4AddressGeneratorTest ();
  virtual bool RunTests (void);
};

Ipv4AddressGeneratorTest::Ipv4AddressGeneratorTest ()
  : Test ("Ipv4AddressGenerator")
{
}

  bool
Ipv4AddressGeneratorTest::RunTests (void)
{
  bool result = true;
  Ipv4Address network;
  Ipv4Address address;
//
// Make sure the network number allocator is working on some of the network
// prefixes.
//
  Ipv4AddressGenerator::Init (Ipv4Address ("1.0.0.0"), Ipv4Mask ("255.0.0.0"),
    Ipv4Address ("0.0.0.0"));
  network = Ipv4AddressGenerator::GetNetwork (Ipv4Mask ("255.0.0.0"));
  NS_TEST_ASSERT_EQUAL (network, Ipv4Address ("1.0.0.0"));
  network = Ipv4AddressGenerator::NextNetwork (Ipv4Mask ("255.0.0.0"));
  NS_TEST_ASSERT_EQUAL (network, Ipv4Address ("2.0.0.0"));

  Ipv4AddressGenerator::Init (Ipv4Address ("0.1.0.0"), 
    Ipv4Mask ("255.255.0.0"), Ipv4Address ("0.0.0.0"));
  network = Ipv4AddressGenerator::GetNetwork (Ipv4Mask ("255.255.0.0"));
  NS_TEST_ASSERT_EQUAL (network, Ipv4Address ("0.1.0.0"));
  network = Ipv4AddressGenerator::NextNetwork (Ipv4Mask ("255.255.0.0"));
  NS_TEST_ASSERT_EQUAL (network, Ipv4Address ("0.2.0.0"));

  Ipv4AddressGenerator::Init (Ipv4Address ("0.0.1.0"),
    Ipv4Mask ("255.255.255.0"), Ipv4Address ("0.0.0.0"));
  network = Ipv4AddressGenerator::GetNetwork (Ipv4Mask ("255.255.255.0"));
  NS_TEST_ASSERT_EQUAL (network, Ipv4Address ("0.0.1.0"));
  network = Ipv4AddressGenerator::NextNetwork (Ipv4Mask ("255.255.255.0"));
  NS_TEST_ASSERT_EQUAL (network, Ipv4Address ("0.0.2.0"));

  network = Ipv4AddressGenerator::NextNetwork (Ipv4Mask ("255.0.0.0"));
  NS_TEST_ASSERT_EQUAL (network, Ipv4Address ("3.0.0.0"));
  network = Ipv4AddressGenerator::NextNetwork (Ipv4Mask ("255.255.0.0"));
  NS_TEST_ASSERT_EQUAL (network, Ipv4Address ("0.3.0.0"));
  network = Ipv4AddressGenerator::NextNetwork (Ipv4Mask ("255.255.255.0"));
  NS_TEST_ASSERT_EQUAL (network, Ipv4Address ("0.0.3.0"));
//
// Make sure the address allocator is working.
//
  Ipv4AddressGenerator::Init (Ipv4Address ("1.0.0.0"), Ipv4Mask ("255.0.0.0"),
    Ipv4Address ("0.0.0.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.0.0.0"));
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("1.0.0.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.0.0.0"));
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("1.0.0.4"));

  Ipv4AddressGenerator::Init (Ipv4Address ("0.1.0.0"),
    Ipv4Mask ("255.255.0.0"), Ipv4Address ("0.0.0.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.0.0"));
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.1.0.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.0.0"));
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.1.0.4"));

  Ipv4AddressGenerator::Init (Ipv4Address ("0.0.1.0"),
    Ipv4Mask ("255.255.255.0"), Ipv4Address ("0.0.0.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.255.0"));
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.0.1.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.255.0"));
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.0.1.4"));
//
// Make sure they both play together.
//
  Ipv4AddressGenerator::Init (Ipv4Address ("3.0.0.0"), Ipv4Mask ("255.0.0.0"),
    Ipv4Address ("0.0.0.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.0.0.0"));
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("3.0.0.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.0.0.0"));
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("3.0.0.4"));

  network = Ipv4AddressGenerator::NextNetwork (Ipv4Mask ("255.0.0.0"));
  NS_TEST_ASSERT_EQUAL (network, Ipv4Address ("4.0.0.0"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.0.0.0"));
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("4.0.0.5"));

  Ipv4AddressGenerator::Init (Ipv4Address ("0.3.0.0"), 
    Ipv4Mask ("255.255.0.0"), Ipv4Address ("0.0.0.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.0.0"));
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.3.0.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.0.0"));
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.3.0.4"));

  network = Ipv4AddressGenerator::NextNetwork (Ipv4Mask ("255.255.0.0"));
  NS_TEST_ASSERT_EQUAL (network, Ipv4Address ("0.4.0.0"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.0.0"));
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.4.0.5"));

  Ipv4AddressGenerator::Init (Ipv4Address ("0.0.3.0"),
    Ipv4Mask ("255.255.255.0"), Ipv4Address ("0.0.0.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.255.0"));
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.0.3.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.255.0"));
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.0.3.4"));

  network = Ipv4AddressGenerator::NextNetwork (Ipv4Mask ("255.255.255.0"));
  NS_TEST_ASSERT_EQUAL (network, Ipv4Address ("0.0.4.0"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.255.0"));
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.0.4.5"));
//
// A quick kindof-semi-almost-real example.  First, initialize our /24 network
// to 192.168.0.0 and begin allocating with ip address 0.0.0.3 out of that
// prefix.
//
  Ipv4AddressGenerator::Init (Ipv4Address ("192.168.0.0"),
    Ipv4Mask ("255.255.255.0"), Ipv4Address ("0.0.0.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.255.0"));
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("192.168.0.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.255.0"));
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("192.168.0.4"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.255.0"));
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("192.168.0.5"));
//
// Allocate the next network out of our /24 network (this should be 
// 192.168.1.0) and begin allocating with IP address 0.0.0.3 out of that
// prefix.
//
  Ipv4AddressGenerator::NextNetwork (Ipv4Mask ("255.255.255.0"));
  Ipv4AddressGenerator::InitAddress (Ipv4Address ("0.0.0.3"),
    Ipv4Mask ("255.255.255.0"));
//
// The first address we should get is the previous numbers ORed together, which
// is 192.168.1.3, of course.
//
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.255.0"));
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("192.168.1.3"));
//
// Make sure that the address collision logic works.
//
  Ipv4AddressGenerator::Reset ();

  Ipv4AddressGenerator::AddAllocated ("0.0.0.5");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.10");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.15");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.20");

  Ipv4AddressGenerator::AddAllocated ("0.0.0.4");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.3");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.2");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.1");

  Ipv4AddressGenerator::AddAllocated ("0.0.0.6");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.7");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.8");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.9");

  Ipv4AddressGenerator::AddAllocated ("0.0.0.11");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.12");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.13");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.14");

  Ipv4AddressGenerator::AddAllocated ("0.0.0.19");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.18");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.17");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.16");

  Ipv4AddressGenerator::TestMode ();
  bool added = Ipv4AddressGenerator::AddAllocated ("0.0.0.21");
  NS_TEST_ASSERT_EQUAL (added, true);

  added = Ipv4AddressGenerator::AddAllocated ("0.0.0.4");
  NS_TEST_ASSERT_EQUAL (added, false);

  added = Ipv4AddressGenerator::AddAllocated ("0.0.0.9");
  NS_TEST_ASSERT_EQUAL (added, false);

  added = Ipv4AddressGenerator::AddAllocated ("0.0.0.16");
  NS_TEST_ASSERT_EQUAL (added, false);

  added = Ipv4AddressGenerator::AddAllocated ("0.0.0.21");
  NS_TEST_ASSERT_EQUAL (added, false);

  Ipv4AddressGenerator::Reset ();

  return result;
}

static Ipv4AddressGeneratorTest g_addressGeneratorTest;

} // namespace ns3

#endif /* RUN_SELF_TEST */
