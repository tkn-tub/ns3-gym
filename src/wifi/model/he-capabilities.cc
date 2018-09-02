/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

#include "he-capabilities.h"

namespace ns3 {

HeCapabilities::HeCapabilities ()
  : m_plusHtcHeSupport (0),
    m_twtRequesterSupport (0),
    m_twtResponderSupport (0),
    m_fragmentationSupport (0),
    m_maximumNumberOfFragmentedMsdus (0),
    m_minimumFragmentSize (0),
    m_triggerFrameMacPaddingDuration (0),
    m_multiTidAggregationSupport (0),
    m_heLinkAdaptation (0),
    m_allAckSupport (0),
    m_ulMuResponseSchedulingSupport (0),
    m_aBsrSupport (0),
    m_broadcastTwtSupport (0),
    m_32bitBaBitmapSupport (0),
    m_muCascadeSupport (0),
    m_ackEnabledMultiTidAggregationSupport (0),
    m_groupAddressedMultiStaBlockAckInDlMuSupport (0),
    m_omiAcontrolSupport (0),
    m_ofdmaRaSupport (0),
    m_maximumAmpduLengthExponent (0),
    m_amsduFragmentationSupport (0),
    m_flexibleTwtScheduleSupport (0),
    m_rxControlFrameToMultiBss (0),
    m_bsrpAmpduAggregation (0),
    m_qtpSupport (0),
    m_aBqrSupport (0),
    m_dualBandSupport (0),
    m_channelWidthSet (0),
    m_preamblePuncturingRx (0),
    m_deviceClass (0),
    m_ldpcCodingInPayload (0),
    m_heLtfAndGiForHePpdus (0),
    m_heLtfAndGiForNdp (0),
    m_stbcTxAndRx (0),
    m_doppler (0),
    m_ulMu (0),
    m_dcmEncodingTx (0),
    m_dcmEncodingRx (0),
    m_ulHeMuPpduPayloadSupport (0),
    m_suBeamformer (0),
    m_suBeamformee (0),
    m_muBeamformer (0),
    m_beamformeeStsForSmallerOrEqualThan80Mhz (0),
    m_nstsTotalForSmallerOrEqualThan80Mhz (0),
    m_beamformeeStsForLargerThan80Mhz (0),
    m_nstsTotalForLargerThan80Mhz (0),
    m_numberOfSoundingDimensionsForSmallerOrEqualThan80Mhz (0),
    m_numberOfSoundingDimensionsForLargerThan80Mhz (0),
    m_ngEqual16ForSuFeedbackSupport (0),
    m_ngEqual16ForMuFeedbackSupport (0),
    m_codebookSize42ForSuSupport (0),
    m_codebookSize75ForSuSupport (0),
    m_beamformingFeedbackWithTriggerFrame (0),
    m_heErSuPpduPayload (0),
    m_dlMuMimoOnPartialBandwidth (0),
    m_ppeThresholdPresent (0),
    m_srpBasedSrSupport (0),
    m_powerBoostFactorAlphaSupport (0),
    m_4TimesHeLtfAnd800NsGiSupportForHePpdus (0),
    m_highestNssSupportedM1 (0),
    m_highestMcsSupported (0),
    m_heSupported (0)
{
  m_txBwMap.resize (8,0);
  m_rxBwMap.resize (8,0);
}

WifiInformationElementId
HeCapabilities::ElementId () const
{
  return IE_EXTENSION;
}

WifiInformationElementId
HeCapabilities::ElementIdExt () const
{
  return IE_EXT_HE_CAPABILITIES;
}

void
HeCapabilities::SetHeSupported (uint8_t hesupported)
{
  m_heSupported = hesupported;
}

uint8_t
HeCapabilities::GetInformationFieldSize () const
{
  //we should not be here if ht is not supported
  NS_ASSERT (m_heSupported > 0);
  return 19; //todo: variable length!
}

Buffer::Iterator
HeCapabilities::Serialize (Buffer::Iterator i) const
{
  if (m_heSupported < 1)
    {
      return i;
    }
  return WifiInformationElement::Serialize (i);
}

uint16_t
HeCapabilities::GetSerializedSize () const
{
  if (m_heSupported < 1)
    {
      return 0;
    }
  return WifiInformationElement::GetSerializedSize ();
}

void
HeCapabilities::SerializeInformationField (Buffer::Iterator start) const
{
  if (m_heSupported == 1)
    {
      //write the corresponding value for each bit
      start.WriteHtolsbU32 (GetHeMacCapabilitiesInfo1 ());
      start.WriteU8 (GetHeMacCapabilitiesInfo2 ());
      start.WriteHtolsbU64 (GetHePhyCapabilitiesInfo1 ());
      start.WriteU8 (GetHePhyCapabilitiesInfo2 ());
      start.WriteHtolsbU32 (GetSupportedMcsAndNss ());
      //todo: add another 32-bits field if 160 MHz channel is supported (variable length)
      //todo: optional PPE Threshold field (variable length)
    }
}

uint8_t
HeCapabilities::DeserializeInformationField (Buffer::Iterator start, uint8_t length)
{
  Buffer::Iterator i = start;
  uint32_t macCapabilities1 = i.ReadLsbtohU32 ();
  uint8_t macCapabilities2 = i.ReadU8 ();
  uint64_t phyCapabilities1 = i.ReadLsbtohU64 ();
  uint8_t phyCapabilities2 = i.ReadU8 ();
  uint32_t mcsset = i.ReadU32 ();
  SetHeMacCapabilitiesInfo (macCapabilities1, macCapabilities2);
  SetHePhyCapabilitiesInfo (phyCapabilities1, phyCapabilities2);
  SetSupportedMcsAndNss (mcsset);
  //todo: add another 32-bits field if 160 MHz channel is supported (variable length)
  //todo: optional PPE Threshold field (variable length)
  return length;
}

void
HeCapabilities::SetHeMacCapabilitiesInfo (uint32_t ctrl1, uint8_t ctrl2)
{
  m_plusHtcHeSupport = ctrl1 & 0x01;
  m_twtRequesterSupport = (ctrl1 >> 1) & 0x01;
  m_twtResponderSupport = (ctrl1 >> 2) & 0x01;
  m_fragmentationSupport = (ctrl1 >> 3) & 0x03;
  m_maximumNumberOfFragmentedMsdus = (ctrl1 >> 5) & 0x07;
  m_minimumFragmentSize = (ctrl1 >> 8) & 0x03;
  m_triggerFrameMacPaddingDuration = (ctrl1 >> 10) & 0x03;
  m_multiTidAggregationSupport = (ctrl1 >> 12) & 0x07;
  m_heLinkAdaptation = (ctrl1 >> 15) & 0x03;
  m_allAckSupport = (ctrl1 >> 17) & 0x01;
  m_ulMuResponseSchedulingSupport = (ctrl1 >> 18) & 0x01;
  m_aBsrSupport = (ctrl1 >> 19) & 0x01;
  m_broadcastTwtSupport = (ctrl1 >> 20) & 0x01;
  m_32bitBaBitmapSupport = (ctrl1 >> 21) & 0x01;
  m_muCascadeSupport = (ctrl1 >> 22) & 0x01;
  m_ackEnabledMultiTidAggregationSupport = (ctrl1 >> 23) & 0x01;
  m_groupAddressedMultiStaBlockAckInDlMuSupport = (ctrl1 >> 24) & 0x01;
  m_omiAcontrolSupport = (ctrl1 >> 25) & 0x03;
  m_ofdmaRaSupport = (ctrl1 >> 26) & 0x01;
  m_maximumAmpduLengthExponent = (ctrl1 >> 27) & 0x03;
  m_amsduFragmentationSupport = (ctrl1 >> 29) & 0x01;
  m_flexibleTwtScheduleSupport = (ctrl1 >> 30) & 0x01;
  m_rxControlFrameToMultiBss = (ctrl1 >> 31) & 0x01;
  m_bsrpAmpduAggregation = ctrl2 & 0x01;
  m_qtpSupport = (ctrl2 >> 1) & 0x01;
  m_aBqrSupport = (ctrl2 >> 2) & 0x01;
}

uint32_t
HeCapabilities::GetHeMacCapabilitiesInfo1 () const
{
  uint32_t val = 0;
  val |= m_plusHtcHeSupport & 0x01;
  val |= (m_twtRequesterSupport & 0x01) << 1;
  val |= (m_twtResponderSupport & 0x01) << 2;
  val |= (m_fragmentationSupport & 0x03) << 3;
  val |= (m_maximumNumberOfFragmentedMsdus & 0x07) << 5;
  val |= (m_minimumFragmentSize & 0x03) << 8;
  val |= (m_triggerFrameMacPaddingDuration & 0x03) << 10;
  val |= (m_multiTidAggregationSupport & 0x07) << 12;
  val |= (m_heLinkAdaptation & 0x03) << 15;
  val |= (m_allAckSupport & 0x01) << 17;
  val |= (m_ulMuResponseSchedulingSupport & 0x01) << 18;
  val |= (m_aBsrSupport & 0x01) << 19;
  val |= (m_broadcastTwtSupport & 0x01) << 20;
  val |= (m_32bitBaBitmapSupport & 0x01) << 21;
  val |= (m_muCascadeSupport & 0x01) << 22;
  val |= (m_ackEnabledMultiTidAggregationSupport & 0x01) << 23;
  val |= (m_groupAddressedMultiStaBlockAckInDlMuSupport & 0x01) << 24;
  val |= (m_omiAcontrolSupport & 0x03) << 25;
  val |= (m_ofdmaRaSupport & 0x01) << 26;
  val |= (m_maximumAmpduLengthExponent & 0x03) << 27;
  val |= (m_amsduFragmentationSupport & 0x01) << 29;
  val |= (m_flexibleTwtScheduleSupport & 0x01) << 30;
  val |= (m_rxControlFrameToMultiBss & 0x01) << 31;
  return val;
}

uint8_t
HeCapabilities::GetHeMacCapabilitiesInfo2 () const
{
  uint8_t val = 0;
  val |= m_bsrpAmpduAggregation & 0x01;
  val |= (m_qtpSupport & 0x01) << 1;
  val |= (m_aBqrSupport & 0x01) << 2;
  return val;
}

void
HeCapabilities::SetHePhyCapabilitiesInfo (uint64_t ctrl1, uint8_t ctrl2)
{
  m_dualBandSupport = ctrl1 & 0x01;
  m_channelWidthSet = (ctrl1 >> 1) & 0x7f;
  m_preamblePuncturingRx = (ctrl1 >> 8) & 0x0f;
  m_deviceClass = (ctrl1 >> 12) & 0x01;
  m_ldpcCodingInPayload = (ctrl1 >> 13) & 0x01;
  m_heLtfAndGiForHePpdus = (ctrl1 >> 14) & 0x03;
  m_heLtfAndGiForNdp = (ctrl1 >> 16) & 0x03;
  m_stbcTxAndRx = (ctrl1 >> 18) & 0x03;
  m_doppler = (ctrl1 >> 20) & 0x03;
  m_ulMu = (ctrl1 >> 22) & 0x03;
  m_dcmEncodingTx = (ctrl1 >> 24) & 0x0f;
  m_dcmEncodingRx = (ctrl1 >> 27) & 0x0f;
  m_ulHeMuPpduPayloadSupport = (ctrl1 >> 30) & 0x01;
  m_suBeamformer = (ctrl1 >> 31) & 0x01;
  m_suBeamformee = (ctrl1 >> 32) & 0x01;
  m_muBeamformer = (ctrl1 >> 33) & 0x01;
  m_beamformeeStsForSmallerOrEqualThan80Mhz = (ctrl1 >> 34) & 0x07;
  m_nstsTotalForSmallerOrEqualThan80Mhz = (ctrl1 >> 37) & 0x07;
  m_beamformeeStsForLargerThan80Mhz = (ctrl1 >> 40) & 0x07;
  m_nstsTotalForLargerThan80Mhz = (ctrl1 >> 43) & 0x07;
  m_numberOfSoundingDimensionsForSmallerOrEqualThan80Mhz = (ctrl1 >> 46) & 0x07;
  m_numberOfSoundingDimensionsForLargerThan80Mhz = (ctrl1 >> 49) & 0x07;
  m_ngEqual16ForSuFeedbackSupport = (ctrl1 >> 52) & 0x01;
  m_ngEqual16ForMuFeedbackSupport = (ctrl1 >> 53) & 0x01;
  m_codebookSize42ForSuSupport = (ctrl1 >> 54) & 0x01;
  m_codebookSize75ForSuSupport = (ctrl1 >> 55) & 0x01;
  m_beamformingFeedbackWithTriggerFrame = (ctrl1 >> 56) & 0x07;
  m_heErSuPpduPayload = (ctrl1 >> 59) & 0x01;
  m_dlMuMimoOnPartialBandwidth = (ctrl1 >> 60) & 0x01;
  m_ppeThresholdPresent = (ctrl1 >> 61) & 0x01;
  m_srpBasedSrSupport = (ctrl1 >> 62) & 0x01;
  m_powerBoostFactorAlphaSupport = (ctrl1 >> 63) & 0x01;
  m_4TimesHeLtfAnd800NsGiSupportForHePpdus = ctrl2 & 0x01;
}

uint64_t
HeCapabilities::GetHePhyCapabilitiesInfo1 () const
{
  uint64_t val = 0;
  val |= m_dualBandSupport & 0x01;
  val |= (m_channelWidthSet & 0x7f) << 1;
  val |= (m_preamblePuncturingRx & 0x0f) << 8;
  val |= (m_deviceClass & 0x01) << 12;
  val |= (m_ldpcCodingInPayload & 0x01) << 13;
  val |= (m_heLtfAndGiForHePpdus & 0x03) << 14;
  val |= (m_heLtfAndGiForNdp & 0x03) << 16;
  val |= (m_stbcTxAndRx & 0x03) << 18;
  val |= (m_doppler & 0x03) << 20;
  val |= (m_ulMu & 0x03) << 22;
  val |= (m_dcmEncodingTx & 0x0f) << 24;
  val |= (m_dcmEncodingRx & 0x0f) << 27;
  val |= (m_ulHeMuPpduPayloadSupport & 0x01) << 30;
  val |= (m_suBeamformer & 0x01) << 31;
  val |= (static_cast<uint64_t> (m_suBeamformee) & 0x01) << 32;
  val |= (static_cast<uint64_t> (m_muBeamformer) & 0x01) << 33;
  val |= (static_cast<uint64_t> (m_beamformeeStsForSmallerOrEqualThan80Mhz) & 0x07) << 34;
  val |= (static_cast<uint64_t> (m_nstsTotalForSmallerOrEqualThan80Mhz) & 0x07) << 37;
  val |= (static_cast<uint64_t> (m_beamformeeStsForLargerThan80Mhz) & 0x07) << 40;
  val |= (static_cast<uint64_t> (m_nstsTotalForLargerThan80Mhz) & 0x07) << 43;
  val |= (static_cast<uint64_t> (m_numberOfSoundingDimensionsForSmallerOrEqualThan80Mhz) & 0x07) << 46;
  val |= (static_cast<uint64_t> (m_numberOfSoundingDimensionsForLargerThan80Mhz) & 0x07) << 49;
  val |= (static_cast<uint64_t> (m_ngEqual16ForSuFeedbackSupport) & 0x01) << 52;
  val |= (static_cast<uint64_t> (m_ngEqual16ForMuFeedbackSupport) & 0x01) << 53;
  val |= (static_cast<uint64_t> (m_codebookSize42ForSuSupport) & 0x01) << 54;
  val |= (static_cast<uint64_t> (m_codebookSize75ForSuSupport) & 0x01) << 55;
  val |= (static_cast<uint64_t> (m_beamformingFeedbackWithTriggerFrame) & 0x07) << 56;
  val |= (static_cast<uint64_t> (m_heErSuPpduPayload) & 0x01) << 59;
  val |= (static_cast<uint64_t> (m_dlMuMimoOnPartialBandwidth) & 0x01) << 60;
  val |= (static_cast<uint64_t> (m_ppeThresholdPresent) & 0x01) << 61;
  val |= (static_cast<uint64_t> (m_srpBasedSrSupport) & 0x01) << 62;
  val |= (static_cast<uint64_t> (m_powerBoostFactorAlphaSupport) & 0x01) << 63;
  return val;
}

uint8_t
HeCapabilities::GetHePhyCapabilitiesInfo2 () const
{
  uint8_t val = 0;
  val |= m_4TimesHeLtfAnd800NsGiSupportForHePpdus & 0x01;
  return val;
}

void
HeCapabilities::SetSupportedMcsAndNss (uint16_t ctrl)
{
  m_highestNssSupportedM1 = ctrl & 0x07;
  m_highestMcsSupported = (ctrl >> 3) & 0x07;
  NS_ASSERT (m_highestMcsSupported <= 4);
  uint8_t i;
  for (i = 0; i < 5; i++)
    {
      m_txBwMap[i] = (ctrl >> (6 + i)) & 0x01;
    }
  for (i = 0; i < 5; i++)
    {
      m_rxBwMap[i] = (ctrl >> (11 + i)) & 0x01;
    }
  //todo: MCS NSS Descriptors
}

uint16_t
HeCapabilities::GetSupportedMcsAndNss () const
{
  uint16_t val = 0;
  val |= m_highestNssSupportedM1 & 0x07;
  val |= (m_highestMcsSupported & 0x07) << 3;
  uint8_t i;
  for (i = 0; i < 5; i++)
    {
      val |= (m_txBwMap[i] & 0x01) << (6 + 1);
    }
  for (i = 0; i < 5; i++)
    {
      val |= (m_rxBwMap[i] & 0x01) << (11 + 1);
    }
  //todo: MCS NSS Descriptors
  return val;
}

//todo: PPE threshold

bool
HeCapabilities::IsSupportedTxMcs (uint8_t mcs) const
{
  NS_ASSERT (mcs >= 0 && mcs <= 11);
  if (mcs <= 7)
    {
      return true;
    }
  if (mcs == 8 && m_highestMcsSupported >= 1)
    {
      return true;
    }
  if (mcs == 9 && m_highestMcsSupported >= 2)
    {
      return true;
    }
  if (mcs == 10 && m_highestMcsSupported >= 3)
    {
      return true;
    }
  if (mcs == 11 && m_highestMcsSupported == 4)
    {
      return true;
    }
  return false;
}

bool
HeCapabilities::IsSupportedRxMcs (uint8_t mcs) const
{
  NS_ASSERT (mcs >= 0 && mcs <= 11);
  if (mcs <= 7)
    {
      return true;
    }
  if (mcs == 8 && m_highestMcsSupported >= 1)
    {
      return true;
    }
  if (mcs == 9 && m_highestMcsSupported >= 2)
    {
      return true;
    }
  if (mcs == 10 && m_highestMcsSupported >= 3)
    {
      return true;
    }
  if (mcs == 11 && m_highestMcsSupported == 4)
    {
      return true;
    }
  return false;
}

void
HeCapabilities::SetChannelWidthSet (uint8_t channelWidthSet)
{
  NS_ASSERT (channelWidthSet <= 0x2f);
  m_channelWidthSet = channelWidthSet;
}

void
HeCapabilities::SetHeLtfAndGiForHePpdus (uint8_t heLtfAndGiForHePpdus)
{
  NS_ASSERT (heLtfAndGiForHePpdus <= 0x03);
  m_heLtfAndGiForHePpdus = heLtfAndGiForHePpdus;
}

void
HeCapabilities::SetMaxAmpduLengthExponent (uint8_t exponent)
{
  NS_ASSERT (exponent <= 7);
  m_maximumAmpduLengthExponent = exponent;
}

void
HeCapabilities::SetHighestMcsSupported (uint8_t mcs)
{
  NS_ASSERT (mcs >= 7 && mcs <= 11);
  m_highestMcsSupported = mcs - 7;
}

void
HeCapabilities::SetHighestNssSupported (uint8_t nss)
{
  NS_ASSERT (nss >= 1 && nss <= 8);
  m_highestNssSupportedM1 = nss - 1;
}

uint8_t
HeCapabilities::GetChannelWidthSet (void) const
{
  return m_channelWidthSet;
}

uint8_t
HeCapabilities::GetHeLtfAndGiForHePpdus (void) const
{
  return m_heLtfAndGiForHePpdus;
}

uint8_t
HeCapabilities::GetHighestMcsSupported (void) const
{
  return m_highestMcsSupported + 7;
}

uint8_t
HeCapabilities::GetHighestNssSupported (void) const
{
  return m_highestNssSupportedM1 + 1;
}

/**
 * output stream output operator
 * \param os the output stream
 * \param HeCapabilities the HE capabilities
 * \returns the output stream
 */
std::ostream &
operator << (std::ostream &os, const HeCapabilities &HeCapabilities)
{
  os << HeCapabilities.GetHeMacCapabilitiesInfo1 () << "|"
     << +HeCapabilities.GetHeMacCapabilitiesInfo2 () << "|"
     << HeCapabilities.GetHePhyCapabilitiesInfo1 () << "|"
     << +HeCapabilities.GetHePhyCapabilitiesInfo2 () << "|"
     << HeCapabilities.GetSupportedMcsAndNss ();
  return os;
}

} //namespace ns3
