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
 * Authors: Yana Podkosova <yanapdk@rambler.ru>
 *          Kirill Andreev <andreev@iitp.ru>
 */


#include "mesh-wifi-preq-information-element.h"
#include "ns3/address-utils.h"
#include "ns3/assert.h"
#define ELEMENT_ID (22)
namespace ns3{
/*************************
 * DestinationAddressUnit
 ************************/
DestinationAddressUnit::DestinationAddressUnit()
{
        m_do = false;
	m_rf = false;
	m_destSeqNumber = 0;
	uint8_t mac_buffer[6];
	for (int j = 0; j < 6; j++)
	{
		mac_buffer[j] = 0;
	}
	m_destinationAddress.CopyFrom(mac_buffer);
}
void
DestinationAddressUnit::SetFlags(bool doFlag, bool rfFlag)
{
	m_do = doFlag;
	m_rf = rfFlag;
}

void
DestinationAddressUnit::SetDestSeqNumber(uint32_t dest_seq_number)
{
	m_destSeqNumber = dest_seq_number;
}
void 
DestinationAddressUnit::SetDestinationAddress(Mac48Address dest_address)
{
	m_destinationAddress = dest_address;
}
bool
DestinationAddressUnit::IsDo()
{
	return m_do;
}

bool
DestinationAddressUnit::IsRf()
{
	return m_rf;
}

uint32_t
DestinationAddressUnit::GetDestSeqNumber() const
{
	return m_destSeqNumber;
}
Mac48Address 
DestinationAddressUnit::GetDestinationAddress() const
{
	return m_destinationAddress;
}
/********************************
 * WifiPreqInformationElement
 *******************************/
WifiPreqInformationElement::~WifiPreqInformationElement()
{
}

TypeId
WifiPreqInformationElement::GetTypeId()
{
	static TypeId tid = TypeId ("ns3::WifiPreqInformationElement")
		.SetParent<Object> ();
	return tid;
}
void
WifiPreqInformationElement::Print(std::ostream &os) const
{
	//TODO:fill this method
}

TypeId
WifiPreqInformationElement::GetInstanceTypeId(void) const
{
	return GetTypeId();
}

WifiPreqInformationElement::WifiPreqInformationElement():
	m_maxSize(32),
	m_flags(0),
	m_hopCount(0),
	m_ttl(0),
	m_preqId(0),
	m_originatorAddress(Mac48Address::GetBroadcast()),
	m_originatorSeqNumber(0),
	m_lifetime(0),
	m_metric(0),
	m_destCount(0)
{
}
void
WifiPreqInformationElement::SetUnicastPreq()
{
	m_flags |= 1<<1;
}

void
WifiPreqInformationElement::SetNeedNotPrep()
{
	m_flags |= 1<<2;
}
//void
//WifiPreqInformationElement::SetFlags(uint8_t flags)
//{
//	m_flags = flags;
//}
void
WifiPreqInformationElement::SetHopcount(uint8_t hopcount)
{
	m_hopCount = hopcount;
}
void
WifiPreqInformationElement::SetTTL(uint8_t ttl)
{
	m_ttl = ttl;
}
void
WifiPreqInformationElement::SetPreqID(uint32_t preq_id)
{
	m_preqId = preq_id;
}
void
WifiPreqInformationElement::SetMetric(uint32_t metric)
{
	m_metric = metric;
}
void 
WifiPreqInformationElement::SetOriginatorAddress(Mac48Address originator_address)
{
	m_originatorAddress = originator_address;
}
void
WifiPreqInformationElement::SetOriginatorSeqNumber(uint32_t originator_seq_number)
{
	m_originatorSeqNumber = originator_seq_number;
}
void 
WifiPreqInformationElement::SetLifetime(uint32_t lifetime)
{
	m_lifetime = lifetime;
}
void
WifiPreqInformationElement::SetDestCount(uint8_t dest_count)
{
	m_destCount = dest_count;
}

//uint8_t
//WifiPreqInformationElement::GetFlags() const
//{
//	return m_flags;
//}
bool
WifiPreqInformationElement::IsUnicastPreq() const
{
	return (m_flags & (1<<1));
}

bool
WifiPreqInformationElement::IsNeedNotPrep() const
{
	return (m_flags & (1<<2));
}

uint8_t
WifiPreqInformationElement::GetHopCount() const
{
	return m_hopCount;
}
uint8_t
WifiPreqInformationElement::GetTtl() const
{
	return m_ttl;
}
uint32_t
WifiPreqInformationElement::GetPreqID() const
{
	return m_preqId;
}
uint32_t 
WifiPreqInformationElement::GetMetric() const
{
	return m_metric;
}
Mac48Address
WifiPreqInformationElement::GetOriginatorAddress() const
{
	return m_originatorAddress;
}
uint32_t
WifiPreqInformationElement::GetOriginatorSeqNumber() const
{
	return m_originatorSeqNumber;
}
uint32_t 
WifiPreqInformationElement::GetLifetime() const
{
	return m_lifetime;
}

uint8_t
WifiPreqInformationElement::GetDestCount() const
{
	return m_destCount;
}

void
WifiPreqInformationElement::DecrementTtl()
{
	m_ttl --;
	m_hopCount ++;
}

void
WifiPreqInformationElement::IncrementMetric(uint32_t metric)
{
	m_metric +=metric;
}

void
WifiPreqInformationElement::Serialize(Buffer::Iterator i) const
{
	i.WriteU8 (ELEMENT_ID);
	//TODO:Check maxsize
	uint8_t length = m_destCount*11+28;
	if(m_destCount> m_maxSize)
		length -=(m_destCount-m_maxSize)*11;
	i.WriteU8(length);
	i.WriteU8 (m_flags);
	i.WriteU8 (m_hopCount);
	i.WriteU8 (m_ttl);
	i.WriteHtonU32 (m_preqId);
	WriteTo(i, m_originatorAddress);
	i.WriteHtonU32 (m_originatorSeqNumber);
	i.WriteHtonU32 (m_lifetime);
	i.WriteHtonU32 (m_metric);
	i.WriteU8 (m_destCount);
	int written = 0;
	for (std::vector<Ptr<DestinationAddressUnit> >::const_iterator j = m_destinations.begin(); j!= m_destinations.end(); j++)
	{
		uint8_t flags = 0;
		if((*j)->IsDo())
			flags +=128;
		if((*j)->IsRf())
			flags +=64;
		i.WriteU8(flags);
		WriteTo(i,(*j)->GetDestinationAddress());
		i.WriteHtonU32 ((*j)->GetDestSeqNumber());
		written++;
		if (written > m_maxSize)
			break;
	}
}

uint32_t
WifiPreqInformationElement::Deserialize(Buffer::Iterator start)
{
	Buffer::Iterator i = start;
	uint8_t ElementId;
	ElementId = i.ReadU8();
	NS_ASSERT (ElementId == ELEMENT_ID);
	uint8_t length;
	length = i.ReadU8();
	m_flags = i.ReadU8();
	m_hopCount = i.ReadU8();
	m_ttl = i.ReadU8();
	m_preqId = i.ReadNtohU32();
	ReadFrom(i,m_originatorAddress);
	m_originatorSeqNumber = i.ReadNtohU32();
        m_lifetime = i.ReadNtohU32();
	m_metric = i.ReadNtohU32();
	m_destCount = i.ReadU8();
	for (int j = 0; j < m_destCount; j++ )
	{
		Ptr<DestinationAddressUnit> new_element = Create<DestinationAddressUnit>();
		bool doFlag = false;
		bool rfFlag = false;
		uint8_t flags = i.ReadU8();
		if(flags >= 128)
		{
			doFlag = true;
			flags -=128;
		}
		if(flags >=64)
			rfFlag = true;
		new_element->SetFlags(doFlag, rfFlag);
		Mac48Address addr;
		ReadFrom(i,addr);
		new_element->SetDestinationAddress(addr);
		new_element->SetDestSeqNumber(i.ReadNtohU32());
		m_destinations.push_back(new_element);
		NS_ASSERT(28+j*11 < length);
	}
	return i.GetDistanceFrom(start);
}
uint32_t
WifiPreqInformationElement::GetSerializedSize() const
{
	uint32_t retval = 
		 1	//Element ID
		+1	//Length
		+1	//Flags
		+1	//Hopcount
		+1	//TTL
		+4	//PREQ ID
		+6	//Source address (originator)
		+4	//Originator seqno
		+4	//Lifetime
		+4	//metric
		+1;	//destination count
	if(m_destCount > m_maxSize)
		retval+=(m_maxSize*11);
	else
		retval +=(m_destCount*11);
	return retval;
}
std::vector<Ptr<DestinationAddressUnit> >
WifiPreqInformationElement::GetDestinationList()
{
	return m_destinations;
}
void
WifiPreqInformationElement::AddDestinationAddressElement(
		bool doFlag, bool rfFlag,
		Mac48Address dest_address,
                uint32_t dest_seq_number
		)
{
	for(std::vector<Ptr<DestinationAddressUnit> >::iterator i = m_destinations.begin(); i!=m_destinations.end(); i++ )
		if((*i)->GetDestinationAddress() == dest_address)
			return;
	Ptr<DestinationAddressUnit>new_element = Create<DestinationAddressUnit>();
	new_element->SetFlags(doFlag, rfFlag);
	new_element->SetDestinationAddress(dest_address);
	new_element->SetDestSeqNumber(dest_seq_number);
	m_destinations.push_back(new_element);
	m_destCount++;
}

void
WifiPreqInformationElement::DelDestinationAddressElement(Mac48Address dest_address)
{
	for(std::vector<Ptr<DestinationAddressUnit> >::iterator i = m_destinations.begin(); i!=m_destinations.end(); i++ )
	if((*i)->GetDestinationAddress() == dest_address)
		{
			m_destinations.erase(i);
			m_destCount--;
			break;
		}
}

void
WifiPreqInformationElement::ClearDestinationAddressElement()
{
	int i;
	for(std::vector<Ptr<DestinationAddressUnit> >::iterator j = m_destinations.begin(); j!= m_destinations.end(); j++)
		(*j) = 0;
	for(i = 0; i < m_destCount; i ++)
		m_destinations.pop_back();
	m_destinations.clear();
};

}
