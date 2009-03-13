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
 *          Evgeny Khorov <horov@frtk.ru>
 */
#ifndef MAC_HIGH_MESH_H
#define MAC_HIGH_MESH_H

#include <stdint.h>
#include <map>
#include "ns3/mac48-address.h"
#include "ns3/mac48-address-comparator.h"
#include "mgt-headers.h"
#include "mesh-mgt-headers.h"
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include "mesh-wifi-beacon-timing-element.h"
#include "wifi-remote-station-manager.h"
#include "mesh-wifi-peer-manager.h"
#include "wifi-mac.h"

namespace ns3 {

	class WifiMacHeader;
	class DcaTxop;
	class WifiPhy;
	class DcfManager;
	class MacRxMiddle;
	class MacLow;
	class WifiPeerManager;
	/**
	 * \ingroup mesh
	 * 
	 * \brief Implements MAC-layer of mesh point: beaconing and
	 * peer link management.
	 *
	 * \details Handles sending and receiving beacons. Tells all
	 * needed information about beacons to MeshWifiPeerManager class, which
	 * handles peer link management state machine. Asks
	 * WifiPeerManager wether
	 * receive or discard frame obtained from neighbour.
	 * Now only three output queues are used: 
	 * one for beacons (PIFS and no backoff),
	 * one for background traffic,
	 * one for management and priority traffic.
	 */
	class MeshWifiMac : public WifiMac
	{
		public:
			static TypeId		GetTypeId ();

			MeshWifiMac ();
			~MeshWifiMac ();
			// inherited from WifiMac.
			virtual void		SetSlot (Time slotTime);
			virtual void		SetSifs (Time sifs);
			virtual void		SetPifs (Time pifs);
  			virtual void		SetCtsTimeout (Time ctsTimeout);
  			virtual void		SetAckTimeout (Time ackTimeout);
			virtual void		SetEifsNoDifs (Time eifsNoDifs);
			virtual Time		GetSlot () const;
			virtual Time		GetSifs () const;
			virtual Time		GetPifs () const;
  			virtual Time		GetCtsTimeout () const;
  			virtual Time		GetAckTimeout () const;
			virtual Time		GetEifsNoDifs () const;
			virtual void		SetWifiPhy (Ptr<WifiPhy> phy);
			virtual void		SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> stationManager);
  			virtual void		Enqueue (Ptr<const Packet> packet, Mac48Address to, Mac48Address from);
			virtual void		Enqueue (Ptr<const Packet> packet, Mac48Address to);
  			virtual bool		SupportsSendFrom () const;
  			virtual void		SetForwardUpCallback (Callback<void,Ptr<Packet>, Mac48Address, Mac48Address> upCallback);
			virtual void		SetLinkUpCallback (Callback<void> linkUp);
			virtual void		SetLinkDownCallback (Callback<void> linkDown);
			virtual	Mac48Address	GetAddress () const;
			virtual Mac48Address	GetBssid () const;
			virtual Ssid		GetSsid () const;
			virtual void		SetAddress (Mac48Address address);
			virtual void		SetSsid (Ssid ssid);
			/**
			 * \param interval is an interval between two
			 * successive beacons
			 */
			void			SetBeaconInterval (Time interval);
			/**
			 * \returns interval between two beacons
			 */
			Time			GetBeaconInterval () const;
			/**
			 * \param delay is the maximum software delay.
			 *
			 * \details Software delay is calculated as uniformely
			 * distributed random value between zero and
			 * given parameter.
			 *
			 * All management frames are sent after software delay is passed,
			 * for example, beacon is formed at software
			 * delay before putting it to the queue.
			 *
			 * \attention The software delay is supposed
			 * to be independent from traffic intensity.
			 *
			 */
			void			SetSoftwareDelay(Time delay);
			/**
			 * \returns software delay parameter
			 */
			Time			GetSoftwareDelay();
			/**
			 * \param manager is pointer to the
			 * WifiPeerManager class, which implements
			 * peer-link management state machine.
			 *
			 * \details The purpose of peer manager is
			 * obtaining all needed information from
			 * beacons and send peer link management
			 * frames.
			 *
			 * This funcion sets all needed callbacks to
			 * a given peer manager
			 */
                        void                    SetPeerLinkManager(Ptr<WifiPeerManager> manager);
			virtual void		SetPreqReceivedCallback(
					Callback<void, WifiPreqInformationElement&, const Mac48Address&, const uint32_t&> cb);
			/**
			 * \brief this callback is set by Hwmp routing
			 * protocol and executed when MAC has received
			 * PREP.
			 * \param cb is a callback to be executed when
			 * receiving PREP.
			 */
			virtual void		SetPrepReceivedCallback(
					Callback<void, WifiPrepInformationElement&, const Mac48Address&, const uint32_t&> cb);
			/**
			 * \brief this callback is set by Hwmp routing
			 * protocol and executed when MAC has received
			 * PERR.
			 * \param cb is a callback to be executed when
			 * receiving PERR.
			 */
			virtual void		SetPerrReceivedCallback(
					Callback<void, WifiPerrInformationElement&, const Mac48Address&> cb);

			/**
			 * \brief this callback is set by Hwmp routing
			 * protocol and executed when MAC has detected
			 * the peer link failure
			 * \param cb is a callback to be executed when
			 * peer failure has ben detected
			 */
			virtual void            SetPeerStatusCallback(
					Callback<void, Mac48Address, bool, uint32_t> cb);
			/**
			 * \brief Sends a PREQ frame.
			 * \param preq is preq information element
			 * formed by protocol. This function just adds
			 * a proper WifiMacHeader
			 * \attention This method is public, because
			 * HWMP makes a callback using this method
			 */
			virtual void		SendPreq(const WifiPreqInformationElement& preq);
			/**
			 * \brief Sends a PREP frame.
			 * \param prep is prep information element
			 * formed by protocol. This function just adds
			 * a proper WifiMacHeader
			 * \param to is an address of retransmitter of
			 * the prep
			 * \attention This method is public, because
			 * HWMP makes a callback using this method
			 */

			virtual void		SendPrep(const WifiPrepInformationElement& prep, const Mac48Address& to);
			/**
			 * \brief Sends a PERR frame.
			 * \param perr is perr information element
			 * formed by protocol. This function just adds
			 * a proper WifiMacHeader.
			 * \param receivers is list of addresses where
			 * to send PERR (unicast PERR case)
			 * \attention This method is public, because
			 * HWMP makes a callback using this method
			 */
			virtual void		SendPerr(const WifiPerrInformationElement& perr, std::vector<Mac48Address> receivers);
			/**
			 * \brief Sends PeerLinkOpen frame to a given
			 * address. Mac only forms a proper
			 * WifiMacHeader.
			 * \param peer_element is peer link frame to
			 * be sent
			 * \param peerAddress is the address of
			 * destination of given frame
			 */
			void			SendPeerLinkOpen(
					PeerLinkManagementElement peer_element,
					Mac48Address peerAddress
					);
			/**
			 * \brief Sends PeerLinkConfirm frame to a given
			 * address. Mac only forms a proper
			 * WifiMacHeader.
			 * \param peer_element is peer link frame to
			 * be sent
			 * \param peerAddress is the address of
			 * destination of given frame
			 * \param aid is the assocciation ID stored in
			 * peer manager
			 */
			void			SendPeerLinkConfirm(
					PeerLinkManagementElement peer_element,
					Mac48Address peerAddress,
					uint16_t aid
					);
			/**
			 * \brief Sends PeerLinkClose frame to a given
			 * address. Mac only forms a proper
			 * WifiMacHeader.
			 * \param peer_element is peer link frame to
			 * be sent
			 * \param peerAddress is the address of
			 * destination of given frame
			 */
			void			SendPeerLinkClose(
					PeerLinkManagementElement peer_element,
					Mac48Address peerAddress
					);
			/**
			 * \brief this method is executed by peer
			 * manager when peer link opened or closed
			 * \param status true when link was opened,
			 * false if link was closed
			 * \param peerAddress is the address of
			 * destination of given frame
			 */
			virtual void		PeerLinkStatus(Mac48Address peerAddress, bool status);

			/**
			 * \brief Peer Manager notifyes MAC about new
			 * peer link or peer link failure.
			 * \details This method should pass this event
			 * to HWMP, and it should generate new routing
			 * information or should generate Path Error
			 * \param peerAddress it the address of
			 * neighbour
			 * \param status If true - new peer link, perr
			 * link failure otherwise
			 */
			void			PeerEvent(
					Mac48Address	peerAddress,
					bool		status
					);
		private:
			void		Receive (Ptr<Packet> packet, WifiMacHeader const *hdr);
			virtual void	ForwardUp (Ptr<Packet> packet, Mac48Address src, Mac48Address dst);
			void		ForwardDown(
					Ptr<const Packet> packet,
					Mac48Address from,
					Mac48Address to
					);
			void		TxOk (WifiMacHeader const &hdr);
			void		TxFailed (WifiMacHeader const &hdr);
			/**
			 * \brief At the software delay before TBTT
			 * SendOneBeacon is ececuted. It forms a
			 * beacon frame body. Then calculates software
			 * delay and schedules QueueOneBeacon
			 */
			void		SendOneBeacon ();
			/**
			 * \brief Puts a formed beacon to the output
			 * queue. SendOneBeacon forms a beacon and
			 * after software delay QueueOneBeacon is
			 * executed
			 * \param beacon_hdr is beacon to be queued
			 */
			void		QueueOneBeacon (MgtMeshBeaconHeader beacon_hdr);
			/**
			 * \brief Executed after software delay by
			 * SendPeerLinkOpen/Confirm/Close frames and
			 * puts this frame to the output queue.
			 * \param peer_frame peer link management
			 * frame
			 * \param peerAddress the address of
			 * destination.
			 */
			void		QueuePeerLinkFrame(MeshMgtPeerLinkManFrame peer_frame, Mac48Address peerAddress);
			/**
			 * \brief Executed after software delay by
			 * SendPreq/Prep/Perr frames and
			 * puts this frame to the output queue.
			 * \param packet is packet body,
			 * \param hdr is WifiMacHeader
			 */
			void		QueuePathSelectionFrame(Ptr<Packet> packet, const WifiMacHeader hdr);
			void		SetBeaconGeneration (bool enable);
			bool		GetBeaconGeneration () const;
			SupportedRates	GetSupportedRates () const;
			virtual void	DoDispose ();

			Ptr<DcaTxop>			m_BE;
			Ptr<DcaTxop>			m_BK;
			Ptr<DcaTxop>			m_VI;
			Ptr<DcaTxop>			m_VO;
			Ptr<DcaTxop>			m_beaconDca;
			Ptr<WifiRemoteStationManager>	m_stationManager;
			Ptr<WifiPhy>			m_phy;
			Callback<void,Ptr<Packet>, Mac48Address, Mac48Address>
							m_upCallback;
			Time				m_beaconInterval;
			Time				m_randomStart;

			DcfManager *			m_dcfManager;
			MacRxMiddle *			m_rxMiddle;
			Ptr<MacLow>			m_low;
			Mac48Address			m_address;
			Ssid				m_MeshId;
			EventId				m_beaconFormEvent;
			EventId				m_beaconSendEvent;
			Time				m_slot;
			Time				m_sifs;
  			Time				m_pifs;
			Time				m_ackTimeout;
			Time				m_ctsTimeout;

			Time				m_eifsNoDifs;
			MeshConfigurationElement	m_meshConfig; //Stores my configuration;
			//Peer Descriptor pointer:
			Ptr<WifiPeerManager> 		m_peerManager;
			Time				m_softwareDelay;
			/**
			 * \attention Software delay is used for management
			 * frames, so, if this delay is calculated
			 * independenly, the packet order may be
			 * broken, so the last mgt frame sending time
			 * should be keeped within MAC
			 */
			Time				m_lastMgtFrame;
			/**
			 * \returns the value of software delay
			 * uniformely distributed between 0 and
			 * m_softwareDealy (initiated by
			 * SetSoftwareDelay method).
			 */
			Time				CalcSwDelay();
			/**
			 * \brief keeps delay for sending first
			 * beacon. this delay is uniformely
			 * distributed between 0 and given value
			 */
			Time				m_beaconFormingRandomDelay;
			Callback<void, WifiPreqInformationElement&, const Mac48Address&, const uint32_t&>
							m_preqReceived;
			Callback<void, WifiPrepInformationElement&, const Mac48Address&, const uint32_t&>
							m_prepReceived;
			Callback<void, WifiPerrInformationElement&, const Mac48Address&>
							m_perrReceived;
			Callback<void, Mac48Address, bool, uint32_t>
							m_peerStatusCallback;
			/**
			 * \brief metric calculation parameters
			 */
			uint32_t			CalculateMetric(Mac48Address peerAddress);
			std::map<Mac48Address, uint32_t, mac48addrComparator>
							m_metricDatabase;
	};

} // namespace ns3


#endif /* MAC_HIGH_MESH_H */
/*@}*/
