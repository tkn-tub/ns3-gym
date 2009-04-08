/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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


#ifndef MESH_WIFI_MAC_HEADER_H
#define MESH_WIFI_MAC_HEADER_H

#include "ns3/header.h"
#include "ns3/mac48-address.h"

namespace ns3 {
namespace dot11s {
/**
 * \ingroup dot11s
 */
class WifiMeshHeader : public Header //7.1.3.5b
{
public:
  WifiMeshHeader ();
  ~WifiMeshHeader ();
  static TypeId GetTypeId ();
  virtual TypeId GetInstanceTypeId () const;
  virtual void Print (std::ostream &os) const;

  void   SetAddr4 (Mac48Address address);
  void   SetAddr5 (Mac48Address address);
  void   SetAddr6 (Mac48Address address);
  Mac48Address  GetAddr4 () const;
  Mac48Address  GetAddr5 () const;
  Mac48Address  GetAddr6 () const;

  void   SetMeshSeqno (uint32_t seqno);
  uint32_t  GetMeshSeqno () const;

  void   SetMeshTtl (uint8_t TTL);
  uint8_t  GetMeshTtl () const;

  void   SetAddressExt (uint8_t num_of_addresses);
  uint8_t  GetAddressExt () const;

  virtual uint32_t GetSerializedSize () const;
  virtual void  Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
private:
  uint8_t  m_meshFlags;
  uint8_t  m_meshTtl;
  uint32_t m_meshSeqno;
  Mac48Address m_addr4;
  Mac48Address m_addr5;
  Mac48Address m_addr6;
};

/**
 * \ingroup mesh
 */
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
    enum PeerLinkMgtActionValue  peerLink;
    enum LinkMetricActionValue  linkMetrtic;
    enum PathSelectionActionValue  pathSelection;
    enum InterworkActionValue  interwork;
    enum ResourceCoordinationActionValue resourceCoordination;
  } ACTION_VALUE;
  void   SetAction (enum CategoryValue type,ACTION_VALUE action);
  enum CategoryValue GetCategory ();
  ACTION_VALUE  GetAction ();
  static TypeId  GetTypeId ();
  virtual TypeId  GetInstanceTypeId () const;
  virtual void  Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize () const;
  virtual void  Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
private:
  uint8_t m_category;
  uint8_t m_actionValue;
};
} //namespace dot11s
} // namespace ns3
#endif /* MESH_WIFI_MAC_HEADER_H */
