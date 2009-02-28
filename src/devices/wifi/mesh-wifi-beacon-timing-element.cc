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


#include "mesh-wifi-beacon-timing-element.h"
#define ELEMENT_ID (19)
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

NeighboursTimingUnitsList
WifiBeaconTimingElement::GetNeighboursTimingElementsList()
{
	return m_neighbours;
}

void
WifiBeaconTimingElement::AddNeighboursTimingElementUnit(
		uint16_t	aid,
		Time		last_beacon,	//MicroSeconds!
		Time		beacon_interval	//MicroSeconds!
		)
{
	//Firs we lookup if this element already exists
	for(NeighboursTimingUnitsList::iterator i = m_neighbours.begin(); i!= m_neighbours.end(); i++)
		if(
				   ((*i)->GetAID() == AID_TO_U8(aid))
				&& ((*i)->GetLastBeacon() == TIMESTAMP_TO_U16(last_beacon))
				&& ((*i)->GetBeaconInterval() == BEACON_INTERVAL_TO_U16(beacon_interval))
				)
			return;
	Ptr<WifiBeaconTimingElementUnit>new_element = Create<WifiBeaconTimingElementUnit>();
	new_element->SetAID(AID_TO_U8(aid));
	new_element->SetLastBeacon(TIMESTAMP_TO_U16(last_beacon));
	new_element->SetBeaconInterval(BEACON_INTERVAL_TO_U16(beacon_interval));
	m_neighbours.push_back(new_element);
	m_numOfUnits++;
}

void
WifiBeaconTimingElement::DelNeighboursTimingElementUnit(
		uint16_t	aid,
		Time		last_beacon,	//MicroSeconds!
		Time		beacon_interval	//MicroSeconds!
		)
{
	for(NeighboursTimingUnitsList::iterator i = m_neighbours.begin(); i!= m_neighbours.end(); i++)
		if(
				   ((*i)->GetAID() == AID_TO_U8(aid))
				&& ((*i)->GetLastBeacon() == TIMESTAMP_TO_U16(last_beacon))
				&& ((*i)->GetBeaconInterval() == BEACON_INTERVAL_TO_U16(beacon_interval))
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
	for(NeighboursTimingUnitsList::iterator j = m_neighbours.begin(); j!= m_neighbours.end(); j++)
	{
		to_delete++;
		(*j) = 0;
	}
	for(i = 0; i < to_delete; i ++)
		m_neighbours.pop_back();
	m_neighbours.clear();

}

uint32_t
WifiBeaconTimingElement::GetSerializedSize (void) const
{
	return (2+5*m_numOfUnits > m_maxSize) ? 2+((m_maxSize-2)/5)*5 : 2+5*m_numOfUnits;
}
Buffer::Iterator
WifiBeaconTimingElement::Serialize (Buffer::Iterator i) const
{
	uint8_t can_be_written = (2+5*m_numOfUnits > m_maxSize) ? ((m_maxSize-2)/5) : m_numOfUnits;
	int actually_written = 0;
	i.WriteU8 (ELEMENT_ID);
	i.WriteU8 (can_be_written);
	for(NeighboursTimingUnitsList::const_iterator j = m_neighbours.begin(); j!= m_neighbours.end(); j++)
	{
		i.WriteU8	((*j)->GetAID());
		i.WriteHtonU16	((*j)->GetLastBeacon());
		i.WriteHtonU16	((*j)->GetBeaconInterval());
		actually_written++;
		if(actually_written > can_be_written)
			break;
	}
	if(can_be_written < m_numOfUnits)
	{
		//move written units to the end of our list, so they
		//can be sent in timing element with next beacon
		//TODO:swap elements
		
	}
	return i;
}
Buffer::Iterator
WifiBeaconTimingElement::Deserialize (Buffer::Iterator i)
{
	uint8_t elementId;
	uint8_t num_to_read = 0;
	int j;
	elementId = i.ReadU8();
	NS_ASSERT(elementId == ELEMENT_ID);
	num_to_read = i.ReadU8();
	for(j=0;j<num_to_read; j++)
	{		
		Ptr<WifiBeaconTimingElementUnit> new_element = Create<WifiBeaconTimingElementUnit>();
		new_element->SetAID(i.ReadU8());
		new_element->SetLastBeacon(i.ReadNtohU16());
		new_element->SetBeaconInterval(i.ReadNtohU16());
		m_neighbours.push_back(new_element);
	}
	return i;
};
} //namespace ns3
