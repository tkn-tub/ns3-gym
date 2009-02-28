/* -*-  Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef WIFI_MAC_HEADER_H
#define WIFI_MAC_HEADER_H

#include "ns3/header.h"
#include "ns3/mac48-address.h"
#include "ns3/nstime.h"
#include <stdint.h>

namespace ns3 {

enum WifiMacType_e {
  WIFI_MAC_CTL_RTS = 0,
  WIFI_MAC_CTL_CTS,
  WIFI_MAC_CTL_ACK,
  WIFI_MAC_CTL_BACKREQ,
  WIFI_MAC_CTL_BACKRESP,

  WIFI_MAC_MGT_BEACON,
  WIFI_MAC_MGT_ASSOCIATION_REQUEST,
  WIFI_MAC_MGT_ASSOCIATION_RESPONSE,
  WIFI_MAC_MGT_DISASSOCIATION,
  WIFI_MAC_MGT_REASSOCIATION_REQUEST,
  WIFI_MAC_MGT_REASSOCIATION_RESPONSE,
  WIFI_MAC_MGT_PROBE_REQUEST,
  WIFI_MAC_MGT_PROBE_RESPONSE,
  WIFI_MAC_MGT_AUTHENTICATION,
  WIFI_MAC_MGT_DEAUTHENTICATION,
//Mesh management frames:  
  WIFI_MAC_MGT_MULTIHOP_ACTION,

  WIFI_MAC_DATA,
  WIFI_MAC_DATA_CFACK,
  WIFI_MAC_DATA_CFPOLL,
  WIFI_MAC_DATA_CFACK_CFPOLL,
  WIFI_MAC_DATA_NULL,
  WIFI_MAC_DATA_NULL_CFACK,
  WIFI_MAC_DATA_NULL_CFPOLL,
  WIFI_MAC_DATA_NULL_CFACK_CFPOLL,
  WIFI_MAC_QOSDATA,
  WIFI_MAC_QOSDATA_CFACK,
  WIFI_MAC_QOSDATA_CFPOLL,
  WIFI_MAC_QOSDATA_CFACK_CFPOLL,
  WIFI_MAC_QOSDATA_NULL,
  WIFI_MAC_QOSDATA_NULL_CFPOLL,
  WIFI_MAC_QOSDATA_NULL_CFACK_CFPOLL,
};

class WifiMacHeader : public Header 
{
public:

  WifiMacHeader ();
  ~WifiMacHeader ();

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);


  void SetAssocReq (void);
  void SetAssocResp (void);
  void SetProbeReq (void);
  void SetProbeResp (void);
  void SetBeacon (void);
  void SetTypeData (void);
  void SetMultihopAction();
  void SetDsFrom (void);
  void SetDsNotFrom (void);
  void SetDsTo (void);
  void SetDsNotTo (void);
  void SetAddr1 (Mac48Address address);
  void SetAddr2 (Mac48Address address);
  void SetAddr3 (Mac48Address address);
  void SetAddr4 (Mac48Address address);
  void SetType (enum WifiMacType_e type);
  void SetRawDuration (uint16_t duration);
  void SetDuration (Time duration);
  void SetId (uint16_t id);
  void SetSequenceNumber (uint16_t seq);
  void SetFragmentNumber (uint8_t frag);
  void SetNoMoreFragments (void);
  void SetMoreFragments (void);
  void SetRetry (void);
  void SetNoRetry (void);
  void SetQosTid (uint8_t tid);
  void SetQosTxopLimit (uint8_t txop);

  Mac48Address GetAddr1 (void) const;
  Mac48Address GetAddr2 (void) const;
  Mac48Address GetAddr3 (void) const;
  Mac48Address GetAddr4 (void) const;
  enum WifiMacType_e GetType (void) const;
  bool IsFromDs (void) const;
  bool IsToDs (void) const;
  bool IsData (void) const;
  bool IsQosData (void) const;
  bool IsCtl (void) const;
  bool IsMgt (void) const;
  bool IsCfpoll (void) const;
  bool IsRts (void) const;
  bool IsCts (void) const;
  bool IsAck (void) const;
  bool IsAssocReq (void) const;
  bool IsAssocResp (void) const;
  bool IsReassocReq (void) const;
  bool IsReassocResp (void) const;
  bool IsProbeReq (void) const;
  bool IsProbeResp (void) const;
  bool IsBeacon (void) const;
  bool IsDisassociation (void) const;
  bool IsAuthentication (void) const;
  bool IsDeauthentication (void) const;
  bool IsMultihopAction(void) const;
  uint16_t GetRawDuration (void) const;
  Time GetDuration (void) const;
  uint16_t GetSequenceControl (void) const;
  uint16_t GetSequenceNumber (void) const;
  uint16_t GetFragmentNumber (void) const;
  bool IsRetry (void) const;
  bool IsMoreFragments (void) const;
  bool IsQosBlockAck (void) const;
  bool IsQosNoAck (void) const;
  bool IsQosAck (void) const;
  uint8_t GetQosTid (void) const;
  uint8_t GetQosTxopLimit (void) const;

  uint32_t GetSize (void) const;
  char const *GetTypeString (void) const;


private:
  uint16_t GetFrameControl (void) const;
  uint16_t GetQosControl (void) const;
  void SetFrameControl (uint16_t control);
  void SetSequenceControl (uint16_t seq);
  void SetQosControl (uint16_t qos);
  void PrintFrameControl (std::ostream &os) const;

  uint8_t m_ctrlType;
  uint8_t m_ctrlSubtype;
  uint8_t m_ctrlToDs;
  uint8_t m_ctrlFromDs;
  uint8_t m_ctrlMoreFrag;
  uint8_t m_ctrlRetry;
  uint8_t m_ctrlPwrMgt;
  uint8_t m_ctrlMoreData;
  uint8_t m_ctrlWep;
  uint8_t m_ctrlOrder;
  uint16_t m_duration;
  Mac48Address m_addr1;
  Mac48Address m_addr2;
  Mac48Address m_addr3;
  uint8_t m_seqFrag;
  uint16_t m_seqSeq;
  Mac48Address m_addr4;
  uint8_t m_qosTid;
  uint8_t m_qosEosp;
  uint8_t m_qosAckPolicy;
  uint16_t m_qosStuff;
};
class WifiMeshHeader : public Header //7.1.3.5b
{
	public:
		WifiMeshHeader ();
		~WifiMeshHeader ();
		static TypeId GetTypeId (void);
		virtual TypeId GetInstanceTypeId (void) const;
		virtual void Print (std::ostream &os) const;

		void			SetAddr5 (Mac48Address address);
		void			SetAddr6 (Mac48Address address);
		void			SetAddr7 (Mac48Address address);
		Mac48Address		GetAddr5 ();
		Mac48Address		GetAddr6 ();
		Mac48Address		GetAddr7 ();

		void			SetMeshSeqno (uint32_t seqno);
		uint32_t		GetMeshSeqno ();

		void			SetMeshTtl (uint8_t TTL);
		uint8_t		GetMeshTtl ();

		void			SetAddressExt (uint8_t num_of_addresses);
		uint8_t		GetAddressExt ();

		virtual uint32_t	GetSerializedSize (void) const;
		virtual void		Serialize (Buffer::Iterator start) const;
		virtual uint32_t	Deserialize (Buffer::Iterator start);
	private:
		uint8_t	m_meshFlags;
		uint8_t	m_meshTtl;
		uint32_t	m_meshSeqno;
		Mac48Address	m_addr5;
		Mac48Address	m_addr6;
		Mac48Address	m_addr7;
};
class WifiMeshMultihopActionHeader : public Header //7.2.3.14
{
	//Multichop action frame consists of Mesh header, Action, and
	//the last information. Mesh header is present within all data
	//frames and multihop action frames, so Mesh header is a
	//separate structure. Each MultihopAction frames (frames like
	//PREQ, PREP and other) start form Category field and Action
	//value field, so the Multihop Action Frame should containt
	//three fields: Category, Action Value;
	public:
		WifiMeshMultihopActionHeader ();
		~WifiMeshMultihopActionHeader ();
		enum CategoryValue //table 7-24 staring from 4
		{
			MESH_PEER_LINK_MGT =4,
			MESH_LINK_METRIC,
			MESH_PATH_SELECTION,
			MESH_INTERWORK_ACTION,
			MESH_RESOURCE_COORDINATION,
		};
		enum PeerLinkMgtActionValue
		{
			PEER_LINK_OPEN = 0,
			PEER_LINK_CONFIRM,
			PEER_LINK_CLOSE,
		};
		enum LinkMetricActionValue
		{
			LINK_METRIC_REQUEST = 0,
			LINK_METRIC_REPORT,
		};
		enum PathSelectionActionValue
		{
			PATH_REQUEST = 0,
			PATH_REPLY,
			PATH_ERROR,
			ROOT_ANNOUNCEMENT,
		};
		enum InterworkActionValue
		{
			PORTAL_ANNOUNCEMENT = 0,
		};
		enum ResourceCoordinationActionValue
		{
			CONGESTION_CONTROL_NOTIFICATION = 0,
			MDA_SETUP_REQUEST,
			MDA_SETUP_REPLY,
			MDAOP_ADVERTISMENT_REQUEST,
			MDAOP_ADVERTISMENTS,
			MDAOP_SET_TEARDOWN,
			BEACON_TIMING_REQUEST,
			BEACON_TIMING_RESPONSE,
			TBTT_ADJASTMENT_REQUEST,
			MESH_CHANNEL_SWITCH_ANNOUNCEMENT,
		};
		typedef union
		{
			enum PeerLinkMgtActionValue		peerLink;
			enum LinkMetricActionValue		linkMetrtic;
			enum PathSelectionActionValue		pathSelection;
			enum InterworkActionValue		interwork;
			enum ResourceCoordinationActionValue	resourceCoordination;
		} ACTION_VALUE;
		void			SetAction(enum CategoryValue type,ACTION_VALUE action);
		enum CategoryValue	GetCategory();
		ACTION_VALUE		GetAction();
		static TypeId		GetTypeId (void);
		virtual	TypeId		GetInstanceTypeId (void) const;
		virtual void		Print (std::ostream &os) const;
		virtual uint32_t	GetSerializedSize (void) const;
		virtual void		Serialize (Buffer::Iterator start) const;
		virtual uint32_t	Deserialize (Buffer::Iterator start);
	private:
		uint8_t	m_category;
		uint8_t	m_actionValue;
};
} // namespace ns3



#endif /* WIFI_MAC_HEADER_H */
