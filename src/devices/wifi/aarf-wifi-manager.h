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
#ifndef AARF_MAC_STATIONS_H
#define AARF_MAC_STATIONS_H

#include "arf-wifi-manager.h"

namespace ns3 {

/**
 * \brief AARF Rate control algorithm
 *
 * This class implements the AARF rate control algorithm which
 * was initially described in <i>IEEE 802.11 Rate Adaptation:
 * A Practical Approach</i>, by M. Lacage, M.H. Manshaei, and 
 * T. Turletti.
 */
class AarfWifiManager : public ArfWifiManager 
{
public:
  static TypeId GetTypeId (void);
  AarfWifiManager ();
  virtual ~AarfWifiManager ();
private:
  virtual class WifiRemoteStation *CreateStation (void);
  uint32_t m_minTimerThreshold;
  uint32_t m_minSuccessThreshold;
  double m_successK;
  uint32_t m_maxSuccessThreshold;
  double m_timerK;
};

class AarfWifiRemoteStation : public ArfWifiRemoteStation
{
public:
  AarfWifiRemoteStation (Ptr<AarfWifiManager> stations,
                         uint32_t minTimerThreshold,
                         uint32_t minSuccessThreshold,
                         double successK,
                         uint32_t maxSuccessThreshold,
                         double timerK);
  virtual ~AarfWifiRemoteStation ();

private:
  virtual void ReportRecoveryFailure (void);
  virtual void ReportFailure (void);

  double m_successK;
  uint32_t m_maxSuccessThreshold;
  double m_timerK;
};

} // namespace ns3


#endif /* AARF_MAC_STATIONS_H */
