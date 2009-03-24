/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/* 
 * Copyright (c) 2009 IITP RAS
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
 * Author: Pavel Boyko <boyko@iitp.ru>
 */

#include "ns3/wifi-information-element.h"
#include "ns3/packet.h"
#include "ns3/log.h"

namespace ns3 {

TypeId
WifiInformationElement::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::WifiInformationElement")
                      .SetParent<Header> ();
  return tid;
}

TypeId
WifiInformationElement::GetInstanceTypeId () const
{
  return GetTypeId ();
}
uint32_t WifiInformationElement::GetSerializedSize () const
{
  return (GetInformationSize () + 2);
}

void WifiInformationElement::Serialize (Buffer::Iterator i) const
{
  i.WriteU8 (ElementId ());
  i.WriteU8 (GetInformationSize ());
  SerializeInformation (i);
}

uint32_t WifiInformationElement::Deserialize (Buffer::Iterator i)
{
  if(i.ReadU8 () != ElementId())
    return 0;
  uint8_t length = i.ReadU8 ();
  
  return (DeserializeInformation (i, length) + 2);
}

void WifiInformationElement::Print (std::ostream &os) const
{
  os << "<information_element id=" << ElementId () << ">\n";
  PrintInformation (os);
  os << "</information_element>\n";
}

bool WifiInformationElement::FindFirst(Ptr<Packet> packet)
{
  const uint8_t * data = packet->PeekData();
  uint32_t position = 0;
  while(position < packet->GetSize ())
  {
    if(data[position] == ElementId())
    {
      Ptr<Packet> myIe = packet->CreateFragment(position, data[position+1]+2);
      NS_ASSERT(myIe->GetSize() == (uint32_t)(data[position+1]+2));
      myIe->RemoveHeader(*this);
      return true;
    }
    else
    {
      if(data[position] > ElementId())
        return false;
      position +=data[position+1]+2;
    }
  }
  return false;
}

bool operator< (WifiInformationElement const & a, WifiInformationElement const & b)
{
  return (a.ElementId () < b.ElementId());
}


}


