/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

/*
 * Documentation kindly pointed out by Tom Henderson:
 * http://wiki.ethereal.com/Development/LibpcapFileFormat
 */

#include <fstream>

#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/abort.h"
#include "ns3/simulator.h"
#include "pcap-writer.h"
#include "packet.h"

NS_LOG_COMPONENT_DEFINE ("PcapWriter");

namespace ns3 {

enum {
  PCAP_ETHERNET = 1,
  PCAP_PPP      = 9,
  PCAP_RAW_IP   = 101,
  PCAP_80211    = 105,
  PCAP_80211_PRISM = 119,
  PCAP_80211_RADIOTAP  = 127,
};

PcapWriter::PcapWriter ()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC ("m_writer = 0");
  m_writer = 0;
}

PcapWriter::~PcapWriter ()
{
  NS_LOG_FUNCTION (this);

  if (m_writer != 0)
    {
      NS_LOG_LOGIC ("m_writer nonzero " << m_writer);
      if (m_writer->is_open ())
        {
          NS_LOG_LOGIC ("m_writer open.  Closing " << m_writer);
          m_writer->close ();
        }

      NS_LOG_LOGIC ("Deleting writer " << m_writer);
      delete m_writer;

      NS_LOG_LOGIC ("m_writer = 0");
      m_writer = 0;
    }
  else
    {
      NS_LOG_LOGIC ("m_writer == 0");
    }
}

void
PcapWriter::Open (std::string const &name)
{
  NS_LOG_FUNCTION (this << name);
  NS_ABORT_MSG_UNLESS (m_writer == 0, "PcapWriter::Open(): m_writer already allocated (std::ofstream leak detected)");

  m_writer = new std::ofstream ();
  NS_ABORT_MSG_UNLESS (m_writer, "PcapWriter::Open(): Cannot allocate m_writer");

  NS_LOG_LOGIC ("Created writer " << m_writer);

  m_writer->open (name.c_str (), std::ios_base::binary | std::ios_base::out);
  NS_ABORT_MSG_IF (m_writer->fail (), "PcapWriter::Open(): m_writer->open(" << name.c_str () << ") failed");

  NS_ASSERT_MSG (m_writer->is_open (), "PcapWriter::Open(): m_writer not open");

  NS_LOG_LOGIC ("Writer opened successfully");
}

void 
PcapWriter::WriteEthernetHeader (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  WriteHeader (PCAP_ETHERNET);
}

void 
PcapWriter::WriteIpHeader (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  WriteHeader (PCAP_RAW_IP);
}

void
PcapWriter::WriteWifiHeader (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  WriteHeader (PCAP_80211);
}

void
PcapWriter::WriteWifiRadiotapHeader (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  WriteHeader (PCAP_80211_RADIOTAP);
}

void
PcapWriter::WriteWifiPrismHeader (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  WriteHeader (PCAP_80211_PRISM);
}

void 
PcapWriter::WritePppHeader (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  WriteHeader (PCAP_PPP);
}

void 
PcapWriter::WriteHeader (uint32_t network)
{
  NS_LOG_FUNCTION (this << network);
  Write32 (0xa1b2c3d4);
  Write16 (2);
  Write16 (4);
  Write32 (0);
  Write32 (0);
  Write32 (0xffff);
  Write32 (network);
  m_pcapMode = network;
}

void 
PcapWriter::WritePacket (Ptr<const Packet> packet)
{
  if (m_writer != 0) 
    {
      uint64_t current = Simulator::Now ().GetMicroSeconds ();
      uint64_t s = current / 1000000;
      uint64_t us = current % 1000000;
      Write32 (s & 0xffffffff);
      Write32 (us & 0xffffffff);
      Write32 (packet->GetSize ());
      Write32 (packet->GetSize ());
      WriteData (packet->PeekData (), packet->GetSize ());
    }
}


void PcapWriter::WriteWifiMonitorPacket(Ptr<const Packet> packet, uint16_t channelFreqMhz, 
                                        uint32_t rate, bool isShortPreamble, bool isTx, 
                                        double signalDbm, double noiseDbm)
{  
  NS_LOG_FUNCTION (this << packet->GetSize() << channelFreqMhz << rate << isShortPreamble << isTx << signalDbm << noiseDbm);

  if (m_writer == 0) 
    {
      return;
    }

  if (m_pcapMode == PCAP_80211)
    {
      WritePacket (packet);    
      return;
    }
  
  /* the following is common between PRISM and RADIOTAP */
  
  uint64_t current = Simulator::Now ().GetMicroSeconds ();
  uint64_t s = current / 1000000;
  uint64_t us = current % 1000000;
  Write32 (s & 0xffffffff);
  Write32 (us & 0xffffffff);
    

  // MAC timestamp. Actually according to radiotap specifications
  // (http://www.radiotap.org/defined-fields/TSFT) this should be
  // the time "when the first bit of the MPDU arrived at the
  // MAC". This is not exactly what we're doing here, but to handle
  // this properly we would need to first of all investigate how
  // real devices (e.g. madwifi) handle this case, especially for TX
  // packets (radiotap specs says TSFT is not used for TX packets,
  // but madwifi actually uses it).
  uint64_t tsft = current;      
    
  if (m_pcapMode == PCAP_80211_PRISM)
    {
      
#define PRISM_MSG_CODE		 0x00000044
#define PRISM_MSG_LENGTH         144
#define PRISM_DID_HOSTTIME	 0x00010044
#define PRISM_DID_MACTIME	 0x00020044
#define PRISM_DID_CHANNEL	 0x00030044
#define PRISM_DID_RSSI	         0x00040044
#define PRISM_DID_SQ		 0x00050044
#define PRISM_DID_SIGNAL	 0x00060044
#define PRISM_DID_NOISE	         0x00070044
#define PRISM_DID_RATE	         0x00080044
#define PRISM_DID_ISTX	         0x00090044
#define PRISM_DID_FRMLEN	 0x000A0044

#define PRISM_STATUS_PRESENT    0
#define PRISM_STATUS_ABSENT     1
#define PRISM_ITEM_LENGTH       4



      uint32_t size = packet->GetSize () + PRISM_MSG_LENGTH;
      Write32 (size); // total packet size
      Write32 (size); // captured size

      Write32(PRISM_MSG_CODE);
      Write32(PRISM_MSG_LENGTH);
      WriteData((const uint8_t *)"unknown wifi device!!!!!!!!", 16);
    
      Write32(PRISM_DID_HOSTTIME);
      Write16(PRISM_STATUS_PRESENT);
      Write16(PRISM_ITEM_LENGTH); 
      // madwifi reports hosttime in jiffies. 
      // We calculate jiffies assuming HZ = 10
      Write32((uint32_t) (Now ().GetMilliSeconds () / 10 ) ); 

      Write32(PRISM_DID_MACTIME);
      Write16(PRISM_STATUS_PRESENT);
      Write16(PRISM_ITEM_LENGTH); 
      // This looses precision, which is a well-known issue of the prism
      // header format.
      Write32((uint32_t) tsft);

      Write32(PRISM_DID_CHANNEL);
      Write16(PRISM_STATUS_PRESENT);
      Write16(PRISM_ITEM_LENGTH); 
      // convert from frequency to channel number. This conversion is
      // correct only for IEEE 802.11b/g channels 1-13.
      Write32((channelFreqMhz - 2407) / 5);

      Write32(PRISM_DID_RSSI);
      Write16(PRISM_STATUS_PRESENT);
      Write16(PRISM_ITEM_LENGTH); 
      // madwifi here reports a value which is said to be "the value in
      // dBm above noise". Apart from the fact that this is incorrect
      // (if it is relative to a value in dBm, then it must be in dB,
      // not in dBm again), this means that in fact it is not a RSSI
      // (which stands for Received Signal Strength Indicator) but it is
      // rather a Signal to Noise Ratio (SNR), of course in dB.
      // Anyway, in the end we calculate the value exactly as madwifi does.
      Write32((uint32_t)round(signalDbm - noiseDbm));

      // SQ field not used. I would expect a PRISM_STATUS_ABSENT to be
      // needed here, but if you look at the prism header that madwifi
      // produces you'll just see that the whole field structure is
      // zeroed. 
      Write32(0);
      Write16(0);
      Write16(0); 
      Write32(0);

      Write32(PRISM_DID_SIGNAL);
      Write16(PRISM_STATUS_PRESENT);
      Write16(PRISM_ITEM_LENGTH); 
      Write32((uint32_t)round(signalDbm));

      Write32(PRISM_DID_NOISE);
      Write16(PRISM_STATUS_PRESENT);
      Write16(PRISM_ITEM_LENGTH); 
      Write32((uint32_t)round(noiseDbm));
            
      Write32(PRISM_DID_RATE);    
      Write16(PRISM_STATUS_PRESENT);
      Write16(PRISM_ITEM_LENGTH); 
      Write32(rate);
 
      Write32(PRISM_DID_ISTX);
      Write16(PRISM_STATUS_PRESENT);
      Write16(PRISM_ITEM_LENGTH); 
      Write32(isTx ? 1 : 0);

      Write32(PRISM_DID_FRMLEN);
      Write16(PRISM_STATUS_ABSENT);
      Write16(PRISM_ITEM_LENGTH); 
      Write32(packet->GetSize ());    
      
    

    } // PCAP_80211_PRISM

  else if (m_pcapMode == PCAP_80211_RADIOTAP)
    {      
      NS_LOG_LOGIC("writing radiotap packet");
      
#define	RADIOTAP_TSFT               0x00000001
#define	RADIOTAP_FLAGS              0x00000002
#define	RADIOTAP_RATE               0x00000004
#define RADIOTAP_CHANNEL            0x00000008
#define	RADIOTAP_FHSS               0x00000010
#define RADIOTAP_DBM_ANTSIGNAL      0x00000020
#define RADIOTAP_DBM_ANTNOISE       0x00000040
#define RADIOTAP_LOCK_QUALITY       0x00000080
#define RADIOTAP_TX_ATTENUATION     0x00000100    
#define RADIOTAP_DB_TX_ATTENUATION  0x00000200
#define RADIOTAP_DBM_TX_POWER       0x00000200
#define RADIOTAP_ANTENNA            0x00000400
#define RADIOTAP_DB_ANTSIGNAL       0x00000800
#define RADIOTAP_DB_ANTNOISE        0x00001000
#define RADIOTAP_EXT                0x10000000

#define	RADIOTAP_FLAG_NONE	   0x00	
#define	RADIOTAP_FLAG_CFP	   0x01	
#define	RADIOTAP_FLAG_SHORTPRE	   0x02	
#define	RADIOTAP_FLAG_WEP	   0x04	
#define	RADIOTAP_FLAG_FRAG	   0x08	
#define	RADIOTAP_FLAG_FCS	   0x10	
#define	RADIOTAP_FLAG_DATAPAD	   0x20	
#define	RADIOTAP_FLAG_BADFCS	   0x40	

#define	RADIOTAP_CHANNEL_TURBO	  0x0010
#define	RADIOTAP_CHANNEL_CCK	  0x0020
#define	RADIOTAP_CHANNEL_OFDM	  0x0040
#define	RADIOTAP_CHANNEL_2GHZ	  0x0080
#define	RADIOTAP_CHANNEL_5GHZ	  0x0100
#define	RADIOTAP_CHANNEL_PASSIVE  0x0200
#define	RADIOTAP_CHANNEL_DYN	  0x0400
#define	RADIOTAP_CHANNEL_GFSK	  0x0800

#define RADIOTAP_RX_PRESENT (RADIOTAP_TSFT | RADIOTAP_FLAGS | RADIOTAP_RATE | RADIOTAP_CHANNEL | RADIOTAP_DBM_ANTSIGNAL | RADIOTAP_DBM_ANTNOISE)
#define RADIOTAP_RX_LENGTH (8+8+1+1+2+2+1+1)

#define RADIOTAP_TX_PRESENT (RADIOTAP_TSFT | RADIOTAP_FLAGS  | RADIOTAP_RATE | RADIOTAP_CHANNEL)
#define RADIOTAP_TX_LENGTH (8+8+1+1+2+2)

      uint32_t size;
      if (isTx)
        {
          size = packet->GetSize () + RADIOTAP_TX_LENGTH;
        }
      else
        {
          size = packet->GetSize () + RADIOTAP_RX_LENGTH;
        }
      Write32 (size); // total packet size
      Write32 (size); // captured size

      Write8(0); // radiotap version
      Write8(0); // padding

      if (isTx)
        {
          Write16(RADIOTAP_TX_LENGTH); 
          Write32(RADIOTAP_TX_PRESENT); 
        }
      else
        {
          Write16(RADIOTAP_RX_LENGTH); 
          Write32(RADIOTAP_RX_PRESENT); 
        }

      Write64(tsft); 
      
      uint8_t flags = RADIOTAP_FLAG_NONE;
      if (isShortPreamble)
        {
          flags |= RADIOTAP_FLAG_SHORTPRE; 
        }
      Write8(flags);
      

      Write8(rate); 

      Write16((uint16_t) channelFreqMhz); 

      // we might want to make this setting depend on the WifiMode and
      // on the ChannelFrequency at some time in the future. But for now
      // I think a fixed setting is more than enough for most purposes.
      Write16(RADIOTAP_CHANNEL_OFDM | RADIOTAP_CHANNEL_2GHZ); 
    
      if (!isTx)
        {
          
          Write8 (RoundToInt8 (signalDbm)); 
          Write8 (RoundToInt8 (noiseDbm)); 
        }

    } // PCAP_80211_RADIOTAP

    
  else
    {
      NS_LOG_ERROR("unknown PCAP mode");      
      return;
    }    

  // finally, write rest of packet
  WriteData (packet->PeekData (), packet->GetSize ());
}
    
  


int8_t 
PcapWriter::RoundToInt8 (double value)
{
  if (value < -128)
    {
      return -128;
    }
  if (value > 127)
    {
      return 127;
    }
  return ((int8_t) round(value));
}






void
PcapWriter::WriteData (uint8_t const*buffer, uint32_t size)
{
  NS_LOG_FUNCTION(this << size);
  m_writer->write ((char const *)buffer, size);
}


void
PcapWriter::Write64 (uint64_t data)
{
  uint8_t buffer[8];
  buffer[0] = (data >> 0) & 0xff;
  buffer[1] = (data >> 8) & 0xff;
  buffer[2] = (data >> 16) & 0xff;
  buffer[3] = (data >> 24) & 0xff;
  buffer[4] = (data >> 32) & 0xff;
  buffer[5] = (data >> 40) & 0xff;
  buffer[6] = (data >> 48) & 0xff;
  buffer[7] = (data >> 56) & 0xff;
  WriteData (buffer, 8);
}

void
PcapWriter::Write32 (uint32_t data)
{
  uint8_t buffer[4];
  buffer[0] = (data >> 0) & 0xff;
  buffer[1] = (data >> 8) & 0xff;
  buffer[2] = (data >> 16) & 0xff;
  buffer[3] = (data >> 24) & 0xff;
  WriteData (buffer, 4);
}

void
PcapWriter::Write16 (uint16_t data)
{
  uint8_t buffer[2];
  buffer[0] = (data >> 0) & 0xff;
  buffer[1] = (data >> 8) & 0xff;
  WriteData (buffer, 2);
}

void
PcapWriter::Write8 (uint8_t data)
{
  WriteData (&data, 1);
}



} // namespace ns3
