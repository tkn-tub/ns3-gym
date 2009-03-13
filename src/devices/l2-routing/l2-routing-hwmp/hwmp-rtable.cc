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


#include "ns3/object.h"
#include "ns3/assert.h"
#include "ns3/simulator.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "hwmp-rtable.h"

NS_LOG_COMPONENT_DEFINE ("HwmpRtable");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (HwmpRtable);

TypeId
HwmpRtable::GetTypeId(void)
{
	static TypeId tid = TypeId ("ns3::HwmpRtable")
		.SetParent<Object> ()
		.AddConstructor<HwmpRtable> ();
	return tid;

}

HwmpRtable::HwmpRtable()
{
}

HwmpRtable::~HwmpRtable()
{
	DoDispose();
}

void
HwmpRtable::DoDispose()
{
	NS_LOG_UNCOND("RTABLE DISPOSE STARTED");
	m_routes.clear();
	m_roots.clear();
}

void
HwmpRtable::AddReactivePath(
		Mac48Address	destination,
		Mac48Address	retransmitter,
		uint32_t	port,
		uint32_t	metric,
		Time		lifetime,
		uint32_t	seqnum
		)
{
	std::map<Mac48Address, ReactiveRoute, mac48addrComparator>::iterator i = m_routes.find(destination);
	if(i == m_routes.end())
	{
		ReactiveRoute newroute;
		m_routes[destination] = newroute;
	}
	else
	{
		/**
		 * if outport differs from stored, routing info is
		 * actual and metric is worse - we ignore this
		 * information
		 */
		if(
				(i->second.port != port) &&
				(i->second.metric < metric) &&
				/**
				 * The routing info is actual or it
				 * was received from peer
				 */
				((i->second.whenExpire > Simulator::Now())||(i->second.whenExpire == Seconds(0)))
				)
			return;
	}
	i = m_routes.find(destination);
	NS_ASSERT(i != m_routes.end());
	i->second.retransmitter = retransmitter;
	i->second.port = port;
	i->second.metric = metric;
	if(lifetime != Seconds(0))
		i->second.whenExpire = MilliSeconds(Simulator::Now().GetMilliSeconds() + lifetime.GetMilliSeconds());
	else
		/**
		 * Information about peer does not have lifetime
		 */
		i->second.whenExpire = Seconds(0);
	i->second.seqnum = seqnum;
}

void
HwmpRtable::AddProactivePath(
		uint32_t	metric,
		Mac48Address	root,
		Mac48Address	retransmitter,
		uint32_t	port,
		Time		lifetime,
		uint32_t	seqnum
		)
{
	ProactiveRoute newroute;
	m_roots[port] = newroute;
	std::map<uint32_t,ProactiveRoute>::iterator i = m_roots.find(port);
	NS_ASSERT(i != m_roots.end());
	i->second.root = root;
	i->second.retransmitter = retransmitter;
	i->second.metric = metric;
	i->second.whenExpire = MilliSeconds(Simulator::Now().GetMilliSeconds() + lifetime.GetMilliSeconds());
	i->second.seqnum = seqnum;

}

void
HwmpRtable::AddPrecursor(Mac48Address destination, uint32_t port, Mac48Address precursor)
{
	bool should_add = true;
	std::map<Mac48Address, ReactiveRoute, mac48addrComparator>::iterator i = m_routes.find(destination);
	if((i != m_routes.end()) && (i->second.port == port))
	{
		for(unsigned int j = 0 ; j < i->second.precursors.size(); j ++)
			if(i->second.precursors[j] == precursor)
			{
				should_add = false;
				break;
			}
		if(should_add)
			i->second.precursors.push_back(precursor);
	}
	std::map<uint32_t,ProactiveRoute>::iterator k = m_roots.find(port);
	if(k!= m_roots.end())
	{
		for(unsigned int j = 0 ; j < k->second.precursors.size(); j ++)
			if(k->second.precursors[j] == precursor)
				return;
		k->second.precursors.push_back(precursor);
		return;
	}
}

void
HwmpRtable::DeleteProactivePath(uint32_t port)
{
	std::map<uint32_t,ProactiveRoute>::iterator j = m_roots.find(port);
	if(j != m_roots.end())
		m_roots.erase(j);

}
void
HwmpRtable::DeleteProactivePath(Mac48Address root, uint32_t port)
{
	std::map<uint32_t,ProactiveRoute>::iterator j = m_roots.find(port);
	if((j != m_roots.end())&&(j->second.root == root))
		m_roots.erase(j);

}

void
HwmpRtable::DeleteReactivePath(Mac48Address destination, uint32_t port)
{
	std::map<Mac48Address, ReactiveRoute, mac48addrComparator>::iterator i = m_routes.find(destination);
	if(i != m_routes.end())
		if(i->second.port ==  port)
			m_routes.erase(i);
}

HwmpRtable::LookupResult
HwmpRtable::LookupReactive(Mac48Address destination)
{
	LookupResult result;
	result.retransmitter = Mac48Address::GetBroadcast();
	result.metric = MAX_METRIC;
	result.ifIndex = PORT_ANY;
	
	std::map<Mac48Address, ReactiveRoute, mac48addrComparator>::iterator i = m_routes.find(destination);
	if(i == m_routes.end())
		return result;
	result.ifIndex = i->second.port;
	//Seconds(0) means that this is routing
	if(i->second.whenExpire < Simulator::Now())
		if(i->second.retransmitter != destination)
			return result;
	result.retransmitter = i->second.retransmitter;
	result.metric = i->second.metric;
	result.seqnum = i->second.seqnum;
	return result;
}

HwmpRtable::LookupResult
HwmpRtable::LookupProactive(uint32_t port)
{
	LookupResult result;
	result.retransmitter = Mac48Address::GetBroadcast();
	result.metric = MAX_METRIC;
	result.ifIndex = PORT_ANY;
	std::map<uint32_t, ProactiveRoute, mac48addrComparator>::iterator i = m_roots.find(port);
	if(i == m_roots.end())
		return result;
	result.ifIndex = i->first;
	if(i->second.whenExpire < Simulator::Now())
		return result;
	result.retransmitter = i->second.retransmitter;
	result.metric = i->second.metric;
	result.seqnum = i->second.seqnum;
	return result;
}

std::vector<HwmpRtable::FailedDestination>
HwmpRtable::GetUnreachableDestinations(Mac48Address peerAddress, uint32_t port)
{
	std::vector<FailedDestination> retval;
	for(std::map<Mac48Address, ReactiveRoute, mac48addrComparator>::iterator i = m_routes.begin(); i!= m_routes.end(); i++)
		if((i->second.retransmitter == peerAddress)&&(i->second.port == port))
		{
			FailedDestination dst;
			dst.destination = i->first;
			i->second.seqnum ++;
			dst.seqnum = i->second.seqnum;
			retval.push_back(dst);
		}
	/**
	 * Lookup a path to root
	 */
	std::map<uint32_t, ProactiveRoute, mac48addrComparator>::iterator i = m_roots.find(port);
	if((i != m_roots.end())&&(i->second.retransmitter == peerAddress))
	{
		FailedDestination dst;
		dst.destination = i->second.root;
		dst.seqnum = i->second.seqnum;
		retval.push_back(dst);
	}
	return retval;
}
uint32_t
HwmpRtable::RequestSeqnum(Mac48Address destination)
{
	std::map<Mac48Address, ReactiveRoute, mac48addrComparator>::iterator i = m_routes.find(destination);
	if(i == m_routes.end())
		return 0;
	return i->second.seqnum;
}

std::vector<Mac48Address>
HwmpRtable::GetPrecursors(Mac48Address destination, uint32_t port)
{
	std::vector<Mac48Address> retval;
	std::map<uint32_t, ProactiveRoute, mac48addrComparator>::iterator root = m_roots.find(port);
	if((root != m_roots.end()) &&(root->second.root == destination))
	{
		for(unsigned int i = 0; i < root->second.precursors.size(); i ++)
			retval.push_back(root->second.precursors[i]);
	}
	std::map<Mac48Address, ReactiveRoute, mac48addrComparator>::iterator route = m_routes.find(destination);
	if( (route != m_routes.end()) && (route->second.port == port) )
	{
		for(unsigned int i = 0; i < route->second.precursors.size(); i ++)
			retval.push_back(route->second.precursors[i]);
	}
	return retval;
}

}//namespace ns3
