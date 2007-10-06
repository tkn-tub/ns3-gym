/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulation-singleton.h"
#include "ipv4-address-extended.h"

NS_LOG_COMPONENT_DEFINE("Ipv4AddressEx");

namespace ns3 {

class Ipv4NetworkManager
{
public:
  Ipv4NetworkManager ();
  virtual ~Ipv4NetworkManager ();

  Ipv4Address Allocate (const Ipv4Mask mask);
  void Seed (const Ipv4Mask mask, const Ipv4Address network);

private:
  static const uint32_t N_BITS = 32;
  class State
  {
  public:
    uint32_t mask;
    uint32_t network;
  };
  State m_state[N_BITS];
};

Ipv4NetworkManager::Ipv4NetworkManager () 
{
  NS_LOG_FUNCTION;

  uint32_t mask = 0;

  for (uint32_t i = 0; i < N_BITS; ++i)
    {
      m_state[i].mask = mask;
      mask >>= 1;
      mask |= 0x80000000;
      m_state[i].network = 0;
      NS_LOG_LOGIC ("m_state[" << i << "]");
      NS_LOG_LOGIC ("mask = " << std::hex << m_state[i].mask);
      NS_LOG_LOGIC ("network = " << std::hex << m_state[i].network);
    }
}

Ipv4NetworkManager::~Ipv4NetworkManager ()
{
  NS_LOG_FUNCTION;
}

void
Ipv4NetworkManager::Seed (const Ipv4Mask mask, const Ipv4Address network)
{
  NS_LOG_FUNCTION;

  uint32_t maskBits = mask.GetHostOrder ();
  uint32_t networkBits = network.GetHostOrder ();

  for (uint32_t i = 0; i < N_BITS; ++i)
    {
      if (maskBits & 1)
        {
          uint32_t nMaskBits = N_BITS - i;
          NS_ASSERT(nMaskBits >= 0 && nMaskBits < N_BITS);
          m_state[nMaskBits].network = networkBits >> (N_BITS - i);
          return;
        }
      maskBits >>= 1;
    }
  NS_ASSERT_MSG(false, "Impossible");
  return;
}

Ipv4Address
Ipv4NetworkManager::Allocate (const Ipv4Mask mask)
{
  NS_LOG_FUNCTION;

  uint32_t bits = mask.GetHostOrder ();

  for (uint32_t i = 0; i < N_BITS; ++i)
    {
      if (bits & 1)
        {
          uint32_t nBits = N_BITS - i;
          NS_ASSERT(nBits >= 0 && nBits < N_BITS);
          Ipv4Address addr (m_state[nBits].network << i);
          ++m_state[nBits].network;
          return addr;
        }
      bits >>= 1;
    }
  NS_ASSERT_MSG(false, "Impossible");
  return Ipv4Address (bits);
}

class Ipv4AddressManager
{
public:
  Ipv4AddressManager ();
  virtual ~Ipv4AddressManager ();

  Ipv4Address Allocate (const Ipv4Mask mask, const Ipv4Address network);
  void Seed (const Ipv4Mask mask, const Ipv4Address address);

private:
  static const uint32_t N_BITS = 32;
  class State
  {
  public:
    uint32_t mask;
    uint32_t address;
  };
  State m_state[N_BITS];
};

Ipv4AddressManager::Ipv4AddressManager () 
{
  NS_LOG_FUNCTION;

  uint32_t mask = 0;

  for (uint32_t i = 0; i < N_BITS; ++i)
    {
      m_state[i].mask = mask;
      mask >>= 1;
      mask |= 0x80000000;
      m_state[i].address = 0;
      NS_LOG_LOGIC ("m_state[" << i << "]");
      NS_LOG_LOGIC ("mask = " << std::hex << m_state[i].mask);
      NS_LOG_LOGIC ("address = " << std::hex << m_state[i].address);
    }
}

Ipv4AddressManager::~Ipv4AddressManager ()
{
  NS_LOG_FUNCTION;
}

void
Ipv4AddressManager::Seed (const Ipv4Mask mask, const Ipv4Address address)
{
  NS_LOG_FUNCTION;

  uint32_t maskBits = mask.GetHostOrder ();
  uint32_t addressBits = address.GetHostOrder ();

  for (uint32_t i = 0; i < N_BITS; ++i)
    {
      if (maskBits & 1)
        {
          uint32_t nMaskBits = N_BITS - i;
          NS_ASSERT(nMaskBits >= 0 && nMaskBits < N_BITS);
          m_state[nMaskBits].address = addressBits;
          return;
        }
      maskBits >>= 1;
    }
  NS_ASSERT_MSG(false, "Impossible");
  return;
}

Ipv4Address
Ipv4AddressManager::Allocate (const Ipv4Mask mask, const Ipv4Address network)
{
  NS_LOG_FUNCTION;

  uint32_t bits = mask.GetHostOrder ();
  uint32_t net = network.GetHostOrder ();

  for (uint32_t i = 0; i < N_BITS; ++i)
    {
      if (bits & 1)
        {
          uint32_t nBits = N_BITS - i;
          NS_ASSERT(nBits >= 0 && nBits < N_BITS);
          Ipv4Address addr (net | m_state[nBits].address);
          ++m_state[nBits].address;
          NS_ASSERT_MSG((m_state[nBits].mask & m_state[nBits].address) == 0, 
            "Ipv4AddressManager::Allocate(): Overflow");
          return addr;
        }
      bits >>= 1;
    }
  NS_ASSERT_MSG(false, "Impossible");
  return Ipv4Address (bits);
}

void
Ipv4AddressEx::SeedAddress (const Ipv4Mask mask, const Ipv4Address address)
{
  NS_LOG_FUNCTION;

  SimulationSingleton<Ipv4AddressManager>::Get ()->Seed (mask, address);
}

Ipv4Address
Ipv4AddressEx::AllocateAddress (const Ipv4Mask mask, const Ipv4Address network)
{
  NS_LOG_FUNCTION;

  return SimulationSingleton<Ipv4AddressManager>::Get ()->
    Allocate (mask, network);
}

void
Ipv4AddressEx::SeedNetwork (const Ipv4Mask mask, const Ipv4Address address)
{
  NS_LOG_FUNCTION;

  SimulationSingleton<Ipv4NetworkManager>::Get ()->Seed (mask, address);
}

Ipv4Address
Ipv4AddressEx::AllocateNetwork (const Ipv4Mask mask)
{
  NS_LOG_FUNCTION;

  return SimulationSingleton<Ipv4NetworkManager>::Get ()->
    Allocate (mask);
}

}; // namespace ns3
