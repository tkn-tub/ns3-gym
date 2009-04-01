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

#include "ns3/mesh-wifi-interface-mac-plugin.h"
#include "ie-dot11s-perr.h"

namespace ns3 {

class MeshWifiInterfaceMac;

namespace dot11s {

class HwmpProtocol;
class IePreq;
class IePrep;
class IePerr;

/**
 * \ingroup dot11s
 * 
 * \brief Interface MAC plugin for HWMP -- 802.11s routing protocol
 */
class HwmpMacPlugin : public MeshWifiInterfaceMacPlugin
{
public:
  HwmpMacPlugin (uint32_t, Ptr<HwmpProtocol>);
  ~HwmpMacPlugin ();
  ///\name Inherited from MAC plugin
  //\{
  void SetParent (Ptr<MeshWifiInterfaceMac> parent);
  bool Receive (Ptr<Packet> packet, const WifiMacHeader & header);
  bool UpdateOutcomingFrame (Ptr<Packet> packet, WifiMacHeader & header, Mac48Address from, Mac48Address to) const;
  /// Update beacon is empty, because HWMP does not know anything about beacons
  void UpdateBeacon (MeshWifiBeacon & beacon) const {};
  //\}
  
private:
  friend class HwmpProtocol;
  
  ///\name Intercation with HWMP:
  //\{
  void SendPreq(IePreq preq);
  void SendPrep(IePrep prep, Mac48Address receiver);
  void SendOnePerr(IePerr perr, std::vector<Mac48Address> receivers);
  void RequestDestination (Mac48Address dest);
  //\}
  
  /// Sends one PREQ when PreqMinInterval after last PREQ expires (if any PREQ exists in rhe queue)
  void SendOnePreq ();
  void SendPerr ();
private:
  Ptr<MeshWifiInterfaceMac> m_parent;
  uint32_t m_ifIndex;
  Ptr<HwmpProtocol> m_protocol;
  
  ///\name PREQ queue and PREQ timer:
  //\{
  EventId  m_preqTimer;
  std::vector<IePreq>  m_preqQueue;
  std::vector<IePreq>::iterator  m_myPreq;
  //\}
  ///\name PERR timer and stored path error
  //{
  EventId m_perrTimer;
  struct MyPerr {
    IePerr perr;
    std::vector<Mac48Address> receivers;
  };
  MyPerr m_myPerr;
};
} //namespace dot11s
} //namespace ns3
#endif
