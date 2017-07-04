/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Ghada Badawy <gbadawy@gmail.com>
 *          Sébastien Deronne <sebastien.deronne@gmail.com>
 *
 * Ported from yans-wifi-phy.h by several contributors starting
 * with Nicola Baldo and Dean Armstrong
 */

#ifndef SPECTRUM_WIFI_PHY_H
#define SPECTRUM_WIFI_PHY_H

#include "ns3/antenna-model.h"
#include "wifi-phy.h"
#include "wifi-spectrum-phy-interface.h"
#include "ns3/spectrum-channel.h"
#include "ns3/spectrum-interference.h"

namespace ns3 {

/**
 * \brief 802.11 PHY layer model
 * \ingroup wifi
 *
 * This PHY implements a spectrum-aware enhancement of the 802.11 SpectrumWifiPhy
 * model.
 *
 * This PHY model depends on a channel loss and delay
 * model as provided by the ns3::SpectrumPropagationLossModel
 * and ns3::PropagationDelayModel classes.
 *
 */
class SpectrumWifiPhy : public WifiPhy
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  SpectrumWifiPhy ();
  virtual ~SpectrumWifiPhy ();

  /**
   * Set the SpectrumChannel this SpectrumWifiPhy is to be connected to.
   *
   * \param channel the SpectrumChannel this SpectrumWifiPhy is to be connected to
   */
  void SetChannel (const Ptr<SpectrumChannel> channel);
  /**
   * Add a channel number to the list of operational channels.  This method
   * is used to support scanning for strongest base station.
   *
   * \param channelNumber the channel number to add
   */
  void AddOperationalChannel (uint8_t channelNumber);
  /**
   * Return a list of channels to which it may be possible to roam
   * By default, this method will return the current channel number followed
   * by any other channel numbers that have been added.
   *
   * \return vector of channel numbers to which it may be possible to roam
   */
  std::vector<uint8_t> GetOperationalChannelList (void) const;
  /**
   * Clear the list of operational channels.
   */
  void ClearOperationalChannelList (void);

  /**
   * Input method for delivering a signal from the spectrum channel
   * and low-level Phy interface to this SpectrumWifiPhy instance.
   *
   * \param rxParams Input signal parameters
   */
  void StartRx (Ptr<SpectrumSignalParameters> rxParams);

  /**
   * \param packet the packet to send
   * \param txVector the TXVECTOR that has tx parameters such as mode, the transmission mode to use to send
   *        this packet, and txPowerLevel, a power level to use to send this packet. The real transmission
   *        power is calculated as txPowerMin + txPowerLevel * (txPowerMax - txPowerMin) / nTxLevels
   * \param txDuration duration of the transmission.
   */
  void StartTx (Ptr<Packet> packet, WifiTxVector txVector, Time txDuration);

  /**
   * Method to encapsulate the creation of the WifiSpectrumPhyInterface
   * object (used to bind the WifiSpectrumPhy to a SpectrumChannel) and
   * to link it to this SpectrumWifiPhy instance
   *
   * \param device pointer to the NetDevice object including this new object
   */
  void CreateWifiSpectrumPhyInterface (Ptr<NetDevice> device);
  /**
   * \return pointer to WifiSpectrumPhyInterface associated with this Phy
   */
  Ptr<WifiSpectrumPhyInterface> GetSpectrumPhy (void) const;
  /**
   * \param antenna an AntennaModel to include in the transmitted
   * SpectrumSignalParameters (in case any objects downstream of the
   * SpectrumWifiPhy wish to adjust signal properties based on the
   * transmitted antenna model.  This antenna is also used when
   * the underlying WifiSpectrumPhyInterface::GetRxAntenna() method
   * is called.
   *
   * Note:  this method may be split into separate SetTx and SetRx
   * methods in the future if the modelling need for this arises
   */
  void SetAntenna (const Ptr<AntennaModel> antenna);
  /**
   * Get the antenna model used for reception
   *
   * \return the AntennaModel used for reception
   */
  Ptr<AntennaModel> GetRxAntenna (void) const;
  /**
   * \return returns the SpectrumModel that this SpectrumPhy expects to be used
   * for all SpectrumValues that are passed to StartRx. If 0 is
   * returned, it means that any model will be accepted.
   */
  Ptr<const SpectrumModel> GetRxSpectrumModel () const;

  /**
   * \return the width of each band (Hz)
   */
  double GetBandBandwidth (void) const;

  /**
   * \return the width of the guard band (MHz)
   */
  uint32_t GetGuardBandwidth (void) const;

  /**
   * Callback invoked when the Phy model starts to process a signal
   *
   * \param signalType Whether signal is WiFi (true) or foreign (false)
   * \param senderNodeId Node Id of the sender of the signal
   * \param rxPower received signal power (dBm)
   * \param duration Signal duration
   */
  typedef void (* SignalArrivalCallback) (bool signalType, uint32_t senderNodeId, double rxPower, Time duration);

  Ptr<Channel> GetChannel (void) const;

  // The following four methods call to the base WifiPhy class method
  // but also generate a new SpectrumModel if called during runtime

  virtual void SetChannelNumber (uint8_t id);

  virtual void SetFrequency (uint16_t freq);

  virtual void SetChannelWidth (uint8_t channelwidth);

  virtual void ConfigureStandard (WifiPhyStandard standard);

protected:
  // Inherited
  void DoDispose (void);
  void DoInitialize (void);


private:
  /**
   * \param centerFrequency center frequency (MHz)
   * \param channelWidth channel width (MHz) of the channel
   * \param txPowerW power in W to spread across the bands
   * \param modulationClass the modulation class
   * \return Ptr to SpectrumValue
   *
   * This is a helper function to create the right Tx PSD corresponding
   * to the standard in use.
   */
  Ptr<SpectrumValue> GetTxPowerSpectralDensity (uint16_t centerFrequency, uint8_t channelWidth, double txPowerW, WifiModulationClass modulationClass) const;

  /**
   * Perform run-time spectrum model change
   */
  void ResetSpectrumModel (void);

  Ptr<SpectrumChannel> m_channel;        //!< SpectrumChannel that this SpectrumWifiPhy is connected to
  std::vector<uint8_t> m_operationalChannelList; //!< List of possible channels

  Ptr<WifiSpectrumPhyInterface> m_wifiSpectrumPhyInterface; //!< Spectrum phy interface
  Ptr<AntennaModel> m_antenna; //!< antenna model
  mutable Ptr<const SpectrumModel> m_rxSpectrumModel; //!< receive spectrum model
  bool m_disableWifiReception;          //!< forces this Phy to fail to sync on any signal
  TracedCallback<bool, uint32_t, double, Time> m_signalCb; //!< Signal callback

};

} //namespace ns3

#endif /* SPECTRUM_WIFI_PHY_H */
