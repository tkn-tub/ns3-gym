/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Washington
 * Copyright (c) 2011 Atishay Jain
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
#include "ipv6-address-generator.h"

NS_LOG_COMPONENT_DEFINE ("Ipv6AddressGenerator");

namespace ns3 {

class Ipv6AddressGeneratorImpl
{
public:
  Ipv6AddressGeneratorImpl ();
  virtual ~Ipv6AddressGeneratorImpl ();

  void Init (const Ipv6Address net, const Ipv6Prefix prefix,
             const Ipv6Address interfaceId);

  Ipv6Address GetNetwork (const Ipv6Prefix prefix) const;
  Ipv6Address NextNetwork (const Ipv6Prefix prefix);

  void InitAddress (const Ipv6Address interfaceId, const Ipv6Prefix prefix);
  Ipv6Address GetAddress (const Ipv6Prefix prefix) const;
  Ipv6Address NextAddress (const Ipv6Prefix prefix);

  void Reset (void);
  bool AddAllocated (const Ipv6Address addr);

  void TestMode (void);
private:
  static const uint32_t N_BITS = 128;
  static const uint32_t MOST_SIGNIFICANT_BIT = 0x80;

  uint32_t PrefixToIndex (Ipv6Prefix prefix) const;

  class NetworkState
  {
public:
    uint8_t prefix[16];
    uint32_t shift;
    uint8_t network[16];
    uint8_t addr[16];
    uint8_t addrMax[16];
  };

  NetworkState m_netTable[N_BITS];

  class Entry
  {
public:
    uint8_t addrLow[16];
    uint8_t addrHigh[16];
  };

  std::list<Entry> m_entries;
  Ipv6Address m_base;
  bool m_test;
};

Ipv6AddressGeneratorImpl::Ipv6AddressGeneratorImpl ()
  : m_entries (),
    m_base ("::1"),
    m_test (false)
{
  NS_LOG_FUNCTION (this);
  Reset ();
}

void
Ipv6AddressGeneratorImpl::Reset (void)
{
  NS_LOG_FUNCTION (this);

  uint8_t prefix[16] = { 0};

  for (uint32_t i = 0; i < N_BITS; ++i)
    {
      for (uint32_t j = 0; j < 16; ++j)
        {
          m_netTable[i].prefix[j] = prefix[j];
        }
      for (uint32_t j = 0; j < 15; ++j)
        {
          prefix[15 - j] >>= 1;
          prefix[15 - j] |= (prefix[15 - j - 1] & 1);
        }
      prefix[0] |= MOST_SIGNIFICANT_BIT;
      for (uint32_t j = 0; j < 15; ++j)
        {
          m_netTable[i].network[j] = 0;
        }
      m_netTable[i].network[15] = 1;
      for (uint32_t j = 0; j < 15; ++j)
        {
          m_netTable[i].addr[j] = 0;
        }
      m_netTable[i].addr[15] = 1;
      for (uint32_t j = 0; j < 16; ++j)
        {
          m_netTable[i].addrMax[j] = ~prefix[j];
        }
      m_netTable[i].shift = N_BITS - i;
    }
  m_entries.clear ();
  m_base = Ipv6Address ("::1");
  m_test = false;
}

Ipv6AddressGeneratorImpl::~Ipv6AddressGeneratorImpl ()
{
  NS_LOG_FUNCTION (this);
}

void
Ipv6AddressGeneratorImpl::Init (
  const Ipv6Address net,
  const Ipv6Prefix prefix,
  const Ipv6Address interfaceId)
{
  NS_LOG_FUNCTION (this << net << prefix << interfaceId);

  m_base = interfaceId;
  //
  // We're going to be playing with the actual bits in the network and prefix so
  // pull them out into ints.
  //
  uint8_t prefixBits[16];
  prefix.GetBytes (prefixBits);
  uint8_t netBits[16];
  net.GetBytes (netBits);
  uint8_t interfaceIdBits[16];
  interfaceId.GetBytes (interfaceIdBits);
  //
  // Some quick reasonableness testing.
  //
  // Convert the network prefix into an index into the network number table.
  // The network number comes in to us properly aligned for the prefix and so
  // needs to be shifted right into the normalized position (lowest bit of the
  // network number at bit zero of the int that holds it).
  //
  uint32_t index = PrefixToIndex (prefix);
  uint32_t a = m_netTable[index].shift / 8;
  uint32_t b = m_netTable[index].shift % 8;
  for (int32_t j = 15 - a; j >= 0; j--)
    {
      m_netTable[index].network[j + a] = netBits[j];
    }
  for (uint32_t j = 0; j < a; j++)
    {
      m_netTable[index].network[j] = 0;
    }
  for (uint32_t j = 15; j >= a; j--)
    {
      m_netTable[index].network[j] = m_netTable[index].network[j] >> b;
      m_netTable[index].network[j] |= m_netTable[index].network[j - 1] << (8 - b);
    }
  for (int32_t j = 0; j < 16; j++)
    {
      m_netTable[index].addr[j] = interfaceIdBits[j];
    }
  return;
}

Ipv6Address
Ipv6AddressGeneratorImpl::GetNetwork (
  const Ipv6Prefix prefix) const
{
  NS_LOG_FUNCTION (this);
  uint8_t nw[16];
  uint32_t index = PrefixToIndex (prefix);
  uint32_t a = m_netTable[index].shift / 8;
  uint32_t b = m_netTable[index].shift % 8;
  for (uint32_t j = 0; j < 16 - a; ++j)
    {
      nw[j] = m_netTable[index].network[j + a];
    }
  for (uint32_t j = 16 - a; j < 16; ++j)
    {
      nw[j] = 0;
    }
  for (uint32_t j = 0; j < 15; j++)
    {
      nw[j] = nw[j] << b;
      nw[j] |= nw[j + 1] >> (8 - b);
    }
  nw[15] = nw[15] << b;

  return Ipv6Address (nw);
}

Ipv6Address
Ipv6AddressGeneratorImpl::NextNetwork (
  const Ipv6Prefix prefix)
{
  NS_LOG_FUNCTION (this);

  uint32_t index = PrefixToIndex (prefix);
  // Reset the base to what was initialized
  uint8_t interfaceIdBits[16];
  m_base.GetBytes (interfaceIdBits);
  for (int32_t j = 0; j < 16; j++)
    {
      m_netTable[index].addr[j] = interfaceIdBits[j];
    }

  for (int32_t j = 15; j >= 0; j--)
    {
      if (m_netTable[index].network[j] < 0xff)
        {
          ++m_netTable[index].network[j];
          break;
        }
      else
        {
          ++m_netTable[index].network[j];
        }
    }

  uint8_t nw[16];
  uint32_t a = m_netTable[index].shift / 8;
  uint32_t b = m_netTable[index].shift % 8;
  for (uint32_t j = 0; j < 16 - a; ++j)
    {
      nw[j] = m_netTable[index].network[j + a];
    }
  for (uint32_t j = 16 - a; j < 16; ++j)
    {
      nw[j] = 0;
    }
  for (uint32_t j = 0; j < 15; j++)
    {
      nw[j] = nw[j] << b;
      nw[j] |= nw[j + 1] >> (8 - b);
    }
  nw[15] = nw[15] << b;

  return Ipv6Address (nw);

}

void
Ipv6AddressGeneratorImpl::InitAddress (
  const Ipv6Address interfaceId,
  const Ipv6Prefix prefix)
{
  NS_LOG_FUNCTION (this);

  uint32_t index = PrefixToIndex (prefix);
  uint8_t interfaceIdBits[16];
  interfaceId.GetBytes (interfaceIdBits);

  for (uint32_t j = 0; j < 16; ++j)
    {
      m_netTable[index].addr[j] = interfaceIdBits[j];
    }
}

Ipv6Address
Ipv6AddressGeneratorImpl::GetAddress (const Ipv6Prefix prefix) const
{
  NS_LOG_FUNCTION (this);

  uint32_t index = PrefixToIndex (prefix);

  uint8_t nw[16];
  uint32_t a = m_netTable[index].shift / 8;
  uint32_t b = m_netTable[index].shift % 8;
  for (uint32_t j = 0; j < 16 - a; ++j)
    {
      nw[j] = m_netTable[index].network[j + a];
    }
  for (uint32_t j = 16 - a; j < 16; ++j)
    {
      nw[j] = 0;
    }
  for (uint32_t j = 0; j < 15; j++)
    {
      nw[j] = nw[j] << b;
      nw[j] |= nw[j + 1] >> (8 - b);
    }
  nw[15] = nw[15] << b;
  for (uint32_t j = 0; j < 16; j++)
    {
      nw[j] |= m_netTable[index].addr[j];
    }

  return Ipv6Address (nw);
}

Ipv6Address
Ipv6AddressGeneratorImpl::NextAddress (const Ipv6Prefix prefix)
{
  NS_LOG_FUNCTION (this);

  uint32_t index = PrefixToIndex (prefix);

  uint8_t ad[16];
  uint32_t a = m_netTable[index].shift / 8;
  uint32_t b = m_netTable[index].shift % 8;
  for (uint32_t j = 0; j < 16 - a; ++j)
    {
      ad[j] = m_netTable[index].network[j + a];
    }
  for (uint32_t j = 16 - a; j < 16; ++j)
    {
      ad[j] = 0;
    }
  for (uint32_t j = 0; j < 15; j++)
    {
      ad[j] = ad[j] << b;
      ad[j] |= ad[j + 1] >> (8 - b);
    }
  ad[15] = ad[15] << b;
  for (uint32_t j = 0; j < 16; j++)
    {
      ad[j] |= m_netTable[index].addr[j];
    }
  Ipv6Address addr = Ipv6Address (ad);

  for (int32_t j = 15; j >= 0; j--)
    {
      if (m_netTable[index].addr[j] < 0xff)
        {
          ++m_netTable[index].addr[j];
          break;
        }
      else
        {
          ++m_netTable[index].addr[j];
        }
    }

  //
  // Make a note that we've allocated this address -- used for address collision
  // detection.
  //
  AddAllocated (addr);
  return addr;
}

bool
Ipv6AddressGeneratorImpl::AddAllocated (const Ipv6Address address)
{
  NS_LOG_FUNCTION (this << address);

  uint8_t addr[16];
  address.GetBytes (addr);

  std::list<Entry>::iterator i;

  for (i = m_entries.begin (); i != m_entries.end (); ++i)
    {
      NS_LOG_LOGIC ("examine entry: " << Ipv6Address ((*i).addrLow) <<
                    " to " << Ipv6Address ((*i).addrHigh));
      //
      // First things first.  Is there an address collision -- that is, does the
      // new address fall in a previously allocated block of addresses.
      //
      if (!(Ipv6Address (addr) < Ipv6Address ((*i).addrLow))
          && ((Ipv6Address (addr) < Ipv6Address ((*i).addrHigh))
              || (Ipv6Address (addr) == Ipv6Address ((*i).addrHigh))))
        {
          NS_LOG_LOGIC ("Ipv6AddressGeneratorImpl::Add(): Address Collision: " << Ipv6Address (addr));
          if (!m_test)
            {
              NS_FATAL_ERROR ("Ipv6AddressGeneratorImpl::Add(): Address Collision: " << Ipv6Address (addr));
            }
          return false;
        }
      //
      // If the new address is less than the lowest address in the current
      // block and can't be merged into to the current block, then insert it
      // as a new block before the current block.
      //
      uint8_t taddr[16];
      for (uint32_t j = 0; j < 16; j++)
        {
          taddr[j] = (*i).addrLow[j];
        }
      taddr[15] -= 1;
      if (Ipv6Address (addr) < Ipv6Address (taddr))
        {
          break;
        }
      //
      // If the new address fits at the end of the block, look ahead to the next
      // block and make sure it's not a collision there.  If we won't overlap,
      // then just extend the current block by one address.  We expect that
      // completely filled network ranges will be a fairly rare occurrence,
      // so we don't worry about collapsing address range blocks.
      //
      for (uint32_t j = 0; j < 16; j++)
        {
          taddr[j] = (*i).addrLow[j];
        }
      taddr[15] += 1;
      if (Ipv6Address (addr) == Ipv6Address (taddr))
        {
          std::list<Entry>::iterator j = i;
          ++j;

          if (j != m_entries.end ())
            {
              if (Ipv6Address (addr) == Ipv6Address ((*j).addrLow))
                {
                  NS_LOG_LOGIC ("Ipv6AddressGeneratorImpl::Add(): "
                                "Address Collision: " << Ipv6Address (addr));
                  if (!m_test)
                    {
                      NS_FATAL_ERROR ("Ipv6AddressGeneratorImpl::Add(): Address Collision: " << Ipv6Address (addr));
                    }
                  return false;
                }
            }

          NS_LOG_LOGIC ("New addrHigh = " << Ipv6Address (addr));
          for (uint32_t j = 0; j < 16; j++)
            {
              (*i).addrHigh[j] = addr[j];
            }
          return true;
        }
      //
      // If we get here, we know that the next lower block of addresses
      // couldn't have been extended to include this new address since the
      // code immediately above would have been executed and that next lower
      // block extended upward.  So we know it's safe to extend the current
      // block down to includ the new address.
      //
      for (uint32_t j = 0; j < 16; j++)
        {
          taddr[j] = (*i).addrLow[j];
        }
      taddr[15] -= 1;
      if ((Ipv6Address (addr) == Ipv6Address (taddr)))
        {
          NS_LOG_LOGIC ("New addrLow = " << Ipv6Address (addr));
          for (uint32_t j = 0; j < 16; j++)
            {
              (*i).addrLow[j] = addr[j];
            }
          return true;
        }
    }

  Entry entry;
  for (uint32_t j = 0; j < 16; j++)
    {
      entry.addrLow[j] = entry.addrHigh[j] = addr[j];
    }
  m_entries.insert (i, entry);
  return true;
}

void
Ipv6AddressGeneratorImpl::TestMode (void)
{
  NS_LOG_FUNCTION (this);
  m_test = true;
}

uint32_t
Ipv6AddressGeneratorImpl::PrefixToIndex (Ipv6Prefix prefix) const
{
  //
  // We've been given a prefix that has a higher order bit set for each bit of
  // the network number.  In order to translate this prefix into an index,
  // we just need to count the number of zero bits in the prefix.  We do this
  // in a loop in which we shift the prefix right until we find the first
  // nonzero bit.  This tells us the number of zero bits, and from this we
  // infer the number of nonzero bits which is the number of bits in the prefix.
  //
  // We use the number of bits in the prefix as the number of bits in the
  // network number and as the index into the network number state table.
  //
  uint8_t prefixBits[16];
  prefix.GetBytes (prefixBits);

  for (uint32_t i = 15; i >= 0; --i)
    {
      for (uint32_t j = 0; j < 8; ++j)
        {
          if (prefixBits[i] & 1)
            {
              uint32_t index = N_BITS - (15 - i) * 8 - j;
              NS_ABORT_MSG_UNLESS (index > 0 && index < N_BITS, "Ip64AddressGenerator::PrefixToIndex(): Illegal Prefix");
              return index;
            }
          prefixBits[i] >>= 1;
        }
    }
  NS_ASSERT_MSG (false, "Ipv6AddressGenerator::PrefixToIndex(): Impossible");
  return 0;
}

void
Ipv6AddressGenerator::Init (
  const Ipv6Address net,
  const Ipv6Prefix prefix,
  const Ipv6Address interfaceId)
{
  NS_LOG_FUNCTION_NOARGS ();

  SimulationSingleton<Ipv6AddressGeneratorImpl>::Get ()
  ->Init (net, prefix, interfaceId);
}

Ipv6Address
Ipv6AddressGenerator::NextNetwork (const Ipv6Prefix prefix)
{
  NS_LOG_FUNCTION_NOARGS ();

  return SimulationSingleton<Ipv6AddressGeneratorImpl>::Get ()
         ->NextNetwork (prefix);
}

Ipv6Address
Ipv6AddressGenerator::GetNetwork (const Ipv6Prefix prefix)
{
  NS_LOG_FUNCTION_NOARGS ();

  return SimulationSingleton<Ipv6AddressGeneratorImpl>::Get ()
         ->GetNetwork (prefix);
}

void
Ipv6AddressGenerator::InitAddress (
  const Ipv6Address interfaceId,
  const Ipv6Prefix prefix)
{
  NS_LOG_FUNCTION_NOARGS ();

  SimulationSingleton<Ipv6AddressGeneratorImpl>::Get ()
  ->InitAddress (interfaceId, prefix);
}

Ipv6Address
Ipv6AddressGenerator::GetAddress (const Ipv6Prefix prefix)
{
  NS_LOG_FUNCTION_NOARGS ();

  return SimulationSingleton<Ipv6AddressGeneratorImpl>::Get ()
         ->GetAddress (prefix);
}

Ipv6Address
Ipv6AddressGenerator::NextAddress (const Ipv6Prefix prefix)
{
  NS_LOG_FUNCTION_NOARGS ();

  return SimulationSingleton<Ipv6AddressGeneratorImpl>::Get ()
         ->NextAddress (prefix);
}

void
Ipv6AddressGenerator::Reset (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  return SimulationSingleton<Ipv6AddressGeneratorImpl>::Get ()
         ->Reset ();
}

bool
Ipv6AddressGenerator::AddAllocated (const Ipv6Address addr)
{
  NS_LOG_FUNCTION_NOARGS ();

  return SimulationSingleton<Ipv6AddressGeneratorImpl>::Get ()
         ->AddAllocated (addr);
}

void
Ipv6AddressGenerator::TestMode (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  SimulationSingleton<Ipv6AddressGeneratorImpl>::Get ()
  ->TestMode ();
}

} // namespace ns3

