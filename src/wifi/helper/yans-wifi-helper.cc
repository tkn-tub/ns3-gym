/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 *          Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#include "ns3/trace-helper.h"
#include "yans-wifi-helper.h"
#include "ns3/error-rate-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/yans-wifi-phy.h"
#include "ns3/ampdu-subframe-header.h"
#include "ns3/wifi-net-device.h"
#include "ns3/radiotap-header.h"
#include "ns3/pcap-file-wrapper.h"
#include "ns3/simulator.h"
#include "ns3/config.h"
#include "ns3/names.h"
#include "ns3/abort.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("YansWifiHelper");

static void
AsciiPhyTransmitSinkWithContext (
  Ptr<OutputStreamWrapper> stream,
  std::string context,
  Ptr<const Packet> p,
  WifiMode mode,
  WifiPreamble preamble,
  uint8_t txLevel)
{
  NS_LOG_FUNCTION (stream << context << p << mode << preamble << txLevel);
  *stream->GetStream () << "t " << Simulator::Now ().GetSeconds () << " " << context << " " << *p << std::endl;
}

static void
AsciiPhyTransmitSinkWithoutContext (
  Ptr<OutputStreamWrapper> stream,
  Ptr<const Packet> p,
  WifiMode mode,
  WifiPreamble preamble,
  uint8_t txLevel)
{
  NS_LOG_FUNCTION (stream << p << mode << preamble << txLevel);
  *stream->GetStream () << "t " << Simulator::Now ().GetSeconds () << " " << *p << std::endl;
}

static void
AsciiPhyReceiveSinkWithContext (
  Ptr<OutputStreamWrapper> stream,
  std::string context,
  Ptr<const Packet> p,
  double snr,
  WifiMode mode,
  enum WifiPreamble preamble)
{
  NS_LOG_FUNCTION (stream << context << p << snr << mode << preamble);
  *stream->GetStream () << "r " << Simulator::Now ().GetSeconds () << " " << context << " " << *p << std::endl;
}

static void
AsciiPhyReceiveSinkWithoutContext (
  Ptr<OutputStreamWrapper> stream,
  Ptr<const Packet> p,
  double snr,
  WifiMode mode,
  enum WifiPreamble preamble)
{
  NS_LOG_FUNCTION (stream << p << snr << mode << preamble);
  *stream->GetStream () << "r " << Simulator::Now ().GetSeconds () << " " << *p << std::endl;
}

YansWifiChannelHelper::YansWifiChannelHelper ()
{
}

YansWifiChannelHelper
YansWifiChannelHelper::Default (void)
{
  YansWifiChannelHelper helper;
  helper.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  helper.AddPropagationLoss ("ns3::LogDistancePropagationLossModel");
  return helper;
}

void
YansWifiChannelHelper::AddPropagationLoss (std::string type,
                                           std::string n0, const AttributeValue &v0,
                                           std::string n1, const AttributeValue &v1,
                                           std::string n2, const AttributeValue &v2,
                                           std::string n3, const AttributeValue &v3,
                                           std::string n4, const AttributeValue &v4,
                                           std::string n5, const AttributeValue &v5,
                                           std::string n6, const AttributeValue &v6,
                                           std::string n7, const AttributeValue &v7)
{
  ObjectFactory factory;
  factory.SetTypeId (type);
  factory.Set (n0, v0);
  factory.Set (n1, v1);
  factory.Set (n2, v2);
  factory.Set (n3, v3);
  factory.Set (n4, v4);
  factory.Set (n5, v5);
  factory.Set (n6, v6);
  factory.Set (n7, v7);
  m_propagationLoss.push_back (factory);
}

void
YansWifiChannelHelper::SetPropagationDelay (std::string type,
                                            std::string n0, const AttributeValue &v0,
                                            std::string n1, const AttributeValue &v1,
                                            std::string n2, const AttributeValue &v2,
                                            std::string n3, const AttributeValue &v3,
                                            std::string n4, const AttributeValue &v4,
                                            std::string n5, const AttributeValue &v5,
                                            std::string n6, const AttributeValue &v6,
                                            std::string n7, const AttributeValue &v7)
{
  ObjectFactory factory;
  factory.SetTypeId (type);
  factory.Set (n0, v0);
  factory.Set (n1, v1);
  factory.Set (n2, v2);
  factory.Set (n3, v3);
  factory.Set (n4, v4);
  factory.Set (n5, v5);
  factory.Set (n6, v6);
  factory.Set (n7, v7);
  m_propagationDelay = factory;
}

Ptr<YansWifiChannel>
YansWifiChannelHelper::Create (void) const
{
  Ptr<YansWifiChannel> channel = CreateObject<YansWifiChannel> ();
  Ptr<PropagationLossModel> prev = 0;
  for (std::vector<ObjectFactory>::const_iterator i = m_propagationLoss.begin (); i != m_propagationLoss.end (); ++i)
    {
      Ptr<PropagationLossModel> cur = (*i).Create<PropagationLossModel> ();
      if (prev != 0)
        {
          prev->SetNext (cur);
        }
      if (m_propagationLoss.begin () == i)
        {
          channel->SetPropagationLossModel (cur);
        }
      prev = cur;
    }
  Ptr<PropagationDelayModel> delay = m_propagationDelay.Create<PropagationDelayModel> ();
  channel->SetPropagationDelayModel (delay);
  return channel;
}

int64_t
YansWifiChannelHelper::AssignStreams (Ptr<YansWifiChannel> c, int64_t stream)
{
  return c->AssignStreams (stream);
}

YansWifiPhyHelper::YansWifiPhyHelper ()
  : m_channel (0),
    m_pcapDlt (PcapHelper::DLT_IEEE802_11)
{
  m_phy.SetTypeId ("ns3::YansWifiPhy");
}

YansWifiPhyHelper
YansWifiPhyHelper::Default (void)
{
  YansWifiPhyHelper helper;
  helper.SetErrorRateModel ("ns3::NistErrorRateModel");
  return helper;
}

void
YansWifiPhyHelper::SetChannel (Ptr<YansWifiChannel> channel)
{
  m_channel = channel;
}

void
YansWifiPhyHelper::SetChannel (std::string channelName)
{
  Ptr<YansWifiChannel> channel = Names::Find<YansWifiChannel> (channelName);
  m_channel = channel;
}

void
YansWifiPhyHelper::Set (std::string name, const AttributeValue &v)
{
  m_phy.Set (name, v);
}

void
YansWifiPhyHelper::SetErrorRateModel (std::string name,
                                      std::string n0, const AttributeValue &v0,
                                      std::string n1, const AttributeValue &v1,
                                      std::string n2, const AttributeValue &v2,
                                      std::string n3, const AttributeValue &v3,
                                      std::string n4, const AttributeValue &v4,
                                      std::string n5, const AttributeValue &v5,
                                      std::string n6, const AttributeValue &v6,
                                      std::string n7, const AttributeValue &v7)
{
  m_errorRateModel = ObjectFactory ();
  m_errorRateModel.SetTypeId (name);
  m_errorRateModel.Set (n0, v0);
  m_errorRateModel.Set (n1, v1);
  m_errorRateModel.Set (n2, v2);
  m_errorRateModel.Set (n3, v3);
  m_errorRateModel.Set (n4, v4);
  m_errorRateModel.Set (n5, v5);
  m_errorRateModel.Set (n6, v6);
  m_errorRateModel.Set (n7, v7);
}

Ptr<WifiPhy>
YansWifiPhyHelper::Create (Ptr<Node> node, Ptr<NetDevice> device) const
{
  Ptr<YansWifiPhy> phy = m_phy.Create<YansWifiPhy> ();
  Ptr<ErrorRateModel> error = m_errorRateModel.Create<ErrorRateModel> ();
  phy->SetErrorRateModel (error);
  phy->SetChannel (m_channel);
  phy->SetDevice (device);
  return phy;
}

void
YansWifiPhyHelper::PcapSniffTxEvent (
  Ptr<PcapFileWrapper> file,
  Ptr<const Packet>    packet,
  uint16_t             channelFreqMhz,
  uint16_t             channelNumber,
  uint32_t             rate,
  WifiPreamble         preamble,
  WifiTxVector         txVector,
  struct mpduInfo      aMpdu)
{
  uint32_t dlt = file->GetDataLinkType ();

  switch (dlt)
    {
    case PcapHelper::DLT_IEEE802_11:
      file->Write (Simulator::Now (), packet);
      return;
    case PcapHelper::DLT_PRISM_HEADER:
      {
        NS_FATAL_ERROR ("PcapSniffTxEvent(): DLT_PRISM_HEADER not implemented");
        return;
      }
    case PcapHelper::DLT_IEEE802_11_RADIO:
      {
        Ptr<Packet> p = packet->Copy ();
        RadiotapHeader header;
        uint8_t frameFlags = RadiotapHeader::FRAME_FLAG_NONE;
        header.SetTsft (Simulator::Now ().GetMicroSeconds ());

        //Our capture includes the FCS, so we set the flag to say so.
        frameFlags |= RadiotapHeader::FRAME_FLAG_FCS_INCLUDED;

        if (preamble == WIFI_PREAMBLE_SHORT)
          {
            frameFlags |= RadiotapHeader::FRAME_FLAG_SHORT_PREAMBLE;
          }

        if (txVector.IsShortGuardInterval ())
          {
            frameFlags |= RadiotapHeader::FRAME_FLAG_SHORT_GUARD;
          }

        header.SetFrameFlags (frameFlags);
        header.SetRate (rate);

        uint16_t channelFlags = 0;
        switch (rate)
          {
          case 2:  //1Mbps
          case 4:  //2Mbps
          case 10: //5Mbps
          case 22: //11Mbps
            channelFlags |= RadiotapHeader::CHANNEL_FLAG_CCK;
            break;

          default:
            channelFlags |= RadiotapHeader::CHANNEL_FLAG_OFDM;
            break;
          }

        if (channelFreqMhz < 2500)
          {
            channelFlags |= RadiotapHeader::CHANNEL_FLAG_SPECTRUM_2GHZ;
          }
        else
          {
            channelFlags |= RadiotapHeader::CHANNEL_FLAG_SPECTRUM_5GHZ;
          }

        header.SetChannelFrequencyAndFlags (channelFreqMhz, channelFlags);

        if (preamble == WIFI_PREAMBLE_HT_MF || preamble == WIFI_PREAMBLE_HT_GF || preamble == WIFI_PREAMBLE_NONE)
          {
            uint8_t mcsRate = 0;
            uint8_t mcsKnown = RadiotapHeader::MCS_KNOWN_NONE;
            uint8_t mcsFlags = RadiotapHeader::MCS_FLAGS_NONE;

            mcsKnown |= RadiotapHeader::MCS_KNOWN_INDEX;
            mcsRate = rate - 128;

            mcsKnown |= RadiotapHeader::MCS_KNOWN_BANDWIDTH;
            if (txVector.GetChannelWidth () == 40)
              {
                mcsFlags |= RadiotapHeader::MCS_FLAGS_BANDWIDTH_40;
              }

            mcsKnown |= RadiotapHeader::MCS_KNOWN_GUARD_INTERVAL;
            if (txVector.IsShortGuardInterval ())
              {
                mcsFlags |= RadiotapHeader::MCS_FLAGS_GUARD_INTERVAL;
              }

            mcsKnown |= RadiotapHeader::MCS_KNOWN_HT_FORMAT;
            if (preamble == WIFI_PREAMBLE_HT_GF)
              {
                mcsFlags |= RadiotapHeader::MCS_FLAGS_HT_GREENFIELD;
              }

            mcsKnown |= RadiotapHeader::MCS_KNOWN_NESS;
            if (txVector.GetNess () & 0x01) //bit 1
              {
                mcsFlags |= RadiotapHeader::MCS_FLAGS_NESS_BIT_0;
              }
            if (txVector.GetNess () & 0x02) //bit 2
              {
                mcsKnown |= RadiotapHeader::MCS_KNOWN_NESS_BIT_1;
              }

            mcsKnown |= RadiotapHeader::MCS_KNOWN_FEC_TYPE; //only BCC is currently supported

            mcsKnown |= RadiotapHeader::MCS_KNOWN_STBC;
            if (txVector.IsStbc ())
              {
                mcsFlags |= RadiotapHeader::MCS_FLAGS_STBC_STREAMS;
              }

            header.SetMcsFields (mcsKnown, mcsFlags, mcsRate);
          }

        if (txVector.IsAggregation ())
          {
            uint16_t ampduStatusFlags = RadiotapHeader::A_MPDU_STATUS_NONE;
            ampduStatusFlags |= RadiotapHeader::A_MPDU_STATUS_LAST_KNOWN;
            /* For PCAP file, MPDU Delimiter and Padding should be removed by the MAC Driver */
            AmpduSubframeHeader hdr;
            uint32_t extractedLength;
            p->RemoveHeader (hdr);
            extractedLength = hdr.GetLength ();
            p = p->CreateFragment (0, static_cast<uint32_t> (extractedLength));
            if (aMpdu.type == LAST_MPDU_IN_AGGREGATE || (hdr.GetEof () == true && hdr.GetLength () > 0))
              {
                ampduStatusFlags |= RadiotapHeader::A_MPDU_STATUS_LAST;
              }
            header.SetAmpduStatus (aMpdu.mpduRefNumber, ampduStatusFlags, hdr.GetCrc ());
          }

        if (preamble == WIFI_PREAMBLE_VHT)
          {
            uint16_t vhtKnown = RadiotapHeader::VHT_KNOWN_NONE;
            uint8_t vhtFlags = RadiotapHeader::VHT_FLAGS_NONE;
            uint8_t vhtBandwidth = 0;
            uint8_t vhtMcsNss[4] = {0,0,0,0};
            uint8_t vhtCoding = 0;
            uint8_t vhtGroupId = 0;
            uint16_t vhtPartialAid = 0;

            vhtKnown |= RadiotapHeader::VHT_KNOWN_STBC;
            if (txVector.IsStbc ())
              {
                vhtFlags |= RadiotapHeader::VHT_FLAGS_STBC;
              }

            vhtKnown |= RadiotapHeader::VHT_KNOWN_GUARD_INTERVAL;
            if (txVector.IsShortGuardInterval ())
              {
                vhtFlags |= RadiotapHeader::VHT_FLAGS_GUARD_INTERVAL;
              }

            vhtKnown |= RadiotapHeader::VHT_KNOWN_BEAMFORMED; //Beamforming is currently not supported

            vhtKnown |= RadiotapHeader::VHT_KNOWN_BANDWIDTH;
            //not all bandwidth values are currently supported
            if (txVector.GetChannelWidth () == 40)
              {
                vhtBandwidth = 1;
              }
            else if (txVector.GetChannelWidth () == 80)
              {
                vhtBandwidth = 4;
              }
            else if (txVector.GetChannelWidth () == 160)
              {
                vhtBandwidth = 11;
              }

            //only SU PPDUs are currently supported
            vhtMcsNss[0] |= (txVector.GetNss () & 0x0f);
            vhtMcsNss[0] |= (((rate - 128) << 4) & 0xf0);

            header.SetVhtFields (vhtKnown, vhtFlags, vhtBandwidth, vhtMcsNss, vhtCoding, vhtGroupId, vhtPartialAid);
          }

        p->AddHeader (header);
        file->Write (Simulator::Now (), p);
        return;
      }
    default:
      NS_ABORT_MSG ("PcapSniffTxEvent(): Unexpected data link type " << dlt);
    }
}

void
YansWifiPhyHelper::PcapSniffRxEvent (
  Ptr<PcapFileWrapper>  file,
  Ptr<const Packet>     packet,
  uint16_t              channelFreqMhz,
  uint16_t              channelNumber,
  uint32_t              rate,
  WifiPreamble          preamble,
  WifiTxVector          txVector,
  struct mpduInfo       aMpdu,
  struct signalNoiseDbm signalNoise)
{
  uint32_t dlt = file->GetDataLinkType ();

  switch (dlt)
    {
    case PcapHelper::DLT_IEEE802_11:
      file->Write (Simulator::Now (), packet);
      return;
    case PcapHelper::DLT_PRISM_HEADER:
      {
        NS_FATAL_ERROR ("PcapSniffRxEvent(): DLT_PRISM_HEADER not implemented");
        return;
      }
    case PcapHelper::DLT_IEEE802_11_RADIO:
      {
        Ptr<Packet> p = packet->Copy ();
        RadiotapHeader header;
        uint8_t frameFlags = RadiotapHeader::FRAME_FLAG_NONE;
        header.SetTsft (Simulator::Now ().GetMicroSeconds ());

        //Our capture includes the FCS, so we set the flag to say so.
        frameFlags |= RadiotapHeader::FRAME_FLAG_FCS_INCLUDED;

        if (preamble == WIFI_PREAMBLE_SHORT)
          {
            frameFlags |= RadiotapHeader::FRAME_FLAG_SHORT_PREAMBLE;
          }

        if (txVector.IsShortGuardInterval ())
          {
            frameFlags |= RadiotapHeader::FRAME_FLAG_SHORT_GUARD;
          }

        header.SetFrameFlags (frameFlags);
        header.SetRate (rate);

        uint16_t channelFlags = 0;
        switch (rate)
          {
          case 2:  //1Mbps
          case 4:  //2Mbps
          case 10: //5Mbps
          case 22: //11Mbps
            channelFlags |= RadiotapHeader::CHANNEL_FLAG_CCK;
            break;

          default:
            channelFlags |= RadiotapHeader::CHANNEL_FLAG_OFDM;
            break;
          }

        if (channelFreqMhz < 2500)
          {
            channelFlags |= RadiotapHeader::CHANNEL_FLAG_SPECTRUM_2GHZ;
          }
        else
          {
            channelFlags |= RadiotapHeader::CHANNEL_FLAG_SPECTRUM_5GHZ;
          }

        header.SetChannelFrequencyAndFlags (channelFreqMhz, channelFlags);

        header.SetAntennaSignalPower (signalNoise.signal);
        header.SetAntennaNoisePower (signalNoise.noise);

        if (preamble == WIFI_PREAMBLE_HT_MF || preamble == WIFI_PREAMBLE_HT_GF || preamble == WIFI_PREAMBLE_NONE)
          {
            uint8_t mcsRate = 0;
            uint8_t mcsKnown = RadiotapHeader::MCS_KNOWN_NONE;
            uint8_t mcsFlags = RadiotapHeader::MCS_FLAGS_NONE;

            mcsKnown |= RadiotapHeader::MCS_KNOWN_INDEX;
            mcsRate = rate - 128;

            mcsKnown |= RadiotapHeader::MCS_KNOWN_BANDWIDTH;
            if (txVector.GetChannelWidth () == 40)
              {
                mcsFlags |= RadiotapHeader::MCS_FLAGS_BANDWIDTH_40;
              }

            mcsKnown |= RadiotapHeader::MCS_KNOWN_GUARD_INTERVAL;
            if (txVector.IsShortGuardInterval ())
              {
                mcsFlags |= RadiotapHeader::MCS_FLAGS_GUARD_INTERVAL;
              }

            mcsKnown |= RadiotapHeader::MCS_KNOWN_HT_FORMAT;
            if (preamble == WIFI_PREAMBLE_HT_GF)
              {
                mcsFlags |= RadiotapHeader::MCS_FLAGS_HT_GREENFIELD;
              }

            mcsKnown |= RadiotapHeader::MCS_KNOWN_NESS;
            if (txVector.GetNess () & 0x01) //bit 1
              {
                mcsFlags |= RadiotapHeader::MCS_FLAGS_NESS_BIT_0;
              }
            if (txVector.GetNess () & 0x02) //bit 2
              {
                mcsKnown |= RadiotapHeader::MCS_KNOWN_NESS_BIT_1;
              }

            mcsKnown |= RadiotapHeader::MCS_KNOWN_FEC_TYPE; //only BCC is currently supported

            mcsKnown |= RadiotapHeader::MCS_KNOWN_STBC;
            if (txVector.IsStbc ())
              {
                mcsFlags |= RadiotapHeader::MCS_FLAGS_STBC_STREAMS;
              }

            header.SetMcsFields (mcsKnown, mcsFlags, mcsRate);
          }

        if (txVector.IsAggregation ())
          {
            uint16_t ampduStatusFlags = RadiotapHeader::A_MPDU_STATUS_NONE;
            ampduStatusFlags |= RadiotapHeader::A_MPDU_STATUS_DELIMITER_CRC_KNOWN;
            ampduStatusFlags |= RadiotapHeader::A_MPDU_STATUS_LAST_KNOWN;
            /* For PCAP file, MPDU Delimiter and Padding should be removed by the MAC Driver */
            AmpduSubframeHeader hdr;
            uint32_t extractedLength;
            p->RemoveHeader (hdr);
            extractedLength = hdr.GetLength ();
            p = p->CreateFragment (0, static_cast<uint32_t> (extractedLength));
            if (aMpdu.type == LAST_MPDU_IN_AGGREGATE || (hdr.GetEof () == true && hdr.GetLength () > 0))
              {
                ampduStatusFlags |= RadiotapHeader::A_MPDU_STATUS_LAST;
              }
            header.SetAmpduStatus (aMpdu.mpduRefNumber, ampduStatusFlags, hdr.GetCrc ());
          }

        if (preamble == WIFI_PREAMBLE_VHT)
          {
            uint16_t vhtKnown = RadiotapHeader::VHT_KNOWN_NONE;
            uint8_t vhtFlags = RadiotapHeader::VHT_FLAGS_NONE;
            uint8_t vhtBandwidth = 0;
            uint8_t vhtMcsNss[4] = {0,0,0,0};
            uint8_t vhtCoding = 0;
            uint8_t vhtGroupId = 0;
            uint16_t vhtPartialAid = 0;

            vhtKnown |= RadiotapHeader::VHT_KNOWN_STBC;
            if (txVector.IsStbc ())
              {
                vhtFlags |= RadiotapHeader::VHT_FLAGS_STBC;
              }

            vhtKnown |= RadiotapHeader::VHT_KNOWN_GUARD_INTERVAL;
            if (txVector.IsShortGuardInterval ())
              {
                vhtFlags |= RadiotapHeader::VHT_FLAGS_GUARD_INTERVAL;
              }

            vhtKnown |= RadiotapHeader::VHT_KNOWN_BEAMFORMED; //Beamforming is currently not supported

            vhtKnown |= RadiotapHeader::VHT_KNOWN_BANDWIDTH;
            //not all bandwidth values are currently supported
            if (txVector.GetChannelWidth () == 40)
              {
                vhtBandwidth = 1;
              }
            else if (txVector.GetChannelWidth () == 80)
              {
                vhtBandwidth = 4;
              }
            else if (txVector.GetChannelWidth () == 160)
              {
                vhtBandwidth = 11;
              }

            //only SU PPDUs are currently supported
            vhtMcsNss[0] |= (txVector.GetNss () & 0x0f);
            vhtMcsNss[0] |= (((rate - 128) << 4) & 0xf0);

            header.SetVhtFields (vhtKnown, vhtFlags, vhtBandwidth, vhtMcsNss, vhtCoding, vhtGroupId, vhtPartialAid);
          }

        p->AddHeader (header);
        file->Write (Simulator::Now (), p);
        return;
      }
    default:
      NS_ABORT_MSG ("PcapSniffRxEvent(): Unexpected data link type " << dlt);
    }
}

void
YansWifiPhyHelper::SetPcapDataLinkType (enum SupportedPcapDataLinkTypes dlt)
{
  switch (dlt)
    {
    case DLT_IEEE802_11:
      m_pcapDlt = PcapHelper::DLT_IEEE802_11;
      return;
    case DLT_PRISM_HEADER:
      m_pcapDlt = PcapHelper::DLT_PRISM_HEADER;
      return;
    case DLT_IEEE802_11_RADIO:
      m_pcapDlt = PcapHelper::DLT_IEEE802_11_RADIO;
      return;
    default:
      NS_ABORT_MSG ("YansWifiPhyHelper::SetPcapFormat(): Unexpected format");
    }
}

PcapHelper::DataLinkType
YansWifiPhyHelper::GetPcapDataLinkType (void) const
{
  return m_pcapDlt;
}

void
YansWifiPhyHelper::EnablePcapInternal (std::string prefix, Ptr<NetDevice> nd, bool promiscuous, bool explicitFilename)
{
  //All of the Pcap enable functions vector through here including the ones
  //that are wandering through all of devices on perhaps all of the nodes in
  //the system. We can only deal with devices of type WifiNetDevice.
  Ptr<WifiNetDevice> device = nd->GetObject<WifiNetDevice> ();
  if (device == 0)
    {
      NS_LOG_INFO ("YansWifiHelper::EnablePcapInternal(): Device " << &device << " not of type ns3::WifiNetDevice");
      return;
    }

  Ptr<WifiPhy> phy = device->GetPhy ();
  NS_ABORT_MSG_IF (phy == 0, "YansWifiPhyHelper::EnablePcapInternal(): Phy layer in WifiNetDevice must be set");

  PcapHelper pcapHelper;

  std::string filename;
  if (explicitFilename)
    {
      filename = prefix;
    }
  else
    {
      filename = pcapHelper.GetFilenameFromDevice (prefix, device);
    }

  Ptr<PcapFileWrapper> file = pcapHelper.CreateFile (filename, std::ios::out, m_pcapDlt);

  phy->TraceConnectWithoutContext ("MonitorSnifferTx", MakeBoundCallback (&YansWifiPhyHelper::PcapSniffTxEvent, file));
  phy->TraceConnectWithoutContext ("MonitorSnifferRx", MakeBoundCallback (&YansWifiPhyHelper::PcapSniffRxEvent, file));
}

void
YansWifiPhyHelper::EnableAsciiInternal (
  Ptr<OutputStreamWrapper> stream,
  std::string prefix,
  Ptr<NetDevice> nd,
  bool explicitFilename)
{
  //All of the ascii enable functions vector through here including the ones
  //that are wandering through all of devices on perhaps all of the nodes in
  //the system. We can only deal with devices of type WifiNetDevice.
  Ptr<WifiNetDevice> device = nd->GetObject<WifiNetDevice> ();
  if (device == 0)
    {
      NS_LOG_INFO ("YansWifiHelper::EnableAsciiInternal(): Device " << device << " not of type ns3::WifiNetDevice");
      return;
    }

  //Our trace sinks are going to use packet printing, so we have to make sure
  //that is turned on.
  Packet::EnablePrinting ();

  uint32_t nodeid = nd->GetNode ()->GetId ();
  uint32_t deviceid = nd->GetIfIndex ();
  std::ostringstream oss;

  //If we are not provided an OutputStreamWrapper, we are expected to create
  //one using the usual trace filename conventions and write our traces
  //without a context since there will be one file per context and therefore
  //the context would be redundant.
  if (stream == 0)
    {
      //Set up an output stream object to deal with private ofstream copy
      //constructor and lifetime issues. Let the helper decide the actual
      //name of the file given the prefix.
      AsciiTraceHelper asciiTraceHelper;

      std::string filename;
      if (explicitFilename)
        {
          filename = prefix;
        }
      else
        {
          filename = asciiTraceHelper.GetFilenameFromDevice (prefix, device);
        }

      Ptr<OutputStreamWrapper> theStream = asciiTraceHelper.CreateFileStream (filename);
      //We could go poking through the phy and the state looking for the
      //correct trace source, but we can let Config deal with that with
      //some search cost.  Since this is presumably happening at topology
      //creation time, it doesn't seem much of a price to pay.
      oss.str ("");
      oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WifiNetDevice/Phy/State/RxOk";
      Config::ConnectWithoutContext (oss.str (), MakeBoundCallback (&AsciiPhyReceiveSinkWithoutContext, theStream));

      oss.str ("");
      oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WifiNetDevice/Phy/State/Tx";
      Config::ConnectWithoutContext (oss.str (), MakeBoundCallback (&AsciiPhyTransmitSinkWithoutContext, theStream));

      return;
    }

  //If we are provided an OutputStreamWrapper, we are expected to use it, and
  //to provide a context. We are free to come up with our own context if we
  //want, and use the AsciiTraceHelper Hook*WithContext functions, but for
  //compatibility and simplicity, we just use Config::Connect and let it deal
  //with coming up with a context.
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WifiNetDevice/Phy/State/RxOk";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiPhyReceiveSinkWithContext, stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WifiNetDevice/Phy/State/Tx";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiPhyTransmitSinkWithContext, stream));
}

} //namespace ns3
