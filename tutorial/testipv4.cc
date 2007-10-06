/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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

#include "ns3/log.h"
#include "ipv4-address-extended.h"

NS_LOG_COMPONENT_DEFINE ("TestIpv4");

using namespace ns3;

int 
main (int argc, char *argv[])
{
  LogComponentEnable ("TestIpv4", LOG_LEVEL_ALL);

  NS_LOG_INFO ("Test Ipv4");

  Ipv4Mask mask1 ("255.0.0.0");

  for (uint32_t i = 0; i < 10; ++i)
    {
      Ipv4Address network = Ipv4AddressEx::AllocateNetwork (mask1);
      Ipv4Address address = Ipv4AddressEx::AllocateAddress (mask1, network);
      NS_LOG_INFO ("address = " << address);
    }

  Ipv4Mask mask2 ("255.255.0.0");
  Ipv4AddressEx::SeedNetwork (mask2, "192.168.0.0");
  Ipv4AddressEx::SeedAddress (mask2, "0.0.0.3");

  Ipv4Address network1 = Ipv4AddressEx::AllocateNetwork (mask2);
  for (uint32_t i = 0; i < 10; ++i)
    {
      Ipv4Address address = Ipv4AddressEx::AllocateAddress (mask2, network1);
      NS_LOG_INFO ("address = " << address);
    }

  Ipv4Mask mask3 ("255.255.255.0");

  for (uint32_t i = 0; i < 10; ++i)
    {
      Ipv4Address network = Ipv4AddressEx::AllocateNetwork (mask3);
      Ipv4Address address = Ipv4AddressEx::AllocateAddress (mask3, network);
      NS_LOG_INFO ("address = " << address);
    }
}
