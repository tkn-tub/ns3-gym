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
 */


#ifndef MESH_MGT_HEADERS_H
#define MESH_MGT_HEADERS_H

#include <stdint.h>

#include "ns3/header.h"
#include "ns3/status-code.h"
#include "ns3/dot11s-peer-management-element.h"
#include "ns3/supported-rates.h"
#include "ns3/mesh-configuration-element.h"
#include "ns3/mesh-wifi-preq-information-element.h"
#include "ns3/mesh-wifi-prep-information-element.h"
#include "ns3/mesh-wifi-perr-information-element.h"
#include "ns3/mesh-wifi-rann-information-element.h"
#include "ns3/mesh-configuration-element.h"
#include "ns3/mesh-wifi-beacon-timing-element.h"
#include "ns3/mgt-headers.h"
#include "ns3/ssid.h"

namespace ns3 {
/**
 * \ingroup mesh
 */
class MgtMeshBeaconHeader : public MgtBeaconHeader
{
public:
  void    SetMeshConfigurationElement(MeshConfigurationElement mesh_config);
  void    SetWifiBeaconTimingElement(WifiBeaconTimingElement wifi_timing);
  MeshConfigurationElement GetMeshConfigurationElement();
  WifiBeaconTimingElement  GetWifiBeaconTimingElement();
  virtual uint32_t  GetSerializedSize () const;
  virtual void   Serialize (Buffer::Iterator start) const;
  virtual uint32_t  Deserialize (Buffer::Iterator start);

private:
  MeshConfigurationElement m_meshConfig;
  WifiBeaconTimingElement  m_meshTiming;
};

/**
 * \ingroup mesh
 */
class MeshMgtPeerLinkManFrame : public Header
{
public:
  MeshMgtPeerLinkManFrame ();
  void    SetAid(uint16_t aid);
  void    SetSupportedRates(SupportedRates rates);
  void    SetQosField(uint16_t qos);
  void    SetMeshId(Ssid Id);
  void    SetMeshConfigurationElement(MeshConfigurationElement MeshConf);
  void    SetPeerLinkManagementElement(PeerLinkManagementElement MeshPeerElement);

  uint16_t         GetAid();
  SupportedRates   GetSupportedRates();
  uint16_t         GetQosField();
  Ssid             GetMeshId();
  MeshConfigurationElement  GetMeshConfigurationElement();
  PeerLinkManagementElement GetPeerLinkManagementElement();

  static  TypeId   GetTypeId();
  virtual TypeId   GetInstanceTypeId() const;
  virtual void     Print(std::ostream &os) const;
  virtual uint32_t GetSerializedSize() const;
  virtual void     Serialize(Buffer::Iterator start) const;
  virtual uint32_t Deserialize(Buffer::Iterator start);
  //Subtype defining methods:
  void    SetOpen();
  void    SetConfirm();
  void    SetClose();

  bool    IsOpen();
  bool    IsConfirm();
  bool    IsClose();

private:
  uint8_t   Subtype;
  static const uint8_t MESH_MGT_HEADER_PEER_OPEN = 1;
  static const uint8_t MESH_MGT_HEADER_PEER_CONFIRM = 2;
  static const uint8_t MESH_MGT_HEADER_PEER_CLOSE = 3;
  // Standart is also requires a ReasonCode to be within
  // PeerLinkClose frame format, but it is present within
  // PeerLinkManagementElement, so we did not duplicate
  // it.
  uint16_t   Aid;  //only in Confirm
  SupportedRates   Rates;  //only in Open and Confirm
  uint16_t   QoS;  //only in Open and Confirm
  Ssid    MeshId;  //only in Open and Confirm
  MeshConfigurationElement MeshConfig; //only in Open and Confirm
  PeerLinkManagementElement PeerLinkMan; //in all types of frames
};

}//namespace NS3
#endif

