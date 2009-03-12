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
 *          Aleksey Kovalenko <kovalenko@iitp.ru>
 *          Ivan Pustogarov <chinsa1@rambler.ru>
 *          Evgeny Khorov <horov@frtk.ru>
 */


#include "mesh-wifi-peer-manager.h"
#include "dot11s-parameters.h"
#include "ns3/simulator.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/random-variable.h"
NS_LOG_COMPONENT_DEFINE ("WifiPeerManager");
namespace ns3 {
/***************************************************
 *	PeerLinkDescriptor
 ***************************************************/
WifiPeerLinkDescriptor::WifiPeerLinkDescriptor():
	m_localLinkId(0),
	m_peerLinkId(0),
	m_state(IDLE),
	m_retryCounter(0),
	m_maxBeaconLoss(3)
{}

	void
WifiPeerLinkDescriptor::SetPeerAddress(Mac48Address macaddr)
{
	m_peerAddress = macaddr;
}

	void
WifiPeerLinkDescriptor::SetLocalAddress(Mac48Address macaddr)
{
	m_localAddress = macaddr;
}

	void
WifiPeerLinkDescriptor::SetLocalLinkId(uint16_t id)
{
	m_localLinkId = id;
}
	void
WifiPeerLinkDescriptor::SetLocalAid(uint16_t aid)
{
	m_assocId = aid;
}
	void
WifiPeerLinkDescriptor::SetBeaconInformation(Time lastBeacon, Time beaconInterval)
{
	m_lastBeacon = lastBeacon;
	m_beaconInterval = beaconInterval;
	m_beaconLossTimer.Cancel();
	Time delay = Seconds(beaconInterval.GetSeconds()*m_maxBeaconLoss);
	NS_ASSERT(delay.GetMicroSeconds()!= 0);
	m_beaconLossTimer = Simulator::Schedule(delay, &WifiPeerLinkDescriptor::BeaconLoss, this);
}

	void
WifiPeerLinkDescriptor::SetMaxBeaconLoss(uint8_t maxBeaconLoss)
{
	m_maxBeaconLoss = maxBeaconLoss;
}

	void
WifiPeerLinkDescriptor::SetLinkStatusCallback(Callback<void, Mac48Address, Mac48Address, bool> cb)
{
	m_linkStatusCallback = cb;
}
	void
WifiPeerLinkDescriptor::BeaconLoss()
{
	StateMachine(CNCL);
}

	void
WifiPeerLinkDescriptor::SetBeaconTimingElement(WifiBeaconTimingElement beaconTiming)
{
	m_beaconTiming = beaconTiming;
}

Mac48Address
WifiPeerLinkDescriptor::GetPeerAddress() const
{
	return m_peerAddress;
}

Mac48Address
WifiPeerLinkDescriptor::GetLocalAddress() const
{
	return m_localAddress;
}


uint16_t
WifiPeerLinkDescriptor::GetLocalAid()const
{
	return m_assocId;
}

Time
WifiPeerLinkDescriptor::GetLastBeacon() const
{
	return m_lastBeacon;
}

Time
WifiPeerLinkDescriptor::GetBeaconInterval() const
{
	return m_beaconInterval;
}
WifiBeaconTimingElement
WifiPeerLinkDescriptor::GetBeaconTimingElement() const
{
	return m_beaconTiming;
}

	void
WifiPeerLinkDescriptor::ClearTimingElement()
{
	m_beaconTiming.ClearTimingElement();
}


void  WifiPeerLinkDescriptor::MLMECancelPeerLink(dot11sReasonCode reason)
{
	StateMachine(CNCL,reason);
}

void  WifiPeerLinkDescriptor::MLMEPassivePeerLinkOpen()
{
	StateMachine(PASOPN);
}

void  WifiPeerLinkDescriptor::MLMEActivePeerLinkOpen()
{
	StateMachine(ACTOPN);
}
void WifiPeerLinkDescriptor::MLMEPeeringRequestReject()
{
	StateMachine(REQ_RJCT, PEER_LINK_CANCELLED);
}
#if 0
void  WifiPeerLinkDescriptor::MLMEBindSecurityAssociation()
{
	StateMachine(BNDSA);
}
#endif
	void
WifiPeerLinkDescriptor::SetMac(Ptr<MeshWifiMac> mac)
{
	m_mac = mac;
}
void WifiPeerLinkDescriptor::PeerLinkClose(uint16_t localLinkId,uint16_t peerLinkId, dot11sReasonCode reason)
{
	if( peerLinkId != 0 && m_localLinkId != peerLinkId)
		return;
	if( m_peerLinkId == 0)
		m_peerLinkId = localLinkId;
	else if ( m_peerLinkId != localLinkId )
		return;
	StateMachine(CLS_ACPT,reason);
}

void WifiPeerLinkDescriptor::PeerLinkOpenAccept(uint16_t localLinkId, MeshConfigurationElement  conf)
{
	if( m_peerLinkId == 0)
		m_peerLinkId = localLinkId;
	m_configuration = conf;
	StateMachine(OPN_ACPT);
}

void WifiPeerLinkDescriptor::PeerLinkOpenReject(uint16_t localLinkId, MeshConfigurationElement  conf,dot11sReasonCode reason)
{
	if( m_peerLinkId == 0)
		m_peerLinkId = localLinkId;
	m_configuration = conf; 
	StateMachine(OPN_RJCT,reason);
}

	void
WifiPeerLinkDescriptor::PeerLinkConfirmAccept(uint16_t localLinkId,uint16_t peerLinkId, uint16_t peerAid, MeshConfigurationElement  conf)
{
	if( m_localLinkId != peerLinkId)
		return;
	if( m_peerLinkId == 0)
		m_peerLinkId = localLinkId;
	else if ( m_peerLinkId != localLinkId )
		return;
	m_configuration = conf;
	m_peerAssocId = peerAid;
	StateMachine(CNF_ACPT);
}

void   WifiPeerLinkDescriptor:: PeerLinkConfirmReject(uint16_t localLinkId, uint16_t peerLinkId, 
		MeshConfigurationElement  conf,dot11sReasonCode reason)
{
	if( m_localLinkId != peerLinkId)
		return;
	if( m_peerLinkId == 0)
		m_peerLinkId = localLinkId;
	else if ( m_peerLinkId != localLinkId )
		return;
	m_configuration = conf;
	StateMachine(CNF_RJCT,reason);
}

bool
WifiPeerLinkDescriptor::LinkIsEstab() const
{
	if(m_state == ESTAB)
		return true;
	return false;
}

bool
WifiPeerLinkDescriptor::LinkIsIdle() const
{
	if(m_state == IDLE)
		return true;
	return false;
}

	void
WifiPeerLinkDescriptor::StateMachine(PeerEvent event,dot11sReasonCode reasoncode)
{
	switch(m_state)
	{
		case IDLE:
			switch(event)
			{
				case PASOPN:
					m_state = LISTEN;
					break;
				case ACTOPN:
					m_state = OPN_SNT;
					SendPeerLinkOpen();
					SetRetryTimer();
					break;
				default:{}
			}
			break;
		case LISTEN:
			switch(event)
			{
				case CNCL:
				case CLS_ACPT:
					m_state=IDLE;
					// TODO Callback MLME-SignalPeerLinkStatus
					break;
				case REQ_RJCT:
					SendPeerLinkClose(reasoncode);
					break;
				case ACTOPN:
					m_state=OPN_SNT;
					SendPeerLinkOpen();
					SetRetryTimer();
					break;
				case OPN_ACPT:
					m_state = OPN_RCVD;
					SendPeerLinkConfirm();
					SendPeerLinkOpen();
					SetRetryTimer();
					break;
				default:{}
			}
			break;
		case OPN_SNT:
			switch(event)
			{
				case TOR1:
					SendPeerLinkOpen();
					m_retryCounter++;
					SetRetryTimer();
					break;
				case CNF_ACPT:
					m_state = CNF_RCVD;
					ClearRetryTimer();
					SetConfirmTimer();
					break;
				case OPN_ACPT:
					m_state = OPN_RCVD;
					SendPeerLinkConfirm();
					break;
				case CLS_ACPT:
					m_state = HOLDING;
					ClearRetryTimer();
					SendPeerLinkClose(MESH_CLOSE_RCVD);
					SetHoldingTimer();
					break;
				case OPN_RJCT:
				case CNF_RJCT:
					m_state = HOLDING;
					ClearRetryTimer();
					SendPeerLinkClose(reasoncode);
					SetHoldingTimer();
					break;
				case TOR2:
					m_state = HOLDING;
					ClearRetryTimer();
					SendPeerLinkClose(MESH_MAX_RETRIES);
					SetHoldingTimer();
					break;
				case CNCL:
					m_state = HOLDING;
					ClearRetryTimer();
					SendPeerLinkClose(PEER_LINK_CANCELLED);
					SetHoldingTimer();
					break;
				default:{}
			}
			break;
		case CNF_RCVD:
			switch(event)
			{
				case CNF_ACPT:
					break;
				case OPN_ACPT:
					m_state = ESTAB;
NS_LOG_DEBUG("I am "<<m_localAddress<<", established link with "<<m_peerAddress<<", at "<<Simulator::Now());
					ClearConfirmTimer();
					SendPeerLinkConfirm();
					m_linkStatusCallback(m_localAddress, m_peerAddress, true);
					// TODO Callback MLME-SignalPeerLinkStatus
					break;
				case CLS_ACPT:
					m_state = HOLDING;
					ClearConfirmTimer();
					SendPeerLinkClose(MESH_CLOSE_RCVD);
					SetHoldingTimer();
					break;
				case CNF_RJCT:
				case OPN_RJCT:
					m_state = HOLDING;
					ClearConfirmTimer();
					SendPeerLinkClose(reasoncode);
					SetHoldingTimer();
					break;
				case CNCL:
					m_state = HOLDING;
					ClearConfirmTimer();
					SendPeerLinkClose(PEER_LINK_CANCELLED);
					SetHoldingTimer();
					break;
				case TOC:
					m_state = HOLDING;
					SendPeerLinkClose(MESH_CONFIRM_TIMEOUT);
					SetHoldingTimer();
					break;
				default:{}
			}
			break;
		case OPN_RCVD:
			switch(event)
			{
				case TOR1:
					SendPeerLinkOpen();
					m_retryCounter++;
					SetRetryTimer();
					break;
				case CNF_ACPT:
NS_LOG_DEBUG("I am "<<m_localAddress<<", established link with "<<m_peerAddress<<", at "<<Simulator::Now());
					m_state = ESTAB;
					ClearRetryTimer();
					m_linkStatusCallback(m_localAddress, m_peerAddress, true);
					// TODO Callback MLME-SignalPeerLinkStatus
					break;
				case CLS_ACPT:
					m_state = HOLDING;
					ClearRetryTimer();
					SendPeerLinkClose(MESH_CLOSE_RCVD);
					SetHoldingTimer();
					break;
				case OPN_RJCT:
				case CNF_RJCT:
					m_state = HOLDING;
					ClearRetryTimer();
					SendPeerLinkClose(reasoncode);
					SetHoldingTimer();
					break;
				case TOR2:
					m_state = HOLDING;
					ClearRetryTimer();
					SendPeerLinkClose(MESH_MAX_RETRIES);
					SetHoldingTimer();
					break;
				case CNCL:
					m_state = HOLDING;
					ClearRetryTimer();
					SendPeerLinkClose(PEER_LINK_CANCELLED);
					SetHoldingTimer();
					break;
				default:{}
			}
			break;
		case ESTAB:
			switch(event)
			{
#if 0
				case BNDSA:
					m_state = ESTAB;
					// TODO Callback MLME-SignalPeerLinkStatus
					// TODO
					break;
#endif
				case OPN_ACPT:
					SendPeerLinkConfirm();
					break;
				case CLS_ACPT:
NS_LOG_DEBUG("I am "<<m_localAddress<<", CLOSED link with "<<m_peerAddress<<", at "<<Simulator::Now()<<" Close received");
					m_state = HOLDING;
					SendPeerLinkClose(MESH_CLOSE_RCVD);
					SetHoldingTimer();
					m_linkStatusCallback(m_localAddress, m_peerAddress, false);
					break;
				case OPN_RJCT:
				case CNF_RJCT:
NS_LOG_DEBUG("I am "<<m_localAddress<<", CLOSED link with "<<m_peerAddress<<", at "<<Simulator::Now()<<" Rejected open or confirm");
					m_state = HOLDING;
					ClearRetryTimer();
					SendPeerLinkClose(reasoncode);
					SetHoldingTimer();
					m_linkStatusCallback(m_localAddress, m_peerAddress, false);
					break;
				case CNCL:
NS_LOG_DEBUG("I am "<<m_localAddress<<", CLOSED link with "<<m_peerAddress<<", at "<<Simulator::Now()<<" Link cancelled");
					m_state = HOLDING;
					SendPeerLinkClose(PEER_LINK_CANCELLED);
					SetHoldingTimer();
					m_linkStatusCallback(m_localAddress, m_peerAddress, false);
					break;
				default:{}
			}
			break;
		case HOLDING:
			switch(event)
			{
				case CLS_ACPT:
					ClearHoldingTimer();
				case TOH:
					m_state = IDLE;
					// TODO Callback MLME-SignalPeerLinkStatus
					break;
				case OPN_ACPT:
				case CNF_ACPT:
					m_state = HOLDING;
					// reason not spec in D2.0
					SendPeerLinkClose(PEER_LINK_CANCELLED);
					break;
				case OPN_RJCT:
				case CNF_RJCT:
					m_state = HOLDING;
					SendPeerLinkClose(reasoncode);
					break;
				default:{}
			}
			break;
	}
}

void WifiPeerLinkDescriptor::ClearRetryTimer()
{
	m_retryTimer.Cancel(); 
}

void WifiPeerLinkDescriptor::ClearConfirmTimer()
{
	m_confirmTimer.Cancel(); 
}

void WifiPeerLinkDescriptor::ClearHoldingTimer()
{
	m_holdingTimer.Cancel(); 
}

void WifiPeerLinkDescriptor::SendPeerLinkClose(dot11sReasonCode reasoncode)
{
	PeerLinkManagementElement peerElement;
	peerElement.SetPeerClose(m_localLinkId, m_peerLinkId, reasoncode);
	m_mac->SendPeerLinkClose(peerElement,m_peerAddress);
}

void WifiPeerLinkDescriptor::SendPeerLinkOpen()
{
	PeerLinkManagementElement peerElement;
	peerElement.SetPeerOpen(m_localLinkId);
	NS_ASSERT(m_mac!=NULL);
	m_mac->SendPeerLinkOpen(peerElement,m_peerAddress);
}

void WifiPeerLinkDescriptor::SendPeerLinkConfirm()
{
	PeerLinkManagementElement peerElement;
	peerElement.SetPeerConfirm(m_localLinkId, m_peerLinkId);
	m_mac->SendPeerLinkConfirm(peerElement,m_peerAddress, m_assocId);
}

void WifiPeerLinkDescriptor::SetHoldingTimer()
{
	m_holdingTimer = Simulator::Schedule(dot11sParameters::dot11MeshHoldingTimeout,&WifiPeerLinkDescriptor::HoldingTimeout,this); 
}

void WifiPeerLinkDescriptor::HoldingTimeout()
{
	StateMachine(TOH); 
}

void WifiPeerLinkDescriptor::SetRetryTimer()
{
	m_retryTimer = Simulator::Schedule(dot11sParameters::dot11MeshRetryTimeout,&WifiPeerLinkDescriptor::RetryTimeout,this); 
}

void WifiPeerLinkDescriptor::RetryTimeout()
{
	if( m_retryCounter < dot11sParameters::dot11MeshMaxRetries)
		StateMachine(TOR1);
	else 
		StateMachine(TOR2);
}

void WifiPeerLinkDescriptor::SetConfirmTimer()
{
	m_confirmTimer = Simulator::Schedule(dot11sParameters::dot11MeshConfirmTimeout,&WifiPeerLinkDescriptor::ConfirmTimeout,this); 
}

void WifiPeerLinkDescriptor::ConfirmTimeout()
{
	StateMachine(TOC); 
}


/***************************************************
 *	PeerManager
 ***************************************************/
NS_OBJECT_ENSURE_REGISTERED (WifiPeerManager);

	TypeId
WifiPeerManager::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::WifiPeerManager")
		.SetParent<Object> ()
		.AddConstructor<WifiPeerManager> ()
		//peerLinkCleanupTimeout. This constant is not specified in Draft 2.0
		.AddAttribute ("PeerLinkCleanupPeriod",
				"PeerLinkCleanupPeriod",
				TimeValue (MilliSeconds (80)),
				MakeTimeAccessor (&WifiPeerManager::m_peerLinkCleanupPeriod),
				MakeTimeChecker ()
			      )
		//MaxBeaconLost. This constant is not specified in Draft 2.0
		.AddAttribute ("MaxBeaconLost", "Max Beacon Lost",
				UintegerValue (3),
				MakeUintegerAccessor (&WifiPeerManager::m_maxBeaconLoss),
				MakeUintegerChecker<uint8_t> ()
			      )
		//maximum number of peer links.
		.AddAttribute ("MaxNumberOfPeerLinks",
				"Maximum number of peer links ",
				UintegerValue (32),
				MakeUintegerAccessor (&WifiPeerManager::m_maxNumberOfPeerLinks),
				MakeUintegerChecker<uint8_t> ()
			      );
	return tid;

}
WifiPeerManager::WifiPeerManager()
{
	m_assocId = 0;
	m_numberOfActivePeers = 0;
	// firs peerLinkId is 1, because 0 means "unknown"
	m_localLinkId = 1;
	m_cleanupEvent = Simulator::Schedule (m_peerLinkCleanupPeriod, &WifiPeerManager::PeerCleanup, this);
}
WifiPeerManager::~WifiPeerManager()
{
	m_cleanupEvent.Cancel();
	//TODO: delete a list of descriptors
	for(
			PeerDescriptorsMap::iterator j = m_peerDescriptors.begin();
			j!= m_peerDescriptors.end();
			j++)
	{
		int to_delete = 0;
		for(std::vector<Ptr<WifiPeerLinkDescriptor> >::iterator i = j->second.begin(); i!= j->second.end(); i++)
		{
			to_delete ++;
			(*i)->ClearTimingElement();
			(*i) = 0;
		}
		for(int i = 0; i < to_delete; i ++)
			j->second.pop_back();
		j->second.clear();
	}
	m_peerDescriptors.clear();

}
void
WifiPeerManager::SetSentBeaconTimers(
		Mac48Address portAddress, 
		Time ReferenceTbtt,
		Time BeaconInterval
		)
{
	BeaconInfoMap::iterator myBeacon = m_myBeaconInfo.find(portAddress);
	NS_ASSERT(myBeacon != m_myBeaconInfo.end());
	myBeacon->second.referenceTbtt = ReferenceTbtt;
	myBeacon->second.beaconInterval = BeaconInterval;

}

void
WifiPeerManager::SetReceivedBeaconTimers(
		Mac48Address portAddress, 
		Mac48Address peerAddress,
		Time lastBeacon,
		Time beaconInterval,
		WifiBeaconTimingElement beaconTiming
		)
{
	PeerDescriptorsMap::iterator port = m_peerDescriptors.find(portAddress);
	NS_ASSERT(port!=m_peerDescriptors.end());
	for(std::vector<Ptr<WifiPeerLinkDescriptor> >::iterator i = port->second.begin(); i!= port->second.end(); i++)
	{
		if((*i)->GetPeerAddress() == peerAddress)
		{
			(*i)->SetBeaconTimingElement(beaconTiming);
			(*i)->SetBeaconInformation(lastBeacon, beaconInterval);
			return;
		}
	}
	Ptr<WifiPeerLinkDescriptor> new_descriptor =
		AddDescriptor(portAddress, peerAddress, Simulator::Now(), beaconInterval);
	new_descriptor->SetBeaconTimingElement(beaconTiming);
}

	bool
WifiPeerManager::AttachPorts(std::vector<Ptr<WifiNetDevice> > ports)
{
	NS_ASSERT(ports.size()!=0);
	for(std::vector<Ptr<WifiNetDevice> >::iterator i = ports.begin(); i != ports.end(); i++)
	{
		MeshWifiMac * meshWifiMac = dynamic_cast<MeshWifiMac *> (PeekPointer ((*i)->GetMac()));
		if(meshWifiMac == NULL)
			return false;
		meshWifiMac->SetPeerLinkManager(this);
		//Add a mac pointer:
		m_macPointers[meshWifiMac->GetAddress()] = meshWifiMac;
		//Add descriptor array:
		std::vector<Ptr<WifiPeerLinkDescriptor> > descriptors;
		m_peerDescriptors[meshWifiMac->GetAddress()] = descriptors;
		//Add beacon timers:
		struct BeaconInfo myBeacon;
		m_myBeaconInfo[meshWifiMac->GetAddress()] = myBeacon;
	}
	return true;
}
	void
WifiPeerManager::AskIfOpenNeeded(Mac48Address portAddress, Mac48Address peerAddress)
{
	PeerDescriptorsMap::iterator port = m_peerDescriptors.find(portAddress);
	NS_ASSERT(port!=m_peerDescriptors.end());
	for(std::vector<Ptr<WifiPeerLinkDescriptor> >::iterator i = port->second.begin(); i!= port->second.end(); i++)
		if((*i)->GetPeerAddress() == peerAddress)
		{
			if(ShouldSendOpen(portAddress, peerAddress))
				(*i)->MLMEActivePeerLinkOpen();
			break;
		}
}

void
WifiPeerManager::SetOpenReceived(
		Mac48Address portAddress,
		Mac48Address peerAddress,
		PeerLinkManagementElement peerMan,
		MeshConfigurationElement conf
		)
{
	dot11sReasonCode reasonCode;
	if(!ShouldAcceptOpen(portAddress, peerAddress,reasonCode))
		return;
	PeerDescriptorsMap::iterator port = m_peerDescriptors.find(portAddress);
	NS_ASSERT(port!=m_peerDescriptors.end());
	for(std::vector<Ptr<WifiPeerLinkDescriptor> >::iterator i = port->second.begin(); i!= port->second.end(); i++)
		if((*i)->GetPeerAddress() == peerAddress)
		{
			(*i)->PeerLinkOpenAccept(peerMan.GetLocalLinkId(), conf);
			return;
		}
	 BeaconInfoMap::iterator myBeacon =  m_myBeaconInfo.find(portAddress);
	 NS_ASSERT(myBeacon!=m_myBeaconInfo.end());
	Ptr<WifiPeerLinkDescriptor>new_descriptor = AddDescriptor(
			portAddress,
			peerAddress,
			Simulator::Now(),
			myBeacon->second.beaconInterval
			);
	new_descriptor->PeerLinkOpenAccept(peerMan.GetLocalLinkId(), conf);
}
void
WifiPeerManager::SetConfirmReceived(
		Mac48Address portAddress,
		Mac48Address peerAddress,
		uint16_t peerAid,
		PeerLinkManagementElement peerMan,
		MeshConfigurationElement meshConfig
		)
{
	PeerDescriptorsMap::iterator port = m_peerDescriptors.find(portAddress);
	NS_ASSERT(port!= m_peerDescriptors.end());
	for(std::vector<Ptr<WifiPeerLinkDescriptor> >::iterator i = port->second.begin(); i!= port->second.end(); i++)
		if((*i)->GetPeerAddress() == peerAddress)
			(*i)->PeerLinkConfirmAccept(peerMan.GetLocalLinkId(), peerMan.GetPeerLinkId(), peerAid, meshConfig);
}

void
WifiPeerManager::SetCloseReceived(
		Mac48Address portAddress,
		Mac48Address peerAddress,
		PeerLinkManagementElement peerMan
		)
{
	PeerDescriptorsMap::iterator port = m_peerDescriptors.find(portAddress);
	NS_ASSERT(port!= m_peerDescriptors.end());
	for(std::vector<Ptr<WifiPeerLinkDescriptor> >::iterator i = port->second.begin(); i!= port->second.end(); i++)
		if((*i)->GetPeerAddress() == peerAddress)
		{
			(*i)->PeerLinkClose(peerMan.GetLocalLinkId(), peerMan.GetPeerLinkId(), peerMan.GetReasonCode());
			return;
		}
}

void
WifiPeerManager::ConfigurationMismatch(
		Mac48Address portAddress,
		Mac48Address peerAddress
		)
{
	PeerDescriptorsMap::iterator port = m_peerDescriptors.find(portAddress);
	NS_ASSERT(port!= m_peerDescriptors.end());
	for(std::vector<Ptr<WifiPeerLinkDescriptor> >::iterator i = port->second.begin(); i!= port->second.end(); i++)
		if((*i)->GetPeerAddress() == peerAddress)
		{
			(*i)->MLMECancelPeerLink(MESH_CONFIGURATION_POLICY_VIOLATION);
			return;
		}

}

	WifiBeaconTimingElement
WifiPeerManager::GetWifiBeaconTimingElementForMyBeacon(Mac48Address portAddress)
{
	PeerDescriptorsMap::iterator port = m_peerDescriptors.find(portAddress);
	NS_ASSERT(port!= m_peerDescriptors.end());
	WifiBeaconTimingElement return_val;
	for(std::vector<Ptr<WifiPeerLinkDescriptor> >::iterator i = port->second.begin(); i!= port->second.end(); i++)
	{
		//Just go through all neighbor entries and add it to timing element:
		return_val.AddNeighboursTimingElementUnit(
				(*i)->GetLocalAid(),
				(*i)->GetLastBeacon(),
				(*i)->GetBeaconInterval()
				);
	}
	return return_val;

}
	WifiBeaconTimingElement
WifiPeerManager::GetWifiBeaconTimingElementForAddress(
		Mac48Address portAddress,
		Mac48Address addr)
{
	PeerDescriptorsMap::iterator port = m_peerDescriptors.find(portAddress);
	NS_ASSERT(port!= m_peerDescriptors.end());
	WifiBeaconTimingElement return_val;
	for(std::vector<Ptr<WifiPeerLinkDescriptor> >::iterator i = port->second.begin(); i!= port->second.end(); i++)
		if((*i)->GetPeerAddress() == addr)
			return_val =  (*i)->GetBeaconTimingElement();
	return return_val;


}
Ptr<WifiPeerLinkDescriptor>
WifiPeerManager::AddDescriptor(
		Mac48Address portAddress,
		Mac48Address peerAddress,
		Time lastBeacon,
		Time beaconInterval)
{
	Ptr<WifiPeerLinkDescriptor> new_descriptor = Create<WifiPeerLinkDescriptor>();
	if(m_assocId ==0xff)
		m_assocId = 0;
	if(m_localLinkId == 0xff)
		m_localLinkId = 0;
	new_descriptor->SetLocalAid(m_assocId++);
	new_descriptor->SetLocalLinkId(m_localLinkId++);
	new_descriptor->SetPeerAddress(peerAddress);
	new_descriptor->SetBeaconInformation(lastBeacon, beaconInterval);
	//DEBUG ONLY:
	new_descriptor->SetLocalAddress(portAddress);
	//check if port address is wrong
	MeshMacMap::iterator pos = m_macPointers.find(portAddress);
	NS_ASSERT(pos!= m_macPointers.end());
	//check if descriptors array exist
	PeerDescriptorsMap::iterator port = m_peerDescriptors.find(portAddress);
	NS_ASSERT(port !=m_peerDescriptors.end());
	new_descriptor->SetMac(pos->second);
	new_descriptor->SetMaxBeaconLoss(m_maxBeaconLoss);
	new_descriptor->SetLinkStatusCallback(MakeCallback(&WifiPeerManager::PeerLinkStatus, this));
	NS_ASSERT(port != m_peerDescriptors.end());
	m_peerDescriptors[portAddress].push_back(new_descriptor);
	return new_descriptor;
}

	void
WifiPeerManager::PeerCleanup()
{
	for(
		PeerDescriptorsMap::iterator j = m_peerDescriptors.begin();
		j!= m_peerDescriptors.end();
		j++)
	{
		std::vector<unsigned int> to_erase;
		for(unsigned int i = 0; i< j->second.size(); i++)
			if(j->second[i]->LinkIsIdle())
			{
				j->second[i]->ClearTimingElement();
				j->second[i] = 0;
				to_erase.push_back(i);
			}
		if(to_erase.size() == 0)
			return;
		for(unsigned int i = to_erase.size()-1 ; i >= 0; i--)
			j->second.erase(j->second.begin() + to_erase[i]);
		to_erase.clear();
	}
	m_cleanupEvent = Simulator::Schedule (m_peerLinkCleanupPeriod, &WifiPeerManager::PeerCleanup, this);
}

	std::vector<Mac48Address>
WifiPeerManager::GetNeighbourAddressList(Mac48Address portAddress, Mac48Address peerAddress)
{
	PeerDescriptorsMap::iterator port = m_peerDescriptors.find(portAddress);
	NS_ASSERT(port!= m_peerDescriptors.end());
	std::vector<Mac48Address> return_value;
	for(std::vector<Ptr<WifiPeerLinkDescriptor> >::iterator i = port->second.begin(); i!= port->second.end(); i++)
		return_value.push_back((*i)->GetPeerAddress());
	return return_value;
}

	bool
WifiPeerManager::IsActiveLink(Mac48Address portAddress, Mac48Address peerAddress)
{
	PeerDescriptorsMap::iterator port = m_peerDescriptors.find(portAddress);
	NS_ASSERT(port!= m_peerDescriptors.end());
	for(std::vector<Ptr<WifiPeerLinkDescriptor> >::iterator i = port->second.begin(); i!= port->second.end(); i++)
		if((*i)->GetPeerAddress() == peerAddress)
			return ((*i)->LinkIsEstab());
	return false;
}

	bool
WifiPeerManager::ShouldSendOpen(Mac48Address portAddress, Mac48Address peerAddress)
{
	if(m_numberOfActivePeers > m_maxNumberOfPeerLinks)
		return false;
	return true;
}

	bool
WifiPeerManager::ShouldAcceptOpen(Mac48Address portAddress, Mac48Address peerAddress,dot11sReasonCode & reasonCode)
{
	if(m_numberOfActivePeers > m_maxNumberOfPeerLinks)
	{
		reasonCode = MESH_MAX_PEERS;
		return false;
	}
	return true;
}

	Time
WifiPeerManager::GetNextBeaconShift(
		Mac48Address portAddress,
		Time myNextTBTT
		)
{
	//REMINDER:: in timing element  1) last beacon reception time is measured in units of 256 microseconds
	//                              2) beacon interval is mesured in units of 1024 microseconds
	//                              3) hereafter TU = 1024 microseconds
	//Im my MAC everything is stored in MicroSeconds

	uint32_t myNextTBTTInTimeUnits = 0;
	uint32_t futureBeaconInTimeUnits = 0;
	//Going through all my timing elements and detecting future beacon collisions
	PeerDescriptorsMap::iterator port = m_peerDescriptors.find(portAddress);
	NS_ASSERT(port!= m_peerDescriptors.end());
	BeaconInfoMap::iterator myBeacon = m_myBeaconInfo.find(portAddress);
	NS_ASSERT(myBeacon!=m_myBeaconInfo.end());
	for(std::vector<Ptr<WifiPeerLinkDescriptor> >::iterator i = port->second.begin(); i!= port->second.end(); i++)
	{
		NeighboursTimingUnitsList neighbours;
		neighbours = (*i)->GetBeaconTimingElement().GetNeighboursTimingElementsList();
		//first let's form the list of all kown TBTTs
		for(NeighboursTimingUnitsList::const_iterator j = neighbours.begin(); j!= neighbours.end(); j++)
		{
			uint16_t beaconIntervalTimeUnits;
			beaconIntervalTimeUnits = (*j)->GetBeaconInterval();

			//The last beacon time in timing elememt in Time Units
			uint32_t lastBeaconInTimeUnits;
			lastBeaconInTimeUnits = (*j)->GetLastBeacon()/4;

			//The time of my next beacon sending in Time Units
			myNextTBTTInTimeUnits = myNextTBTT.GetMicroSeconds()/1024;

			//My beacon interval in Time Units
			uint32_t myBeaconIntervalInTimeUnits;
			myBeaconIntervalInTimeUnits = myBeacon->second.beaconInterval.GetMicroSeconds()/1024;

			//The time the beacon of other station will be sent
			//we need the time just after my next TBTT (or equal to my TBTT)
			futureBeaconInTimeUnits = lastBeaconInTimeUnits + beaconIntervalTimeUnits;

			//We apply MBAC only if beacon Intervals are equal
			if (beaconIntervalTimeUnits == myBeaconIntervalInTimeUnits)
			{
				//We know when the neighbor STA transmitted it's beacon
				//Now we need to know when it's going to send it's beacon in the future
				//So let's use the valuse of it's beacon interval
				while (myNextTBTTInTimeUnits >= futureBeaconInTimeUnits)
					futureBeaconInTimeUnits = futureBeaconInTimeUnits + beaconIntervalTimeUnits;
				//If we found that my TBTT coincide with another STA's TBTT
				//break all cylce and return time shift for my next TBTT
				if (myNextTBTTInTimeUnits == futureBeaconInTimeUnits)
					break;
			}

		}
		if (myNextTBTTInTimeUnits == futureBeaconInTimeUnits)
			break;
	}

	//TBTTs coincide, so let's calculate the shift
	if (myNextTBTTInTimeUnits == futureBeaconInTimeUnits) {
		NS_LOG_DEBUG("MBCA: Future beacon collision is detected, applying avoidance mechanism");
		UniformVariable randomSign(-1, 1);
		int coefficientSign = -1;
		if (randomSign.GetValue() >= 0)
			coefficientSign = 1;
		UniformVariable randomShift(1, 15);
		//So, the shift is a random integer variable uniformly distributed in [-15;-1] U [1;15]
		int beaconShift = randomShift.GetInteger(1,15) * coefficientSign;
		NS_LOG_DEBUG("Shift value = " << beaconShift << " beacon TUs");
		//We need the result not in Time Units, but in microseconds
		return MicroSeconds(beaconShift * 1024);
	}
	//No collision detecterf, hence no shift is needed
	else
		return MicroSeconds(0);
}

void
WifiPeerManager::PeerLinkStatus(Mac48Address portAddress, Mac48Address peerAddress, bool status)
{
	MeshMacMap::iterator pos = m_macPointers.find(portAddress);
	NS_ASSERT(pos!= m_macPointers.end());
	pos->second->PeerLinkStatus(peerAddress, status);
}
} //namespace NS3
