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
 * Author: Gary Pei <guangyu.pei@boeing.com>
 */
#ifndef LR_WPAN_PHY_H
#define LR_WPAN_PHY_H

#include <stdint.h>
#include <math.h>
#include <ns3/callback.h>
#include "ns3/object.h"
#include <ns3/packet-burst.h>
#include <ns3/spectrum-phy.h>
#include <ns3/spectrum-channel.h>
#include <ns3/spectrum-type.h>
#include <ns3/spectrum-interference.h>
#include <ns3/spectrum-value.h>
#include <ns3/lr-wpan-spectrum-value-helper.h>
#include <ns3/mobility-model.h>
#include <ns3/packet.h>
#include <ns3/nstime.h>
#include <ns3/net-device.h>
#include <ns3/event-id.h>

namespace ns3 {

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
  IEEE_802_15_4_PHY_READ_OLNY = 0xb,
  IEEE_802_15_4_PHY_UNSPECIFIED = 0xc // all cases not covered by ieee802.15.4
} LrWpanPhyEnumeration;

/** IEEE802.15.4-2006 PHY PIB Attributes Table 23
 * in section 6.4.2
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
} LrWpanPhyPIBAttributes;

/**
 * This method implements the PD SAP: PdDataIndication
 *
 *  @param psduLength number of bytes in the PSDU
 *  @param p the packet to be transmitted
 *  @param lqi Link quality (LQI) value measured during reception of the PPDU
 */
typedef Callback< void, uint32_t, Ptr<Packet>, uint32_t > PdDataIndicationCallback;

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
                  LrWpanPhyPIBAttributes* > PlmeGetAttributeConfirmCallback;

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
 * Make LrWpanPhy a SpectrumPhy so we can enable the eventual modeling of
 * device interference
 */
class LrWpanPhy : public SpectrumPhy
{

public:
  // The second is true if the first is flagged as error
  typedef std::pair<Ptr<Packet>, bool>  PacketAndStatus;

  static TypeId GetTypeId (void);

  LrWpanPhy ();
  virtual ~LrWpanPhy ();

  // inherited from SpectrumPhy
  void SetMobility (Ptr<Object> m);
  Ptr<Object> GetMobility ();
  void SetChannel (Ptr<SpectrumChannel> c);
  Ptr<SpectrumChannel> GetChannel (void);
  void SetDevice (Ptr<Object> d);
  Ptr<Object> GetDevice ();
  virtual Ptr<const SpectrumModel> GetRxSpectrumModel () const;
  /**
   * Get the SpectrumType used by this PHY
   *
   * @return
   */
  SpectrumType GetSpectrumType ();

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
    * @param p the PacketBurst associated with the incoming waveform
    * @param rxPsd the Power Spectral Density of the incoming
    * waveform. The units of the PSD are the same specified for SpectrumChannel::StartTx().
    * @param st spectrum type
    * @param duration the duration of the incoming waveform
    */
  virtual void StartRx (Ptr<PacketBurst> p, Ptr <const SpectrumValue> rxPsd, SpectrumType st, Time duration);

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
  void PlmeSetAttributeRequest (LrWpanPibAttributeIdentifier id, LrWpanPhyPIBAttributes* attribute);

  /**
   * Start a transmission
   *
   *
   * @param p the packet to be transmitted
   *
   * @return true if an error occurred and the transmission was not
   * started, false otherwise.
   */
  bool StartTx (Ptr<Packet> p);

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

protected:
  static const uint32_t aMaxPhyPacketSize; // Table 22 in section 6.4.1 of ieee802.15.4
  static const uint32_t aTurnaroundTime;   // Table 22 in section 6.4.1 of ieee802.15.4
  static const LrWpanPhyDataAndSymbolRates dataSymbolRates[7];
  static const LrWpanPhyPpduHeaderSymbolNumber ppduHeaderSymbolNumbers[7];

private:
  virtual void DoDispose (void);
  void ChangeState (LrWpanPhyEnumeration newState);
  void SetMyPhyOption (void);
  LrWpanPhyOption GetMyPhyOption (void);
  void EndTx ();
  void EndRx ();
  void EndEd ();
  void EndCca ();
  void EndSetTRXState ();
  Time CalculateTxTime (Ptr<const Packet> packet);
  double GetPpduHeaderTxTime (void);
  bool ChannelSupported (uint8_t);
  Ptr<Object> m_mobility;
  Ptr<Object> m_device;
  Ptr<SpectrumChannel> m_channel;
  Ptr<SpectrumValue> m_txPsd;
  Ptr<const SpectrumValue> m_rxPsd;
  Ptr<const SpectrumValue> m_noise;
  LrWpanPhyPIBAttributes m_phyPIBAttributes;
  LrWpanPhyEnumeration m_trxState;
  LrWpanPhyEnumeration m_trxStateDeferSet;
  LrWpanPhyEnumeration m_trxStateTurnaround;
  LrWpanPhyEnumeration m_sensedChannelState;
  LrWpanPhyEnumeration m_state;
  PdDataIndicationCallback m_pdDataIndicationCallback;
  PdDataConfirmCallback m_pdDataConfirmCallback;
  PlmeCcaConfirmCallback m_plmeCcaConfirmCallback;
  PlmeEdConfirmCallback m_plmeEdConfirmCallback;
  PlmeGetAttributeConfirmCallback m_plmeGetAttributeConfirmCallback;
  PlmeSetTRXStateConfirmCallback m_plmeSetTRXStateConfirmCallback;
  PlmeSetAttributeConfirmCallback m_plmeSetAttributeConfirmCallback;
  LrWpanPhyOption m_phyOption;
  double m_rxEdPeakPower;
  double m_rxTotalPower;
  uint32_t m_rxTotalNum;
  double m_rxSensitivity;
  PacketAndStatus m_currentRxPacket;
  PacketAndStatus m_currentTxPacket;

  EventId m_edRequest;
  EventId m_setTRXState;
  EventId m_pdDataRequest;
};


} // namespace ns3

#endif /* LR_WPAN_PHY_H */
