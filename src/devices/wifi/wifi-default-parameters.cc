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
#include "wifi-default-parameters.h"
#include "ns3/default-value.h"
#include "ns3/time-default-value.h"

namespace ns3 {

namespace WifiDefaultParameters {

static NumericDefaultValue<uint32_t> g_maxSsrc
("WifiMaxSsrc", 
 "The MAC maximum number of short retransmission retries (rts retransmissions).",
 7);

static NumericDefaultValue<uint32_t> g_maxSlrc
("WifiMaxSlrc", 
 "The MAC maximum number of long retransmission retries (data retransmissions).",
 7);

static NumericDefaultValue<uint32_t> g_rtsCtsThreshold
("WifiRtsCtsThreshold",
 "The threshold (in bytes) over which rts/cts is used prior to data transmissions.",
 2000);

static NumericDefaultValue<uint32_t> g_fragmentationThreshold
("WifiFragmentationThreshold",
 "The threshold (in bytes) over which data packets are fragmented",
 2000);

static TimeDefaultValue g_apBeaconInterval 
("WifiApBeaconInterval",
 "The interval between two consecutive beacons",
 Seconds (1.0));

static EnumDefaultValue<enum RateControlAlgorithm> g_rateControlAlgorithm
("WifiRateControlAlgorithm",
 "The rate control algorithm to use",
 ARF, "Arf",
 CONSTANT_RATE, "ConstantRate",
 AARF, "Aarf",
 IDEAL, "Ideal",
 0, (void *)0);

static NumericDefaultValue<uint32_t> g_arfSuccessThreshold
("WifiArfSuccessThreshold",
 "The minimum number of sucessfull transmissions to try a new rate.",
 10);

static NumericDefaultValue<uint32_t> g_arfTimerThreshold
("WifiArfTimerThreshold",
 "The 'timer' threshold in the ARF algorithm.",
 15);

static NumericDefaultValue<uint32_t> g_aarfMinSuccessThreshold
("WifiAarfMinSuccessThreshold",
 "The minimum value for the success threshold in the AARF algorithm.",
 10);

static NumericDefaultValue<uint32_t> g_aarfMinTimerThreshold
("WifiAarfMinTimerThreshold",
 "The minimum value for the 'timer' threshold in the AARF algorithm.",
 15);

static NumericDefaultValue<double> g_aarfSuccessK
("WifiAarfSuccessK",
 "Multiplication factor for the success threshold in the AARF algorithm.",
 2.0);
static NumericDefaultValue<uint32_t> g_aarfMaxSuccessThreshold
("WifiAarfMaxSuccessThreshold",
 "Maximum value of the success threshodl in the AARF algorithm.",
 60);
static NumericDefaultValue<double> g_aarfTimerK
("WifiAarfTimerK",
 "Multiplication factor for the timer threshold in the AARF algorithm.",
 2.0);

static NumericDefaultValue<double> g_phyEdThreshold
("WifiPhyEnergyDetectionThreshold",
 "The energy of a received signal should be higher than this threshold (dbm) to allow the PHY layer to detect the signal.",
 -140.0);

static NumericDefaultValue<double> g_phyRxNoise
("WifiPhyRxNoise",
 "Ratio of energy lost by receiver (dB).",
 7);

static NumericDefaultValue<double> g_phyTxPowerBase
("WifiPhyTxPowerBase", 
 "Minimum available transmission level (dbm).",
 16.0206);
static NumericDefaultValue<double> g_phyTxPowerEnd
("WifiPhyTxPowerEnd", 
 "Maximum available transmission level (dbm).",
 16.0206);
static NumericDefaultValue<uint32_t> g_phyNTxPower
("WifiPhyTxPowerLevels", 
 "Number of transmission power levels available between WifiPhyTxPowerBase and WifiPhyTxPowerEnd included.",
 1);
static NumericDefaultValue<double> g_phyTxGain
("WifiPhyTxGain", 
 "Transmission gain (dB).",
 1.0);
static NumericDefaultValue<double> g_phyRxGain
("WifiPhyRxGain", 
 "Reception gain (dB).",
 1.0);
static StringDefaultValue g_ssid
("WifiSsid",
 "The ssid to use. \"\" is the broadcast ssid.",
 "");
static EnumDefaultValue<enum PhyModeParameter> g_dataMode
("WifiConstantDataRate", 
 "The rate to use for data transmissions if using the ConstantRate rate control algorithm.",
 MODE_6MB, "6mb",
 MODE_9MB, "9mb",
 MODE_12MB, "12mb",
 MODE_18MB, "18mb",
 MODE_24MB, "24mb",
 MODE_36MB, "36mb",
 MODE_48MB, "48mb",
 MODE_54MB, "54mb",
 0, (void *)0);
static EnumDefaultValue<enum PhyModeParameter> g_ctlMode
("WifiConstantCtlRate", 
 "The rate to use for control transmissions if using the ConstantRate rate control algorithm.",
 MODE_6MB, "6mb",
 MODE_9MB, "9mb",
 MODE_12MB, "12mb",
 MODE_18MB, "18mb",
 MODE_24MB, "24mb",
 MODE_36MB, "36mb",
 MODE_48MB, "48mb",
 MODE_54MB, "54mb",
 0, (void *)0);
static NumericDefaultValue<double> g_idealBer
("WifiIdealRateControlBerThreshold",
 "The maximum Bit Error Rate acceptable at any transmission mode",
 10e-6);



uint32_t 
GetMaxSsrc (void)
{
  return g_maxSsrc.GetValue ();
}
uint32_t 
GetMaxSlrc (void)
{
  return g_maxSlrc.GetValue ();
}
uint32_t 
GetRtsCtsThreshold (void)
{
  return g_rtsCtsThreshold.GetValue ();
}
uint32_t 
GetFragmentationThreshold (void)
{
  return g_fragmentationThreshold.GetValue ();
}
Time 
GetApBeaconInterval (void)
{
  return g_apBeaconInterval.GetValue ();
}
enum PhyStandard 
GetPhyStandard (void)
{
  return PHY_STANDARD_80211a;
}
enum RateControlAlgorithm 
GetRateControlAlgorithm (void)
{
  return g_rateControlAlgorithm.GetValue ();
}
enum PhyModeParameter 
GetConstantDataRate (void)
{
  return g_dataMode.GetValue ();
}
enum PhyModeParameter 
GetConstantCtlRate (void)
{
  return g_ctlMode.GetValue ();
}
double 
GetIdealRateControlBer (void)
{
  return g_idealBer.GetValue ();
}
uint32_t
GetArfRateControlSuccessThreshold (void)
{
  return g_arfSuccessThreshold.GetValue ();
}
uint32_t
GetArfRateControlTimerThreshold (void)
{
  return g_arfTimerThreshold.GetValue ();
}
uint32_t
GetAarfRateControlMinSuccessThreshold (void)
{
  return g_aarfMinSuccessThreshold.GetValue ();
}
uint32_t
GetAarfRateControlMinTimerThreshold (void)
{
  return g_aarfMinTimerThreshold.GetValue ();
}
double
GetAarfRateControlSuccessK (void)
{
  return g_aarfSuccessK.GetValue ();
}
uint32_t
GetAarfRateControlMaxSuccessThreshold (void)
{
  return g_aarfMaxSuccessThreshold.GetValue ();
}
double
GetAarfRateControlTimerK (void)
{
  return g_aarfTimerK.GetValue ();
}

double 
GetPhyEnergyDetectionThresholdDbm (void)
{
  return g_phyEdThreshold.GetValue ();
}
double
GetPhyRxNoiseDb (void)
{
  return g_phyRxNoise.GetValue ();
}

double
GetPhyTxPowerBaseDbm (void)
{
  return g_phyTxPowerBase.GetValue ();
}

double
GetPhyTxPowerEndDbm (void)
{
  return g_phyTxPowerEnd.GetValue ();
}

uint32_t
GetPhyTxPowerLevels (void)
{
  return g_phyNTxPower.GetValue ();
}

double 
GetPhyTxGainDb (void)
{
  return g_phyTxGain.GetValue ();
}
double 
GetPhyRxGainDb (void)
{
  return g_phyRxGain.GetValue ();
}
Ssid
GetSsid (void)
{
  return Ssid (g_ssid.GetValue ().c_str ());
}


} // namespace WifiDefaultParameters

} // namespace ns3 
