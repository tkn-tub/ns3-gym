/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 The Boeing Company
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
 * Author:
 *  Gary Pei <guangyu.pei@boeing.com>
 *  Sascha Alexander Jopen <jopen@cs.uni-bonn.de>
 */
#ifndef LR_WPAN_PHY_H
#define LR_WPAN_PHY_H

#include "lr-wpan-interference-helper.h"

#include <ns3/spectrum-phy.h>
#include <ns3/traced-callback.h>
#include <ns3/event-id.h>

namespace ns3 {

class Packet;
class SpectrumValue;
class LrWpanErrorModel;
struct LrWpanSpectrumSignalParameters;
class MobilityModel;
class SpectrumChannel;
class SpectrumModel;
class AntennaModel;
class NetDevice;
class UniformRandomVariable;

/**
 * Helper structure to manage the power measurement during ED.
 */
typedef struct
{
  double averagePower;
  Time lastUpdate;
  Time measurementLength;
} LrWpanEdPower;

/**
 * IEEE802.15.4-2006 Table 1 and 2 in section 6.1.1 and 6.1.2
 */
typedef  struct
{
  double bitRate;
  double symbolRate;
} LrWpanPhyDataAndSymbolRates;

/**
 * IEEE802.15.4-2006 Figure 16, Table 19 and 20 in section 6.3
 * this data structure provides number of symbols for the PPDU headers: SHR and PHR
 */
typedef  struct
{
  double shrPreamble;
  double shrSfd;
  double phr;
} LrWpanPhyPpduHeaderSymbolNumber;

/**
 * This Phy option will be used to index various Tables in IEEE802.15.4-2006
 */
typedef enum
{
  IEEE_802_15_4_868MHZ_BPSK         = 0,
  IEEE_802_15_4_915MHZ_BPSK         = 1,
  IEEE_802_15_4_868MHZ_ASK          = 2,
  IEEE_802_15_4_915MHZ_ASK          = 3,
  IEEE_802_15_4_868MHZ_OQPSK        = 4,
  IEEE_802_15_4_915MHZ_OQPSK        = 5,
  IEEE_802_15_4_2_4GHZ_OQPSK        = 6,
  IEEE_802_15_4_INVALID_PHY_OPTION  = 7
} LrWpanPhyOption;

/**
 * IEEE802.15.4-2006 PHY Emumerations Table 18
 * in section 6.2.3
 */
typedef enum
{
  IEEE_802_15_4_PHY_BUSY  = 0x00,
  IEEE_802_15_4_PHY_BUSY_RX = 0x01,
  IEEE_802_15_4_PHY_BUSY_TX = 0x02,
  IEEE_802_15_4_PHY_FORCE_TRX_OFF = 0x03,
  IEEE_802_15_4_PHY_IDLE = 0x04,
  IEEE_802_15_4_PHY_INVALID_PARAMETER = 0x05,
  IEEE_802_15_4_PHY_RX_ON = 0x06,
  IEEE_802_15_4_PHY_SUCCESS = 0x07,
  IEEE_802_15_4_PHY_TRX_OFF = 0x08,
  IEEE_802_15_4_PHY_TX_ON = 0x09,
  IEEE_802_15_4_PHY_UNSUPPORTED_ATTRIBUTE = 0xa,
  IEEE_802_15_4_PHY_READ_ONLY = 0xb,
  IEEE_802_15_4_PHY_UNSPECIFIED = 0xc // all cases not covered by ieee802.15.4
} LrWpanPhyEnumeration;

/**
 * IEEE802.15.4-2006 PHY PIB Attribute Identifiers Table 23 in section 6.4.2
 */
typedef enum
{
  phyCurrentChannel = 0x00,
  phyChannelsSupported = 0x01,
  phyTransmitPower = 0x02,
  phyCCAMode = 0x03,
  phyCurrentPage = 0x04,
  phyMaxFrameDuration = 0x05,
  phySHRDuration = 0x06,
  phySymbolsPerOctet = 0x07
} LrWpanPibAttributeIdentifier;

/**
 * IEEE802.15.4-2006 PHY PIB Attributes Table 23 in section 6.4.2
 */
typedef struct
{
  uint8_t phyCurrentChannel;
  uint32_t phyChannelsSupported[32];
  uint8_t phyTransmitPower;
  uint8_t phyCCAMode;
  uint32_t phyCurrentPage;
  uint32_t phyMaxFrameDuration;
  uint32_t phySHRDuration;
  double phySymbolsPerOctet;
} LrWpanPhyPibAttributes;

/**
 * This method implements the PD SAP: PdDataIndication
 *
 *  @param psduLength number of bytes in the PSDU
 *  @param p the packet to be transmitted
 *  @param lqi Link quality (LQI) value measured during reception of the PPDU
 */
typedef Callback< void, uint32_t, Ptr<Packet>, uint8_t > PdDataIndicationCallback;

/**
 * This method implements the PD SAP: PdDataConfirm
 *
 * @param status the status to be transmitted
 */
typedef Callback< void, LrWpanPhyEnumeration > PdDataConfirmCallback;

/**
 * This method implements the PD SAP: PlmeCcaConfirm
 *
 * @param status the status of CCA
 */
typedef Callback< void, LrWpanPhyEnumeration > PlmeCcaConfirmCallback;

/**
 * This method implements the PD SAP: PlmeEdConfirm
 *
 * @param status the status of ED
 * @param energyLevel the energy level of ED
 */
typedef Callback< void, LrWpanPhyEnumeration,uint8_t > PlmeEdConfirmCallback;

/**
 * This method implements the PD SAP: PlmeGetAttributeConfirm
 *
 * @param status the status of PlmeGetAttributeRequest
 * @param id the identifier of attribute
 * @param attribute the pointer to attribute struct
 */
typedef Callback< void, LrWpanPhyEnumeration,
                  LrWpanPibAttributeIdentifier,
                  LrWpanPhyPibAttributes* > PlmeGetAttributeConfirmCallback;

/**
 * This method implements the PD SAP: PlmeSetTRXStateConfirm
 *
 * @param status the status of PlmeSetTRXStateRequest
 */
typedef Callback< void, LrWpanPhyEnumeration > PlmeSetTRXStateConfirmCallback;

/**
 * This method implements the PD SAP: PlmeSetAttributeConfirm
 *
 * @param status the status of PlmeSetAttributeRequest
 * @param id the identifier of attribute
 */
typedef Callback< void, LrWpanPhyEnumeration,
                  LrWpanPibAttributeIdentifier > PlmeSetAttributeConfirmCallback;

/**
 * \ingroup lr-wpan
 *
 * Make LrWpanPhy a SpectrumPhy so we can enable the eventual modeling of
 * device interference
 */
class LrWpanPhy : public SpectrumPhy
{

public:
  /**
   * Get the type ID.
   *
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * The maximum packet size accepted by the PHY.
   * See Table 22 in section 6.4.1 of IEEE 802.15.4-2006
   */
  static const uint32_t aMaxPhyPacketSize;

  /**
   * The turnaround time for switching the transceiver from RX to TX or vice
   * versa.
   * See Table 22 in section 6.4.1 of IEEE 802.15.4-2006
   */
  static const uint32_t aTurnaroundTime;

  /**
   * Default constructor.
   */
  LrWpanPhy (void);
  virtual ~LrWpanPhy (void);

  // inherited from SpectrumPhy
  void SetMobility (Ptr<MobilityModel> m);
  Ptr<MobilityModel> GetMobility (void);
  void SetChannel (Ptr<SpectrumChannel> c);

  /**
   * Get the currently attached channel.
   *
   * \return the channel
   */
  Ptr<SpectrumChannel> GetChannel (void);
  void SetDevice (Ptr<NetDevice> d);
  Ptr<NetDevice> GetDevice (void);

  /**
   * Set the attached antenna.
   *
   * \param a the antenna
   */
  void SetAntenna (Ptr<AntennaModel> a);
  Ptr<AntennaModel> GetRxAntenna (void);
  virtual Ptr<const SpectrumModel> GetRxSpectrumModel (void) const;

  /**
   * Set the Power Spectral Density of outgoing signals in W/Hz.
   *
   * @param txPsd the Power Spectral Density value
   */
  void SetTxPowerSpectralDensity (Ptr<SpectrumValue> txPsd);

  /**
   * Set the noise power spectral density.
   *
   * @param noisePsd the Noise Power Spectral Density in power units
   * (Watt, Pascal...) per Hz.
   */
  void SetNoisePowerSpectralDensity (Ptr<const SpectrumValue> noisePsd);

  /**
   * Get the noise power spectral density.
   *
   * @return the Noise Power Spectral Density
   */
  Ptr<const SpectrumValue> GetNoisePowerSpectralDensity (void);

  /**
    * Notify the SpectrumPhy instance of an incoming waveform.
    *
    * @param params the SpectrumSignalParameters associated with the incoming waveform
    */
  virtual void StartRx (Ptr<SpectrumSignalParameters> params);

  /**
   *  IEEE 802.15.4-2006 section 6.2.1.1
   *  PD-DATA.request
   *  Request to transfer MPDU from MAC (transmitting)
   *  @param psduLength number of bytes in the PSDU
   *  @param p the packet to be transmitted
   */
  void PdDataRequest (const uint32_t psduLength, Ptr<Packet> p);

  /**
   *  IEEE 802.15.4-2006 section 6.2.2.1
   *  PLME-CCA.request
   *  Perform a CCA per section 6.9.9
   */
  void PlmeCcaRequest (void);

  /**
   *  IEEE 802.15.4-2006 section 6.2.2.3
   *  PLME-ED.request
   *  Perform an ED per section 6.9.7
   */
  void PlmeEdRequest (void);

  /**
   *  IEEE 802.15.4-2006 section 6.2.2.5
   *  PLME-GET.request
   *  Get attributes per definition from Table 23 in section 6.4.2
   *  @param id the attributed identifier
   */
  void PlmeGetAttributeRequest (LrWpanPibAttributeIdentifier id);

  /**
   *  IEEE 802.15.4-2006 section 6.2.2.7
   *  PLME-SET-TRX-STATE.request
   *  Set PHY state
   *  @param state in RX_ON,TRX_OFF,FORCE_TRX_OFF,TX_ON
   */
  void PlmeSetTRXStateRequest (LrWpanPhyEnumeration state);

  /**
   *  IEEE 802.15.4-2006 section 6.2.2.9
   *  PLME-SET.request
   *  Set attributes per definition from Table 23 in section 6.4.2
   *  @param id the attributed identifier
   *  @param * attribute the attribute value
   */
  void PlmeSetAttributeRequest (LrWpanPibAttributeIdentifier id, LrWpanPhyPibAttributes* attribute);

  /**
   * set the callback for the end of a RX, as part of the
   * interconnections betweenthe PHY and the MAC. The callback
   * implements PD Indication SAP.
   * @param c the callback
   */
  void SetPdDataIndicationCallback (PdDataIndicationCallback c);

  /**
   * set the callback for the end of a TX, as part of the
   * interconnections betweenthe PHY and the MAC. The callback
   * implements PD SAP.
   * @param c the callback
   */
  void SetPdDataConfirmCallback (PdDataConfirmCallback c);

  /**
   * set the callback for the end of a CCA, as part of the
   * interconnections betweenthe PHY and the MAC. The callback
   * implement PLME CCA confirm SAP
   * @param c the callback
   */
  void SetPlmeCcaConfirmCallback (PlmeCcaConfirmCallback c);

  /**
   * set the callback for the end of an ED, as part of the
   * interconnections betweenthe PHY and the MAC. The callback
   * implement PLME ED confirm SAP
   * @param c the callback
   */
  void SetPlmeEdConfirmCallback (PlmeEdConfirmCallback c);

  /**
   * set the callback for the end of an GetAttribute, as part of the
   * interconnections betweenthe PHY and the MAC. The callback
   * implement PLME GetAttribute confirm SAP
   * @param c the callback
   */
  void SetPlmeGetAttributeConfirmCallback (PlmeGetAttributeConfirmCallback c);

  /**
   * set the callback for the end of an SetTRXState, as part of the
   * interconnections betweenthe PHY and the MAC. The callback
   * implement PLME SetTRXState confirm SAP
   * @param c the callback
   */
  void SetPlmeSetTRXStateConfirmCallback (PlmeSetTRXStateConfirmCallback c);

  /**
   * set the callback for the end of an SetAttribute, as part of the
   * interconnections betweenthe PHY and the MAC. The callback
   * implement PLME SetAttribute confirm SAP
   * @param c the callback
   */
  void SetPlmeSetAttributeConfirmCallback (PlmeSetAttributeConfirmCallback c);

  /**
   * implement PLME SetAttribute confirm SAP
   * @param isData is true for data rate or false for symbol rate
   * @return the rate value of this PHY
   */
  double GetDataOrSymbolRate (bool isData);

  /**
   * set the error model to use
   *
   * @param e pointer to LrWpanErrorModel to use
   */
  void SetErrorModel (Ptr<LrWpanErrorModel> e);

  /**
   * get the error model in use
   *
   * @return pointer to LrWpanErrorModel in use
   */
  Ptr<LrWpanErrorModel> GetErrorModel (void) const;
  
  /**
   * Get the duration of the SHR (preamble and SFD) in symbols, depending on
   * the currently selected channel.
   *
   * \return the SHR duration in symbols
   */
  uint64_t GetPhySHRDuration (void) const;

  /**
   * Get the number of symbols per octet, depending on the currently selected
   * channel.
   *
   * \return the number of symbols per octet
   */
  double GetPhySymbolsPerOctet (void) const;
  
  /**
   * Assign a fixed random variable stream number to the random variables
   * used by this model.  Return the number of streams that have been assigned.
   *
   * \param stream first stream index to use
   * \return the number of stream indices assigned by this model
   */
  int64_t AssignStreams (int64_t stream);

protected:
  /**
   * The data and symbol rates for the different PHY options.
   * See Table 2 in section 6.1.2 IEEE 802.15.4-2006
   */
  static const LrWpanPhyDataAndSymbolRates dataSymbolRates[7];
  /**
   * The preamble, SFD, and PHR lengths in symbols for the different PHY options.
   * See Table 19 and Table 20 in section 6.3 IEEE 802.15.4-2006
   */
  static const LrWpanPhyPpduHeaderSymbolNumber ppduHeaderSymbolNumbers[7];

private:
  /**
   * The second is true if the first is flagged as error/invalid.
   */
  typedef std::pair<Ptr<Packet>, bool>  PacketAndStatus;

  // Inherited from Object.
  virtual void DoDispose (void);

  /**
   * Change the PHY state to the given new state, firing the state change trace.
   *
   * \param newState the new state
   */
  void ChangeTrxState (LrWpanPhyEnumeration newState);

  /**
   * Configure the PHY option according to the current channel and channel page.
   * See IEEE 802.15.4-2006, section 6.1.2, Table 2.
   */
  void SetMyPhyOption (void);

  /**
   * Get the currently configured PHY option.
   * See IEEE 802.15.4-2006, section 6.1.2, Table 2.
   *
   * \return the PHY option
   */
  LrWpanPhyOption GetMyPhyOption (void);

  /**
   * Finish the transmission of a frame. This is called at the end of a frame
   * transmission, applying possibly pending PHY state changes and fireing the
   * appropriate trace sources and confirm callbacks to the MAC.
   */
  void EndTx (void);

  /**
   * Check if the interference destroys a frame currently received. Called
   * whenever a change in interference is detected.
   */
  void CheckInterference (void);

  /**
   * Finish the reception of a frame. This is called at the end of a frame
   * reception, applying possibly pending PHY state changes and fireing the
   * appropriate trace sources and indication callbacks to the MAC. A frame
   * destroyed by noise/interference is dropped here, but not during reception.
   * This method is also called for every packet which only contributes to
   * interference.
   *
   * \param params signal parameters of the packet
   */
  void EndRx (Ptr<LrWpanSpectrumSignalParameters> params);

  /**
   * Cancel an ongoing ED procedure. This is called when the transceiver is
   * switched off or set to TX mode. This calls the appropiate confirm callback
   * of the MAC.
   *
   * \param state the new state which is the cause for canceling ED
   */
  void CancelEd (LrWpanPhyEnumeration state);

  /**
   * Called at the end of the ED procedure. The average energy detected is
   * reported to the MAC.
   */
  void EndEd (void);

  /**
   * Called at the end of the CCA. The channel condition (busy or idle) is
   * reported to the MAC or CSMA/CA.
   */
  void EndCca (void);

  /**
   * Called after applying a deferred transceiver state switch. The result of
   * the state switch is reported to the MAC.
   */
  void EndSetTRXState (void);

  /**
   * Calculate the time required for sending the given packet, including
   * preamble, SFD and PHR.
   *
   * \param packet the packet for which the transmission time should be calculated
   */
  Time CalculateTxTime (Ptr<const Packet> packet);

  /**
   * Calculate the time required for sending the PPDU header, that is the
   * preamble, SFD and PHR.
   */
  Time GetPpduHeaderTxTime (void);

  /**
   * Check if the given channel is supported by the PHY.
   *
   * \param channel the channel to check
   * \return true, if the channel is supported, false otherwise
   */
  bool ChannelSupported (uint8_t channel);

  /**
   * Check if the PHY is busy, which is the case if the PHY is currently sending
   * or receiving a frame.
   *
   * \return true, if the PHY is busy, false otherwise
   */
  bool PhyIsBusy (void) const;

  // Trace sources
  /**
   * The trace source fired when a packet begins the transmission process on
   * the medium.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_phyTxBeginTrace;

  /**
   * The trace source fired when a packet ends the transmission process on
   * the medium.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_phyTxEndTrace;

  /**
   * The trace source fired when the phy layer drops a packet as it tries
   * to transmit it.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_phyTxDropTrace;

  /**
   * The trace source fired when a packet begins the reception process from
   * the medium.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_phyRxBeginTrace;

  /**
   * The trace source fired when a packet ends the reception process from
   * the medium.  Second quantity is received SINR.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet>, double > m_phyRxEndTrace;

  /**
   * The trace source fired when the phy layer drops a packet it has received.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_phyRxDropTrace;

  Ptr<MobilityModel> m_mobility;
  Ptr<NetDevice> m_device;
  Ptr<SpectrumChannel> m_channel;
  Ptr<AntennaModel> m_antenna;
  Ptr<SpectrumValue> m_txPsd;
  Ptr<const SpectrumValue> m_noise;
  Ptr<LrWpanErrorModel> m_errorModel;
  LrWpanPhyPibAttributes m_phyPIBAttributes;

  // State variables
  LrWpanPhyEnumeration m_trxState;  /// transceiver state
  TracedCallback<Time, LrWpanPhyEnumeration, LrWpanPhyEnumeration> m_trxStateLogger;
  LrWpanPhyEnumeration m_trxStatePending;  /// pending state change

  PdDataIndicationCallback m_pdDataIndicationCallback;
  PdDataConfirmCallback m_pdDataConfirmCallback;
  PlmeCcaConfirmCallback m_plmeCcaConfirmCallback;
  PlmeEdConfirmCallback m_plmeEdConfirmCallback;
  PlmeGetAttributeConfirmCallback m_plmeGetAttributeConfirmCallback;
  PlmeSetTRXStateConfirmCallback m_plmeSetTRXStateConfirmCallback;
  PlmeSetAttributeConfirmCallback m_plmeSetAttributeConfirmCallback;
  LrWpanPhyOption m_phyOption;
  LrWpanEdPower m_edPower;
  double m_ccaPeakPower;
  double m_rxSensitivity;
  Ptr<LrWpanInterferenceHelper> m_signal;
  Time m_rxLastUpdate;
  std::pair<Ptr<LrWpanSpectrumSignalParameters>, bool>  m_currentRxPacket;
  PacketAndStatus m_currentTxPacket;

  EventId m_ccaRequest;
  EventId m_edRequest;
  EventId m_setTRXState;
  EventId m_pdDataRequest;

  /**
   * Uniform random variable stream.
   */
  Ptr<UniformRandomVariable> m_random;
};


} // namespace ns3

#endif /* LR_WPAN_PHY_H */
