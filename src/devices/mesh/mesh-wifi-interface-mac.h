/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 *          Pavel Boyko <boyko@iitp.ru>
 */

#ifndef MESHWIFIINTERFACEMAC_H_
#define MESHWIFIINTERFACEMAC_H_

#include <stdint.h>
#include <map>
#include "ns3/mac48-address.h"
#include "ns3/mac48-address-comparator.h"
#include "ns3/mgt-headers.h"
#include "ns3/mesh-mgt-headers.h"
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include "ns3/mesh-wifi-beacon-timing-element.h"
#include "ns3/wifi-remote-station-manager.h"
#include "ns3/mesh-wifi-peer-manager.h"
#include "ns3/wifi-mac.h"
#include "ns3/mesh-wifi-interface-mac-plugin.h"

namespace ns3 {

class WifiMacHeader;
class DcaTxop;
class WifiPhy;
class DcfManager;
class MacRxMiddle;
class MacLow;
/**
 * \ingroup mesh
 *
 * \brief Basic MAC of mesh point Wi-Fi interface. Its function is extendable through plugins mechanism.
 * 
 * Now only three output queues are used:
 *  - beacons (PIFS and no backoff),
 *  - background traffic,
 *  - management and priority traffic.
 *  
 */
class MeshWifiInterfaceMac : public WifiMac
{
public:
  /// Never forget to support typeid
  static TypeId  GetTypeId ();
  /// C-tor
  MeshWifiInterfaceMac();
  /// D-tor
  virtual ~MeshWifiInterfaceMac();
  
  ///\name Inherited from WifiMac
  //\{
  virtual void  SetSlot (Time slotTime);
  virtual void  SetSifs (Time sifs);
  virtual void  SetPifs (Time pifs);
  virtual void  SetCtsTimeout (Time ctsTimeout);
  virtual void  SetAckTimeout (Time ackTimeout);
  virtual void  SetEifsNoDifs (Time eifsNoDifs);
  virtual Time  GetSlot () const;
  virtual Time  GetSifs () const;
  virtual Time  GetPifs () const;
  virtual Time  GetCtsTimeout () const;
  virtual Time  GetAckTimeout () const;
  virtual Time  GetEifsNoDifs () const;
  virtual void  SetWifiPhy (Ptr<WifiPhy> phy);
  virtual void  SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> stationManager);
  virtual void  Enqueue (Ptr<const Packet> packet, Mac48Address to, Mac48Address from);
  virtual void  Enqueue (Ptr<const Packet> packet, Mac48Address to);
  virtual bool  SupportsSendFrom () const;
  virtual void  SetForwardUpCallback (Callback<void,Ptr<Packet>, Mac48Address, Mac48Address> upCallback);
  virtual void  SetLinkUpCallback (Callback<void> linkUp);
  virtual void  SetLinkDownCallback (Callback<void> linkDown);
  virtual Mac48Address GetAddress () const;
  virtual Mac48Address GetBssid () const;
  virtual Ssid  GetSsid () const;
  virtual void  SetAddress (Mac48Address address);
  virtual void  SetSsid (Ssid ssid);
  //\}
  
  ///\name Beacons
  //\{

  /// Set interval between two successive beacons
  void SetBeaconInterval (Time interval);
  /// \return interval between two beacons
  Time GetBeaconInterval () const;
  /** 
   * \brief Next beacon frame time
   * 
   * This is supposed to be used by any entity managing beacon collision avoidance (e.g. Peer management protocol in 802.11s)
   */ 
  Time GetTBTT() const;
  /**
   * \brief Shift TBTT.
   * 
   * This is supposed to be used by any entity managing beacon collision avoidance (e.g. Peer management protocol in 802.11s)
   * 
   * \attention User of ShiftTBTT() must take care to not shift it to the past. 
   */
  void ShiftTBTT(Time shift);
  /**
   * \brief Set maximum software delay. Maximum software delay must be smaller than beacon interval.
   *
   * Software delay models random small internal delay to avoid simultaneous operation of 
   * different mesh points (e.g. on power on). 
   * 
   * Software delay is calculated as uniformely distributed random value between zero and
   * given parameter. All management frames are sent after software delay is passed,
   * for example, beacon is formed at software delay before putting it to the queue.
   *
   * \attention The software delay is supposed to be independent from traffic intensity.
   * 
   * \param delay is the maximum software delay.
   */
  void SetSoftwareDelay(Time delay);
  /// \return Maximum software delay
  Time GetSoftwareDelay();

  //\}
  
  ///\name Plugins
  //\{
  /// Install plugin. TODO return unique ID to allow unregister plugins
  void InstallPlugin(Ptr<MeshWifiInterfaceMacPlugin> plugin);
  //\}
  
private:
  /// Frame receive handler
  void  Receive (Ptr<Packet> packet, WifiMacHeader const *hdr);
  /// Forward frame to mesh point
  virtual void ForwardUp (Ptr<Packet> packet, Mac48Address src, Mac48Address dst);
  /// Send frame. Frame is supposed to be tagged by routing information. TODO: clarify this point 
  void  ForwardDown(Ptr<const Packet> packet, Mac48Address from, Mac48Address to);
  /// Calc software delay value uniformely distributed between 0 and m_softwareDealy, see SetSoftwareDelay().
  Time CalcSwDelay();
  /// Send beacon
  void SendBeacon ();
  /// Schedule next beacon
  void ScheduleNextBeacon ();
  /// Enable/disable beacons
  void SetBeaconGeneration (bool enable);
  /// Get current beaconing status
  bool GetBeaconGeneration () const;
  /// Get list of supported bitrates
  SupportedRates GetSupportedRates () const;
  /// Real d-tor
  virtual void DoDispose ();
  
private:
  ///\name Wifi MAC internals
  //\{
  Ptr<DcaTxop>   m_BE;
  Ptr<DcaTxop>   m_BK;
  Ptr<DcaTxop>   m_VI;
  Ptr<DcaTxop>   m_VO;
  Ptr<DcaTxop>   m_beaconDca;
  Ptr<WifiRemoteStationManager> m_stationManager;
  Ptr<WifiPhy>   m_phy;
  Callback<void, Ptr<Packet>, Mac48Address, Mac48Address>  m_upCallback;
  //\}
  
  ///\name Wifi timing intervals
  //\{
  Time m_slot;
  Time m_sifs;
  Time m_pifs;
  Time m_ackTimeout;
  Time m_ctsTimeout;
  Time m_eifsNoDifs;
  //\}
  
  ///\name Mesh timing intervals 
  //\{
  /// Beaconing interval.
  Time m_beaconInterval;
  /// Maximum delay before first beacon
  Time m_randomStart;
  /// Maximum software delay
  Time m_softwareDelay;
  /// Send time of the last management frame, used to ensure correct frame order 
  Time m_lastMgtFrame;
  /// Time for the next frame
  Time m_tbtt;
  //\}
  
  /// DCF implementation
  DcfManager* m_dcfManager;
  /// Middle MAC sublayer
  MacRxMiddle* m_rxMiddle;
  /// Low MAX sublayer
  Ptr<MacLow> m_low;
  /// My address
  Mac48Address m_address;
  /// SSID
  Ssid m_MeshId;
  
  /// "Timer" for the next beacon 
  EventId m_beaconSendEvent;
  
  typedef std::vector< Ptr<MeshWifiInterfaceMacPlugin> > PluginList; 
  /// List of all installed plugins
  PluginList m_plugins;
};

} // namespace ns3



#endif /* MESHWIFIINTERFACEMAC_H_ */
