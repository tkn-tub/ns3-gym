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
 * Author: Ghada Badawy <gbadawy@rim.com>
 */

#include "ht-capabilities.h"
#include "ns3/assert.h"
#include "ns3/log.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("HtCapabilities");

HtCapabilities::HtCapabilities ()
   :  m_ldpc(0),
      m_supportedChannelWidth(0),
      m_smPowerSave(0),
      m_greenField(0),
      m_shortGuardInterval20(0),
      m_shortGuardInterval40(0),
      m_txStbc(0),
      m_rxStbc(0),
      m_htDelayedBlockAck(0),
      m_maxAmsduLength(0),
      m_dssMode40(0),
      m_reserved(0),
      m_fortyMhzIntolerant(0),
      m_lsigProtectionSupport(0),
      m_maxAmpduLength(0),
      m_minMpduStartSpace(0),
      m_ampduReserved(0),
      m_reservedMcsSet1(0),
      m_rxHighestSupportedDataRate(0),
      m_reservedMcsSet2(0),
      m_txMcsSetDefined(0),
      m_txRxMcsSetUnequal(0),
      m_txMaxNSpatialStreams(0),
      m_txUnequalModulation(0),
      m_reservedMcsSet3(7),
      m_htSupported(0)
{
  for (uint32_t k=0; k < MAX_SUPPORTED_MCS ;k++)
   {
     m_rxMcsBitmask[k]=0;
   }
}


WifiInformationElementId
HtCapabilities::ElementId () const
{
  return IE_HT_CAPABILITIES;
}

void 
HtCapabilities::SetHtSupported(uint8_t htsupported)
{
  m_htSupported = htsupported;
}
void
HtCapabilities::SetLdpc (uint8_t ldpc)
{
 m_ldpc=ldpc;
}

void
HtCapabilities::SetSupportedChannelWidth (uint8_t supportedchannelwidth)
{
 m_supportedChannelWidth=supportedchannelwidth;
}

void
HtCapabilities::SetGreenfield (uint8_t greenfield)
{
 m_greenField=greenfield;
}

void
HtCapabilities::SetShortGuardInterval20 (uint8_t shortguardinterval)
{
 m_shortGuardInterval20=shortguardinterval;
}
void
HtCapabilities::SetRxMcsBitmask(uint8_t index)
{
   m_rxMcsBitmask[index]=1;
}

uint8_t*
HtCapabilities::GetRxMcsBitmask()
{
  uint8_t* p;
  p= m_rxMcsBitmask;
  return p;
}

bool 
HtCapabilities::IsSupportedMcs (uint8_t mcs)
{
  if (m_rxMcsBitmask[mcs] == 1)
   {
      return true;
   }
  return false;
  
}
uint8_t
HtCapabilities::GetLdpc (void) const
{
  return m_ldpc;
}

uint8_t
HtCapabilities::GetSupportedChannelWidth (void) const
{
  return m_supportedChannelWidth;
}

uint8_t
HtCapabilities::GetGreenfield (void) const
{
  return m_greenField;
}
uint8_t
HtCapabilities::GetShortGuardInterval20(void) const
{
  return m_shortGuardInterval20;
}

uint8_t
HtCapabilities::GetInformationFieldSize () const
{
  // we should not be here if ht is not supported
  NS_ASSERT (m_htSupported > 0);
  return 19;
}
Buffer::Iterator
HtCapabilities::Serialize (Buffer::Iterator i) const
{
  if (m_htSupported< 1)
    {
       return i;
    }
  return WifiInformationElement::Serialize (i);
}
uint16_t
HtCapabilities::GetSerializedSize () const
{
  if (m_htSupported < 1)
    {
      return 0;
    }
  return WifiInformationElement::GetSerializedSize ();
}

uint16_t
HtCapabilities::GetHtCapabilitiesInfo (void) const
{
  uint16_t val = 0;
  val |= m_ldpc;
  val |= (m_supportedChannelWidth << 1)& (0x1 << 1);
  val |= (m_smPowerSave << 2)& (0x3 << 2) ;
  val |= (m_greenField << 4)& (0x1 << 4) ;
  val |= (m_shortGuardInterval20 << 5)& (0x1 << 5);
  val |= (m_shortGuardInterval40 << 6)& (0x1 << 6);
  val |= (m_txStbc << 7)& (0x1 << 7);
  val |= (m_rxStbc << 8)& (0x3 << 8);
  val |= (m_htDelayedBlockAck << 10)& (0x1 << 10);
  val |= (m_maxAmsduLength << 11)& (0x1 << 11);
  val |= (m_dssMode40 << 12)& (0x1 << 12);
  val |= (m_reserved<< 13)& (0x1 << 13);
  val |= (m_fortyMhzIntolerant << 14)& (0x1 << 14);
  val |= (m_lsigProtectionSupport << 15)& (0x1 << 15);
  return val;
}

void
HtCapabilities::SetHtCapabilitiesInfo(uint16_t ctrl)
{
  m_ldpc = ctrl & 0x01;
  m_supportedChannelWidth = (ctrl >> 1) & 0x01;
  m_smPowerSave = (ctrl >> 2) & 0x03;
  m_greenField = (ctrl >> 4) & 0x01;
  m_shortGuardInterval20  = (ctrl >> 5) & 0x01;
  m_shortGuardInterval40  = (ctrl >> 6) & 0x01;
  m_txStbc = (ctrl >> 7) & 0x01;
  m_rxStbc = (ctrl >> 8) & 0x03;
  m_htDelayedBlockAck = (ctrl >> 10) & 0x01;
  m_maxAmsduLength = (ctrl >> 11) & 0x01;
  m_dssMode40= (ctrl >> 12) & 0x01;
  m_reserved= (ctrl >> 13) & 0x01;
  m_fortyMhzIntolerant= (ctrl >> 14) & 0x01;
  m_lsigProtectionSupport= (ctrl >> 15) & 0x01;
}
uint8_t
HtCapabilities::GetAmpduParameters (void) const
{
  uint8_t val = 0;
  val |=  m_maxAmpduLength & 0x3;
  val |= ( m_minMpduStartSpace << 2)& (0x7 << 2);
  val |= (m_ampduReserved << 5)& (0x7 << 5) ;
  return val;
}

void
HtCapabilities::SetAmpduParameters (uint8_t ctrl)
{
  m_maxAmpduLength = ctrl & 0x03;
  m_minMpduStartSpace = (ctrl >> 2) & 0x07;
  m_ampduReserved =(ctrl >> 5) & 0x07;
}

void
HtCapabilities::SetSupportedMcsSet (uint64_t ctrl2, uint64_t ctrl1)
{
  for(uint64_t i=0 ; i < 77;i++)
    {
      if (i < 64)
        {
          m_rxMcsBitmask[i]=(ctrl1 >> i) & 0x01; 
        }
      else
        {
           m_rxMcsBitmask[i]=( ctrl2 >> (i-64))& 0x01 ;
        }
    }
  m_reservedMcsSet1 = (ctrl2 >> 12) & 0x07;
  m_rxHighestSupportedDataRate = (ctrl2 >> 15) & 0x03ff;
  m_reservedMcsSet2 = (ctrl2 >> 25) & 0x3f;
  m_txMcsSetDefined  = (ctrl2 >> 31) & 0x01;
  m_txRxMcsSetUnequal  = (ctrl2 >> 32) & 0x01;
  m_txMaxNSpatialStreams = (ctrl2 >> 33) & 0x03;
  m_txUnequalModulation = (ctrl2 >> 35) & 0x01;
  m_reservedMcsSet3 = (ctrl2 >> 36) & 0x07ffffff;
}
uint64_t
HtCapabilities::GetSupportedMcsSet1 (void) const
{
  uint64_t val=0;
  for(uint64_t i=63 ; i >0 ;i--)
    {
          val = (val << 1) | (m_rxMcsBitmask[i] & 0x01);
    }
  val = (val << 1) | (m_rxMcsBitmask[0] & 0x01);
  return val;
}
uint64_t
HtCapabilities::GetSupportedMcsSet2 (void) const
{
  uint64_t val=0; 
  val = val | (m_reservedMcsSet3 & 0x07ffffff);
  val = (val << 1) | (m_txUnequalModulation & 0x01);
  val = (val << 2) | (m_txMaxNSpatialStreams & 0x03);
  val = (val << 1) | (m_txRxMcsSetUnequal & 0x01);
  val = (val << 1) | (m_txMcsSetDefined & 0x01);
  val = (val << 6) | (m_reservedMcsSet2 & 0x3f);
  val = (val << 10) |(m_rxHighestSupportedDataRate & 0x3ff);
  val = (val << 3) |(m_reservedMcsSet1 & 0x07);

  for (uint64_t i=12; i>0;i--)
    {
       val = (val << 1)|( m_rxMcsBitmask[i+64] & 0x01);
    }
  return val;
}

void
HtCapabilities::SerializeInformationField (Buffer::Iterator start) const
{
   if (m_htSupported == 1)
    {
       // write the corresponding value for each bit
       start. WriteHtolsbU16 (GetHtCapabilitiesInfo());
       start. WriteU8 (GetAmpduParameters());
       start. WriteHtolsbU64 (GetSupportedMcsSet2());
       start. WriteHtolsbU64 (GetSupportedMcsSet1());
    }
}

uint8_t
HtCapabilities::DeserializeInformationField (Buffer::Iterator start,
                                             uint8_t length)
{ 
  Buffer::Iterator i = start;
  uint16_t htinfo = i.ReadLsbtohU16 ();
  uint8_t ampduparam = i.ReadU8 ();
  uint64_t mcsset1=i.ReadLsbtohU64 ();
  uint64_t mcsset2 = i.ReadLsbtohU64 ();
  SetHtCapabilitiesInfo(htinfo);
  SetAmpduParameters(ampduparam);
  SetSupportedMcsSet(mcsset1,mcsset2);
  return length;
}

ATTRIBUTE_HELPER_CPP (HtCapabilities);

std::ostream &
operator << (std::ostream &os, const HtCapabilities &htcapabilities)
{
  os <<  bool (htcapabilities.GetLdpc ())
     << "|" << bool (htcapabilities.GetSupportedChannelWidth ())
     << "|" << bool (htcapabilities.GetGreenfield ())
     << "|" << bool (htcapabilities.GetShortGuardInterval20 ());

  return os;
}

std::istream &operator >> (std::istream &is,HtCapabilities &htcapabilities)
{
  bool c1, c2, c3,c4;
  is >>  c1 >> c2 >> c3 >>c4;
  htcapabilities.SetLdpc (c1);
  htcapabilities.SetSupportedChannelWidth (c2);
  htcapabilities.SetGreenfield (c3);
  htcapabilities.SetShortGuardInterval20 (c4);

  return is;
}

} // namespace ns3
