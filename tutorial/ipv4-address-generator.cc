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
#include "ipv4-address-generator.h"

NS_LOG_COMPONENT_DEFINE("Ipv4AddressGenerator");

namespace ns3 {

class Ipv4NetworkGeneratorImpl
{
public:
  Ipv4NetworkGeneratorImpl ();
  virtual ~Ipv4NetworkGeneratorImpl ();

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

Ipv4NetworkGeneratorImpl::Ipv4NetworkGeneratorImpl () 
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

Ipv4NetworkGeneratorImpl::~Ipv4NetworkGeneratorImpl ()
{
  NS_LOG_FUNCTION;
}

  void
Ipv4NetworkGeneratorImpl::Seed (
  const Ipv4Mask mask, 
  const Ipv4Address network)
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
Ipv4NetworkGeneratorImpl::Allocate (const Ipv4Mask mask)
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

class Ipv4AddressGeneratorImpl
{
public:
  Ipv4AddressGeneratorImpl ();
  virtual ~Ipv4AddressGeneratorImpl ();

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

Ipv4AddressGeneratorImpl::Ipv4AddressGeneratorImpl () 
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

Ipv4AddressGeneratorImpl::~Ipv4AddressGeneratorImpl ()
{
  NS_LOG_FUNCTION;
}

  void
Ipv4AddressGeneratorImpl::Seed (
  const Ipv4Mask mask, 
  const Ipv4Address address)
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
Ipv4AddressGeneratorImpl::Allocate (
  const Ipv4Mask mask,
  const Ipv4Address network)
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
            "Ipv4AddressGeneratorImpl::Allocate(): Overflow");
          return addr;
        }
      bits >>= 1;
    }
  NS_ASSERT_MSG(false, "Impossible");
  return Ipv4Address (bits);
}

  void
Ipv4AddressGenerator::SeedAddress (
  const Ipv4Mask mask, 
  const Ipv4Address address)
{
  NS_LOG_FUNCTION;

  SimulationSingleton<Ipv4AddressGeneratorImpl>::Get ()->Seed (mask, address);
}

  Ipv4Address
Ipv4AddressGenerator::AllocateAddress (
  const Ipv4Mask mask, 
  const Ipv4Address network)
{
  NS_LOG_FUNCTION;

  return SimulationSingleton<Ipv4AddressGeneratorImpl>::Get ()->
    Allocate (mask, network);
}

  void
Ipv4AddressGenerator::SeedNetwork (
  const Ipv4Mask mask,
  const Ipv4Address address)
{
  NS_LOG_FUNCTION;

  SimulationSingleton<Ipv4NetworkGeneratorImpl>::Get ()->Seed (mask, address);
}

  Ipv4Address
Ipv4AddressGenerator::AllocateNetwork (const Ipv4Mask mask)
{
  NS_LOG_FUNCTION;

  return SimulationSingleton<Ipv4NetworkGeneratorImpl>::Get ()->
    Allocate (mask);
}

}; // namespace ns3
