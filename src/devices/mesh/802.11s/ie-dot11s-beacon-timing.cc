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


#include "ns3/ie-dot11s-beacon-timing.h"
#include "ns3/log.h"
namespace ns3 {
/*******************************************
 * WifiBeaconTimingElementUnit
 *******************************************/
WifiBeaconTimingElementUnit::WifiBeaconTimingElementUnit()
{
  AID = 0;
  LastBeacon = 0;
  BeaconInterval = 0;
}

void
WifiBeaconTimingElementUnit::SetAID(uint8_t aid)
{
  AID = aid;
}

void
WifiBeaconTimingElementUnit::SetLastBeacon(uint16_t last_beacon)
{
  LastBeacon = last_beacon;
}

void
WifiBeaconTimingElementUnit::SetBeaconInterval(uint16_t beacon_interval)
{
  BeaconInterval = beacon_interval;
}

uint8_t
WifiBeaconTimingElementUnit::GetAID()
{
  return AID;
}

uint16_t
WifiBeaconTimingElementUnit::GetLastBeacon()
{
  return LastBeacon;
}

uint16_t
WifiBeaconTimingElementUnit::GetBeaconInterval()
{
  return BeaconInterval;
}

/*******************************************
 * WifiBeaconTimingElement
 *******************************************/
WifiBeaconTimingElement::WifiBeaconTimingElement()
{
  m_maxSize = DEFAULT_MAX_SIZE;
  m_numOfUnits = 0;
}

WifiBeaconTimingElement::NeighboursTimingUnitsList
WifiBeaconTimingElement::GetNeighboursTimingElementsList()
{
  return m_neighbours;
}

void
WifiBeaconTimingElement::AddNeighboursTimingElementUnit(
  uint16_t aid,
  Time  last_beacon, //MicroSeconds!
  Time  beacon_interval //MicroSeconds!
)
{
  if(m_numOfUnits == 50)
    return;
  //Firs we lookup if this element already exists
  for (NeighboursTimingUnitsList::iterator i = m_neighbours.begin(); i!= m_neighbours.end(); i++)
    if (
      ((*i)->GetAID() == AidToU8(aid))
      && ((*i)->GetLastBeacon() == TimestampToU16(last_beacon))
      && ((*i)->GetBeaconInterval() == BeaconIntervalToU16(beacon_interval))
    )
      return;
  Ptr<WifiBeaconTimingElementUnit>new_element = Create<WifiBeaconTimingElementUnit>();
  new_element->SetAID(AidToU8(aid));
  new_element->SetLastBeacon(TimestampToU16(last_beacon));
  new_element->SetBeaconInterval(BeaconIntervalToU16(beacon_interval));
  m_neighbours.push_back(new_element);
  m_numOfUnits++;
}

void
WifiBeaconTimingElement::DelNeighboursTimingElementUnit(
  uint16_t aid,
  Time  last_beacon, //MicroSeconds!
  Time  beacon_interval //MicroSeconds!
)
{
  for (NeighboursTimingUnitsList::iterator i = m_neighbours.begin(); i!= m_neighbours.end(); i++)
    if (
      ((*i)->GetAID() == AidToU8(aid))
      && ((*i)->GetLastBeacon() == TimestampToU16(last_beacon))
      && ((*i)->GetBeaconInterval() == BeaconIntervalToU16(beacon_interval))
    )
      {
        m_neighbours.erase(i);
        m_numOfUnits--;
        break;
      }

}

void
WifiBeaconTimingElement::ClearTimingElement()
{
  uint16_t to_delete = 0;
  uint16_t i;
  for (NeighboursTimingUnitsList::iterator j = m_neighbours.begin(); j!= m_neighbours.end(); j++)
    {
      to_delete++;
      (*j) = 0;
    }
  for (i = 0; i < to_delete; i ++)
    m_neighbours.pop_back();
  m_neighbours.clear();

}

uint16_t
WifiBeaconTimingElement::GetInformationSize () const
{
  return (5*m_numOfUnits);
}

void
WifiBeaconTimingElement::PrintInformation(std::ostream& os) const
{
  //TODO
}

void
WifiBeaconTimingElement::SerializeInformation (Buffer::Iterator i) const
{
  for (NeighboursTimingUnitsList::const_iterator j = m_neighbours.begin(); j!= m_neighbours.end(); j++)
  {
    i.WriteU8 ((*j)->GetAID());
    i.WriteHtonU16 ((*j)->GetLastBeacon());
    i.WriteHtonU16 ((*j)->GetBeaconInterval());
  }
}
uint16_t 
WifiBeaconTimingElement::DeserializeInformation (Buffer::Iterator start, uint8_t length)
{
  Buffer::Iterator i = start;
  m_numOfUnits = length/5;
  for (int j = 0; j < m_numOfUnits; j ++)
    {
      Ptr<WifiBeaconTimingElementUnit> new_element = Create<WifiBeaconTimingElementUnit>();
      new_element->SetAID(i.ReadU8());
      new_element->SetLastBeacon(i.ReadNtohU16());
      new_element->SetBeaconInterval(i.ReadNtohU16());
      m_neighbours.push_back(new_element);
    }
  return i.GetDistanceFrom(start);
};

uint16_t
WifiBeaconTimingElement::TimestampToU16(Time x)
{
  return ((uint16_t)((x.GetMicroSeconds() >> 8)&0xffff));
};

uint16_t 
WifiBeaconTimingElement::BeaconIntervalToU16(Time x)
{
  return ((uint16_t)(x.GetMicroSeconds() >>10)&0xffff);
};

uint8_t
WifiBeaconTimingElement::AidToU8(uint16_t x)
{
  return (uint8_t)(x&0xff);
};
} //namespace ns3
