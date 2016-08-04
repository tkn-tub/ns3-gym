/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
 * Copyright (c) 2009 MIRKO BANCHI
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
 *          Mirko Banchi <mk.banchi@gmail.com>
 */

#include "wifi-helper.h"
#include "ns3/wifi-net-device.h"
#include "ns3/wifi-mac.h"
#include "ns3/regular-wifi-mac.h"
#include "ns3/dca-txop.h"
#include "ns3/edca-txop-n.h"
#include "ns3/minstrel-wifi-manager.h"
#include "ns3/ap-wifi-mac.h"
#include "ns3/wifi-phy.h"
#include "ns3/ampdu-subframe-header.h"
#include "ns3/wifi-remote-station-manager.h"
#include "ns3/wifi-channel.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/mobility-model.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/radiotap-header.h"
#include "ns3/pcap-file-wrapper.h"
#include "ns3/abort.h"
#include "ns3/config.h"
#include "ns3/simulator.h"
#include "ns3/names.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("WifiHelper");

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

WifiPhyHelper::WifiPhyHelper ()
  : m_pcapDlt (PcapHelper::DLT_IEEE802_11)
{
}

WifiPhyHelper::~WifiPhyHelper ()
{
}

void
WifiPhyHelper::Set (std::string name, const AttributeValue &v)
{
  m_phy.Set (name, v);
}

void
WifiPhyHelper::SetErrorRateModel (std::string name,
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

void
WifiPhyHelper::PcapSniffTxEvent (
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
WifiPhyHelper::PcapSniffRxEvent (
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
WifiPhyHelper::SetPcapDataLinkType (enum SupportedPcapDataLinkTypes dlt)
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
      NS_ABORT_MSG ("WifiPhyHelper::SetPcapFormat(): Unexpected format");
    }
}

PcapHelper::DataLinkType
WifiPhyHelper::GetPcapDataLinkType (void) const
{
  return m_pcapDlt;
}

void
WifiPhyHelper::EnablePcapInternal (std::string prefix, Ptr<NetDevice> nd, bool promiscuous, bool explicitFilename)
{
  //All of the Pcap enable functions vector through here including the ones
  //that are wandering through all of devices on perhaps all of the nodes in
  //the system. We can only deal with devices of type WifiNetDevice.
  Ptr<WifiNetDevice> device = nd->GetObject<WifiNetDevice> ();
  if (device == 0)
    {
      NS_LOG_INFO ("WifiHelper::EnablePcapInternal(): Device " << &device << " not of type ns3::WifiNetDevice");
      return;
    }

  Ptr<WifiPhy> phy = device->GetPhy ();
  NS_ABORT_MSG_IF (phy == 0, "WifiPhyHelper::EnablePcapInternal(): Phy layer in WifiNetDevice must be set");

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

  phy->TraceConnectWithoutContext ("MonitorSnifferTx", MakeBoundCallback (&WifiPhyHelper::PcapSniffTxEvent, file));
  phy->TraceConnectWithoutContext ("MonitorSnifferRx", MakeBoundCallback (&WifiPhyHelper::PcapSniffRxEvent, file));
}

void
WifiPhyHelper::EnableAsciiInternal (
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
      NS_LOG_INFO ("WifiHelper::EnableAsciiInternal(): Device " << device << " not of type ns3::WifiNetDevice");
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

WifiHelper::~WifiHelper ()
{
}

WifiHelper::WifiHelper ()
  : m_standard (WIFI_PHY_STANDARD_80211a)
{
  SetRemoteStationManager ("ns3::ArfWifiManager");
}

WifiHelper
WifiHelper::Default (void)
{
  WifiHelper helper;
  helper.SetRemoteStationManager ("ns3::ArfWifiManager");
  return helper;
}

void
WifiHelper::SetRemoteStationManager (std::string type,
                                     std::string n0, const AttributeValue &v0,
                                     std::string n1, const AttributeValue &v1,
                                     std::string n2, const AttributeValue &v2,
                                     std::string n3, const AttributeValue &v3,
                                     std::string n4, const AttributeValue &v4,
                                     std::string n5, const AttributeValue &v5,
                                     std::string n6, const AttributeValue &v6,
                                     std::string n7, const AttributeValue &v7)
{
  m_stationManager = ObjectFactory ();
  m_stationManager.SetTypeId (type);
  m_stationManager.Set (n0, v0);
  m_stationManager.Set (n1, v1);
  m_stationManager.Set (n2, v2);
  m_stationManager.Set (n3, v3);
  m_stationManager.Set (n4, v4);
  m_stationManager.Set (n5, v5);
  m_stationManager.Set (n6, v6);
  m_stationManager.Set (n7, v7);
}

void
WifiHelper::SetStandard (enum WifiPhyStandard standard)
{
  m_standard = standard;
}

NetDeviceContainer
WifiHelper::Install (const WifiPhyHelper &phyHelper,
                     const WifiMacHelper &macHelper, NodeContainer c) const
{
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<WifiNetDevice> device = CreateObject<WifiNetDevice> ();
      Ptr<WifiRemoteStationManager> manager = m_stationManager.Create<WifiRemoteStationManager> ();
      Ptr<WifiMac> mac = macHelper.Create ();
      Ptr<WifiPhy> phy = phyHelper.Create (node, device);
      mac->SetAddress (Mac48Address::Allocate ());
      mac->ConfigureStandard (m_standard);
      phy->ConfigureStandard (m_standard);
      device->SetMac (mac);
      device->SetPhy (phy);
      device->SetRemoteStationManager (manager);
      node->AddDevice (device);
      devices.Add (device);
      NS_LOG_DEBUG ("node=" << node << ", mob=" << node->GetObject<MobilityModel> ());
    }
  return devices;
}

NetDeviceContainer
WifiHelper::Install (const WifiPhyHelper &phy,
                     const WifiMacHelper &mac, Ptr<Node> node) const
{
  return Install (phy, mac, NodeContainer (node));
}

NetDeviceContainer
WifiHelper::Install (const WifiPhyHelper &phy,
                     const WifiMacHelper &mac, std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  return Install (phy, mac, NodeContainer (node));
}

void
WifiHelper::EnableLogComponents (void)
{
  LogComponentEnable ("Aarfcd", LOG_LEVEL_ALL);
  LogComponentEnable ("AdhocWifiMac", LOG_LEVEL_ALL);
  LogComponentEnable ("AmrrWifiRemoteStation", LOG_LEVEL_ALL);
  LogComponentEnable ("ApWifiMac", LOG_LEVEL_ALL);
  LogComponentEnable ("ArfWifiManager", LOG_LEVEL_ALL);
  LogComponentEnable ("Cara", LOG_LEVEL_ALL);
  LogComponentEnable ("DcaTxop", LOG_LEVEL_ALL);
  LogComponentEnable ("DcfManager", LOG_LEVEL_ALL);
  LogComponentEnable ("DsssErrorRateModel", LOG_LEVEL_ALL);
  LogComponentEnable ("EdcaTxopN", LOG_LEVEL_ALL);
  LogComponentEnable ("InterferenceHelper", LOG_LEVEL_ALL);
  LogComponentEnable ("Jakes", LOG_LEVEL_ALL);
  LogComponentEnable ("MacLow", LOG_LEVEL_ALL);
  LogComponentEnable ("MacRxMiddle", LOG_LEVEL_ALL);
  LogComponentEnable ("MsduAggregator", LOG_LEVEL_ALL);
  LogComponentEnable ("MsduStandardAggregator", LOG_LEVEL_ALL);
  LogComponentEnable ("NistErrorRateModel", LOG_LEVEL_ALL);
  LogComponentEnable ("OnoeWifiRemoteStation", LOG_LEVEL_ALL);
  LogComponentEnable ("PropagationLossModel", LOG_LEVEL_ALL);
  LogComponentEnable ("RegularWifiMac", LOG_LEVEL_ALL);
  LogComponentEnable ("RraaWifiManager", LOG_LEVEL_ALL);
  LogComponentEnable ("StaWifiMac", LOG_LEVEL_ALL);
  LogComponentEnable ("SupportedRates", LOG_LEVEL_ALL);
  LogComponentEnable ("WifiChannel", LOG_LEVEL_ALL);
  LogComponentEnable ("WifiPhyStateHelper", LOG_LEVEL_ALL);
  LogComponentEnable ("WifiPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("WifiRemoteStationManager", LOG_LEVEL_ALL);
  LogComponentEnable ("YansErrorRateModel", LOG_LEVEL_ALL);
  LogComponentEnable ("YansWifiChannel", LOG_LEVEL_ALL);
  LogComponentEnable ("YansWifiPhy", LOG_LEVEL_ALL);
}

int64_t
WifiHelper::AssignStreams (NetDeviceContainer c, int64_t stream)
{
  int64_t currentStream = stream;
  Ptr<NetDevice> netDevice;
  for (NetDeviceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      netDevice = (*i);
      Ptr<WifiNetDevice> wifi = DynamicCast<WifiNetDevice> (netDevice);
      if (wifi)
        {
          //Handle any random numbers in the PHY objects.
          currentStream += wifi->GetPhy ()->AssignStreams (currentStream);

          //Handle any random numbers in the station managers.
          Ptr<WifiRemoteStationManager> manager = wifi->GetRemoteStationManager ();
          Ptr<MinstrelWifiManager> minstrel = DynamicCast<MinstrelWifiManager> (manager);
          if (minstrel)
            {
              currentStream += minstrel->AssignStreams (currentStream);
            }

          //Handle any random numbers in the MAC objects.
          Ptr<WifiMac> mac = wifi->GetMac ();
          Ptr<RegularWifiMac> rmac = DynamicCast<RegularWifiMac> (mac);
          if (rmac)
            {
              PointerValue ptr;
              rmac->GetAttribute ("DcaTxop", ptr);
              Ptr<DcaTxop> dcaTxop = ptr.Get<DcaTxop> ();
              currentStream += dcaTxop->AssignStreams (currentStream);

              rmac->GetAttribute ("VO_EdcaTxopN", ptr);
              Ptr<EdcaTxopN> vo_edcaTxopN = ptr.Get<EdcaTxopN> ();
              currentStream += vo_edcaTxopN->AssignStreams (currentStream);

              rmac->GetAttribute ("VI_EdcaTxopN", ptr);
              Ptr<EdcaTxopN> vi_edcaTxopN = ptr.Get<EdcaTxopN> ();
              currentStream += vi_edcaTxopN->AssignStreams (currentStream);

              rmac->GetAttribute ("BE_EdcaTxopN", ptr);
              Ptr<EdcaTxopN> be_edcaTxopN = ptr.Get<EdcaTxopN> ();
              currentStream += be_edcaTxopN->AssignStreams (currentStream);

              rmac->GetAttribute ("BK_EdcaTxopN", ptr);
              Ptr<EdcaTxopN> bk_edcaTxopN = ptr.Get<EdcaTxopN> ();
              currentStream += bk_edcaTxopN->AssignStreams (currentStream);

              //if an AP, handle any beacon jitter
              Ptr<ApWifiMac> apmac = DynamicCast<ApWifiMac> (rmac);
              if (apmac)
                {
                  currentStream += apmac->AssignStreams (currentStream);
                }
            }
        }
    }
  return (currentStream - stream);
}

} //namespace ns3
