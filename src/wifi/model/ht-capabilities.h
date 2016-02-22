/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013
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
 * Authors: Ghada Badawy <gbadawy@gmail.com>
 *          Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#ifndef HT_CAPABILITIES_H
#define HT_CAPABILITIES_H

#include <stdint.h>
#include "ns3/buffer.h"
#include "ns3/attribute-helper.h"
#include "ns3/wifi-information-element.h"

/**
 * This defines the maximum number of supported MCSs that a STA is
 * allowed to have. Currently this number is set for IEEE 802.11n
 */
#define MAX_SUPPORTED_MCS  (77)

namespace ns3 {

/**
 * \brief The HT Capabilities Information Element
 * \ingroup wifi
 *
 * This class knows how to serialise and deserialise the HT Capabilities Information Element
 *
 * \see attribute_HtCapabilities
 */
class HtCapabilities : public WifiInformationElement
{
public:
  HtCapabilities ();
  void SetHtSupported (uint8_t htsupported);

  /**
   * Set the HT Capabilties Info field in the HT Capabilities information element.
   *
   * \param ctrl the HT Capabilties Info field in the HT Capabilities information element
   */
  void SetHtCapabilitiesInfo (uint16_t ctrl);
  /**
   * Set the A-MPDU Parameters field in the HT Capabilities information element.
   *
   * \param ctrl the A-MPDU Parameters field in the HT Capabilities information element
   */
  void SetAmpduParameters (uint8_t ctrl);
  /**
   * Set the Supported MCS Set field in the HT Capabilities information element.
   *
   * \param ctrl1 the first 64 bytes of the Supported MCS Set field in the HT Capabilities information element
   * \param ctrl2 the last 64 bytes of the Supported MCS Set field in the HT Capabilities information element
   */
  void SetSupportedMcsSet (uint64_t ctrl1, uint64_t ctrl2);
  /**
   * Set the Extended HT Capabilties field in the HT Capabilities information element.
   *
   * \param ctrl the Extended HT Capabilties field in the HT Capabilities information element
   */
  void SetExtendedHtCapabilities (uint16_t ctrl);
  /**
   * Set the Transmit Beamforming (TxBF) Capabilties field in the HT Capabilities information element.
   *
   * \param ctrl the Transmit Beamforming (TxBF) Capabilties field in the HT Capabilities information element
   */
  void SetTxBfCapabilities (uint32_t ctrl);
  /**
   * Set the the Antenna Selection (ASEL) Capabilties field in the HT Capabilities information element.
   *
   * \param ctrl the Antenna Selection (ASEL) Capabilties field in the HT Capabilities information element
   */
  void SetAntennaSelectionCapabilities (uint8_t ctrl);

  void SetLdpc (uint8_t ldpc);
  void SetSupportedChannelWidth (uint8_t supportedchannelwidth);
  void SetGreenfield (uint8_t greenfield);
  void SetShortGuardInterval20 (uint8_t shortguardinterval);
  void SetShortGuardInterval40 (uint8_t shortguardinterval);
  void SetMaxAmsduLength (uint8_t maxamsdulength);
  void SetLSigProtectionSupport (uint8_t lsigprotection);

  void SetMaxAmpduLength (uint8_t maxampdulength);

  void SetRxMcsBitmask (uint8_t index);
  void SetRxHighestSupportedDataRate (uint16_t maxsupportedrate);
  void SetTxMcsSetDefined (uint8_t txmcssetdefined);
  void SetTxRxMcsSetUnequal (uint8_t txrxmcssetunequal);
  void SetTxMaxNSpatialStreams (uint8_t maxtxspatialstreams);
  void SetTxUnequalModulation (uint8_t txunequalmodulation);

  /*
   * Return the HT Capabilties Info field in the HT Capabilities information element.
   *
   * \return the HT Capabilties Info field in the HT Capabilities information element
   */
  uint16_t GetHtCapabilitiesInfo (void) const;
  /*
   * Return the A-MPDU Parameters field in the HT Capabilities information element.
   *
   * \return the A-MPDU Parameters field in the HT Capabilities information element
   */
  uint8_t GetAmpduParameters (void) const;
  /*
   * Return the first 64 bytes of the Supported MCS Set field in the HT Capabilities information element.
   *
   * \return the first 64 bytes of the Supported MCS Set field in the HT Capabilities information element
   */
  uint64_t GetSupportedMcsSet1 (void) const;
  /*
   * Return the last 64 bytes of the Supported MCS Set field in the HT Capabilities information element.
   *
   * \return the last 64 bytes of the Supported MCS Set field in the HT Capabilities information element
   */
  uint64_t GetSupportedMcsSet2 (void) const;
  /*
   * Return the Extended HT Capabilties field in the HT Capabilities information element.
   *
   * \return the Extended HT Capabilties field in the HT Capabilities information element
   */
  uint16_t GetExtendedHtCapabilities (void) const;
  /*
   * Return the Transmit Beamforming (TxBF) Capabilties field in the HT Capabilities information element.
   *
   * \return the Transmit Beamforming (TxBF) Capabilties field in the HT Capabilities information element
   */
  uint32_t GetTxBfCapabilities (void) const;
  /*
   * Return the Antenna Selection (ASEL) Capabilties field in the HT Capabilities information element.
   *
   * \return the Antenna Selection (ASEL) Capabilties field in the HT Capabilities information element
   */
  uint8_t GetAntennaSelectionCapabilities (void) const;

  uint8_t GetLdpc (void) const;
  uint8_t GetSupportedChannelWidth (void) const;
  uint8_t GetGreenfield (void) const;
  uint8_t GetShortGuardInterval20 (void) const;
  uint8_t GetShortGuardInterval40 (void) const;
  uint8_t GetMaxAmsduLength (void) const;
  uint8_t GetLSigProtectionSupport (void) const;

  uint8_t GetMaxAmpduLength (void) const;

  uint8_t* GetRxMcsBitmask ();
  bool IsSupportedMcs (uint8_t mcs);
  uint16_t GetRxHighestSupportedDataRate (void) const;
  uint8_t GetTxMcsSetDefined (void) const;
  uint8_t GetTxRxMcsSetUnequal (void) const;
  uint8_t GetTxMaxNSpatialStreams (void) const;
  uint8_t GetTxUnequalModulation (void) const;

  WifiInformationElementId ElementId () const;
  uint8_t GetInformationFieldSize () const;
  void SerializeInformationField (Buffer::Iterator start) const;
  uint8_t DeserializeInformationField (Buffer::Iterator start,
                                       uint8_t length);
  /**
   * This information element is a bit special in that it is only
   * included if the STA is an HT STA. To support this we
   * override the Serialize and GetSerializedSize methods of
   * WifiInformationElement.
   *
   * \param start
   *
   * \return an iterator
   */
  Buffer::Iterator Serialize (Buffer::Iterator start) const;
  /**
   * Return the serialized size of this HT capability
   * information element.
   *
   * \return the serialized size of this HT capability information element
   */
  uint16_t GetSerializedSize () const;


private:
  //HT Capabilties Info field
  uint8_t m_ldpc;
  uint8_t m_supportedChannelWidth;
  uint8_t m_smPowerSave;
  uint8_t m_greenField;
  uint8_t m_shortGuardInterval20;
  uint8_t m_shortGuardInterval40;
  uint8_t m_txStbc;
  uint8_t m_rxStbc;
  uint8_t m_htDelayedBlockAck;
  uint8_t m_maxAmsduLength;
  uint8_t m_dssMode40;
  uint8_t m_psmpSupport;
  uint8_t m_fortyMhzIntolerant;
  uint8_t m_lsigProtectionSupport;

  //A-MPDU Parameters field
  uint8_t m_maxAmpduLength;
  uint8_t m_minMpduStartSpace;
  uint8_t m_ampduReserved;

  //Supported MCS Set field
  uint8_t m_reservedMcsSet1;
  uint16_t m_rxHighestSupportedDataRate;
  uint8_t m_reservedMcsSet2;
  uint8_t m_txMcsSetDefined;
  uint8_t m_txRxMcsSetUnequal;
  uint8_t m_txMaxNSpatialStreams;
  uint8_t m_txUnequalModulation;
  uint32_t m_reservedMcsSet3;
  uint8_t m_rxMcsBitmask[MAX_SUPPORTED_MCS];

  //HT Extended Capabilties field
  uint8_t m_pco;
  uint8_t m_pcoTransitionTime;
  uint8_t m_reservedExtendedCapabilities;
  uint8_t m_mcsFeedback;
  uint8_t m_htcSupport;
  uint8_t m_reverzeDirectionResponder;
  uint8_t m_reservedExtendedCapabilities2;

  //Transmit Beamforming Capabilities field
  uint8_t m_implicitRxBfCapable;
  uint8_t m_rxStaggeredSoundingCapable;
  uint8_t m_txStaggeredSoundingCapable;
  uint8_t m_rxNdpCapable;
  uint8_t m_txNdpCapable;
  uint8_t m_implicitTxBfCapable;
  uint8_t m_calibration;
  uint8_t m_explicitCsiTxBfCapable;
  uint8_t m_explicitNoncompressedSteeringCapable;
  uint8_t m_explicitCompressedSteeringCapable;
  uint8_t m_explicitTxBfCsiFeedback;
  uint8_t m_explicitNoncompressedBfFeedbackCapable;
  uint8_t m_explicitCompressedBfFeedbackCapable;
  uint8_t m_minimalGrouping;
  uint8_t m_csiNBfAntennasSupported;
  uint8_t m_noncompressedSteeringNBfAntennasSupported;
  uint8_t m_compressedSteeringNBfAntennasSupported;
  uint8_t m_csiMaxNRowsBfSupported;
  uint8_t m_channelEstimationCapability;
  uint8_t m_reservedTxBf;

  //ASEL Capabilities field
  uint8_t m_antennaSelectionCapability;
  uint8_t m_explicitCsiFeedbackBasedTxASelCapable;
  uint8_t m_antennaIndicesFeedbackBasedTxASelCapable;
  uint8_t m_explicitCsiFeedbackCapable;
  uint8_t m_antennaIndicesFeedbackCapable;
  uint8_t m_rxASelCapable;
  uint8_t m_txSoundingPpdusCapable;
  uint8_t m_reservedASel;

  //This is used to decide whether this element should be added to the frame or not
  uint8_t m_htSupported;
};

std::ostream &operator << (std::ostream &os, const HtCapabilities &htcapabilities);
std::istream &operator >> (std::istream &is, HtCapabilities &htcapabilities);

ATTRIBUTE_HELPER_HEADER (HtCapabilities);

} //namespace ns3

#endif /* HT_CAPABILITY_H */
