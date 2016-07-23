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

#include <stdint.h>
#include "ns3/callback.h"
#include "ns3/antenna-model.h"
#include "ns3/event-id.h"
#include "ns3/packet.h"
#include "ns3/object.h"
#include "ns3/traced-callback.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "ns3/random-variable-stream.h"
#include "ns3/mobility-model.h"
#include "wifi-phy.h"
#include "wifi-mode.h"
#include "wifi-preamble.h"
#include "wifi-phy-standard.h"
#include "interference-helper.h"

#include "wifi-spectrum-phy-interface.h"
#include "ns3/spectrum-channel.h"
#include "ns3/spectrum-interference.h"

namespace ns3 {

#define VHT_PHY 126
#define HT_PHY 127

class WifiPhyStateHelper;

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
  static TypeId GetTypeId (void);

  SpectrumWifiPhy ();
  virtual ~SpectrumWifiPhy ();

  /**
   * Set the SpectrumChannel this SpectrumWifiPhy is to be connected to.
   *
   * \param channel the SpectrumChannel this SpectrumWifiPhy is to be connected to
   */
  void SetChannel (Ptr<SpectrumChannel> channel);
  /**
   * \return the required time for channel switch operation of this WifiPhy
   */
  Time GetChannelSwitchDelay (void) const;
  /**
   * Add a channel number to the list of operational channels.  This method
   * is used to support scanning for strongest base station.
   *
   * \param channelNumber the channel number to add
   */
  void AddOperationalChannel (uint16_t channelNumber);
  /**
   * Return a list of channels to which it may be possible to roam
   * By default, this method will return the current channel number followed
   * by any other channel numbers that have been added.
   *
   * \return vector of channel numbers to which it may be possible to roam
   */
  std::vector<uint16_t> GetOperationalChannelList (void) const;
  /**
   * Clear the list of operational channels.
   */
  void ClearOperationalChannelList (void);

  /**
   * Starting receiving the plcp of a packet (i.e. the first bit of the preamble has arrived).
   *
   * \param packet the arriving packet
   * \param rxPowerDbm the receive power in dBm
   * \param txVector the TXVECTOR of the arriving packet
   * \param preamble the preamble of the arriving packet
   * \param mpdutype the type of the MPDU as defined in WifiPhy::mpduType.
   * \param rxDuration the duration needed for the reception of the packet
   */
  void StartReceivePreambleAndHeader (Ptr<Packet> packet,
                                      double rxPowerDbm,
                                      WifiTxVector txVector,
                                      WifiPreamble preamble,
                                      enum mpduType mpdutype,
                                      Time rxDuration);
  /**
   * Starting receiving the payload of a packet (i.e. the first bit of the packet has arrived).
   *
   * \param packet the arriving packet
   * \param txVector the TXVECTOR of the arriving packet
   * \param preamble the preamble of the arriving packet
   * \param mpdutype the type of the MPDU as defined in WifiPhy::mpduType.
   * \param event the corresponding event of the first time the packet arrives
   */
  void StartReceivePacket (Ptr<Packet> packet,
                           WifiTxVector txVector,
                           WifiPreamble preamble,
                           enum mpduType mpdutype,
                           Ptr<InterferenceHelper::Event> event);

  /**
   * Sets the RX loss (dB) in the Signal-to-Noise-Ratio due to non-idealities in the receiver.
   *
   * \param noiseFigureDb noise figure in dB
   */
  void SetRxNoiseFigure (double noiseFigureDb);
  /**
   * Sets the minimum available transmission power level (dBm).
   *
   * \param start the minimum transmission power level (dBm)
   */
  void SetTxPowerStart (double start);
  /**
   * Sets the maximum available transmission power level (dBm).
   *
   * \param end the maximum transmission power level (dBm)
   */
  void SetTxPowerEnd (double end);
  /**
   * Sets the number of transmission power levels available between the
   * minimum level and the maximum level. Transmission power levels are
   * equally separated (in dBm) with the minimum and the maximum included.
   *
   * \param n the number of available levels
   */
  void SetNTxPower (uint32_t n);
  /**
   * Sets the transmission gain (dB).
   *
   * \param gain the transmission gain in dB
   */
  void SetTxGain (double gain);
  /**
   * Sets the reception gain (dB).
   *
   * \param gain the reception gain in dB
   */
  void SetRxGain (double gain);
  /**
   * Sets the energy detection threshold (dBm).
   * The energy of a received signal should be higher than
   * this threshold (dbm) to allow the PHY layer to detect the signal.
   *
   * \param threshold the energy detction threshold in dBm
   */
  void SetEdThreshold (double threshold);
  /**
   * Sets the CCA threshold (dBm). The energy of a received signal
   * should be higher than this threshold to allow the PHY
   * layer to declare CCA BUSY state.
   *
   * \param threshold the CCA threshold in dBm
   */
  void SetCcaMode1Threshold (double threshold);
  /**
   * Sets the error rate model.
   *
   * \param rate the error rate model
   */
  void SetErrorRateModel (Ptr<ErrorRateModel> rate);
  /**
   * Sets the device this PHY is associated with.
   *
   * \param device the device this PHY is associated with
   */
  void SetDevice (Ptr<NetDevice> device);
  /**
   * \brief assign a mobility model to this device
   *
   * This method allows a user to specify a mobility model that should be
   * associated with this physical layer.  Calling this method is optional
   * and only necessary if the user wants to override the mobility model
   * that is aggregated to the node.
   *
   * \param mobility the mobility model this PHY is associated with
   */
  void SetMobility (Ptr<MobilityModel> mobility);
  /**
   * Return the RX noise figure (dBm).
   *
   * \return the RX noise figure in dBm
   */
  double GetRxNoiseFigure (void) const;
  /**
   * Return the transmission gain (dB).
   *
   * \return the transmission gain in dB
   */
  double GetTxGain (void) const;
  /**
   * Return the reception gain (dB).
   *
   * \return the reception gain in dB
   */
  double GetRxGain (void) const;
  /**
   * Return the energy detection threshold (dBm).
   *
   * \return the energy detection threshold in dBm
   */
  double GetEdThreshold (void) const;
  /**
   * Return the CCA threshold (dBm).
   *
   * \return the CCA threshold in dBm
   */
  double GetCcaMode1Threshold (void) const;
  /**
   * Return the error rate model this PHY is using.
   *
   * \return the error rate model this PHY is using
   */
  Ptr<ErrorRateModel> GetErrorRateModel (void) const;
  /**
   * Return the device this PHY is associated with
   *
   * \return the device this PHY is associated with
   */
  Ptr<NetDevice> GetDevice (void) const;
  /**
   * Return the mobility model this PHY is associated with.
   * This method will return either the mobility model that has been
   * explicitly set by a call to SpectrumWifiPhy::SetMobility(), or else
   * will return the mobility model (if any) that has been aggregated
   * to the node.
   *
   * \return the mobility model this PHY is associated with
   */
  Ptr<MobilityModel> GetMobility (void);
  /**
   * Return the minimum available transmission power level (dBm).
   * \return the minimum available transmission power level (dBm)
   */
  virtual double GetTxPowerStart (void) const;
  /**
   * Return the maximum available transmission power level (dBm).
   * \return the maximum available transmission power level (dBm)
   */
  virtual double GetTxPowerEnd (void) const;
  /**
   * Return the number of available transmission power levels.
   *
   * \return the number of available transmission power levels
   */
  virtual uint32_t GetNTxPower (void) const;

  /**
   * Input method for delivering a signal from the spectrum channel
   * and low-level Phy interface to this SpectrumWifiPhy instance.
   *
   * \param rxParams Input signal parameters
   */
  void StartRx (Ptr<SpectrumSignalParameters> rxParams);
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
  void SetAntenna (Ptr<AntennaModel> antenna);
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
   * Callback invoked at the end of a frame reception, to notify whether
   * the frame was received successfully (true) or not (false)
   */
  typedef Callback<void,bool> RxCallback;
  /**
   * Set the packet received callback (invoked at the end of a frame
   * reception), to notify whether the frame was received successfully
   * or not.
   *
   * \param callback the function to hook to the callback
   */
  void SetPacketReceivedCallback (RxCallback callback);

  /**
   * Callback invoked when the Phy model starts to process a signal
   *
   * \param signalType Whether signal is WiFi (true) or foreign (false)
   * \param senderNodeId Node Id of the sender of the signal
   * \param rxPower received signal power (dBm)
   * \param duration Signal duration
   */
  typedef void (* SignalArrivalCallback) (bool signalType, uint32_t senderNodeId, double rxPower, Time duration);

  virtual void SetReceiveOkCallback (WifiPhy::RxOkCallback callback);
  virtual void SetReceiveErrorCallback (WifiPhy::RxErrorCallback callback);
  virtual void SendPacket (Ptr<const Packet> packet, WifiTxVector txVector, enum WifiPreamble preamble);
  virtual void SendPacket (Ptr<const Packet> packet, WifiTxVector txVector, enum WifiPreamble preamble, enum mpduType mpdutype);
  virtual void RegisterListener (WifiPhyListener *listener);
  virtual void UnregisterListener (WifiPhyListener *listener);
  virtual void SetSleepMode (void);
  virtual void ResumeFromSleep (void);
  virtual bool IsStateCcaBusy (void);
  virtual bool IsStateIdle (void);
  virtual bool IsStateBusy (void);
  virtual bool IsStateRx (void);
  virtual bool IsStateTx (void);
  virtual bool IsStateSwitching (void);
  virtual bool IsStateSleep (void);
  virtual Time GetStateDuration (void);
  virtual Time GetDelayUntilIdle (void);
  virtual Time GetLastRxStartTime (void) const;
  virtual uint32_t GetNModes (void) const;
  virtual WifiMode GetMode (uint32_t mode) const;
  virtual bool IsModeSupported (WifiMode mode) const;
  virtual bool IsMcsSupported (WifiMode mcs);
  virtual double CalculateSnr (WifiTxVector txVector, double ber) const;
  virtual Ptr<WifiChannel> GetChannel (void) const;

  virtual void ConfigureStandard (enum WifiPhyStandard standard);

  /**
   * Assign a fixed random variable stream number to the random variables
   * used by this model.  Return the number of streams (possibly zero) that
   * have been assigned.
   *
   * \param stream first stream index to use
   * \return the number of stream indices assigned by this model
   */
  int64_t AssignStreams (int64_t stream);

  /**
   * \param tx the number of transmitters on this node.
   */
  virtual void SetNumberOfTransmitAntennas (uint32_t tx);
  /**
   * \return the number of transmitters on this node.
   */
  virtual uint32_t GetNumberOfTransmitAntennas (void) const;
  /**
   * \param rx the number of receivers on this node.
   */
  virtual void SetNumberOfReceiveAntennas (uint32_t rx);
  /**
   * \return the number of receivers on this node.
   */
  virtual uint32_t GetNumberOfReceiveAntennas (void) const;
  /**
   * Enable or disable short/long guard interval.
   *
   * \param guardInterval Enable or disable guard interval
   */
  virtual void SetGuardInterval (bool guardInterval);
  /**
   * Return whether guard interval is being used.
   *
   * \return true if guard interval is being used, false otherwise
   */
  virtual bool GetGuardInterval (void) const;
  /**
   * Enable or disable LDPC.
   * \param ldpc Enable or disable LDPC
   */
  virtual void SetLdpc (bool ldpc);
  /**
   * Return if LDPC is supported.
   *
   * \return true if LDPC is supported, false otherwise
   */
  virtual bool GetLdpc (void) const;
  /**
   * Enable or disable STBC.
   *
   * \param stbc Enable or disable STBC
   */
  virtual void SetStbc (bool stbc);
  /**
   * Return whether STBC is supported.
   *
   * \return true if STBC is supported, false otherwise
   */
  virtual bool GetStbc (void) const;
  /**
   * Enable or disable Greenfield support.
   *
   * \param greenfield Enable or disable Greenfield
   */
  virtual void SetGreenfield (bool greenfield);
  /**
   * Return whether Greenfield is supported.
   *
   * \return true if Greenfield is supported, false otherwise
   */
  virtual bool GetGreenfield (void) const;
  /**
   * Enable or disable short PLCP preamble.
   *
   * \param preamble sets whether short PLCP preamble is supported or not
   */
  virtual void SetShortPlcpPreambleSupported (bool preamble);
  /**
   * Return whether short PLCP preamble is supported.
   *
   * \returns if short PLCP preamble is supported or not
   */
  virtual bool GetShortPlcpPreambleSupported (void) const;

  virtual uint8_t GetSupportedRxSpatialStreams (void) const;
  virtual uint8_t GetSupportedTxSpatialStreams (void) const;
  virtual uint32_t GetNBssMembershipSelectors (void) const;
  virtual uint32_t GetBssMembershipSelector (uint32_t selector) const;
  virtual WifiModeList GetMembershipSelectorModes (uint32_t selector);

  /**
   * \return the number of MCS supported by this phy
   */
  virtual uint8_t GetNMcs (void) const;
  virtual WifiMode GetMcs (uint8_t mcs) const;

protected:
  // Inherited
  virtual void DoDispose (void);
  virtual void DoInitialize (void);
  virtual bool DoChannelSwitch (uint16_t id);
  virtual bool DoFrequencySwitch (uint32_t frequency);

private:
  /**
   * Configure SpectrumWifiPhy with appropriate channel frequency and
   * supported rates for 802.11a standard.
   */
  void Configure80211a (void);
  /**
   * Configure SpectrumWifiPhy with appropriate channel frequency and
   * supported rates for 802.11b standard.
   */
  void Configure80211b (void);
  /**
   * Configure SpectrumWifiPhy with appropriate channel frequency and
   * supported rates for 802.11g standard.
   */
  void Configure80211g (void);
  /**
   * Configure SpectrumWifiPhy with appropriate channel frequency and
   * supported rates for 802.11a standard with 10MHz channel spacing.
   */
  void Configure80211_10Mhz (void);
  /**
   * Configure SpectrumWifiPhy with appropriate channel frequency and
   * supported rates for 802.11a standard with 5MHz channel spacing.
   */
  void Configure80211_5Mhz ();
  void ConfigureHolland (void);
  /**
   * Configure SpectrumWifiPhy with appropriate channel frequency and
   * supported rates for 802.11n standard.
   */
  void Configure80211n (void);
  /**
   * Configure SpectrumWifiPhy with appropriate channel frequency and
   * supported rates for 802.11ac standard.
   */
  void Configure80211ac (void);
  /**
   * Configure the device Mcs set with the appropriate HtMcs modes for
   * the number of available transmit spatial streams
   */
  void ConfigureHtDeviceMcsSet (void);
  /**
   * Return the energy detection threshold.
   *
   * \return the energy detection threshold.
   */
  double GetEdThresholdW (void) const;
  /**
   * Convert from dBm to Watts.
   *
   * \param dbm the power in dBm
   *
   * \return the equivalent Watts for the given dBm
   */
  double DbmToW (double dbm) const;
  /**
   * Convert from dB to ratio.
   *
   * \param db
   *
   * \return ratio
   */
  double DbToRatio (double db) const;
  /**
   * Convert from Watts to dBm.
   *
   * \param w the power in Watts
   *
   * \return the equivalent dBm for the given Watts
   */
  double WToDbm (double w) const;
  /**
   * Convert from ratio to dB.
   *
   * \param ratio
   *
   * \return dB
   */
  double RatioToDb (double ratio) const;
  /**
   * Get the power of the given power level in dBm.
   * In SpectrumWifiPhy implementation, the power levels are equally spaced (in dBm).
   *
   * \param power the power level
   *
   * \return the transmission power in dBm at the given power level
   */
  double GetPowerDbm (uint8_t power) const;
  /**
   * The last bit of the packet has arrived.
   *
   * \param packet the packet that the last bit has arrived
   * \param preamble the preamble of the arriving packet
   * \param mpdutype the type of the MPDU as defined in WifiPhy::mpduType.
   * \param event the corresponding event of the first time the packet arrives
   */
  void EndReceive (Ptr<Packet> packet, enum WifiPreamble preamble, enum mpduType mpdutype, Ptr<InterferenceHelper::Event> event);

  /**
   * Check if Phy state should move to CCA busy state based on current
   * state of interference tracker.  In this model, CCA becomes busy when
   * the aggregation of all signals as tracked by the InterferenceHelper
   * class is higher than the CcaMode1Threshold
   */
  void SwitchMaybeToCcaBusy (void);

  /**
   * \param centerFrequency center frequency (MHz)
   * \param channelWidth channel width (MHz) of the channel
   * \param txPowerW power in W to spread across the bands
   * \return Ptr to SpectrumValue
   *
   * This is a helper function to create the right Tx PSD corresponding
   * to the standard in use.
   */
  Ptr<SpectrumValue> GetTxPowerSpectralDensity (uint32_t centerFrequency, uint32_t channelWidth, double txPowerW) const;

  double   m_edThresholdW;        //!< Energy detection threshold in watts
  double   m_ccaMode1ThresholdW;  //!< Clear channel assessment (CCA) threshold in watts
  double   m_txGainDb;            //!< Transmission gain (dB)
  double   m_rxGainDb;            //!< Reception gain (dB)
  double   m_txPowerBaseDbm;      //!< Minimum transmission power (dBm)
  double   m_txPowerEndDbm;       //!< Maximum transmission power (dBm)
  uint32_t m_nTxPower;            //!< Number of available transmission power levels

  Ptr<SpectrumChannel> m_channel;        //!< SpectrumChannel that this SpectrumWifiPhy is connected to
  std::vector<uint16_t> m_operationalChannelList; //!< List of possible channels
  Ptr<NetDevice>       m_device;         //!< Pointer to the device
  Ptr<MobilityModel>   m_mobility;       //!< Pointer to the mobility model

  uint32_t m_numberOfTransmitters;  //!< Number of transmitters
  uint32_t m_numberOfReceivers;     //!< Number of receivers
  bool     m_ldpc;                  //!< Flag if LDPC is used
  bool     m_stbc;                  //!< Flag if STBC is used
  bool     m_greenfield;            //!< Flag if GreenField format is supported
  bool     m_guardInterval;         //!< Flag if short guard interval is used
  bool     m_shortPreamble;         //!< Flag if short PLCP preamble is supported

  /**
   * This vector holds the set of transmission modes that this
   * WifiPhy(-derived class) can support. In conversation we call this
   * the DeviceRateSet (not a term you'll find in the standard), and
   * it is a superset of standard-defined parameters such as the
   * OperationalRateSet, and the BSSBasicRateSet (which, themselves,
   * have a superset/subset relationship).
   *
   * Mandatory rates relevant to this WifiPhy can be found by
   * iterating over this vector looking for WifiMode objects for which
   * WifiMode::IsMandatory() is true.
   *
   * A quick note is appropriate here (well, here is as good a place
   * as any I can find)...
   *
   * In the standard there is no text that explicitly precludes
   * production of a device that does not support some rates that are
   * mandatory (according to the standard) for PHYs that the device
   * happens to fully or partially support.
   *
   * This approach is taken by some devices which choose to only support,
   * for example, 6 and 9 Mbps ERP-OFDM rates for cost and power
   * consumption reasons (i.e., these devices don't need to be designed
   * for and waste current on the increased linearity requirement of
   * higher-order constellations when 6 and 9 Mbps more than meet their
   * data requirements). The wording of the standard allows such devices
   * to have an OperationalRateSet which includes 6 and 9 Mbps ERP-OFDM
   * rates, despite 12 and 24 Mbps being "mandatory" rates for the
   * ERP-OFDM PHY.
   *
   * Now this doesn't actually have any impact on code, yet. It is,
   * however, something that we need to keep in mind for the
   * future. Basically, the key point is that we can't be making
   * assumptions like "the Operational Rate Set will contain all the
   * mandatory rates".
   */
  WifiModeList m_deviceRateSet;
  WifiModeList m_deviceMcsSet;

  std::vector<uint32_t> m_bssMembershipSelectorSet;
  EventId m_endRxEvent;
  EventId m_endPlcpRxEvent;

  Ptr<UniformRandomVariable> m_random;  //!< Provides uniform random variables.
  double m_channelStartingFrequency;    //!< Standard-dependent center frequency of 0-th channel in MHz
  Ptr<WifiPhyStateHelper> m_state;      //!< Pointer to WifiPhyStateHelper
  InterferenceHelper m_interference;    //!< Pointer to InterferenceHelper
  Ptr<WifiSpectrumPhyInterface> m_wifiSpectrumPhyInterface;
  Ptr<AntennaModel> m_antenna;
  mutable Ptr<const SpectrumModel> m_rxSpectrumModel;
  RxCallback m_rxCallback;
  Time m_channelSwitchDelay;            //!< Time required to switch between channel
  uint16_t m_mpdusNum;                  //!< carries the number of expected mpdus that are part of an A-MPDU
  bool m_plcpSuccess;                   //!< Flag if the PLCP of the packet or the first MPDU in an A-MPDU has been received
  uint32_t m_txMpduReferenceNumber;     //!< A-MPDU reference number to identify all transmitted subframes belonging to the same received A-MPDU
  uint32_t m_rxMpduReferenceNumber;     //!< A-MPDU reference number to identify all received subframes belonging to the same received A-MPDU
  bool m_disableWifiReception;          //!< forces this Phy to fail to sync on any signal
  TracedCallback<bool, uint32_t, double, Time> m_signalCb;

};

} //namespace ns3

#endif /* SPECTRUM_WIFI_PHY_H */
