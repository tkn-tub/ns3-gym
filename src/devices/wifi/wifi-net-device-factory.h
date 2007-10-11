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

#ifndef NETWORK_INTERFACE_Wifi_FACTORY_H
#define NETWORK_INTERFACE_Wifi_FACTORY_H

#include <stdint.h>
#include "ssid.h"
#include "ns3/object.h"

namespace ns3 {

class WifiNetDevice;
class AdhocWifiNetDevice;
class NqstaWifiNetDevice;
class NqapWifiNetDevice;
class DcaTxop;
class Position;

class WifiNetDeviceFactory : public Object
{
public:
  WifiNetDeviceFactory ();
  virtual ~WifiNetDeviceFactory ();

  void SetArf (void);
  void SetAarf (void);
  void SetCr (uint8_t dataMode, uint8_t ctlMode);
  void SetIdeal (double ber);

  /* absolute reception threshold. dBm. */
  void SetPhyEdThresholdDbm (double dbm);
  /* Ratio of energy lost by receiver. dB. */
  void SetPhyRxNoiseDb (double rxNoise);
  /* absolute transmission energy. dBm. */
  void SetPhyTxPowerDbm (double txPowerBase,
             double txPowerEnd,
             uint8_t nTxPower);

  /* no unit. */
  void SetPropSystemLoss  (double systemLoss);
  void SetPropTxGainDbm (double txGain);
  void SetPropRxGainDbm (double rxGain);
  void SetPropFrequencyHz (double frequency);

  void SetMacRtsCtsThreshold (uint32_t size);
  void SetMacFragmentationThreshold (uint32_t size);
  void SetMacMaxSsrc (uint32_t ssrc);
  void SetMacMaxSlrc (uint32_t slrc);

  void SetSsid (Ssid ssid);

  Ptr<AdhocWifiNetDevice> CreateAdhoc (Ptr<Node> node);
  Ptr<NqstaWifiNetDevice> CreateNqsta (Ptr<Node> node);
  Ptr<NqapWifiNetDevice> CreateNqap (Ptr<Node> node);
private:
  void InitializeInterface (Ptr<WifiNetDevice> interface, Position *position) const;
  DcaTxop *CreateDca (Ptr<const WifiNetDevice> interface) const;
  enum {
    RATE_ARF,
    RATE_AARF,
    RATE_CR,
    RATE_IDEAL
  } m_rateControlMode;

  uint8_t m_crDataMode;
  uint8_t m_crCtlMode;

  double m_idealBer;

  double m_phyEdThresholdDbm;
  double m_phyRxNoiseDb;
  double m_phyTxPowerBaseDbm;
  double m_phyTxPowerEndDbm;
  uint8_t m_phyNTxPower;

  double m_propSystemLoss;
  double m_propTxGainDbm;
  double m_propRxGainDbm;
  double m_propFrequencyHz;

  uint32_t m_macRtsCtsThreshold;
  uint32_t m_macFragmentationThreshold;
  uint32_t m_macMaxSsrc;
  uint32_t m_macMaxSlrc;

  Ssid m_ssid;
};

}; // namespace ns3

#endif /* NETWORK_INTERFACE_Wifi_FACTORY_H */
