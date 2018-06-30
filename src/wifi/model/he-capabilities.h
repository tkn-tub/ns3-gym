/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016
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
 * Author: Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#ifndef HE_CAPABILITIES_H
#define HE_CAPABILITIES_H

#include "wifi-information-element.h"

namespace ns3 {

/**
 * \ingroup wifi
 *
 * The IEEE 802.11ax HE Capabilities
 */
class HeCapabilities : public WifiInformationElement
{
public:
  HeCapabilities ();
  /**
   * Set HE supported
   * \param hesupported the HE supported indicator
   */
  void SetHeSupported (uint8_t hesupported);
  /**
   * Get the wifi information element ID
   * \returns the wifi information element ID
   */
  WifiInformationElementId ElementId () const;
  /**
   * Get information field size
   * \returns the information field size
   */
  uint8_t GetInformationFieldSize () const;
  /**
   * Serialize information field
   * \param start the information field iterator
   */
  void SerializeInformationField (Buffer::Iterator start) const;
  /**
   * Deserialize information field
   * \param start the information field iterator
   * \param length the length
   * \returns the information field size
   */
  uint8_t DeserializeInformationField (Buffer::Iterator start, uint8_t length);

  /**
   * Set the HE MAC Capabilities Info field in the HE Capabilities information element.
   *
   * \param ctrl1 the HE MAC Capabilities Info field 1 in the HE Capabilities information element
   * \param ctrl2 the HE MAC Capabilities Info field 2 in the HE Capabilities information element
   */
  void SetHeMacCapabilitiesInfo (uint32_t ctrl1, uint8_t ctrl2);
  /**
   * Set the HE PHY Capabilities Info field in the HE Capabilities information element.
   *
   * \param ctrl1 the HE PHY Capabilities Info field 1 in the HE Capabilities information element
   * \param ctrl2 the HE PHY Capabilities Info field 2 in the HE Capabilities information element
   */
  void SetHePhyCapabilitiesInfo (uint64_t ctrl1, uint8_t ctrl2);
  /**
   * Set the MCS and NSS field in the HE Capabilities information element.
   *
   * \param ctrl the MCS and NSS field in the HE Capabilities information element
   */
  void SetSupportedMcsAndNss (uint16_t ctrl);

  /**
   * Return the 4 first octets of the HE MAC Capabilities Info field in the HE Capabilities information element.
   *
   * \return the 4 first octets of the HE MAC Capabilities Info field in the HE Capabilities information element
   */
  uint32_t GetHeMacCapabilitiesInfo1 () const;
  /**
   * Return the last octet of the HE MAC Capabilities Info field in the HE Capabilities information element.
   *
   * \return the last octet of the HE MAC Capabilities Info field in the HE Capabilities information element
   */
  uint8_t GetHeMacCapabilitiesInfo2 () const;
  /**
   * Return the 8 first octets of the HE PHY Capabilities Info field in the HE Capabilities information element.
   *
   * \return the 8 first octets of the HE PHY Capabilities Info field in the HE Capabilities information element
   */
  uint64_t GetHePhyCapabilitiesInfo1 () const;
  /**
   * Return the last octet of the HE PHY Capabilities Info field in the HE Capabilities information element.
   *
   * \return the last octet of the HE PHY Capabilities Info field in the HE Capabilities information element
   */
  uint8_t GetHePhyCapabilitiesInfo2 () const;
  /**
   * Return the MCS and NSS field in the HE Capabilities information element.
   *
   * \return the MCS and NSS field in the HE Capabilities information element
   */
  uint16_t GetSupportedMcsAndNss () const;

  // PHY Capabilities Info fields
  /**
   * Set channel width set.
   *
   * \param channelWidthSet the channel width set
   */
  void SetChannelWidthSet (uint8_t channelWidthSet);
  /**
   * Set HE LTF and GI for HE PDPUs.
   *
   * \param heLtfAndGiForHePpdus the HE LTF and GI
   */
  void SetHeLtfAndGiForHePpdus (uint8_t heLtfAndGiForHePpdus);
  /**
   * Get channel width set.
   *
   * \returns the channel width set
   */
  uint8_t GetChannelWidthSet (void) const;
  /**
   * Get HE LTF and GI for HE PDPUs.
   *
   * \returns the HE LTF and GI
   */
  uint8_t GetHeLtfAndGiForHePpdus (void) const;
  /**
   * Get highest MCS supported.
   *
   * \returns the highest MCS is supported
   */
  uint8_t GetHighestMcsSupported (void) const;
  /**
   * Get highest NSS supported.
   *
   * \returns the highest supported NSS
   */
  uint8_t GetHighestNssSupported (void) const;

  // MAC Capabilities Info fields
  /**
   * Set maximum AMPDU length exponent.
   *
   * \param exponent the maximum AMPDU length exponent
   */
  void SetMaxAmpduLengthExponent (uint8_t exponent);

  // MCS and NSS field information
  /**
   * Set highest MCS supported.
   *
   * \param mcs the MCS
   */
  void SetHighestMcsSupported (uint8_t mcs);
  /**
   * Set highest NSS supported.
   *
   * \param nss the NSS
   */
  void SetHighestNssSupported (uint8_t nss);

  /**
   * Is transmit MCS supported.
   *
   * \param mcs the MCS
   * \returns true if MCS transmit supported
   */
  bool IsSupportedTxMcs (uint8_t mcs) const;
  /**
   * Is reeive MCS supported.
   *
   * \param mcs the MCS
   * \returns true if MCS receive supported
   */
  bool IsSupportedRxMcs (uint8_t mcs) const;

  /*
   * This information element is a bit special in that it is only
   * included if the STA is an HE STA. To support this we
   * override the Serialize and GetSerializedSize methods of
   * WifiInformationElement.
   */

  /**
   * Serialize function.
   *
   * \param start the information element iterator
   * \returns true if MCS transmit supported
   */
  Buffer::Iterator Serialize (Buffer::Iterator start) const;
  /**
   * Set serialize size function.
   *
   * \returns the serialized size
   */
  uint16_t GetSerializedSize () const;


private:
  //MAC Capabilities Info fields
  uint8_t m_plusHtcHeSupport; //!< HTC HE support
  uint8_t m_twtRequesterSupport; //!< TWT requester support
  uint8_t m_twtResponderSupport; //!< TWT responder support
  uint8_t m_fragmentationSupport; //!< fragmentation support
  uint8_t m_maximumNumberOfFragmentedMsdus; //!< maximum number of fragmentation MSDUs
  uint8_t m_minimumFragmentSize; //!< minimum fragment size
  uint8_t m_triggerFrameMacPaddingDuration; //!< trigger frame MAC padding duration
  uint8_t m_multiTidAggregationSupport; //!< multi TID aggregation support
  uint8_t m_heLinkAdaptation; //!< HE link adaptation
  uint8_t m_allAckSupport; //!< ack support
  uint8_t m_ulMuResponseSchedulingSupport; //!< UL MU response scheduling support
  uint8_t m_aBsrSupport; //!< BSR support
  uint8_t m_broadcastTwtSupport; //!< broadcast TXT support
  uint8_t m_32bitBaBitmapSupport; //!< 32bit BA bitmap support
  uint8_t m_muCascadeSupport; //!< MU cascade support
  uint8_t m_ackEnabledMultiTidAggregationSupport; //!< ack enabled multi TID aggregation support
  uint8_t m_groupAddressedMultiStaBlockAckInDlMuSupport; //!< group addressed multi STA block ack in DL support
  uint8_t m_omiAcontrolSupport; //!< OMI A control support
  uint8_t m_ofdmaRaSupport; //!< OFDM RA support
  uint8_t m_maximumAmpduLengthExponent; //!< maximum AMPDU length exponenet
  uint8_t m_amsduFragmentationSupport; //!< AMSDU fragmentation support
  uint8_t m_flexibleTwtScheduleSupport; //!< flexible TWT schedule support
  uint8_t m_rxControlFrameToMultiBss; //!< receive control frame to multi bias
  uint8_t m_bsrpAmpduAggregation; //!< BSRP AMPDU aggregation
  uint8_t m_qtpSupport; //!< QTP support
  uint8_t m_aBqrSupport; //!< ABQR support

  //PHY Capabilities Info fields
  uint8_t m_dualBandSupport; //!< dual band support
  uint8_t m_channelWidthSet; //!< channel width set
  uint8_t m_preamblePuncturingRx; //!< preamble puncturing receive
  uint8_t m_deviceClass; //!< device class
  uint8_t m_ldpcCodingInPayload; //!< LDPC coding in payload
  uint8_t m_heLtfAndGiForHePpdus; //!< HE LTF and GI for HE PDUs
  uint8_t m_heLtfAndGiForNdp; //!< HE LTF and GI for NDP
  uint8_t m_stbcTxAndRx; //!< STBC transmit and receive
  uint8_t m_doppler; //!< doppler
  uint8_t m_ulMu; //!< UL MU
  uint8_t m_dcmEncodingTx; //!< DCM encoding transmit
  uint8_t m_dcmEncodingRx; //!< DCM encoding receive
  uint8_t m_ulHeMuPpduPayloadSupport; //!< UL HE MU PDPU payload support
  uint8_t m_suBeamformer; //!< SU beam former
  uint8_t m_suBeamformee; //!< SU beam formee
  uint8_t m_muBeamformer; //!< MU beam former
  uint8_t m_beamformeeStsForSmallerOrEqualThan80Mhz; //!< beam formee STS for < 80 MHz
  uint8_t m_nstsTotalForSmallerOrEqualThan80Mhz; //!< STS total for < 80 MHz
  uint8_t m_beamformeeStsForLargerThan80Mhz; //!< beam formee STS for > 80MHz
  uint8_t m_nstsTotalForLargerThan80Mhz; //!< STS total for > 80 MHz
  uint8_t m_numberOfSoundingDimensionsForSmallerOrEqualThan80Mhz; //!< # of sounding diemnsions for < 80 MHz
  uint8_t m_numberOfSoundingDimensionsForLargerThan80Mhz; //!< # of sounding dimensions for > 80 MHz
  uint8_t m_ngEqual16ForSuFeedbackSupport; //!< equal 16 for SU feedback
  uint8_t m_ngEqual16ForMuFeedbackSupport; //!< equal 16 for MU feedback
  uint8_t m_codebookSize42ForSuSupport; //!< codebook size 42 for SU
  uint8_t m_codebookSize75ForSuSupport; //!< codebook size 75 for SU
  uint8_t m_beamformingFeedbackWithTriggerFrame; //!< beam forming feedback with trigger frame
  uint8_t m_heErSuPpduPayload; //!< HE ER SU PPDU payload
  uint8_t m_dlMuMimoOnPartialBandwidth; //!< DL MU MIMO on partial bandwidth
  uint8_t m_ppeThresholdPresent; //!< PPE threshold present
  uint8_t m_srpBasedSrSupport; //!< SRP based SR support
  uint8_t m_powerBoostFactorAlphaSupport; //!< power boost factor alpha support
  uint8_t m_4TimesHeLtfAnd800NsGiSupportForHePpdus; //!< 4 times HE LFT and 800 HS GI support for HE PPDUs

  //MCS and NSS field information
  uint8_t m_highestNssSupportedM1; //!< highest NSS support M1
  uint8_t m_highestMcsSupported; //!< highest MCS support
  std::vector<uint8_t> m_txBwMap; //!< transmit BW map
  std::vector<uint8_t> m_rxBwMap; //!< receive BW map

  /// This is used to decide if this element should be added to the frame or not
  uint8_t m_heSupported;
};

std::ostream &operator << (std::ostream &os, const HeCapabilities &HeCapabilities);

} //namespace ns3

#endif /* HE_CAPABILITY_H */
