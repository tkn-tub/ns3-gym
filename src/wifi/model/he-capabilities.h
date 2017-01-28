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

#include "ns3/buffer.h"
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
  void SetHeSupported (uint8_t hesupported);

  WifiInformationElementId ElementId () const;
  uint8_t GetInformationFieldSize () const;
  void SerializeInformationField (Buffer::Iterator start) const;
  uint8_t DeserializeInformationField (Buffer::Iterator start, uint8_t length);

  /**
   * Set the HE MAC Capabilties Info field in the HE Capabilities information element.
   *
   * \param ctrl the HE MAC Capabilties Info field in the HE Capabilities information element
   */
  void SetHeMacCapabilitiesInfo (uint32_t ctrl1, uint8_t ctrl2);
  /**
   * Set the HE PHY Capabilties Info field in the HE Capabilities information element.
   *
   * \param ctrl the HE PHY Capabilties Info field in the HE Capabilities information element
   */
  void SetHePhyCapabilitiesInfo (uint64_t ctrl1, uint8_t ctrl2);
  /**
   * Set the MCS and NSS field in the HE Capabilities information element.
   *
   * \param ctrl the MCS and NSS field in the HE Capabilities information element
   */
  void SetSupportedMcsAndNss (uint16_t ctrl);

  /*
   * Return the 4 first octets of the HE MAC Capabilties Info field in the HE Capabilities information element.
   *
   * \return the 4 first octets of the HE MAC Capabilties Info field in the HE Capabilities information element
   */
  uint32_t GetHeMacCapabilitiesInfo1 () const;
  /*
   * Return the last octet of the HE MAC Capabilties Info field in the HE Capabilities information element.
   *
   * \return the last octet of the HE MAC Capabilties Info field in the HE Capabilities information element
   */
  uint8_t GetHeMacCapabilitiesInfo2 () const;
  /*
   * Return the 8 first octets of the HE PHY Capabilties Info field in the HE Capabilities information element.
   *
   * \return the 8 first octets of the HE PHY Capabilties Info field in the HE Capabilities information element
   */
  uint64_t GetHePhyCapabilitiesInfo1 () const;
  /*
   * Return the last octet of the HE PHY Capabilties Info field in the HE Capabilities information element.
   *
   * \return the last octet of the HE PHY Capabilties Info field in the HE Capabilities information element
   */
  uint8_t GetHePhyCapabilitiesInfo2 () const;
  /*
   * Return the MCS and NSS field in the HE Capabilities information element.
   *
   * \return the MCS and NSS field in the HE Capabilities information element
   */
  uint16_t GetSupportedMcsAndNss () const;

  // PHY Capabilities Info fields
  void SetChannelWidthSet (uint8_t channelWidthSet);
  void SetHeLtfAndGiForHePpdus (uint8_t heLtfAndGiForHePpdus);
  uint8_t GetChannelWidthSet (void) const;
  uint8_t GetHeLtfAndGiForHePpdus (void) const;
  uint8_t GetHighestMcsSupported (uint8_t mcs) const;
  uint8_t GetHighestNssSupported (void) const;

  // MAC Capabilities Info fields
  void SetMaxAmpduLengthExponent (uint8_t exponent);

  // MCS and NSS field information
  void SetHighestMcsSupported (uint8_t mcs);
  void SetHighestNssSupported (uint8_t nss);

  bool IsSupportedTxMcs (uint8_t mcs) const;
  bool IsSupportedRxMcs (uint8_t mcs) const;

  /*
   * This information element is a bit special in that it is only
   * included if the STA is an VHT STA. To support this we
   * override the Serialize and GetSerializedSize methods of
   * WifiInformationElement.
   */
  Buffer::Iterator Serialize (Buffer::Iterator start) const;
  uint16_t GetSerializedSize () const;


private:
  //MAC Capabilities Info fields
  uint8_t m_plusHtcHeSupport;
  uint8_t m_twtRequesterSupport;
  uint8_t m_twtResponderSupport;
  uint8_t m_fragmentationSupport;
  uint8_t m_maximumNumberOfFragmentedMsdus;
  uint8_t m_minimumFragmentSize;
  uint8_t m_triggerFrameMacPaddingDuration;
  uint8_t m_multiTidAggregationSupport;
  uint8_t m_heLinkAdaptation;
  uint8_t m_allAckSupport;
  uint8_t m_ulMuResponseSchedulingSupport;
  uint8_t m_aBsrSupport;
  uint8_t m_broadcastTwtSupport;
  uint8_t m_32bitBaBitmapSupport;
  uint8_t m_muCascadeSupport;
  uint8_t m_ackEnabledMultiTidAggregationSupport;
  uint8_t m_groupAddressedMultiStaBlockAckInDlMuSupport;
  uint8_t m_omiAcontrolSupport;
  uint8_t m_ofdmaRaSupport;
  uint8_t m_maximumAmpduLengthExponent;
  uint8_t m_amsduFragmentationSupport;
  uint8_t m_flexibleTwtScheduleSupport;
  uint8_t m_rxControlFrameToMultiBss;
  uint8_t m_bsrpAmpduAggregation;
  uint8_t m_qtpSupport;
  uint8_t m_aBqrSupport;

  //PHY Capabilities Info fields
  uint8_t m_dualBandSupport;
  uint8_t m_channelWidthSet;
  uint8_t m_preamblePuncturingRx;
  uint8_t m_deviceClass;
  uint8_t m_ldpcCodingInPayload;
  uint8_t m_heLtfAndGiForHePpdus;
  uint8_t m_heLtfAndGiForNdp;
  uint8_t m_stbcTxAndRx;
  uint8_t m_doppler;
  uint8_t m_ulMu;
  uint8_t m_dcmEncodingTx;
  uint8_t m_dcmEncodingRx;
  uint8_t m_ulHeMuPpduPayloadSupport;
  uint8_t m_suBeamformer;
  uint8_t m_suBeamformee;
  uint8_t m_muBeamformer;
  uint8_t m_beamformeeStsForSmallerOrEqualThan80Mhz;
  uint8_t m_nstsTotalForSmallerOrEqualThan80Mhz;
  uint8_t m_beamformeeStsForLargerThan80Mhz;
  uint8_t m_nstsTotalForLargerThan80Mhz;
  uint8_t m_numberOfSoundingDimensionsForSmallerOrEqualThan80Mhz;
  uint8_t m_numberOfSoundingDimensionsForLargerThan80Mhz;
  uint8_t m_ngEqual16ForSuFeedbackSupport;
  uint8_t m_ngEqual16ForMuFeedbackSupport;
  uint8_t m_codebookSize42ForSuSupport;
  uint8_t m_codebookSize75ForSuSupport;
  uint8_t m_beamformingFeedbackWithTriggerFrame;
  uint8_t m_heErSuPpduPayload;
  uint8_t m_dlMuMimoOnPartialBandwidth;
  uint8_t m_ppeThresholdPresent;
  uint8_t m_srpBasedSrSupport;
  uint8_t m_powerBoostFactorAlphaSupport;
  uint8_t m_4TimesHeLtfAnd800NsGiSupportForHePpdus;

  //MCS and NSS field information
  uint8_t m_highestNssSupportedM1;
  uint8_t m_highestMcsSupported;
  std::vector<uint8_t> m_txBwMap;
  std::vector<uint8_t> m_rxBwMap;

  //This is used to decide if this element should be added to the frame or not
  uint8_t m_heSupported;
};

std::ostream &operator << (std::ostream &os, const HeCapabilities &HeCapabilities);
std::istream &operator >> (std::istream &is, HeCapabilities &HeCapabilities);

ATTRIBUTE_HELPER_HEADER (HeCapabilities)

} //namespace ns3

#endif /* HE_CAPABILITY_H */
