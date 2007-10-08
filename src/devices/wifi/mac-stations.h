/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
 * All rights reserved.
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

namespace ns3 {

class MacStation;

class MacStations {
public:
  MacStations ();
  virtual ~MacStations ();
  
  MacStation *Lookup (Mac48Address address);
private:
  typedef std::list <std::pair<Mac48Address, MacStation *> > Stations;
  typedef std::list <std::pair<Mac48Address, MacStation *> >::iterator StationsI;
  virtual class MacStation *CreateStation (void) = 0;
  Stations m_stations;
};

} // namespace ns3

namespace ns3 {

class MacStation {
public:
  MacStation ();

  virtual ~MacStation ();

  bool IsAssociated (void) const;
  bool IsWaitAssocTxOk (void) const;
  void RecordWaitAssocTxOk (void);
  void RecordGotAssocTxOk (void);
  void RecordGotAssocTxFailed (void);
  void RecordDisassociated (void);

  // reception-related method
  virtual void ReportRxOk (double rxSnr, uint8_t txMode) = 0;

  // transmission-related methods
  virtual void ReportRtsFailed (void) = 0;
  virtual void ReportDataFailed (void) = 0;
  virtual void ReportRtsOk (double ctsSnr, uint8_t ctsMode, uint8_t rtsSnr) = 0;
  virtual void ReportDataOk (double ackSnr, uint8_t ackMode, uint8_t dataSnr) = 0;
  virtual uint8_t GetDataMode (int size) = 0;
  virtual uint8_t GetRtsMode (void) = 0;
  virtual uint8_t SnrToSnr (double snr) = 0;

private:
  enum {
    DISASSOC,
    WAIT_ASSOC_TX_OK,
    GOT_ASSOC_TX_OK
  } m_state;
};

} // namespace ns3 

#endif /* MAC_STATIONS_H */
