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


#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/node.h"
#include "ns3/random-variable.h"

#include "mesh-wifi-mac.h"
#include "dca-txop.h"
#include "wifi-mac-header.h"
#include "mesh-wifi-mac-header.h"
#include "mgt-headers.h"
#include "wifi-phy.h"
#include "dcf-manager.h"
#include "mac-rx-middle.h"
#include "mac-low.h"
#include "ns3/tx-statistics.h"
#include "ns3/hwmp.h"

NS_LOG_COMPONENT_DEFINE ("MeshWifiMac");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (MeshWifiMac);

TypeId
MeshWifiMac::GetTypeId ()
{
	static TypeId tid = TypeId ("ns3::MeshWifiMac")
		.SetParent<WifiMac> ()
		.AddConstructor<MeshWifiMac> ()
		.AddAttribute ("BeaconInterval", "Beacon Interval",
				TimeValue (Seconds (1.0)),
				MakeTimeAccessor (&MeshWifiMac::m_beaconInterval),
				MakeTimeChecker ()
				)
		.AddAttribute ("RandomStart", "Window when beacon generating starts (uniform random) in seconds",
				TimeValue (Seconds (0.1)),
				MakeTimeAccessor (&MeshWifiMac::m_randomStart),
				MakeTimeChecker ()
				)
		.AddAttribute ("SoftwareDelay", "Window of uniformely distributed random software handling delay",
				TimeValue (MicroSeconds (500)),
				MakeTimeAccessor (&MeshWifiMac::m_softwareDelay),
				MakeTimeChecker ()
				)
		.AddAttribute ("BeaconGeneration", "Enable/Disable Beaconing.",
				BooleanValue (true),
				MakeBooleanAccessor (
					&MeshWifiMac::SetBeaconGeneration,
					&MeshWifiMac::GetBeaconGeneration
					),
					MakeBooleanChecker ()
				);
	return tid;
}

MeshWifiMac::MeshWifiMac ()
{
	NS_LOG_FUNCTION (this);
	m_rxMiddle = new MacRxMiddle ();
	m_rxMiddle->SetForwardCallback (MakeCallback (&MeshWifiMac::Receive, this));

	m_low = CreateObject<MacLow> ();
	m_low->SetRxCallback (MakeCallback (&MacRxMiddle::Receive, m_rxMiddle));

	m_dcfManager = new DcfManager ();
	m_dcfManager->SetupLowListener (m_low);

	m_beaconDca = CreateObject<DcaTxop> ();
	m_beaconDca->SetLow (m_low);
	m_beaconDca->SetMinCw (0);
	m_beaconDca->SetMaxCw (0);
	m_beaconDca->SetAifsn(1);
	m_beaconDca->SetManager (m_dcfManager);

	m_VO = CreateObject<DcaTxop> ();
	m_VO->SetLow (m_low);
	m_VO->SetMinCw (3);
	m_VO->SetMaxCw (7);
	m_VO->SetManager (m_dcfManager);
	m_VO->SetTxOkCallback (MakeCallback (&MeshWifiMac::TxOk, this));
	m_VO->SetTxFailedCallback (MakeCallback (&MeshWifiMac::TxFailed, this));

	m_BE = CreateObject<DcaTxop> ();
	m_BE->SetLow (m_low);
	m_BE->SetManager (m_dcfManager);
	m_BE->SetTxOkCallback (MakeCallback (&MeshWifiMac::TxOk, this));
	m_BE->SetTxFailedCallback (MakeCallback (&MeshWifiMac::TxFailed, this));
}

MeshWifiMac::~MeshWifiMac ()
{
	NS_LOG_FUNCTION (this);
}
/*
 * Public Methods:
 */
void 
MeshWifiMac::SetSlot (Time slotTime)
{
	NS_LOG_FUNCTION (this << slotTime);
	m_dcfManager->SetSlot (slotTime);
	m_slot = slotTime;
}

void 
MeshWifiMac::SetSifs (Time sifs) 
{
	NS_LOG_FUNCTION (this << sifs); 
	m_dcfManager->SetSifs (sifs);
	m_sifs = sifs;
}
void 
MeshWifiMac::SetAckTimeout (Time ackTimeout)
{
	m_low->SetAckTimeout (ackTimeout);
}

void 
MeshWifiMac::SetCtsTimeout (Time ctsTimeout)
{
	m_low->SetCtsTimeout (ctsTimeout);
}

void 
MeshWifiMac::SetPifs (Time pifs) 
{
	NS_LOG_FUNCTION (this << pifs); 
	m_pifs = pifs;
}
void
MeshWifiMac::SetEifsNoDifs (Time eifsNoDifs)
{
	NS_LOG_FUNCTION (this << eifsNoDifs);
	m_dcfManager->SetEifsNoDifs (eifsNoDifs);
	m_eifsNoDifs = eifsNoDifs;
}

Time 
MeshWifiMac::GetSlot () const
{
	return m_slot;
}

Time 
MeshWifiMac::GetSifs () const
{
	return m_sifs;
}

Time 
MeshWifiMac::GetEifsNoDifs () const
{
	return m_eifsNoDifs;
}

Time 
MeshWifiMac::GetAckTimeout () const
{
	return m_low->GetAckTimeout ();
}

Time 
MeshWifiMac::GetCtsTimeout () const
{
	return m_low->GetCtsTimeout ();
}

Time 
MeshWifiMac::GetPifs () const
{
	return m_low->GetPifs ();
}

void
MeshWifiMac::SetWifiPhy (Ptr<WifiPhy> phy)
{
	NS_LOG_FUNCTION (this << phy);
	m_phy = phy;
	m_dcfManager->SetupPhyListener (phy);
	m_low->SetPhy (phy);
}

void 
MeshWifiMac::SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> stationManager)
{
	NS_LOG_FUNCTION (this << stationManager);
	m_stationManager = stationManager;
	m_BE->SetWifiRemoteStationManager (stationManager);
	m_VO->SetWifiRemoteStationManager (stationManager);
	m_beaconDca->SetWifiRemoteStationManager (stationManager);
	m_low->SetWifiRemoteStationManager (stationManager);
}

void
MeshWifiMac::SetPeerLinkManager (Ptr<WifiPeerManager> manager)
{
  m_peerManager = manager;
}

void 
MeshWifiMac::Enqueue (Ptr<const Packet> packet, Mac48Address to, Mac48Address from)
{
	NS_LOG_FUNCTION (this << packet << to << from);
	ForwardDown (packet, from, to);
}

void 
MeshWifiMac::Enqueue (Ptr<const Packet> packet, Mac48Address to)
{
	NS_LOG_FUNCTION (this << packet << to);
	ForwardDown (packet, m_low->GetAddress (), to);
}

bool 
MeshWifiMac::SupportsSendFrom () const
{
	return true;
}

void 
MeshWifiMac::SetForwardUpCallback (Callback<void,Ptr<Packet>, Mac48Address, Mac48Address> upCallback)
{
	NS_LOG_FUNCTION (this);
	m_upCallback = upCallback;
}

void 
MeshWifiMac::SetLinkUpCallback (Callback<void> linkUp)
{
	NS_LOG_FUNCTION (this);
	if (!linkUp.IsNull ())
	{
		linkUp ();
	}
}

void 
MeshWifiMac::SetLinkDownCallback (Callback<void> linkDown)
{
	NS_LOG_FUNCTION (this);
}

Mac48Address 
MeshWifiMac::GetAddress () const
{
	return m_address;
}
Mac48Address
MeshWifiMac::GetBssid () const
{
	return m_address;
}

Ssid 
MeshWifiMac::GetSsid () const
{
	return m_MeshId;
}

void 
MeshWifiMac::SetAddress (Mac48Address address)
{
	NS_LOG_FUNCTION (address);
	m_low->SetAddress(address);
	m_address = address;
}

void 
MeshWifiMac::SetSsid (Ssid ssid)
{
	NS_LOG_FUNCTION (ssid);
	m_MeshId = ssid;
}

void 
MeshWifiMac::SetBeaconInterval (Time interval)
{
	NS_LOG_FUNCTION (this << interval);
	m_beaconInterval = interval;
}

void
MeshWifiMac::DoDispose ()
{
	NS_LOG_FUNCTION (this);
	delete m_rxMiddle;
	delete m_dcfManager;
	//Delete smart pointers:
	m_rxMiddle = 0;
	m_low = 0;
	m_dcfManager = 0;
	m_phy = 0;
	m_BE = 0;
	m_VO = 0;
	m_peerManager = 0;
	m_beaconSendEvent.Cancel ();
	m_beaconFormEvent.Cancel ();
	m_beaconDca = 0;
	WifiMac::DoDispose ();
}

void 
MeshWifiMac::ForwardUp (Ptr<Packet> packet, Mac48Address src, Mac48Address dst)
{
	NS_LOG_FUNCTION (this << packet << src);
	m_upCallback (packet, src, dst);
}

void 
MeshWifiMac::ForwardDown (Ptr<const Packet> packet, Mac48Address from, Mac48Address to)
{
	//TODO:Classify and queue
	WifiMacHeader hdr;
	Ptr<Packet> packet_to_send = packet->Copy();
	WifiMeshHeader meshHdr;
	//Address 1 we receive from HWMP tag
	HwmpTag tag;
	NS_ASSERT(packet->FindFirstMatchingTag(tag));
	meshHdr.SetMeshTtl(tag.GetTtl());
	meshHdr.SetMeshSeqno(tag.GetSeqno());
#if 0
	NS_LOG_DEBUG(
			"TX Packet sa = "<<from<<
			", da = "<<to<<
			", ra = "<<tag.GetAddress()<<
			", I am "<<GetAddress()<<
			", ttl = "<<(int)meshHdr.GetMeshTtl()
			);
#endif
	if(to!= Mac48Address::GetBroadcast())
		NS_ASSERT(tag.GetAddress()!=Mac48Address::GetBroadcast());
	hdr.SetTypeData ();
	hdr.SetAddr1 (tag.GetAddress());
	hdr.SetAddr2 (GetAddress ());
	hdr.SetAddr3 (to);
	hdr.SetAddr4(from);
	hdr.SetDsFrom ();
	hdr.SetDsTo ();
	//TODO: classify should be fixed!
	packet_to_send->AddHeader(meshHdr);
	WifiRemoteStation *destination = m_stationManager->Lookup (to);
  
	if (destination->IsBrandNew ())
	{
		// in adhoc mode, we assume that every destination
		// supports all the rates we support.
		for (uint32_t i = 0; i < m_phy->GetNModes (); i++)
		{
			destination->AddSupportedMode (m_phy->GetMode (i));
		}
		destination->RecordDisassociated ();
	}
	m_BE->Queue (packet_to_send, hdr);
}

SupportedRates
MeshWifiMac::GetSupportedRates () const
{
	// send the set of supported rates and make sure that we indicate
	// the Basic Rate set in this set of supported rates.
	SupportedRates rates;
	for (uint32_t i = 0; i < m_phy->GetNModes (); i++)
	{
		WifiMode mode = m_phy->GetMode (i);
		rates.AddSupportedRate (mode.GetDataRate ());
	}
	// set the basic rates
	for (uint32_t j = 0; j < m_stationManager->GetNBasicModes (); j++)
	{
		WifiMode mode = m_stationManager->GetBasicMode (j);
		rates.SetBasicRate (mode.GetDataRate ());
	}
	return rates;
}

void
MeshWifiMac::SendOneBeacon ()
{
	NS_LOG_FUNCTION (this);
	NS_LOG_DEBUG(GetAddress()<<" is sending beacon");
	//Software delay should never be bigger than beacon interval!
	NS_ASSERT(!m_beaconSendEvent.IsRunning());
	Time last_software_delay = m_beaconFormingRandomDelay;
	MgtMeshBeaconHeader beacon;
	beacon.SetSsid (GetSsid ());
	beacon.SetSupportedRates (GetSupportedRates ());
	beacon.SetBeaconIntervalUs (m_beaconInterval.GetMicroSeconds ());
	beacon.SetWifiBeaconTimingElement(m_peerManager->GetWifiBeaconTimingElementForMyBeacon(GetAddress()));
	m_beaconSendEvent = Simulator::Schedule(
			m_beaconFormingRandomDelay,
			&MeshWifiMac::QueueOneBeacon,
			this,
			beacon);
	UniformVariable coefficient(0.0, m_beaconFormingRandomDelay.GetSeconds());
	m_beaconFormingRandomDelay = Seconds(coefficient.GetValue());

        //MBCA functionality, TODO: add the switch off/on MBCA mechanism capability
        //Let's find when we send the next beacon
        Time myNextTBTT = Simulator::Now ()+m_beaconInterval+last_software_delay;
        //The mext function compares my TBTT with neighbor's TBTTs using timing elemnt
        //and returns time shift for the next beacon if future collision is detected
 	Time nextBeaconShift = m_peerManager->GetNextBeaconShift(GetAddress(), myNextTBTT);

	m_beaconFormEvent = Simulator::Schedule (m_beaconInterval-m_beaconFormingRandomDelay+last_software_delay+nextBeaconShift, &MeshWifiMac::SendOneBeacon, this);
}

void
MeshWifiMac::QueueOneBeacon (MgtMeshBeaconHeader beacon_hdr)
{
	WifiMacHeader hdr;
	hdr.SetBeacon ();
	hdr.SetAddr1 (Mac48Address::GetBroadcast ());
	hdr.SetAddr2 (GetAddress ());
	hdr.SetAddr3 (GetAddress ());
	hdr.SetDsNotFrom ();
	hdr.SetDsNotTo ();
	Ptr<Packet> packet = Create<Packet> ();

	packet->AddHeader (beacon_hdr);
	m_beaconDca->Queue(packet, hdr);
	m_peerManager->SetSentBeaconTimers(
			GetAddress(),
			Simulator::Now (),
			MicroSeconds(beacon_hdr.GetBeaconIntervalUs())
			);

}
void
MeshWifiMac::SetBeaconGeneration (bool enable)
{
	NS_LOG_FUNCTION (this << enable);
	if (enable)
	{
		UniformVariable coefficient(0.0, m_randomStart.GetSeconds());
		Time randomStart = Seconds(coefficient.GetValue());
		// Beacons must be sent with a random delay,
		// otherwise, they will all be broken
		m_beaconFormEvent = Simulator::Schedule (randomStart, &MeshWifiMac::SendOneBeacon, this);
	}
	else
		m_beaconFormEvent.Cancel ();
}

bool
MeshWifiMac::GetBeaconGeneration () const
{
	return m_beaconFormEvent.IsRunning ();
}

void
MeshWifiMac::TxOk (WifiMacHeader const &hdr)
{
}

void
MeshWifiMac::TxFailed (WifiMacHeader const &hdr)
{
}

void 
MeshWifiMac::Receive (Ptr<Packet> packet, WifiMacHeader const *hdr)
{
	if (hdr->IsBeacon ()) 
	{
		MgtMeshBeaconHeader beacon;
		Mac48Address from = hdr->GetAddr2();
		packet->RemoveHeader (beacon);
		NS_LOG_DEBUG("Beacon received from "<<hdr->GetAddr2()<<
				" to "<<GetAddress()<<
				" at "<<Simulator::Now ().GetMicroSeconds ()<<
				" microseconds");
#if 0
		NeighboursTimingUnitsList neighbours;
		neighbours = beacon.GetWifiBeaconTimingElement().GetNeighboursTimingElementsList();
		for(NeighboursTimingUnitsList::const_iterator j = neighbours.begin(); j!= neighbours.end(); j++)
			fprintf(
					stderr,
					"neigbours:\nAID=%u, last_beacon=%u ,beacon_interval=%u\n",
					(*j)->GetAID(),
					(*j)->GetLastBeacon(),
					(*j)->GetBeaconInterval()
					);
#endif
		m_peerManager->SetReceivedBeaconTimers(
				GetAddress(),
				from,
				Simulator::Now (),
				MicroSeconds(beacon.GetBeaconIntervalUs()),
				beacon.GetWifiBeaconTimingElement()
				);
		if(!beacon.GetSsid().IsEqual(GetSsid()))
			return;
		SupportedRates rates = beacon.GetSupportedRates ();
		WifiRemoteStation *peerSta = m_stationManager->Lookup (hdr->GetAddr2 ());
		for (uint32_t i = 0; i < m_phy->GetNModes (); i++)
		{
			WifiMode mode = m_phy->GetMode (i);
			if (rates.IsSupportedRate (mode.GetDataRate ()))
			{
				peerSta->AddSupportedMode (mode);
				if (rates.IsBasicRate (mode.GetDataRate ()))
				{
					m_stationManager->AddBasicMode (mode);
				}
			}
		}
		// TODO:Chack MeshConfigurationElement(now is nothing
		// to be checked)
		m_peerManager->AskIfOpenNeeded(GetAddress(), from);
		return;
	}
	if(hdr->IsMultihopAction())
	{
		WifiMeshHeader meshHdr;
		//no mesh header parameters are needed here:
		//TODO: check TTL
		packet->RemoveHeader(meshHdr);
		WifiMeshMultihopActionHeader multihopHdr;
		//parse multihop action header:
		packet->RemoveHeader(multihopHdr);
		WifiMeshMultihopActionHeader::ACTION_VALUE
			actionValue = multihopHdr.GetAction();
		switch(multihopHdr.GetCategory())
		{
			case WifiMeshMultihopActionHeader::MESH_PEER_LINK_MGT:
			{
				Mac48Address peerAddress; 
				MeshMgtPeerLinkManFrame peer_frame;
				if(hdr->GetAddr1 () != GetAddress ())
					return;
				peerAddress = hdr->GetAddr2();
				packet->RemoveHeader (peer_frame);
				if(actionValue.peerLink != WifiMeshMultihopActionHeader::PEER_LINK_CLOSE)
				{
					//check Supported Rates
					SupportedRates rates = peer_frame.GetSupportedRates();
					for (uint32_t i = 0; i < m_stationManager->GetNBasicModes (); i++)
					{
						WifiMode mode = m_stationManager->GetBasicMode (i);
						if (!rates.IsSupportedRate (mode.GetDataRate ()))
						{
							m_peerManager->ConfigurationMismatch(GetAddress(), peerAddress);
							return;
						}
					}
					//Check SSID
					if(!peer_frame.GetMeshId().IsEqual(GetSsid()))
					{
						m_peerManager->ConfigurationMismatch(GetAddress(), peerAddress);
						return;
					}
				}
				switch(actionValue.peerLink)
				{
					case WifiMeshMultihopActionHeader::PEER_LINK_CONFIRM:
						m_peerManager->SetConfirmReceived(
								GetAddress(),
								peerAddress,
								peer_frame.GetAid(),
								peer_frame.GetPeerLinkManagementElement(),
								m_meshConfig
								);
						return;
					case WifiMeshMultihopActionHeader::PEER_LINK_OPEN:
						m_peerManager->SetOpenReceived(
								GetAddress(),
								peerAddress,
								peer_frame.GetPeerLinkManagementElement(),
								m_meshConfig
								);
						return;
					case WifiMeshMultihopActionHeader::PEER_LINK_CLOSE:
						m_peerManager->SetCloseReceived(
								GetAddress(),
								peerAddress,
								peer_frame.GetPeerLinkManagementElement()
								);
						return;
					default:
						return;
			}
				break;
				}
			case WifiMeshMultihopActionHeader::MESH_PATH_SELECTION:
			{
				if(!m_peerManager->IsActiveLink(GetAddress(), hdr->GetAddr2()))
					return;
				switch(actionValue.pathSelection)
				{
					case WifiMeshMultihopActionHeader::PATH_REQUEST:
						{
						WifiPreqInformationElement preq;
						packet->RemoveHeader(preq);
						//TODO:recalculate
						//metric
						m_preqReceived(preq, hdr->GetAddr2(), CalculateMetric(hdr->GetAddr2()));
						return;
						}
					case WifiMeshMultihopActionHeader::PATH_REPLY:
						{
						WifiPrepInformationElement prep;
						packet->RemoveHeader(prep);
						m_prepReceived(prep, hdr->GetAddr2(), CalculateMetric(hdr->GetAddr2()));
						}
						return;
					case WifiMeshMultihopActionHeader::PATH_ERROR:
						{
						WifiPerrInformationElement perr;
						packet->RemoveHeader(perr);
						m_perrReceived(perr, hdr->GetAddr2());
						}
						return;
					case WifiMeshMultihopActionHeader::ROOT_ANNOUNCEMENT:
						return;
				}
			}
			default:
				break;
		}
	}
	if(hdr->IsData())
	{
		NS_ASSERT((hdr->IsFromDs()) && (hdr->IsToDs()));
		NS_ASSERT(hdr->GetAddr4()!=Mac48Address::GetBroadcast());
		//check seqno
		WifiMeshHeader meshHdr;
		packet->RemoveHeader(meshHdr);
		NS_LOG_DEBUG(
				"DATA TA="<< hdr->GetAddr2()<<
				", da="<<hdr->GetAddr3()<<
				", sa="<<hdr->GetAddr4()<<
				", TTL="<<(int)meshHdr.GetMeshTtl());
		HwmpTag tag;
		//mesh header is present within DATA and multihop action frames, so it must be done within MAC
		tag.SetSeqno(meshHdr.GetMeshSeqno());
		tag.SetAddress(hdr->GetAddr2());
		tag.SetTtl(meshHdr.GetMeshTtl());
		//metric should be later
		packet->RemoveAllTags();
		packet->AddTag(tag);
	if(m_peerManager->IsActiveLink(GetAddress(), hdr->GetAddr2()))
		ForwardUp(packet, hdr->GetAddr4(), hdr->GetAddr3());
	}
}

Time
MeshWifiMac::CalcSwDelay()
{
	UniformVariable coefficient(0.0, m_softwareDelay.GetSeconds());
	Time delay = Seconds(coefficient.GetValue());
	if(delay.GetSeconds() + Simulator::Now().GetSeconds() < m_lastMgtFrame.GetSeconds())
		delay = Seconds(m_lastMgtFrame.GetSeconds() - Simulator::Now().GetSeconds());
	m_lastMgtFrame = Seconds(Simulator::Now().GetSeconds()+delay.GetSeconds());
	NS_ASSERT(delay.GetSeconds() >= 0);
	return delay;
}

void
MeshWifiMac::SendPeerLinkOpen(PeerLinkManagementElement peer_element, Mac48Address peerAddress)
{
	MeshMgtPeerLinkManFrame open;
	open.SetOpen();
	open.SetMeshConfigurationElement(m_meshConfig);
	open.SetPeerLinkManagementElement(peer_element);
	Simulator::Schedule(CalcSwDelay() ,&MeshWifiMac::QueuePeerLinkFrame, this, open, peerAddress);
}

void
MeshWifiMac::SendPeerLinkConfirm(PeerLinkManagementElement peer_element, Mac48Address peerAddress, uint16_t aid)
{	
	MeshMgtPeerLinkManFrame confirm;
	confirm.SetConfirm();
	confirm.SetMeshConfigurationElement(m_meshConfig);
	confirm.SetPeerLinkManagementElement(peer_element);
	confirm.SetAid(aid);
	Simulator::Schedule(CalcSwDelay() ,&MeshWifiMac::QueuePeerLinkFrame, this, confirm, peerAddress);

}

void
MeshWifiMac::SendPeerLinkClose(PeerLinkManagementElement peer_element, Mac48Address peerAddress)
{
	MeshMgtPeerLinkManFrame close;
	close.SetClose();
	close.SetMeshConfigurationElement(m_meshConfig);
	close.SetPeerLinkManagementElement(peer_element);
	Simulator::Schedule(CalcSwDelay() ,&MeshWifiMac::QueuePeerLinkFrame, this, close, peerAddress);

}

void
MeshWifiMac::PeerLinkStatus(Mac48Address peerAddress, bool status)
{
	//pass information to HwmpState
	m_peerStatusCallback(peerAddress, status, CalculateMetric(peerAddress));
}
void
MeshWifiMac::QueuePeerLinkFrame(MeshMgtPeerLinkManFrame peer_frame, Mac48Address peerAddress)
{
	Ptr<Packet> packet = Create<Packet> ();
	//Setting peer frame:
	peer_frame.SetSupportedRates(GetSupportedRates ());
	peer_frame.SetQosField(0);
	peer_frame.SetMeshId(GetSsid());
	packet->AddHeader (peer_frame);
	//multihop header:
	WifiMeshMultihopActionHeader multihopHdr;
	if(peer_frame.IsOpen())
	{
		WifiMeshMultihopActionHeader::ACTION_VALUE action;
		action.peerLink = WifiMeshMultihopActionHeader::PEER_LINK_OPEN;
		multihopHdr.SetAction(WifiMeshMultihopActionHeader::MESH_PEER_LINK_MGT, action);
	}
	if(peer_frame.IsConfirm())
	{
		WifiMeshMultihopActionHeader::ACTION_VALUE action;
		action.peerLink = WifiMeshMultihopActionHeader::PEER_LINK_CONFIRM;
		multihopHdr.SetAction(WifiMeshMultihopActionHeader::MESH_PEER_LINK_MGT, action);
	}
	if(peer_frame.IsClose())
	{
		WifiMeshMultihopActionHeader::ACTION_VALUE action;
		action.peerLink = WifiMeshMultihopActionHeader::PEER_LINK_CLOSE;
		multihopHdr.SetAction(WifiMeshMultihopActionHeader::MESH_PEER_LINK_MGT, action);
	}
	packet->AddHeader(multihopHdr);
	//mesh header:
	WifiMeshHeader meshHdr;
	meshHdr.SetMeshTtl(1);
	meshHdr.SetMeshSeqno(0);
	packet->AddHeader(meshHdr);
	//Wifi Mac header:
	WifiMacHeader hdr;
	hdr.SetMultihopAction();
	hdr.SetAddr1 (peerAddress);
	hdr.SetAddr2 (GetAddress ());
	hdr.SetAddr3 (GetAddress ());
	hdr.SetDsNotFrom ();
	hdr.SetDsNotTo ();
	//queue:
	m_VO->Queue(packet,hdr);
}

void
MeshWifiMac::SetSoftwareDelay(Time delay)
{
	m_softwareDelay = delay;
}

Time
MeshWifiMac::GetSoftwareDelay()
{
	return m_softwareDelay;
}

void
MeshWifiMac::SendPreq(const WifiPreqInformationElement& preq)
{
	//Add a PREQ
	Ptr<Packet> packet = Create<Packet> ();
	packet->AddHeader(preq);
	//Add a Action values:
	WifiMeshMultihopActionHeader multihopHdr;
	WifiMeshMultihopActionHeader::ACTION_VALUE action;
	action.pathSelection = WifiMeshMultihopActionHeader::PATH_REQUEST;
	multihopHdr.SetAction(WifiMeshMultihopActionHeader::MESH_PATH_SELECTION, action);
	packet->AddHeader(multihopHdr);
	//Add a mesh Header
	WifiMeshHeader meshHdr;
	//TODO: normal seqno!
	meshHdr.SetMeshTtl(1);
	meshHdr.SetMeshSeqno(0);
	packet->AddHeader(meshHdr);
	//Add a wifi header:
	WifiMacHeader hdr;
	hdr.SetMultihopAction();
	hdr.SetAddr1 (Mac48Address::GetBroadcast ());
	hdr.SetAddr2 (GetAddress ());
	hdr.SetAddr3 (Mac48Address("00:00:00:00:00:00"));
	hdr.SetDsNotFrom ();
	hdr.SetDsNotTo ();
	Simulator::Schedule(CalcSwDelay() ,&MeshWifiMac::QueuePathSelectionFrame, this, packet, hdr);
	//m_VO->Queue(packet,hdr);

}

void
MeshWifiMac::SendPrep(const WifiPrepInformationElement& prep, const Mac48Address& to)
{
	if(!m_peerManager->IsActiveLink(GetAddress(), to))
		return;
	Ptr<Packet> packet = Create<Packet> ();
	packet->AddHeader(prep);
	//Add a Action values:
	WifiMeshMultihopActionHeader multihopHdr;
	WifiMeshMultihopActionHeader::ACTION_VALUE action;
	action.pathSelection = WifiMeshMultihopActionHeader::PATH_REPLY;
	multihopHdr.SetAction(WifiMeshMultihopActionHeader::MESH_PATH_SELECTION, action);
	packet->AddHeader(multihopHdr);
	//Add a mesh Header
	WifiMeshHeader meshHdr;
	//TODO: normal seqno!
	meshHdr.SetMeshTtl(1);
	meshHdr.SetMeshSeqno(0);
	packet->AddHeader(meshHdr);
	//Add a wifi header:
	WifiMacHeader hdr;
	hdr.SetMultihopAction();
	hdr.SetDsNotTo();
	hdr.SetDsNotFrom();
	hdr.SetAddr1(to);
	hdr.SetAddr2(GetAddress());
	hdr.SetAddr3(Mac48Address("00:00:00:00:00:00"));
	//m_VO->Queue(packet,hdr);
	Simulator::Schedule(CalcSwDelay() ,&MeshWifiMac::QueuePathSelectionFrame, this, packet, hdr);
}

void
MeshWifiMac::SendPerr(const WifiPerrInformationElement& perr, std::vector<Mac48Address> receivers)
{
	NS_ASSERT(receivers.size()!=0);
	Ptr<Packet> packet = Create<Packet> ();
	packet->AddHeader(perr);
	//Add a Action values:
	WifiMeshMultihopActionHeader multihopHdr;
	WifiMeshMultihopActionHeader::ACTION_VALUE action;
	action.pathSelection = WifiMeshMultihopActionHeader::PATH_ERROR;
	multihopHdr.SetAction(WifiMeshMultihopActionHeader::MESH_PATH_SELECTION, action);
	packet->AddHeader(multihopHdr);
	//Add a mesh Header
	WifiMeshHeader meshHdr;
	//TODO: normal seqno!
	meshHdr.SetMeshTtl(1);
	meshHdr.SetMeshSeqno(0);
	packet->AddHeader(meshHdr);
	//Add a wifi header:
	WifiMacHeader hdr;
	hdr.SetMultihopAction();
	hdr.SetDsNotTo();
	hdr.SetDsNotFrom();
	hdr.SetAddr2(GetAddress());
	hdr.SetAddr3(Mac48Address("00:00:00:00:00:00"));
	//m_VO->Queue(packet,hdr);
	for(unsigned int i = 0; i < receivers.size(); i ++)
	{
		NS_LOG_DEBUG(GetAddress()<<" is sending PERR to "<<receivers[i]);
		hdr.SetAddr1(receivers[i]);
		Simulator::Schedule(CalcSwDelay() ,&MeshWifiMac::QueuePathSelectionFrame, this, packet, hdr);
	}
}
void
MeshWifiMac::QueuePathSelectionFrame(Ptr<Packet> packet, const WifiMacHeader hdr)
{
	m_VO->Queue(packet, hdr);
}
void
MeshWifiMac::SetPreqReceivedCallback(
		Callback<void, WifiPreqInformationElement&, const Mac48Address&, const uint32_t&> cb)
{
	m_preqReceived = cb;
}

void
MeshWifiMac::SetPrepReceivedCallback(
		Callback<void, WifiPrepInformationElement&, const Mac48Address&, const uint32_t&> cb)
{
	m_prepReceived = cb;
}

void
MeshWifiMac::SetPerrReceivedCallback(
		Callback<void, WifiPerrInformationElement&, const Mac48Address&> cb)
{
	m_perrReceived = cb;
}

void
MeshWifiMac::SetPeerStatusCallback(
		Callback<void, Mac48Address, bool, uint32_t> cb)
{
	m_peerStatusCallback = cb;
}

uint32_t
MeshWifiMac::CalculateMetric(Mac48Address peerAddress)
{
	//suppose Bt == 1000 bytes;
	uint32_t testLength = 1000;
	WifiRemoteStation *peerSta = m_stationManager->Lookup (peerAddress);
	WifiTxStatistics::RATE_LENGTH_STAT stats = peerSta->GetTxStat().statistics;
	Time overhead = m_sifs  + MicroSeconds(32) + MicroSeconds(8);
	uint32_t metric = (uint32_t)((double)overhead.GetNanoSeconds()/102.4);
	uint32_t maxRate = 0;
	uint32_t packet_sent = 0;
	for(WifiTxStatistics::RATE_LENGTH_STAT::iterator lengthPos = stats.begin(); lengthPos !=stats.end(); lengthPos++)
	{
		if(lengthPos->first > testLength)
			continue;
		for(
				WifiTxStatistics::RATE_STAT::iterator ratePos = lengthPos->second.begin();
				ratePos != lengthPos->second.end();
				ratePos ++
				)
		{
#if 0
			NS_LOG_DEBUG("Rate is "<<ratePos->first
					<<": SUCCESS = "<<ratePos->second.packetsAcked
					<<", RRETRY = " <<ratePos->second.packetsRetried
					<<", FAILURE = "<<ratePos->second.packetsFailed);
#endif
			double coefficient = 
				(double)(
						ratePos->second.packetsRetried
						+ ratePos->second.packetsAcked
					)/((double)ratePos->second.packetsAcked);
			uint16_t avgLength = 0;
			if(ratePos->second.packetsAcked == 0)
				avgLength = lengthPos->first;
			else
				avgLength = ratePos->second.bytesAcked/ratePos->second.packetsAcked;
			uint32_t payload = (uint32_t)((double)avgLength / ((double)ratePos->first)*1e9);
			if(packet_sent < ratePos->second.packetsAcked)
			{
				metric = (uint32_t)(coefficient*((double)overhead.GetNanoSeconds() + (double)payload)/10240);
				packet_sent = ratePos->second.packetsAcked;
				maxRate = ratePos->first;
			}
		}
	}

	//NS_LOG_DEBUG("RATE is "<<maxRate<<" metric is "<<metric);
	//peerSta->ResetTxStat();
	NS_ASSERT(metric !=0);
	return metric;
}

} // namespace ns3
