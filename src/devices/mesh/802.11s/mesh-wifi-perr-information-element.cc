/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */


#include "ns3/mesh-wifi-perr-information-element.h"
#include "ns3/address-utils.h"

namespace ns3 {
WifiPerrInformationElement::~WifiPerrInformationElement()
{
}

TypeId
WifiPerrInformationElement::GetTypeId()
{
  static TypeId tid = TypeId ("ns3::WifiPerrInformationElement")
                      .SetParent<Object> ();
  return tid;
}
void
WifiPerrInformationElement::PrintInformation(std::ostream &os) const
  {
    // FILL
  }
TypeId
WifiPerrInformationElement::GetInstanceTypeId() const
  {
    return GetTypeId();
  }
WifiPerrInformationElement::WifiPerrInformationElement():
    m_numOfDest(0)
{
}
uint8_t
WifiPerrInformationElement::GetNumOfDest()
{
  return m_numOfDest;
}
void
WifiPerrInformationElement::SerializeInformation(Buffer::Iterator i)const
  {
    i.WriteU8 (0);
    i.WriteU8 (m_numOfDest);
    NS_ASSERT (m_numOfDest == m_addressUnits.size());
    for (unsigned int j = 0; j < m_numOfDest; j++)
      {
        WriteTo (i, m_addressUnits[j].destination);
        i.WriteHtonU32 (m_addressUnits[j].seqnum);
      }
  }
uint16_t
WifiPerrInformationElement::DeserializeInformation(Buffer::Iterator start, uint8_t length)
{
  Buffer::Iterator i = start;
  i.Next (1); //Mode flags is not used now
  m_numOfDest = i.ReadU8();
  NS_ASSERT ((2+10*m_numOfDest) == length);
  length = 0; //to avoid compiler warning in optimized builds
  for (unsigned int j = 0; j < m_numOfDest; j++)
    {
      HwmpRtable::FailedDestination unit;
      ReadFrom(i,unit.destination);
      unit.seqnum = i.ReadNtohU32();
      m_addressUnits.push_back(unit);
    }
  return i.GetDistanceFrom(start);
}

uint16_t
WifiPerrInformationElement::GetInformationSize() const
  {
    uint16_t retval =
       1 //ModeFlags
      +1 //NumOfDests
      +6*m_numOfDest
      +4*m_numOfDest;
    return retval;
  }
uint8_t
WifiPerrInformationElement::GetLengthField() const
{
  return 2+10*m_numOfDest;
}
void
WifiPerrInformationElement::AddAddressUnit(HwmpRtable::FailedDestination unit)
{
  for (unsigned int i = 0; i < m_addressUnits.size(); i ++)
    if (m_addressUnits[i].destination == unit.destination)
      return;
  m_addressUnits.push_back(unit);
  m_numOfDest++;
}

std::vector<HwmpRtable::FailedDestination>
WifiPerrInformationElement::GetAddressUnitVector()
{
  return m_addressUnits;
}
void
WifiPerrInformationElement::DeleteAddressUnit(Mac48Address address)
{
  for (std::vector<HwmpRtable::FailedDestination>::iterator i = m_addressUnits.begin(); i != m_addressUnits.end(); i ++)
    if ((*i).destination == address)
      {
        m_numOfDest --;
        m_addressUnits.erase(i);
        break;
      }
}

void
WifiPerrInformationElement::ResetPerr()
{
  m_numOfDest = 0;
  m_addressUnits.clear();
}
}//namespace ns3

