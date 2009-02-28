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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 *          Yana Podkosova <yanapdk@rambler.ru>
 *          Aleksey Kovalenko <kovalenko@iitp.ru>
 */


#include "mesh-mgt-headers.h"
#include "ns3/address-utils.h"
#include "ns3/simulator.h"
#include "ns3/assert.h"

namespace ns3
{
/***********************************************************
*          PeerLinkOpen frame:
************************************************************/
NS_OBJECT_ENSURE_REGISTERED (MeshMgtPeerLinkManFrame);

MeshMgtPeerLinkManFrame::MeshMgtPeerLinkManFrame()
{
}

void
MeshMgtPeerLinkManFrame::SetAid(uint16_t aid)
{
	Aid = aid;
}

void
MeshMgtPeerLinkManFrame::SetSupportedRates(SupportedRates rates)
{
	Rates = rates;
}

void
MeshMgtPeerLinkManFrame::SetQosField(uint16_t qos)
{
	QoS= qos;
}

void
MeshMgtPeerLinkManFrame::SetMeshId(Ssid Id)
{
	MeshId = Id;
}

void
MeshMgtPeerLinkManFrame::SetMeshConfigurationElement(MeshConfigurationElement MeshConf)
{
	MeshConfig = MeshConf;
}

void
MeshMgtPeerLinkManFrame::SetPeerLinkManagementElement(PeerLinkManagementElement MeshPeerElement)
{
	PeerLinkMan = MeshPeerElement;
}

uint16_t
MeshMgtPeerLinkManFrame::GetAid()
{
	return Aid;
}

SupportedRates
MeshMgtPeerLinkManFrame::GetSupportedRates()
{
	return Rates;
}

uint16_t
MeshMgtPeerLinkManFrame::GetQosField()
{
	return QoS;
}

Ssid
MeshMgtPeerLinkManFrame::GetMeshId()
{
	return MeshId;
}

MeshConfigurationElement
MeshMgtPeerLinkManFrame::GetMeshConfigurationElement()
{
	return MeshConfig;
}

PeerLinkManagementElement
MeshMgtPeerLinkManFrame::GetPeerLinkManagementElement()
{
	return PeerLinkMan;
}

TypeId
MeshMgtPeerLinkManFrame::GetTypeId(void)
{
	static TypeId tid = 
		TypeId ("ns3::MeshMgtPeerLinkManFrame")
		.SetParent<Header> ()
		.AddConstructor<MeshMgtPeerLinkManFrame> ()
		;
	return tid;
}

TypeId
MeshMgtPeerLinkManFrame::GetInstanceTypeId(void) const
{
	return GetTypeId();
}

void
MeshMgtPeerLinkManFrame::Print(std::ostream &os) const
{
	//TODO:fill this method
}

uint32_t
MeshMgtPeerLinkManFrame::GetSerializedSize(void) const
{
	uint32_t size = 1; //Subtype
	if(MESH_MGT_HEADER_PEER_CONFIRM == Subtype)
		size +=2; //AID of remote peer
	if(MESH_MGT_HEADER_PEER_CLOSE != Subtype)
	{
		size += Rates.GetSerializedSize ();
		size +=2;
		size += MeshId.GetSerializedSize ();
		size += MeshConfig.GetSerializedSize ();
	}
	size += PeerLinkMan.GetSerializedSize ();
	return size;
}

void
MeshMgtPeerLinkManFrame::Serialize(Buffer::Iterator start) const
{
	Buffer::Iterator i = start;
	i.WriteU8(Subtype); //Like a Category in Standart
	if(MESH_MGT_HEADER_PEER_CONFIRM == Subtype)
		
		i.WriteHtonU16(Aid);
	if(MESH_MGT_HEADER_PEER_CLOSE != Subtype)
	{
		i = Rates.Serialize(i);
		//now QoS capabilities
		i.WriteHtonU16(QoS);
		i = MeshId.Serialize (i);
		i = MeshConfig.Serialize (i);
	}
	i = PeerLinkMan.Serialize (i);
}

uint32_t
MeshMgtPeerLinkManFrame::Deserialize(Buffer::Iterator start)
{
	Buffer::Iterator i = start;
	Subtype = i.ReadU8();
	if(MESH_MGT_HEADER_PEER_CONFIRM == Subtype)
		Aid = i.ReadNtohU16();
	if(MESH_MGT_HEADER_PEER_CLOSE != Subtype)
	{
		i = Rates.Deserialize(i);
		QoS = i.ReadNtohU16();
		i = MeshId.Deserialize (i);
		i = MeshConfig.Deserialize (i);
	}
	i = PeerLinkMan.Deserialize (i);
	return i.GetDistanceFrom (start);
}
void
MeshMgtPeerLinkManFrame::SetOpen()
{
	Subtype = MESH_MGT_HEADER_PEER_OPEN;
}

void
MeshMgtPeerLinkManFrame::SetConfirm()
{
	Subtype = MESH_MGT_HEADER_PEER_CONFIRM;
}

void
MeshMgtPeerLinkManFrame::SetClose()
{
	Subtype = MESH_MGT_HEADER_PEER_CLOSE;
}

bool
MeshMgtPeerLinkManFrame::IsOpen()
{
	return (Subtype==MESH_MGT_HEADER_PEER_OPEN);
}

bool
MeshMgtPeerLinkManFrame::IsConfirm()
{
	return (Subtype==MESH_MGT_HEADER_PEER_CONFIRM);
}

bool
MeshMgtPeerLinkManFrame::IsClose()
{
	return (Subtype==MESH_MGT_HEADER_PEER_CLOSE);
}

} //namespace NS3


