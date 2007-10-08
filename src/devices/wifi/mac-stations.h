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

#include <list>
#include <utility>
#include "ns3/mac48-address.h"
#include "wifi-mode.h"

namespace ns3 {

class MacStation;

class MacStations {
public:
  MacStations (WifiMode defaultTxMode);
  virtual ~MacStations ();
  
  MacStation *Lookup (Mac48Address address);
  MacStation *LookupNonUnicast (void);
private:
  typedef std::list <std::pair<Mac48Address, MacStation *> > Stations;
  typedef std::list <std::pair<Mac48Address, MacStation *> >::iterator StationsI;
  virtual class MacStation *CreateStation (WifiMode mode) = 0;
  Stations m_stations;
  MacStation *m_nonUnicast;
  WifiMode m_defaultTxMode;
};

} // namespace ns3

namespace ns3 {

class MacStation {
public:
  MacStation (WifiMode defaultTxMode);
  virtual ~MacStation ();

  void ResetModes (void);
  void AddBasicMode (WifiMode mode);
  void AddExtendedMode (WifiMode mode);

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
  struct WifiRate {
    WifiRate (WifiMode mode, bool isBasic);
    WifiMode mode; 
    bool isBasic;
  };
  typedef std::vector<struct WifiRate> WifiRates;
protected:
  uint32_t GetNModes (void) const;
  WifiMode GetMode (uint32_t i) const;
private:
  bool IsIn (WifiMode mode) const;
  WifiMode GetControlAnswerMode (WifiMode reqMode);
  enum {
    DISASSOC,
    WAIT_ASSOC_TX_OK,
    GOT_ASSOC_TX_OK
  } m_state;
  WifiRates m_rates;
  WifiMode m_defaultTxMode;
};

} // namespace ns3 

#endif /* MAC_STATIONS_H */
