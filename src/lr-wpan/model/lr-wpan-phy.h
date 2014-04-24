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
#include <ns3/random-variable.h>

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
  // The second is true if the first is flagged as error
  typedef std::pair<Ptr<Packet>, bool>  PacketAndStatus;

  static TypeId GetTypeId (void);

  static const uint32_t aMaxPhyPacketSize; // Table 22 in section 6.4.1 of ieee802.15.4
  static const uint32_t aTurnaroundTime;   // Table 22 in section 6.4.1 of ieee802.15.4

  LrWpanPhy ();
  virtual ~LrWpanPhy ();

  // inherited from SpectrumPhy
  void SetMobility (Ptr<MobilityModel> m);
  Ptr<MobilityModel> GetMobility ();
  void SetChannel (Ptr<SpectrumChannel> c);
  Ptr<SpectrumChannel> GetChannel (void);
  void SetDevice (Ptr<NetDevice> d);
  Ptr<NetDevice> GetDevice ();
  void SetAntenna (Ptr<AntennaModel> a);
  Ptr<AntennaModel> GetRxAntenna ();
  virtual Ptr<const SpectrumModel> GetRxSpectrumModel () const;

  /**
   * set the Power Spectral Density of outgoing signals in W/Hz.
   *
   * @param txPsd
   */
  void SetTxPowerSpectralDensity (Ptr<SpectrumValue> txPsd);

  /**
   * \brief set the noise power spectral density
   * @param noisePsd the Noise Power Spectral Density in power units
   * (Watt, Pascal...) per Hz.
   */
  void SetNoisePowerSpectralDensity (Ptr<const SpectrumValue> noisePsd);

  /**
   * \brief get the noise power spectral density
   * @return the Noise Power Spectral Density
   */
  Ptr<const SpectrumValue> GetNoisePowerSpectralDensity (void);

  /**
    * Notify the SpectrumPhy instance of an incoming waveform
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
  
  uint64_t GetPhySHRDuration (void) const;
  double GetPhySymbolsPerOctet (void) const;
  
protected:
  static const LrWpanPhyDataAndSymbolRates dataSymbolRates[7];
  static const LrWpanPhyPpduHeaderSymbolNumber ppduHeaderSymbolNumbers[7];

private:
  virtual void DoDispose (void);
  void ChangeState (LrWpanPhyEnumeration newState);
  void ChangeTrxState (LrWpanPhyEnumeration newState);
  void SetMyPhyOption (void);
  LrWpanPhyOption GetMyPhyOption (void);
  void EndTx ();
  void CheckInterference ();

  void EndRx (Ptr<LrWpanSpectrumSignalParameters> params);
  void EndEd ();
  void EndCca ();
  void EndSetTRXState ();
  Time CalculateTxTime (Ptr<const Packet> packet);
  Time GetPpduHeaderTxTime (void);
  bool ChannelSupported (uint8_t);
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
  bool PhyIsBusy (void) const;  /// helper function

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
  UniformVariable m_random;
};


} // namespace ns3

#endif /* LR_WPAN_PHY_H */
