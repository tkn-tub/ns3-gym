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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "address-utils.h"
#include "inet-socket-address.h"

namespace ns3 {

void WriteTo (Buffer::Iterator &i, Ipv4Address ad)
{
  i.WriteHtonU32 (ad.Get ());
}
void WriteTo (Buffer::Iterator &i, Ipv6Address ad)
{
  uint8_t buf[16];
  ad.GetBytes (buf);
  i.Write (buf, 16);
}
void WriteTo (Buffer::Iterator &i, const Address &ad)
{
  uint8_t mac[Address::MAX_SIZE];
  ad.CopyTo (mac);
  i.Write (mac, ad.GetLength ());
}
void WriteTo (Buffer::Iterator &i, Mac48Address ad)
{
  uint8_t mac[6];
  ad.CopyTo (mac);
  i.Write (mac, 6);
}

void ReadFrom (Buffer::Iterator &i, Ipv4Address &ad)
{
  ad.Set (i.ReadNtohU32 ());
}
void ReadFrom (Buffer::Iterator &i, Ipv6Address &ad)
{
  uint8_t ipv6[16];
  i.Read (ipv6, 16);
  ad.Set (ipv6);
}
void ReadFrom (Buffer::Iterator &i, Address &ad, uint32_t len)
{
  uint8_t mac[Address::MAX_SIZE];
  i.Read (mac, len);
  ad.CopyFrom (mac, len);
}
void ReadFrom (Buffer::Iterator &i, Mac48Address &ad)
{
  uint8_t mac[6];
  i.Read (mac, 6);
  ad.CopyFrom (mac);
}

namespace addressUtils {

bool IsMulticast (const Address &ad)
{
  if (InetSocketAddress::IsMatchingType (ad))
    {
      InetSocketAddress inetAddr = InetSocketAddress::ConvertFrom (ad);
      Ipv4Address ipv4 = inetAddr.GetIpv4 ();
      return ipv4.IsMulticast ();
    }
  // IPv6 case can go here, in future
  return false;
}

} // namespace addressUtils

} // namespace ns3
