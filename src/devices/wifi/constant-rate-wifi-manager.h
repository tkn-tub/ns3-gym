/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
#ifndef CONSTANT_RATE_WIFI_MANAGER_H
#define CONSTANT_RATE_WIFI_MANAGER_H

#include <stdint.h>

#include "wifi-remote-station-manager.h"

namespace ns3 {

/**
 * \brief use constant rates for data and control transmissions
 *
 * This class uses always the same transmission rate for every
 * packet sent.
 */
class ConstantRateWifiManager : public WifiRemoteStationManager
{
public:
  static TypeId GetTypeId (void);
  ConstantRateWifiManager ();
  virtual ~ConstantRateWifiManager ();

  WifiMode GetDataMode (void) const;
  WifiMode GetCtlMode (void) const;
private:
  virtual class WifiRemoteStation *CreateStation (void);

  WifiMode m_dataMode;
  WifiMode m_ctlMode;
};


class ConstantRateWifiRemoteStation : public WifiRemoteStation
{
public:
  ConstantRateWifiRemoteStation (Ptr<ConstantRateWifiManager> stations);
  virtual ~ConstantRateWifiRemoteStation ();

protected:
  virtual void DoReportRxOk (double rxSnr, WifiMode txMode);
  virtual void DoReportRtsFailed (void);
  virtual void DoReportDataFailed (void);
  virtual void DoReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr);
  virtual void DoReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr);
  virtual void DoReportFinalRtsFailed (void);
  virtual void DoReportFinalDataFailed (void);

private:
  virtual Ptr<WifiRemoteStationManager> GetManager (void) const;
  virtual WifiMode DoGetDataMode (uint32_t size);
  virtual WifiMode DoGetRtsMode (void);
  Ptr<ConstantRateWifiManager> m_manager;
};

} // namespace ns3



#endif /* CONSTANT_RATE_WIFI_MANAGER_H */
