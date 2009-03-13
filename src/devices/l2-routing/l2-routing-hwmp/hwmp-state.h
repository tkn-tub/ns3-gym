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


#ifndef HWMP_STATE_H
#define HWMP_STATE_H
#include <map>
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/hwmp-rtable.h"
#include "ns3/packet.h"
#include "ns3/wifi-net-device.h"
#include "ns3/mesh-wifi-mac.h"
#include "ns3/mesh-wifi-preq-information-element.h"
#include "ns3/mesh-wifi-prep-information-element.h"
#include "ns3/mesh-wifi-perr-information-element.h"
#include "ns3/dot11s-parameters.h"
namespace ns3 {
	/**
	 * \ingroup mesh
	 * 
	 * \brief Handles HWMP state machine at each real interface
	 * 
	 * \details Implements the following:
	 * 	1. Keep it's own DSN,
	 * 	2. Keep PREQ and PREP timers adn send this frames in
	 * 	accordance with HWMP-limitations
	 * 	3. Deliver routing information to Hwmp main class
	 * 	4. Notify about broken peers
	 */
	class HwmpState : public Object {
		public:
			static TypeId GetTypeId();
			HwmpState();
			~HwmpState();

			/**
			 * \brief Interface with HWMP - Hwmp can only
			 * request address and collect routing
			 * information
			 */
			void	SetRequestRouteCallback(
					Callback<HwmpRtable::LookupResult, const Mac48Address&> cb);
			void	SetRequestRootPathCallback(
					Callback<HwmpRtable::LookupResult, uint32_t> cb);

			enum InfoType {
				INFO_PREQ,
				INFO_PREP,
				INFO_PERR,
				INFO_PROACTIVE,
				INFO_NEW_PEER,
				INFO_FAILED_PEER
			};
			typedef struct RoutingInfo{
				Mac48Address	me;
				Mac48Address	destination;
				Mac48Address	source;
				Mac48Address	nextHop;
				Mac48Address	prevHop;
				uint32_t	outPort;
				uint32_t	metric;
				std::vector<Mac48Address>
						failedDestinations;
				uint32_t	dsn;
				Time		lifetime;
				enum InfoType	type;
			} INFO;
			void	SetRoutingInfoCallback(
					Callback<void, INFO> cb
					);
			void	SetRetransmittersOfPerrCallback(
					Callback<std::vector<Mac48Address>, std::vector<HwmpRtable::FailedDestination>, uint32_t> cb);
			void	RequestDestination(Mac48Address dest);
			void	SendPathError(std::vector<HwmpRtable::FailedDestination> destinations);
			void	SetAssociatedIfaceId(uint32_t interface);
			uint32_t
				GetAssociatedIfaceId();
			//Mac interaction:
			void	SetMac(Ptr<MeshWifiMac> mac);
			void	SetSendPreqCallback(
					Callback<void, const WifiPreqInformationElement&> cb);
			void	SetSendPrepCallback(
					Callback<void, const WifiPrepInformationElement&> cb);
			void	SetSendPerrCallback(
					Callback<void, const WifiPerrInformationElement&, std::vector<Mac48Address> > cb);
			void	ReceivePreq(WifiPreqInformationElement&, const Mac48Address& from, const uint32_t& metric);
			void	ReceivePrep(WifiPrepInformationElement&, const Mac48Address& from, const uint32_t& metric);
			void	ReceivePerr(WifiPerrInformationElement&, const Mac48Address& from);
			void	PeerStatus(
					const Mac48Address peerAddress,
					const bool status,
					const uint32_t metric
					);
			//Proactive mode routines:
			bool	SetRoot();
			void	UnSetRoot();
			//external handling:
			void	Disable();
			void	Enable();
			//DEBUG purpose:
			Mac48Address GetAddress();
		private:
			static const uint32_t MAX_PREQ_ID = 0xffffffff;
			static const uint32_t MAX_DSN = 0xffffffff;
			//information about associated port:
			Mac48Address	m_address;
			//index when HWMP state is attached
			uint32_t	m_ifIndex;
			//timers for PREQ and PREP:
			EventId		m_preqTimer;
			void		SendOnePreq();
			std::vector<WifiPreqInformationElement>
					m_preqQueue;
			//true means that we can add a destination to
			//existing PREQ element
			//False means that we must send 
			EventId		m_prepTimer;
			void		SendPrep(
					Mac48Address	dst, //dst is PREQ's originator address
					Mac48Address	src, //src is PREQ's destination address
					Mac48Address	retransmitter,
					uint32_t	initMetric,
					uint32_t	dsn,/* taken form PREQ*/
					uint32_t	originatorDsn, //taken from rtable or as m_myDsn ++;
					uint32_t	lifetime	//taken from PREQ
					);
			std::vector<WifiPreqInformationElement>::iterator
					m_myPreq;
			//HWMP interaction callbacks:
			Callback<void, INFO>
					m_routingInfoCallback;
			Callback<std::vector<Mac48Address>, std::vector<HwmpRtable::FailedDestination>, uint32_t>
					m_retransmittersOfPerrCallback;
			Callback<HwmpRtable::LookupResult, const Mac48Address&>
					m_requestRouteCallback;
			Callback<HwmpRtable::LookupResult, uint32_t>
					m_requestRootPathCallback;
			//Mac interaction callbacks:
			Callback<void, const WifiPreqInformationElement&>
					m_preqCallback;
			Callback<void, const WifiPrepInformationElement&, const Mac48Address&>
					m_prepCallback;
			Callback<void, const WifiPerrInformationElement&, std::vector<Mac48Address> >
					m_perrCallback;
			//HwmpCounters:
			uint32_t	m_preqId;
			uint32_t	m_myDsn;
			//Seqno and metric database
			std::map<Mac48Address, uint32_t, mac48addrComparator>
					m_dsnDatabase;
			std::map<Mac48Address, uint32_t, mac48addrComparator>
					m_preqMetricDatabase;
			//Disable/enable functionality
			bool		m_disabled;
			//Proactive PREQ mechanism:
			EventId		m_proactivePreqTimer;
			void		SendProactivePreq();
			/**
			 * \brief Two PERRs may not be sent closer to
			 * each other than
			 * dot11MeshHWMPperrMinInterval, so, each HWMP
			 * state collects all unreachable destinations
			 * and once in dot11MeshHWMPperrMinInterval
			 * should send PERR, and PERR element should
			 * be cleared
			 */
			WifiPerrInformationElement
					m_myPerr;
			std::vector<Mac48Address>
					m_myPerrReceivers;
			void		AddPerrReceiver(Mac48Address receiver);
			EventId		m_perrTimer;
			void		SendOnePerr();
			//Configurable parameters:
			uint8_t		m_maxTtl;
	};
} //namespace ns3
#endif
