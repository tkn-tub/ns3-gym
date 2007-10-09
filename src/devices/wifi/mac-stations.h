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
#ifndef MAC_STATIONS_H
#define MAC_STATIONS_H

#include <vector>
#include <utility>
#include "ns3/mac48-address.h"
#include "wifi-mode.h"

namespace ns3 {

class MacStation;

class MacStations 
{
private:
  typedef std::vector<WifiMode> BasicModes;
public:
  typedef BasicModes::const_iterator BasicModesIterator;

  MacStations (WifiMode defaultTxMode);
  virtual ~MacStations ();
  
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

  MacStation *Lookup (Mac48Address address);
  MacStation *LookupNonUnicast (void);
private:
  typedef std::vector <std::pair<Mac48Address, MacStation *> > Stations;
  virtual class MacStation *CreateStation (void) = 0;
  Stations m_stations;
  WifiMode m_defaultTxMode;
  MacStation *m_nonUnicast;
  BasicModes m_basicModes;
};

} // namespace ns3

namespace ns3 {

class MacStation {
public:
  MacStation ();
  virtual ~MacStation ();

  // Invoked in an AP upon disassociation of a
  // specific STA.
  void Reset (void);
  // Invoked in a STA or AP to store the set of 
  // modes supported by a destination which is
  // also supported locally.
  // The set of supported modes includes
  // the BSSBasicRateSet.
  void AddSupportedMode (WifiMode mode);

  bool IsAssociated (void) const;
  bool IsWaitAssocTxOk (void) const;
  void RecordWaitAssocTxOk (void);
  void RecordGotAssocTxOk (void);
  void RecordGotAssocTxFailed (void);
  void RecordDisassociated (void);

  // reception-related method
  virtual void ReportRxOk (double rxSnr, WifiMode txMode) = 0;

  // transmission-related methods
  virtual void ReportRtsFailed (void) = 0;
  virtual void ReportDataFailed (void) = 0;
  virtual void ReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr) = 0;
  virtual void ReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr) = 0;
  virtual WifiMode GetDataMode (uint32_t size) = 0;
  virtual WifiMode GetRtsMode (void) = 0;

  WifiMode GetCtsMode (WifiMode rtsMode);
  WifiMode GetAckMode (WifiMode dataMode);

private:
  typedef std::vector<WifiMode> SupportedModes;
  virtual MacStations *GetStations (void) const = 0;
protected:
  uint32_t GetNSupportedModes (void) const;
  WifiMode GetSupportedMode (uint32_t i) const;
private:
  bool IsIn (WifiMode mode) const;
  WifiMode GetControlAnswerMode (WifiMode reqMode);
  enum {
    DISASSOC,
    WAIT_ASSOC_TX_OK,
    GOT_ASSOC_TX_OK
  } m_state;
  SupportedModes m_modes;
};

} // namespace ns3 

#endif /* MAC_STATIONS_H */
