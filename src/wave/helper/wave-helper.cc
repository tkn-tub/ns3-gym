/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Author: Junling Bu <linlinjavaer@gmail.com>
 */
#include "ns3/wifi-mac.h"
#include "ns3/wifi-phy.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/string.h"
#include "ns3/wifi-mode.h"
#include "ns3/config.h"
#include "ns3/names.h"
#include "ns3/abort.h"
#include "ns3/ampdu-subframe-header.h"
#include "ns3/wave-net-device.h"
#include "ns3/minstrel-wifi-manager.h"
#include "ns3/radiotap-header.h"
#include "ns3/unused.h"
#include "wave-mac-helper.h"
#include "wave-helper.h"

NS_LOG_COMPONENT_DEFINE ("WaveHelper");

namespace ns3 {

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

static void
PcapSniffTxEvent (
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
            if (txVector.GetChannelWidth () == 40000000)
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
            if (aMpdu.packetType == 2 || (hdr.GetEof () == true && hdr.GetLength () > 0))
              {
                ampduStatusFlags |= RadiotapHeader::A_MPDU_STATUS_LAST;
              }
            header.SetAmpduStatus (aMpdu.referenceNumber, ampduStatusFlags, hdr.GetCrc ());
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
            if (txVector.GetChannelWidth () == 40000000)
              {
                vhtBandwidth = 1;
              }
            else if (txVector.GetChannelWidth () == 80000000)
              {
                vhtBandwidth = 4;
              }
            else if (txVector.GetChannelWidth () == 160000000)
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

static void
PcapSniffRxEvent (
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
            if (txVector.GetChannelWidth () == 40000000)
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
            uint16_t ampduStatusFlags = 0;
            ampduStatusFlags |= RadiotapHeader::A_MPDU_STATUS_DELIMITER_CRC_KNOWN;
            ampduStatusFlags |= RadiotapHeader::A_MPDU_STATUS_LAST_KNOWN;
            /* For PCAP file, MPDU Delimiter and Padding should be removed by the MAC Driver */
            AmpduSubframeHeader hdr;
            uint32_t extractedLength;
            p->RemoveHeader (hdr);
            extractedLength = hdr.GetLength ();
            p = p->CreateFragment (0, static_cast<uint32_t> (extractedLength));
            if (aMpdu.packetType == 2 || (hdr.GetEof () == true && hdr.GetLength () > 0))
              {
                ampduStatusFlags |= RadiotapHeader::A_MPDU_STATUS_LAST;
              }
            header.SetAmpduStatus (aMpdu.referenceNumber, ampduStatusFlags, hdr.GetCrc ());
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
            if (txVector.GetChannelWidth () == 40000000)
              {
                vhtBandwidth = 1;
              }
            else if (txVector.GetChannelWidth () == 80000000)
              {
                vhtBandwidth = 4;
              }
            else if (txVector.GetChannelWidth () == 160000000)
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

/****************************** YansWavePhyHelper ***********************************/
YansWavePhyHelper
YansWavePhyHelper::Default (void)
{
  YansWavePhyHelper helper;
  helper.SetErrorRateModel ("ns3::NistErrorRateModel");
  return helper;
}

void
YansWavePhyHelper::EnablePcapInternal (std::string prefix, Ptr<NetDevice> nd, bool promiscuous, bool explicitFilename)
{
  //
  // All of the Pcap enable functions vector through here including the ones
  // that are wandering through all of devices on perhaps all of the nodes in
  // the system.  We can only deal with devices of type WaveNetDevice.
  //
  Ptr<WaveNetDevice> device = nd->GetObject<WaveNetDevice> ();
  if (device == 0)
    {
      NS_LOG_INFO ("YansWavePhyHelper::EnablePcapInternal(): Device " << &device << " not of type ns3::WaveNetDevice");
      return;
    }

  std::vector<Ptr<WifiPhy> > phys = device->GetPhys ();
  NS_ABORT_MSG_IF (phys.size () == 0, "EnablePcapInternal(): Phy layer in WaveNetDevice must be set");

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

  Ptr<PcapFileWrapper> file = pcapHelper.CreateFile (filename, std::ios::out, GetPcapDataLinkType ());

  std::vector<Ptr<WifiPhy> >::iterator i;
  for (i = phys.begin (); i != phys.end (); ++i)
    {
      Ptr<WifiPhy> phy = (*i);
      phy->TraceConnectWithoutContext ("MonitorSnifferTx", MakeBoundCallback (&PcapSniffTxEvent, file));
      phy->TraceConnectWithoutContext ("MonitorSnifferRx", MakeBoundCallback (&PcapSniffRxEvent, file));
    }
}

void
YansWavePhyHelper::EnableAsciiInternal (
  Ptr<OutputStreamWrapper> stream,
  std::string prefix,
  Ptr<NetDevice> nd,
  bool explicitFilename)
{
  //
  // All of the ascii enable functions vector through here including the ones
  // that are wandering through all of devices on perhaps all of the nodes in
  // the system.  We can only deal with devices of type WaveNetDevice.
  //
  Ptr<WaveNetDevice> device = nd->GetObject<WaveNetDevice> ();
  if (device == 0)
    {
      NS_LOG_INFO ("EnableAsciiInternal(): Device " << device << " not of type ns3::WaveNetDevice");
      return;
    }

  //
  // Our trace sinks are going to use packet printing, so we have to make sure
  // that is turned on.
  //
  Packet::EnablePrinting ();

  uint32_t nodeid = nd->GetNode ()->GetId ();
  uint32_t deviceid = nd->GetIfIndex ();
  std::ostringstream oss;

  //
  // If we are not provided an OutputStreamWrapper, we are expected to create
  // one using the usual trace filename conventions and write our traces
  // without a context since there will be one file per context and therefore
  // the context would be redundant.
  //
  if (stream == 0)
    {
      //
      // Set up an output stream object to deal with private ofstream copy
      // constructor and lifetime issues.  Let the helper decide the actual
      // name of the file given the prefix.
      //
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
      //
      // We could go poking through the phy and the state looking for the
      // correct trace source, but we can let Config deal with that with
      // some search cost.  Since this is presumably happening at topology
      // creation time, it doesn't seem much of a price to pay.
      //
      oss.str ("");
      oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WaveNetDevice/PhyEntities/*/$ns3::WifiPhy/State/RxOk";
      Config::ConnectWithoutContext (oss.str (), MakeBoundCallback (&AsciiPhyReceiveSinkWithoutContext, theStream));

      oss.str ("");
      oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WaveNetDevice/PhyEntities/*/$ns3::WifiPhy/State/Tx";
      Config::ConnectWithoutContext (oss.str (), MakeBoundCallback (&AsciiPhyTransmitSinkWithoutContext, theStream));

      return;
    }

  //
  // If we are provided an OutputStreamWrapper, we are expected to use it, and
  // to provide a context.  We are free to come up with our own context if we
  // want, and use the AsciiTraceHelper Hook*WithContext functions, but for
  // compatibility and simplicity, we just use Config::Connect and let it deal
  // with coming up with a context.
  //
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WaveNetDevice/PhyEntities/*/$ns3::WifiPhy/RxOk";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiPhyReceiveSinkWithContext, stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WaveNetDevice/PhyEntities/*/$ns3::WifiPhy/State/Tx";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiPhyTransmitSinkWithContext, stream));
}

/********************************** WaveHelper ******************************************/
WaveHelper::WaveHelper ()
{
}

WaveHelper::~WaveHelper ()
{
}

WaveHelper
WaveHelper::Default (void)
{
  WaveHelper helper;
  // default 7 MAC entities and single PHY device.
  helper.CreateMacForChannel (ChannelManager::GetWaveChannels ());
  helper.CreatePhys (1);
  helper.SetChannelScheduler ("ns3::DefaultChannelScheduler");
  helper.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                  "DataMode", StringValue ("OfdmRate6MbpsBW10MHz"),
                                  "ControlMode",StringValue ("OfdmRate6MbpsBW10MHz"),
                                  "NonUnicastMode", StringValue ("OfdmRate6MbpsBW10MHz"));
  return helper;
}

void
WaveHelper::CreateMacForChannel (std::vector<uint32_t> channelNumbers)
{
  if (channelNumbers.size () == 0)
    {
      NS_FATAL_ERROR ("the WAVE MAC entities is at least one");
    }
  for (std::vector<uint32_t>::iterator i = channelNumbers.begin (); i != channelNumbers.end (); ++i)
    {
      if (!ChannelManager::IsWaveChannel (*i))
        {
          NS_FATAL_ERROR ("the channel number " << (*i) << " is not a valid WAVE channel number");
        }
    }
  m_macsForChannelNumber = channelNumbers;
}

void
WaveHelper::CreatePhys (uint32_t phys)
{
  if (phys == 0)
    {
      NS_FATAL_ERROR ("the WAVE PHY entities is at least one");
    }
  if (phys > ChannelManager::GetNumberOfWaveChannels ())
    {
      NS_FATAL_ERROR ("the number of assigned WAVE PHY entities is more than the number of valid WAVE channels");
    }
  m_physNumber = phys;
}

void
WaveHelper::SetRemoteStationManager (std::string type,
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
WaveHelper::SetChannelScheduler (std::string type,
                                 std::string n0, const AttributeValue &v0,
                                 std::string n1, const AttributeValue &v1,
                                 std::string n2, const AttributeValue &v2,
                                 std::string n3, const AttributeValue &v3,
                                 std::string n4, const AttributeValue &v4,
                                 std::string n5, const AttributeValue &v5,
                                 std::string n6, const AttributeValue &v6,
                                 std::string n7, const AttributeValue &v7)
{
  m_channelScheduler = ObjectFactory ();
  m_channelScheduler.SetTypeId (type);
  m_channelScheduler.Set (n0, v0);
  m_channelScheduler.Set (n1, v1);
  m_channelScheduler.Set (n2, v2);
  m_channelScheduler.Set (n3, v3);
  m_channelScheduler.Set (n4, v4);
  m_channelScheduler.Set (n5, v5);
  m_channelScheduler.Set (n6, v6);
  m_channelScheduler.Set (n7, v7);
}

NetDeviceContainer
WaveHelper::Install (const WifiPhyHelper &phyHelper,  const WifiMacHelper &macHelper, NodeContainer c) const
{
  try
    {
      const QosWaveMacHelper& qosMac = dynamic_cast<const QosWaveMacHelper&> (macHelper);
      NS_UNUSED (qosMac);
    }
  catch (const std::bad_cast &)
    {
      NS_FATAL_ERROR ("WifiMacHelper should be the class or subclass of QosWaveMacHelper");
    }

  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<WaveNetDevice> device = CreateObject<WaveNetDevice> ();

      device->SetChannelManager (CreateObject<ChannelManager> ());
      device->SetChannelCoordinator (CreateObject<ChannelCoordinator> ());
      device->SetVsaManager (CreateObject<VsaManager> ());
      device->SetChannelScheduler (m_channelScheduler.Create<ChannelScheduler> ());

      for (uint32_t j = 0; j != m_physNumber; ++j)
        {
          Ptr<WifiPhy> phy = phyHelper.Create (node, device);
          phy->ConfigureStandard (WIFI_PHY_STANDARD_80211_10MHZ);
          phy->SetChannelNumber (ChannelManager::GetCch ());
          device->AddPhy (phy);
        }

      for (std::vector<uint32_t>::const_iterator k = m_macsForChannelNumber.begin ();
           k != m_macsForChannelNumber.end (); ++k)
        {
          Ptr<WifiMac> wifiMac = macHelper.Create ();
          Ptr<OcbWifiMac> ocbMac = DynamicCast<OcbWifiMac> (wifiMac);
          // we use WaveMacLow to replace original MacLow
          ocbMac->EnableForWave (device);
          ocbMac->SetWifiRemoteStationManager ( m_stationManager.Create<WifiRemoteStationManager> ());
          ocbMac->ConfigureStandard (WIFI_PHY_STANDARD_80211_10MHZ);
          device->AddMac (*k, ocbMac);
        }

      device->SetAddress (Mac48Address::Allocate ());

      node->AddDevice (device);
      devices.Add (device);
    }
  return devices;
}

NetDeviceContainer
WaveHelper::Install (const WifiPhyHelper &phy, const WifiMacHelper &mac, Ptr<Node> node) const
{
  return Install (phy, mac, NodeContainer (node));
}

NetDeviceContainer
WaveHelper::Install (const WifiPhyHelper &phy, const WifiMacHelper &mac, std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  return Install (phy, mac, NodeContainer (node));
}

void
WaveHelper::EnableLogComponents (void)
{
  WifiHelper::EnableLogComponents ();

  LogComponentEnable ("WaveNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("ChannelCoordinator", LOG_LEVEL_ALL);
  LogComponentEnable ("ChannelManager", LOG_LEVEL_ALL);
  LogComponentEnable ("ChannelScheduler", LOG_LEVEL_ALL);
  LogComponentEnable ("DefaultChannelScheduler", LOG_LEVEL_ALL);
  LogComponentEnable ("VsaManager", LOG_LEVEL_ALL);
  LogComponentEnable ("OcbWifiMac", LOG_LEVEL_ALL);
  LogComponentEnable ("VendorSpecificAction", LOG_LEVEL_ALL);
  LogComponentEnable ("WaveMacLow", LOG_LEVEL_ALL);
  LogComponentEnable ("HigherLayerTxVectorTag", LOG_LEVEL_ALL);
}

int64_t
WaveHelper::AssignStreams (NetDeviceContainer c, int64_t stream)
{
  int64_t currentStream = stream;
  Ptr<NetDevice> netDevice;
  for (NetDeviceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      netDevice = (*i);
      Ptr<WaveNetDevice> wave = DynamicCast<WaveNetDevice> (netDevice);
      if (wave)
        {
          // Handle any random numbers in the PHY objects.
          std::vector<Ptr<WifiPhy> > phys = wave->GetPhys ();
          for (std::vector<Ptr<WifiPhy> >::iterator j = phys.begin (); j != phys.end (); ++i)
            {
              currentStream += (*j)->AssignStreams (currentStream);
            }

          // Handle any random numbers in the MAC objects.
          std::map<uint32_t, Ptr<OcbWifiMac> > macs = wave->GetMacs ();
          for ( std::map<uint32_t, Ptr<OcbWifiMac> >::iterator k = macs.begin (); k != macs.end (); ++k)
            {
              Ptr<RegularWifiMac> rmac = DynamicCast<RegularWifiMac> (k->second);

              // Handle any random numbers in the station managers.
              Ptr<WifiRemoteStationManager> manager = rmac->GetWifiRemoteStationManager ();
              Ptr<MinstrelWifiManager> minstrel = DynamicCast<MinstrelWifiManager> (manager);
              if (minstrel)
                {
                  currentStream += minstrel->AssignStreams (currentStream);
                }

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
            }
        }
    }
  return (currentStream - stream);
}
} // namespace ns3
