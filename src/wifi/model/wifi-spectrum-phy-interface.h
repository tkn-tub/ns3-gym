/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 CTTC
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */

#ifndef WIFI_SPECTRUM_PHY_INTERFACE_H
#define WIFI_SPECTRUM_PHY_INTERFACE_H

#include <ns3/spectrum-phy.h>

namespace ns3 {

class SpectrumWifiPhy;

/**
 * \ingroup wifi
 *
 * This class is an adaptor between class SpectrumWifiPhy (which inherits
 * from WifiPhy) and class SpectrumChannel (which expects objects derived
 * from class SpectrumPhy to be connected to it).
 *
 * The adaptor is used only in the receive direction; in the transmit
 * direction, the class SpectrumWifiPhy constructs signal parameters
 * and directly accesses the SpectrumChannel
 */
class WifiSpectrumPhyInterface : public SpectrumPhy
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  WifiSpectrumPhyInterface ();
  /**
   * Connect SpectrumWifiPhy object
   * \param phy SpectrumWifiPhy object to be connected to this object
   */
  void SetSpectrumWifiPhy (const Ptr<SpectrumWifiPhy> phy);

  // Inherited from SpectrumPhy
  Ptr<NetDevice> GetDevice () const;
  void SetDevice (const Ptr<NetDevice> d);
  void SetMobility (const Ptr<MobilityModel> m);
  Ptr<MobilityModel> GetMobility ();
  void SetChannel (const Ptr<SpectrumChannel> c);
  Ptr<const SpectrumModel> GetRxSpectrumModel () const;
  Ptr<AntennaModel> GetRxAntenna ();
  void StartRx (Ptr<SpectrumSignalParameters> params);


private:
  virtual void DoDispose (void);
  Ptr<SpectrumWifiPhy> m_spectrumWifiPhy; ///< spectrum phy
  Ptr<NetDevice> m_netDevice; ///< the device
  Ptr<SpectrumChannel> m_channel; ///< spectrum channel
};

} // namespace ns3

#endif  /* WIFI_SPECTRUM_PHY_INTERFACE_H */
