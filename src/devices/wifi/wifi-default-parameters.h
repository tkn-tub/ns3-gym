/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#ifndef WIFI_DEFAULT_PARAMETERS_H
#define WIFI_DEFAULT_PARAMETERS_H

#include <stdint.h>
#include "ns3/nstime.h"
#include "ssid.h"

namespace ns3 {

namespace WifiDefaultParameters {

enum PhyStandard {
  PHY_STANDARD_80211a,
};
enum RateControlAlgorithm {
  CONSTANT_RATE,
  ARF,
  AARF,
  IDEAL
};
enum PhyModeParameter {
  MODE_6MB = 6000000,
  MODE_9MB = 9000000,
  MODE_12MB = 12000000,
  MODE_18MB = 18000000,
  MODE_24MB = 24000000,
  MODE_36MB = 36000000,
  MODE_48MB = 48000000,
  MODE_54MB = 54000000
};

uint32_t GetMaxSsrc (void);
uint32_t GetMaxSlrc (void);
uint32_t GetRtsCtsThreshold (void);
uint32_t GetFragmentationThreshold (void);
Time GetApBeaconInterval (void);
enum PhyStandard GetPhyStandard (void);
enum RateControlAlgorithm GetRateControlAlgorithm (void);
enum PhyModeParameter GetConstantDataRate (void);
enum PhyModeParameter GetConstantCtlRate (void);
double GetIdealRateControlBer (void);
uint32_t GetArfRateControlSuccessThreshold (void);
uint32_t GetArfRateControlTimerThreshold (void);
uint32_t GetAarfRateControlMinSuccessThreshold (void);
uint32_t GetAarfRateControlMinTimerThreshold (void);
double GetAarfRateControlSuccessK (void);
uint32_t GetAarfRateControlMaxSuccessThreshold (void);
double GetAarfRateControlTimerK (void);

double GetPhyEnergyDetectionThresholdDbm (void);
double GetPhyRxNoiseDb (void);
double GetPhyTxPowerBaseDbm (void);
double GetPhyTxPowerEndDbm (void);
uint32_t GetPhyTxPowerLevels (void);
double GetPhyTxGainDbm (void);
double GetPhyRxGainDbm (void);

Ssid GetSsid (void);

} // namespace WifiDefaultParameters

} // namespace ns3

#endif /* WIFI_DEFAULT_PARAMETERS_H */
