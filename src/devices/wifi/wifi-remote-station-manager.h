/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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
#ifndef WIFI_REMOTE_STATION_MANAGER_H
#define WIFI_REMOTE_STATION_MANAGER_H

#include <vector>
#include <utility>
#include "ns3/mac48-address.h"
#include "ns3/packet.h"
#include "ns3/object.h"
#include "ns3/traced-value.h"
#include "wifi-mode.h"

namespace ns3 {

class WifiRemoteStation;
class NonUnicastWifiRemoteStation;
class WifiPhy;

/**
 * \brief hold per-remote-station state.
 *
 * The state in this class is used to keep track
 * of association status if we are in an infrastructure
 * network and to perform the selection of tx parameters
 * on a per-packet basis.
 */
class WifiRemoteStationManager : public Object
{
private:
  typedef std::vector<WifiMode> BasicModes;
public:
  typedef BasicModes::const_iterator BasicModesIterator;

  static TypeId GetTypeId (void);

  WifiRemoteStationManager ();
  virtual ~WifiRemoteStationManager ();

  virtual void SetupPhy (Ptr<WifiPhy> phy);

  uint32_t GetMaxSsrc (void) const;
  uint32_t GetMaxSlrc (void) const;
  uint32_t GetRtsCtsThreshold (void) const;
  uint32_t GetFragmentationThreshold (void) const;
  void SetMaxSsrc (uint32_t maxSsrc);
  void SetMaxSlrc (uint32_t maxSlrc);
  void SetRtsCtsThreshold (uint32_t threshold);
  void SetFragmentationThreshold (uint32_t threshold);
  
  // Invoked in a STA upon dis-association
  // or in an AP upon reboot
  void Reset (void);
  // Invoked in a STA upon association to store
  // the set of rates which belong to the 
  // BSSBasicRateSet of the associated AP
  // and which are supported locally.
  // Invoked in an AP to configure the BSSBasicRateSet
  void AddBasicMode (WifiMode mode);

  WifiMode GetDefaultMode (void) const;
  uint32_t GetNBasicModes (void) const;
  WifiMode GetBasicMode (uint32_t i) const;
  BasicModesIterator BeginBasicModes (void) const;
  BasicModesIterator EndBasicModes (void) const;

  bool IsLowLatency (void) const;

  WifiRemoteStation *Lookup (Mac48Address address);
  WifiRemoteStation *LookupNonUnicast (void);
protected:
  friend class WifiRemoteStation;
  virtual void DoDispose (void);
private:
  typedef std::vector <std::pair<Mac48Address, WifiRemoteStation *> > Stations;
  virtual class WifiRemoteStation *CreateStation (void) = 0;
  Stations m_stations;
  WifiMode m_defaultTxMode;
  NonUnicastWifiRemoteStation *m_nonUnicast;
  BasicModes m_basicModes;
  bool m_isLowLatency;
  uint32_t m_maxSsrc;
  uint32_t m_maxSlrc;
  uint32_t m_rtsCtsThreshold;
  uint32_t m_fragmentationThreshold;
};

} // namespace ns3

namespace ns3 {

class WifiRemoteStation {
public:
  
  static TypeId GetTypeId (void);
  
  WifiRemoteStation ();
  virtual ~WifiRemoteStation ();

  // Invoked in an AP upon disassociation of a
  // specific STA.
  void Reset (void);
  // Invoked in a STA or AP to store the set of 
  // modes supported by a destination which is
  // also supported locally.
  // The set of supported modes includes
  // the BSSBasicRateSet.
  void AddSupportedMode (WifiMode mode);

  bool IsBrandNew (void) const;
  bool IsAssociated (void) const;
  bool IsWaitAssocTxOk (void) const;
  void RecordWaitAssocTxOk (void);
  void RecordGotAssocTxOk (void);
  void RecordGotAssocTxFailed (void);
  void RecordDisassociated (void);

  void PrepareForQueue (Ptr<const Packet> packet, uint32_t fullPacketSize);
  WifiMode GetDataMode (Ptr<const Packet> packet, uint32_t fullPacketSize);
  WifiMode GetRtsMode (Ptr<const Packet> packet);
  // transmission-related methods
  void ReportRtsFailed (void);
  void ReportDataFailed (void);
  void ReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr);
  void ReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr);
  void ReportFinalRtsFailed (void);
  void ReportFinalDataFailed (void);

  // reception-related method
  void ReportRxOk (double rxSnr, WifiMode txMode);

  virtual bool NeedRts (Ptr<const Packet> packet);
  virtual bool NeedRtsRetransmission (Ptr<const Packet> packet);
  virtual bool NeedDataRetransmission (Ptr<const Packet> packet);

  virtual bool NeedFragmentation (Ptr<const Packet> packet);
  virtual uint32_t GetNFragments (Ptr<const Packet> packet);
  virtual uint32_t GetFragmentSize (Ptr<const Packet> packet, uint32_t fragmentNumber);
  virtual bool IsLastFragment (Ptr<const Packet> packet, uint32_t fragmentNumber);

  WifiMode GetCtsMode (WifiMode rtsMode);
  WifiMode GetAckMode (WifiMode dataMode);

private:
  typedef std::vector<WifiMode> SupportedModes;
  virtual Ptr<WifiRemoteStationManager> GetManager (void) const = 0;
  virtual WifiMode DoGetDataMode (uint32_t size) = 0;
  virtual WifiMode DoGetRtsMode (void) = 0;
protected:
  virtual void DoReportRtsFailed (void) = 0;
  virtual void DoReportDataFailed (void) = 0;
  virtual void DoReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr) = 0;
  virtual void DoReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr) = 0;
  virtual void DoReportFinalRtsFailed (void) = 0;
  virtual void DoReportFinalDataFailed (void) = 0;
  virtual void DoReportRxOk (double rxSnr, WifiMode txMode) = 0;
  uint32_t GetNSupportedModes (void) const;
  WifiMode GetSupportedMode (uint32_t i) const;
private:
  bool IsIn (WifiMode mode) const;
  WifiMode GetControlAnswerMode (WifiMode reqMode);
  enum {
    BRAND_NEW,
    DISASSOC,
    WAIT_ASSOC_TX_OK,
    GOT_ASSOC_TX_OK
  } m_state;
  SupportedModes m_modes;
  TracedValue<uint32_t> m_ssrc;
  TracedValue<uint32_t> m_slrc;
};

} // namespace ns3 

#endif /* MAC_STATIONS_H */
