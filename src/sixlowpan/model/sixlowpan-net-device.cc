/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Universita' di Firenze, Italy
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
 * Author: Tommaso Pecorella <tommaso.pecorella@unifi.it>
 *         Michele Muccio <michelemuccio@virgilio.it>
 */

#include "ns3/node.h"
#include "ns3/channel.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/boolean.h"
#include "ns3/abort.h"
#include "ns3/simulator.h"
#include "ns3/uinteger.h"
#include "ns3/icmpv6-header.h"
#include "ns3/ipv6-header.h"
#include "ns3/random-variable.h"
#include "ns3/mac16-address.h"
#include "ns3/mac48-address.h"
#include "ns3/mac64-address.h"
#include "ns3/unused.h"
#include "ns3/ipv6-l3-protocol.h"
#include "ns3/ipv6-extension-header.h"
#include "ns3/udp-header.h"
#include "ns3/udp-l4-protocol.h"
#include "sixlowpan-net-device.h"
#include "sixlowpan-header.h"

NS_LOG_COMPONENT_DEFINE ("SixLowPanNetDevice");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (SixLowPanNetDevice);

TypeId SixLowPanNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SixLowPanNetDevice")
    .SetParent<NetDevice> ()
    .AddConstructor<SixLowPanNetDevice> ()
    .AddAttribute ("Rfc6282", "Use RFC6282 (IPHC) if true, RFC4944 (HC1) otherwise.",
                   BooleanValue (true),
                   MakeBooleanAccessor (&SixLowPanNetDevice::m_useIphc),
                   MakeBooleanChecker ())
    .AddAttribute ("OmitUdpChecksum",
                   "Omit the UDP checksum in IPHC compression.",
                   BooleanValue (true),
                   MakeBooleanAccessor (&SixLowPanNetDevice::m_omitUdpChecksum),
                   MakeBooleanChecker ())
    .AddAttribute ("FragmentReassemblyListSize", "The maximum size of the reassembly buffer (in packets). Zero meaning infinite.",
                   UintegerValue (0),
                   MakeUintegerAccessor (&SixLowPanNetDevice::m_fragmentReassemblyListSize),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("FragmentExpirationTimeout",
                   "When this timeout expires, the fragments will be cleared from the buffer.",
                   TimeValue (Seconds (60)),
                   MakeTimeAccessor (&SixLowPanNetDevice::m_fragmentExpirationTimeout),
                   MakeTimeChecker ())
    .AddAttribute ("CompressionThreshold",
                   "The minimum MAC layer payload size.",
                   UintegerValue (0x0),
                   MakeUintegerAccessor (&SixLowPanNetDevice::m_compressionThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ForceEtherType",
                   "Force a specific EtherType in L2 frames.",
                   BooleanValue (false),
                   MakeBooleanAccessor (&SixLowPanNetDevice::m_forceEtherType),
                   MakeBooleanChecker ())
    .AddAttribute ("EtherType",
                   "The specific EtherType to be used in L2 frames.",
                   UintegerValue (0xFFFF),
                   MakeUintegerAccessor (&SixLowPanNetDevice::m_etherType),
                   MakeUintegerChecker<uint16_t> ())
    .AddTraceSource ("Tx", "Send - packet (including 6LoWPAN header), SixLoWPanNetDevice Ptr, interface index.",
                     MakeTraceSourceAccessor (&SixLowPanNetDevice::m_txTrace))
    .AddTraceSource ("Rx", "Receive - packet (including 6LoWPAN header), SixLoWPanNetDevice Ptr, interface index.",
                     MakeTraceSourceAccessor (&SixLowPanNetDevice::m_rxTrace))
    .AddTraceSource ("Drop", "Drop - DropReason, packet (including 6LoWPAN header), SixLoWPanNetDevice Ptr, interface index.",
                     MakeTraceSourceAccessor (&SixLowPanNetDevice::m_dropTrace))
  ;
  return tid;
}

SixLowPanNetDevice::SixLowPanNetDevice ()
  : m_node (0),
    m_netDevice (0),
    m_ifIndex (0)
{
  NS_LOG_FUNCTION (this);
  m_netDevice = 0;
  m_rng = CreateObject<UniformRandomVariable> ();
}

Ptr<NetDevice> SixLowPanNetDevice::GetNetDevice () const
{
  NS_LOG_FUNCTION (this);
  return m_netDevice;
}

void SixLowPanNetDevice::SetNetDevice (Ptr<NetDevice> device)
{
  NS_LOG_FUNCTION (this << device);
  m_netDevice = device;

  NS_LOG_DEBUG ("RegisterProtocolHandler for " << device->GetInstanceTypeId ().GetName ());

  uint16_t protocolType = 0;
  if ( m_forceEtherType )
    {
      protocolType = m_etherType;
    }
  m_node->RegisterProtocolHandler (MakeCallback (&SixLowPanNetDevice::ReceiveFromDevice,
                                                 this),
                                   protocolType, device, false);
}

int64_t SixLowPanNetDevice::AssignStreams (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);
  m_rng->SetStream (stream);
  return 1;
}

void SixLowPanNetDevice::DoDispose ()
{
  NS_LOG_FUNCTION (this);

  m_netDevice = 0;
  m_node = 0;

  for (MapFragmentsTimersI_t iter = m_fragmentsTimers.begin (); iter != m_fragmentsTimers.end (); iter++)
    {
      iter->second.Cancel ();
    }
  m_fragmentsTimers.clear ();

  for (MapFragmentsI_t iter = m_fragments.begin (); iter != m_fragments.end (); iter++)
    {
      iter->second = 0;
    }
  m_fragments.clear ();

  NetDevice::DoDispose ();
}

void SixLowPanNetDevice::ReceiveFromDevice (Ptr<NetDevice> incomingPort,
                                            Ptr<const Packet> packet,
                                            uint16_t protocol,
                                            Address const &src,
                                            Address const &dst,
                                            PacketType packetType)
{
  NS_LOG_FUNCTION (this << incomingPort << packet << protocol << src << dst);
  NS_LOG_DEBUG ("UID is " << packet->GetUid ());

  uint8_t dispatchRawVal = 0;
  SixLowPanDispatch::Dispatch_e dispatchVal;
  Ptr<Packet> copyPkt = packet->Copy ();

  m_rxTrace (copyPkt, m_node->GetObject<SixLowPanNetDevice> (), GetIfIndex ());

  copyPkt->CopyData (&dispatchRawVal, sizeof(dispatchRawVal));
  dispatchVal = SixLowPanDispatch::GetDispatchType (dispatchRawVal);
  bool isPktDecompressed = false;
  bool fragmented = false;

  NS_LOG_DEBUG ( "Packet received: " << *copyPkt );
  NS_LOG_DEBUG ( "Packet length: " << copyPkt->GetSize () );
  NS_LOG_DEBUG ( "Dispatches: " << int(dispatchRawVal) << " - " << int(dispatchVal) );

  if ( dispatchVal == SixLowPanDispatch::LOWPAN_FRAG1 )
    {
      isPktDecompressed = ProcessFragment (copyPkt, src, dst, true);
      fragmented = true;
    }
  else if ( dispatchVal == SixLowPanDispatch::LOWPAN_FRAGN )
    {
      isPktDecompressed = ProcessFragment (copyPkt, src, dst, false);
      fragmented = true;
    }
  if ( fragmented )
    {
      if ( !isPktDecompressed )
        {
          return;
        }
      else
        {
          copyPkt->CopyData (&dispatchRawVal, sizeof(dispatchRawVal));
          dispatchVal = SixLowPanDispatch::GetDispatchType (dispatchRawVal);
        }
    }

  switch ( dispatchVal )
    {
    case SixLowPanDispatch::LOWPAN_MESH:
      NS_LOG_DEBUG ("Unsupported 6LoWPAN encoding: MESH, dropping.");
      m_dropTrace (DROP_UNKNOWN_EXTENSION, copyPkt, m_node->GetObject<SixLowPanNetDevice> (), GetIfIndex ());
      break;
    case SixLowPanDispatch::LOWPAN_BC0:
      NS_LOG_DEBUG ("Unsupported 6LoWPAN encoding: BC0, dropping.");
      m_dropTrace (DROP_UNKNOWN_EXTENSION, copyPkt, m_node->GetObject<SixLowPanNetDevice> (), GetIfIndex ());
      break;
    case SixLowPanDispatch::LOWPAN_IPv6:
      NS_LOG_DEBUG ( "Packet without compression. Length: " << copyPkt->GetSize () );
      {
        SixLowPanIpv6 uncompressedHdr;
        copyPkt->RemoveHeader(uncompressedHdr);
        isPktDecompressed = true;
      }
      break;
    case SixLowPanDispatch::LOWPAN_HC1:
      DecompressLowPanHc1 (copyPkt, src, dst);
      isPktDecompressed = true;
      break;
    case SixLowPanDispatch::LOWPAN_IPHC:
      DecompressLowPanIphc (copyPkt, src, dst);
      isPktDecompressed = true;
      break;
    default:
      NS_LOG_DEBUG ("Unsupported 6LoWPAN encoding: dropping.");
      m_dropTrace (DROP_UNKNOWN_EXTENSION, copyPkt, m_node->GetObject<SixLowPanNetDevice> (), GetIfIndex ());
      break;
    }

  if ( !isPktDecompressed )
    {
      return;
    }

  NS_LOG_DEBUG ( "Packet decompressed length: " << copyPkt->GetSize () );
  NS_LOG_DEBUG ( "Packet decompressed received: " << *copyPkt );

  if (!m_promiscRxCallback.IsNull ())
    {
      m_promiscRxCallback (this, copyPkt, Ipv6L3Protocol::PROT_NUMBER, src, dst, packetType);
    }

  m_rxCallback (this, copyPkt, Ipv6L3Protocol::PROT_NUMBER, src);

  return;
}

void SixLowPanNetDevice::SetIfIndex (const uint32_t index)
{
  NS_LOG_FUNCTION (this << index);
  m_ifIndex = index;
}

uint32_t SixLowPanNetDevice::GetIfIndex (void) const
{
  NS_LOG_FUNCTION (this);
  return m_ifIndex;
}

Ptr<Channel> SixLowPanNetDevice::GetChannel (void) const
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG ( m_netDevice != 0, "Sixlowpan: can't find any lower-layer protocol " << m_netDevice );

  return m_netDevice->GetChannel ();
}

void SixLowPanNetDevice::SetAddress (Address address)
{
  NS_LOG_FUNCTION (this << address);
  NS_ASSERT_MSG ( m_netDevice != 0, "Sixlowpan: can't find any lower-layer protocol " << m_netDevice );

  m_netDevice->SetAddress (address);
}

Address SixLowPanNetDevice::GetAddress (void) const
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG ( m_netDevice != 0, "Sixlowpan: can't find any lower-layer protocol " << m_netDevice );

  return m_netDevice->GetAddress ();
}

bool SixLowPanNetDevice::SetMtu (const uint16_t mtu)
{
  NS_LOG_FUNCTION (this << mtu);
  NS_ASSERT_MSG ( m_netDevice != 0, "Sixlowpan: can't find any lower-layer protocol " << m_netDevice );

  return m_netDevice->SetMtu (mtu);
}

uint16_t SixLowPanNetDevice::GetMtu (void) const
{
  NS_LOG_FUNCTION (this);

  uint16_t mtu = m_netDevice->GetMtu ();

  // RFC 4944, section 4.
  if (mtu < 1280)
    {
      mtu = 1280;
    }
  return mtu;
}

bool SixLowPanNetDevice::IsLinkUp (void) const
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG ( m_netDevice != 0, "Sixlowpan: can't find any lower-layer protocol " << m_netDevice );

  return m_netDevice->IsLinkUp ();
}

void SixLowPanNetDevice::AddLinkChangeCallback (Callback<void> callback)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG ( m_netDevice != 0, "Sixlowpan: can't find any lower-layer protocol " << m_netDevice );

  return m_netDevice->AddLinkChangeCallback (callback);
}

bool SixLowPanNetDevice::IsBroadcast (void) const
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG ( m_netDevice != 0, "Sixlowpan: can't find any lower-layer protocol " << m_netDevice );

  return m_netDevice->IsBroadcast ();
}

Address SixLowPanNetDevice::GetBroadcast (void) const
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG ( m_netDevice != 0, "Sixlowpan: can't find any lower-layer protocol " << m_netDevice );

  return m_netDevice->GetBroadcast ();
}

bool SixLowPanNetDevice::IsMulticast (void) const
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG ( m_netDevice != 0, "Sixlowpan: can't find any lower-layer protocol " << m_netDevice );

  return m_netDevice->IsMulticast ();
}

Address SixLowPanNetDevice::GetMulticast (Ipv4Address multicastGroup) const
{
  NS_LOG_FUNCTION (this << multicastGroup);
  NS_ASSERT_MSG ( m_netDevice != 0, "Sixlowpan: can't find any lower-layer protocol " << m_netDevice );

  return m_netDevice->GetMulticast (multicastGroup);
}

Address SixLowPanNetDevice::GetMulticast (Ipv6Address addr) const
{
  NS_LOG_FUNCTION (this << addr);
  NS_ASSERT_MSG ( m_netDevice != 0, "Sixlowpan: can't find any lower-layer protocol " << m_netDevice );

  return m_netDevice->GetMulticast (addr);
}

bool SixLowPanNetDevice::IsPointToPoint (void) const
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG ( m_netDevice != 0, "Sixlowpan: can't find any lower-layer protocol " << m_netDevice );

  return m_netDevice->IsPointToPoint ();
}

bool SixLowPanNetDevice::IsBridge (void) const
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG ( m_netDevice != 0, "Sixlowpan: can't find any lower-layer protocol " << m_netDevice );

  return m_netDevice->IsBridge ();
}

bool SixLowPanNetDevice::Send (Ptr<Packet> packet,
                               const Address& dest,
                               uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << *packet << dest << protocolNumber);
  bool ret = false;
  Address src;

  ret = DoSend (packet, src, dest, protocolNumber, false);
  return ret;
}

bool SixLowPanNetDevice::SendFrom (Ptr<Packet> packet,
                                   const Address& src,
                                   const Address& dest,
                                   uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << *packet << src << dest << protocolNumber);
  bool ret = false;

  ret = DoSend (packet, src, dest, protocolNumber, true);
  return ret;
}

bool SixLowPanNetDevice::DoSend (Ptr<Packet> packet,
                                 const Address& src,
                                 const Address& dest,
                                 uint16_t protocolNumber,
                                 bool doSendFrom)
{
  NS_LOG_FUNCTION (this << *packet << src << dest << protocolNumber << doSendFrom);
  NS_ASSERT_MSG ( m_netDevice != 0, "Sixlowpan: can't find any lower-layer protocol " << m_netDevice );

  Ptr<Packet> origPacket = packet->Copy ();
  uint32_t origHdrSize = 0;
  uint32_t origPacketSize = packet->GetSize ();
  bool ret = false;

  if (m_forceEtherType)
    {
      protocolNumber = m_etherType;
    }

  if (m_useIphc)
    {
      origHdrSize += CompressLowPanIphc (packet, m_netDevice->GetAddress (), dest);
    }
  else
    {
      origHdrSize += CompressLowPanHc1 (packet, m_netDevice->GetAddress (), dest);
    }

  if ( packet->GetSize () > m_netDevice->GetMtu () )
    {
      NS_LOG_LOGIC ("Fragmentation: Packet size " << packet->GetSize () << " - Mtu " << m_netDevice->GetMtu () );
      // fragment
      std::list<Ptr<Packet> > fragmentList;
      DoFragmentation (packet, origPacketSize, origHdrSize, fragmentList);
      std::list<Ptr<Packet> >::iterator it;
      bool success = true;
      for ( it = fragmentList.begin (); it != fragmentList.end (); it++ )
        {
          NS_LOG_DEBUG ( "SixLowPanNetDevice::Send (Fragment) " << **it );
          m_txTrace (*it, m_node->GetObject<SixLowPanNetDevice> (), GetIfIndex ());
          if (doSendFrom)
            {
              success &= m_netDevice->SendFrom (*it, src, dest, protocolNumber);
            }
          else
            {
              success &= m_netDevice->Send (*it, dest, protocolNumber);
            }
        }
      ret = success;
    }
  else
    {
      if (packet->GetSize () < m_compressionThreshold)
        {
          NS_LOG_LOGIC ("Compressed packet too short, using uncompressed one");
          packet = origPacket;
          SixLowPanIpv6 ipv6UncompressedHdr;
          packet->AddHeader (ipv6UncompressedHdr);
        }

      m_txTrace (packet, m_node->GetObject<SixLowPanNetDevice> (), GetIfIndex ());
      if (doSendFrom)
        {
          NS_LOG_DEBUG ( "SixLowPanNetDevice::SendFrom " << m_node->GetId () << " " << *packet );
          ret = m_netDevice->SendFrom (packet, src, dest, protocolNumber);
        }
      else
        {
          NS_LOG_DEBUG ( "SixLowPanNetDevice::Send " << m_node->GetId () << " " << *packet );
          ret = m_netDevice->Send (packet, dest, protocolNumber);
        }
    }

  return ret;
}

Ptr<Node> SixLowPanNetDevice::GetNode (void) const
{
  NS_LOG_FUNCTION (this);
  return m_node;
}

void SixLowPanNetDevice::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this << node);
  m_node = node;
}

bool SixLowPanNetDevice::NeedsArp (void) const
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG ( m_netDevice != 0, "Sixlowpan: can't find any lower-layer protocol " << m_netDevice );

  return m_netDevice->NeedsArp ();
}

void SixLowPanNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  NS_LOG_FUNCTION (this << &cb);
  m_rxCallback = cb;
}

void SixLowPanNetDevice::SetPromiscReceiveCallback (NetDevice::PromiscReceiveCallback cb)
{
  NS_LOG_FUNCTION (this << &cb);
  m_promiscRxCallback = cb;
}

bool SixLowPanNetDevice::SupportsSendFrom () const
{
  NS_LOG_FUNCTION (this);
  return true;
}

uint32_t
SixLowPanNetDevice::CompressLowPanHc1 (Ptr<Packet> packet, Address const &src, Address const &dst)
{
  NS_LOG_FUNCTION (this << *packet << src << dst);

  Ipv6Header ipHeader;
  SixLowPanHc1 hc1Header;
  uint32_t size = 0;

  if ( packet->PeekHeader (ipHeader) != 0 )
    {
      packet->RemoveHeader (ipHeader);
      size += ipHeader.GetSerializedSize ();

      hc1Header.SetHopLimit (ipHeader.GetHopLimit ());

      uint8_t bufOne[16];
      uint8_t bufTwo[16];
      Ipv6Address srcAddr = ipHeader.GetSourceAddress ();
      srcAddr.GetBytes (bufOne);
      Ipv6Address mySrcAddr = MakeLinkLocalAddressFromMac (src);

      NS_LOG_LOGIC ("Checking source compression: " << mySrcAddr << " - " << srcAddr );

      mySrcAddr.GetBytes (bufTwo);
      bool isSrcSrc = (memcmp (bufOne + 8, bufTwo + 8, 8) == 0);

      if (srcAddr.IsLinkLocal () && isSrcSrc )
        {
          hc1Header.SetSrcCompression (SixLowPanHc1::HC1_PCIC);
        }
      else if (srcAddr.IsLinkLocal () )
        {
          hc1Header.SetSrcCompression (SixLowPanHc1::HC1_PCII);
          hc1Header.SetSrcInterface (bufOne + 8);
        }
      else if ( isSrcSrc )
        {
          hc1Header.SetSrcCompression (SixLowPanHc1::HC1_PIIC);
          hc1Header.SetSrcPrefix (bufOne);
        }
      else
        {
          hc1Header.SetSrcCompression (SixLowPanHc1::HC1_PIII);
          hc1Header.SetSrcInterface (bufOne + 8);
          hc1Header.SetSrcPrefix (bufOne);
        }

      Ipv6Address dstAddr = ipHeader.GetDestinationAddress ();
      dstAddr.GetBytes (bufOne);
      Ipv6Address myDstAddr = MakeLinkLocalAddressFromMac (dst);

      NS_LOG_LOGIC ("Checking destination compression: " << myDstAddr << " - " << dstAddr );

      myDstAddr.GetBytes (bufTwo);
      bool isDstDst = (memcmp (bufOne + 8, bufTwo + 8, 8) == 0);

      if (dstAddr.IsLinkLocal () && isDstDst )
        {
          hc1Header.SetDstCompression (SixLowPanHc1::HC1_PCIC);
        }
      else if (dstAddr.IsLinkLocal () )
        {
          hc1Header.SetDstCompression (SixLowPanHc1::HC1_PCII);
          hc1Header.SetDstInterface (bufOne + 8);
        }
      else if ( isDstDst )
        {
          hc1Header.SetDstCompression (SixLowPanHc1::HC1_PIIC);
          hc1Header.SetDstPrefix (bufOne);
        }
      else
        {
          hc1Header.SetDstCompression (SixLowPanHc1::HC1_PIII);
          hc1Header.SetDstInterface (bufOne + 8);
          hc1Header.SetDstPrefix (bufOne);
        }

      if ( (ipHeader.GetFlowLabel () == 0) && (ipHeader.GetTrafficClass () == 0) )
        {
          hc1Header.SetTcflCompression (true);
        }
      else
        {
          hc1Header.SetTcflCompression (false);
          hc1Header.SetTrafficClass (ipHeader.GetTrafficClass ());
          hc1Header.SetFlowLabel (ipHeader.GetFlowLabel ());
        }

      uint8_t nextHeader = ipHeader.GetNextHeader ();
      hc1Header.SetNextHeader (nextHeader);

      // \todo implement HC2 compression
      hc1Header.SetHc2HeaderPresent (false);

      NS_LOG_DEBUG ("HC1 Compression - HC1 header size = " << hc1Header.GetSerializedSize () );
      NS_LOG_DEBUG ("HC1 Compression - packet size = " << packet->GetSize () );

      packet->AddHeader (hc1Header);

      return size;
    }

  return 0;
}

void
SixLowPanNetDevice::DecompressLowPanHc1 (Ptr<Packet> packet, Address const &src, Address const &dst)
{
  NS_LOG_FUNCTION (this << *packet << src << dst);

  Ipv6Header ipHeader;
  SixLowPanHc1 encoding;

  uint32_t ret = packet->RemoveHeader (encoding);
  NS_LOG_DEBUG ("removed " << ret << " bytes - pkt is " << *packet);
  NS_UNUSED (ret);

  ipHeader.SetHopLimit (encoding.GetHopLimit ());

  switch (encoding.GetSrcCompression ())
    {
      const uint8_t* interface;
      const uint8_t* prefix;
      uint8_t address[16];

    case SixLowPanHc1::HC1_PIII:
      prefix = encoding.GetSrcPrefix ();
      interface = encoding.GetSrcInterface ();
      for (int j = 0; j < 8; j++)
        {
          address[j + 8] = interface[j];
          address[j] = prefix[j];
        }
      ipHeader.SetSourceAddress ( Ipv6Address (address) );
      break;
    case SixLowPanHc1::HC1_PIIC:
      prefix = encoding.GetSrcPrefix ();
      for (int j = 0; j < 8; j++)
        {
          address[j + 8] = 0;
          address[j] = prefix[j];
        }
      ipHeader.SetSourceAddress ( MakeGlobalAddressFromMac (src, Ipv6Address (address)));
      break;
    case SixLowPanHc1::HC1_PCII:
      interface = encoding.GetSrcInterface ();
      address[0] = 0xfe;
      address[1] = 0x80;
      for (int j = 0; j < 8; j++)
        {
          address[j + 8] = interface[j];
        }
      ipHeader.SetSourceAddress ( Ipv6Address (address) );
      break;
    case SixLowPanHc1::HC1_PCIC:
      ipHeader.SetSourceAddress (MakeLinkLocalAddressFromMac (src));
      break;
    }

  switch (encoding.GetDstCompression ())
    {
      const uint8_t* interface;
      const uint8_t* prefix;
      uint8_t address[16];

    case SixLowPanHc1::HC1_PIII:
      prefix = encoding.GetDstPrefix ();
      interface = encoding.GetDstInterface ();
      for (int j = 0; j < 8; j++)
        {
          address[j + 8] = interface[j];
          address[j] = prefix[j];
        }
      ipHeader.SetDestinationAddress ( Ipv6Address (address) );
      break;
    case SixLowPanHc1::HC1_PIIC:
      prefix = encoding.GetDstPrefix ();
      for (int j = 0; j < 8; j++)
        {
          address[j + 8] = 0;
          address[j] = prefix[j];
        }
      ipHeader.SetDestinationAddress ( MakeGlobalAddressFromMac (dst, Ipv6Address (address)));
      break;
    case SixLowPanHc1::HC1_PCII:
      interface = encoding.GetDstInterface ();
      address[0] = 0xfe;
      address[1] = 0x80;
      for (int j = 0; j < 8; j++)
        {
          address[j + 8] = interface[j];
        }
      ipHeader.SetDestinationAddress ( Ipv6Address (address) );
      break;
    case SixLowPanHc1::HC1_PCIC:
      ipHeader.SetDestinationAddress (MakeLinkLocalAddressFromMac (dst));
      break;
    }

  if ( !encoding.IsTcflCompression () )
    {
      ipHeader.SetFlowLabel (encoding.GetFlowLabel ());
      ipHeader.SetTrafficClass (encoding.GetTrafficClass ());
    }
  else
    {
      ipHeader.SetFlowLabel (0);
      ipHeader.SetTrafficClass (0);
    }

  ipHeader.SetNextHeader (encoding.GetNextHeader ());

  ipHeader.SetPayloadLength (packet->GetSize ());

  NS_ASSERT_MSG (encoding.IsHc2HeaderPresent () == false,
                 "6LoWPAN: error in decompressing HC1 encoding, unsupported L4 compressed header present.");

  packet->AddHeader (ipHeader);

  NS_LOG_DEBUG ( "Rebuilt packet: " << *packet << " Size " << packet->GetSize () );
}

uint32_t
SixLowPanNetDevice::CompressLowPanIphc (Ptr<Packet> packet, Address const &src, Address const &dst)
{
  NS_LOG_FUNCTION (this << *packet << src << dst);

  Ipv6Header ipHeader;
  SixLowPanIphc iphcHeader;
  uint32_t size = 0;


  if ( packet->PeekHeader (ipHeader) != 0 )
    {
      packet->RemoveHeader (ipHeader);
      size += ipHeader.GetSerializedSize ();

      // Set the TF field
      if ( (ipHeader.GetFlowLabel () == 0) && (ipHeader.GetTrafficClass () == 0) )
        {
          iphcHeader.SetTf (SixLowPanIphc::TF_ELIDED);
        }
      else if ( (ipHeader.GetFlowLabel () != 0) && (ipHeader.GetTrafficClass () != 0) )
        {
          iphcHeader.SetTf (SixLowPanIphc::TF_FULL);
          iphcHeader.SetEcn ( (ipHeader.GetTrafficClass () & 0xC0) >> 6);
          iphcHeader.SetDscp ( ipHeader.GetTrafficClass () & 0x3F );
          iphcHeader.SetFlowLabel (ipHeader.GetFlowLabel ());
        }
      else if ( (ipHeader.GetFlowLabel () == 0) && (ipHeader.GetTrafficClass () != 0) )
        {
          iphcHeader.SetTf (SixLowPanIphc::TF_FL_ELIDED);
          iphcHeader.SetEcn ( (ipHeader.GetTrafficClass () & 0xC0) >> 6);
          iphcHeader.SetDscp ( ipHeader.GetTrafficClass () & 0x3F );
        }
      else
        {
          iphcHeader.SetTf (SixLowPanIphc::TF_DSCP_ELIDED);
          iphcHeader.SetEcn ( (ipHeader.GetTrafficClass () & 0xC0) >> 6);
          iphcHeader.SetFlowLabel (ipHeader.GetFlowLabel ());
        }

      // Set the NH field and NextHeader

      uint8_t nextHeader = ipHeader.GetNextHeader ();
      if (CanCompressLowPanNhc (nextHeader))
        {
          if (nextHeader == Ipv6Header::IPV6_UDP)
            {
              iphcHeader.SetNh (true);
              size += CompressLowPanUdpNhc (packet, m_omitUdpChecksum);
            }
          else if (nextHeader == Ipv6Header::IPV6_IPV6)
            {
              iphcHeader.SetNh (true);
              size += CompressLowPanIphc (packet, src, dst);
            }
          else
            {
              uint32_t sizeNhc = CompressLowPanNhc (packet, nextHeader, src, dst);
              // the compression might fail due to Extension header size.
              if (sizeNhc)
                {
                  iphcHeader.SetNh (true);
                  size += sizeNhc;
                }
              else
                {
                  iphcHeader.SetNh (false);
                  iphcHeader.SetNextHeader (nextHeader);
                }
            }
        }
      else
        {
          iphcHeader.SetNh (false);
          iphcHeader.SetNextHeader (nextHeader);
        }


      // Set the HLIM field
      if (ipHeader.GetHopLimit () == 1)
        {
          iphcHeader.SetHlim (SixLowPanIphc::HLIM_COMPR_1);
        }
      else if (ipHeader.GetHopLimit () == 0x40)
        {
          iphcHeader.SetHlim (SixLowPanIphc::HLIM_COMPR_64);
        }
      else if (ipHeader.GetHopLimit () == 0xFF)
        {
          iphcHeader.SetHlim (SixLowPanIphc::HLIM_COMPR_255);
        }
      else
        {
          iphcHeader.SetHlim (SixLowPanIphc::HLIM_INLINE);
          // Set the HopLimit
          iphcHeader.SetHopLimit (ipHeader.GetHopLimit ());
        }

      // \todo Add the check of CID if there is context-based compression
      // Set the CID field
      iphcHeader.SetCid (false);

      // \todo Add the check of SAC if there is context-based compression
      // Set the SAC field
      iphcHeader.SetSac (false);

      uint8_t addressBuf[16];
      uint8_t unicastAddrCheckerBuf[16];
      Ipv6Address srcAddr = ipHeader.GetSourceAddress ();
      srcAddr.GetBytes (addressBuf);

      Ipv6Address checker = Ipv6Address ("fe80:0000:0000:0000:0000:00ff:fe00:1");
      checker.GetBytes (unicastAddrCheckerBuf);

      // \todo Add the check of SAC if there is context-based compression
      // Set the Source Address
      iphcHeader.SetSrcAddress (srcAddr);

      Ipv6Address mySrcAddr = MakeLinkLocalAddressFromMac (src);
      NS_LOG_LOGIC ("Checking source compression: " << mySrcAddr << " - " << srcAddr );

      if ( mySrcAddr == srcAddr )
        {
          iphcHeader.SetSam (SixLowPanIphc::HC_COMPR_0);
        }
      else if (memcmp (addressBuf, unicastAddrCheckerBuf, 14) == 0)
        {
          iphcHeader.SetSam (SixLowPanIphc::HC_COMPR_16);
        }
      else if ( srcAddr.IsLinkLocal () )
        {
          iphcHeader.SetSam (SixLowPanIphc::HC_COMPR_64);
        }
      else
        {
          iphcHeader.SetSam (SixLowPanIphc::HC_INLINE);
        }

      // Set the M field
      if (ipHeader.GetDestinationAddress ().IsMulticast ())
        {
          iphcHeader.SetM (true);
        }
      else
        {
          iphcHeader.SetM (false);
        }

      // \todo Add the check of DAC if there is context-based compression
      // Set the DAC field
      iphcHeader.SetDac (false);

      Ipv6Address dstAddr = ipHeader.GetDestinationAddress ();
      dstAddr.GetBytes (addressBuf);

      // \todo Add the check of DAC if there is context-based compression
      // Set the Destination Address
      iphcHeader.SetDstAddress (dstAddr);

      Ipv6Address myDstAddr = MakeLinkLocalAddressFromMac (dst);
      NS_LOG_LOGIC ("Checking destination compression: " << myDstAddr << " - " << dstAddr );

      if ( !iphcHeader.GetM () )
      // Unicast address
        {
          if ( myDstAddr == dstAddr )
            {
              iphcHeader.SetDam (SixLowPanIphc::HC_COMPR_0);
            }
          else if (memcmp (addressBuf, unicastAddrCheckerBuf, 14) == 0)
            {
              iphcHeader.SetDam (SixLowPanIphc::HC_COMPR_16);
            }
          else if ( dstAddr.IsLinkLocal () )
            {
              iphcHeader.SetDam (SixLowPanIphc::HC_COMPR_64);
            }
          else
            {
              iphcHeader.SetDam (SixLowPanIphc::HC_INLINE);
            }
        }
      else
        {
          // Multicast address
          uint8_t multicastAddrCheckerBuf[16];
          Ipv6Address multicastCheckAddress = Ipv6Address ("ff02::1");
          multicastCheckAddress.GetBytes (multicastAddrCheckerBuf);

          // The address takes the form ff02::00XX.
          if ( memcmp (addressBuf, multicastAddrCheckerBuf, 15) == 0 )
            {
              iphcHeader.SetDam (SixLowPanIphc::HC_COMPR_0);
            }
          // The address takes the form ffXX::00XX:XXXX.
          //                            ffXX:0000:0000:0000:0000:0000:00XX:XXXX.
          else if ( (addressBuf[0] == multicastAddrCheckerBuf[0])
                    && (memcmp (addressBuf + 2, multicastAddrCheckerBuf + 2, 11) ) )
            {
              iphcHeader.SetDam (SixLowPanIphc::HC_COMPR_16);
            }
          // The address takes the form ffXX::00XX:XXXX:XXXX.
          //                            ffXX:0000:0000:0000:0000:00XX:XXXX:XXXX.
          else if ( (addressBuf[0] == multicastAddrCheckerBuf[0])
                    && (memcmp (addressBuf + 2, multicastAddrCheckerBuf + 2, 9) ) )
            {
              iphcHeader.SetDam (SixLowPanIphc::HC_COMPR_64);
            }
          else
            {
              iphcHeader.SetDam (SixLowPanIphc::HC_INLINE);
            }
        }

      NS_LOG_DEBUG ("IPHC Compression - IPHC header size = " << iphcHeader.GetSerializedSize () );
      NS_LOG_DEBUG ("IPHC Compression - packet size = " << packet->GetSize () );

      packet->AddHeader (iphcHeader);

      NS_LOG_DEBUG ("Packet after IPHC compression: " << *packet);

      return size;
    }

  return 0;
}

bool
SixLowPanNetDevice::CanCompressLowPanNhc (uint8_t nextHeader)
{
  bool ret = false;

  switch (nextHeader)
    {
    case Ipv6Header::IPV6_UDP:
    case Ipv6Header::IPV6_EXT_HOP_BY_HOP:
    case Ipv6Header::IPV6_EXT_ROUTING:
    case Ipv6Header::IPV6_EXT_FRAGMENTATION:
    case Ipv6Header::IPV6_IPV6:
      ret = true;
      break;
    case Ipv6Header::IPV6_EXT_MOBILITY:
    default:
      ret = false;
    }
  return ret;
}

void
SixLowPanNetDevice::DecompressLowPanIphc (Ptr<Packet> packet, Address const &src, Address const &dst)
{
  NS_LOG_FUNCTION (this << *packet << src << dst);

  Ipv6Header ipHeader;
  SixLowPanIphc encoding;

  uint32_t ret = packet->RemoveHeader (encoding);
  NS_LOG_DEBUG ("removed " << ret << " bytes - pkt is " << *packet);
  NS_UNUSED (ret);

  // Hop Limit
  ipHeader.SetHopLimit (encoding.GetHopLimit ());

  // Source address
  if ( encoding.GetSac () )
    {
      if ( encoding.GetSam () == SixLowPanIphc::HC_INLINE )
        {
          ipHeader.SetSourceAddress ( Ipv6Address::GetAny () );
        }
      else
        {
          NS_ABORT_MSG ("SAC option not yet implemented");
        }
    }
  else
    {
      if ( encoding.GetSam () == SixLowPanIphc::HC_COMPR_0 )
        {
          ipHeader.SetSourceAddress (MakeLinkLocalAddressFromMac (src));
        }
      else
        {
          ipHeader.SetSourceAddress ( encoding.GetSrcAddress () );
        }
    }
  // Destination address
  if ( encoding.GetDac () )
    {
      if ((encoding.GetDam () == SixLowPanIphc::HC_INLINE  && !encoding.GetM ())
          || (encoding.GetDam () == SixLowPanIphc::HC_COMPR_64  && encoding.GetM ())
          || (encoding.GetDam () == SixLowPanIphc::HC_COMPR_16  && encoding.GetM ())
          || (encoding.GetDam () == SixLowPanIphc::HC_COMPR_0  && encoding.GetM ()) )
        {
          NS_ABORT_MSG ("Reserved code found");
        }
      else
        {
          NS_ABORT_MSG ("DAC option not yet implemented");
        }
    }
  else
    {
      if ( !encoding.GetM () && encoding.GetDam () == SixLowPanIphc::HC_COMPR_0 )
        {
          ipHeader.SetDestinationAddress (MakeLinkLocalAddressFromMac (dst));
        }
      else
        {
          ipHeader.SetDestinationAddress ( encoding.GetDstAddress () );
        }
    }

  // Traffic class and Flow Label
  uint8_t traf = 0x00;
  switch (encoding.GetTf ())
    {
    case SixLowPanIphc::TF_FULL:
      traf |= encoding.GetEcn ();
      traf = ( traf << 6 ) | encoding.GetDscp ();
      ipHeader.SetTrafficClass (traf);
      ipHeader.SetFlowLabel ( encoding.GetFlowLabel () & 0xfff ); //Add 4-bit pad
      break;
    case SixLowPanIphc::TF_DSCP_ELIDED:
      traf |= encoding.GetEcn ();
      traf <<= 2;   // Add 2-bit pad
      ipHeader.SetTrafficClass (traf);
      ipHeader.SetFlowLabel (encoding.GetFlowLabel ());
      break;
    case SixLowPanIphc::TF_FL_ELIDED:
      traf |= encoding.GetEcn ();
      traf = ( traf << 6 ) | encoding.GetDscp ();
      ipHeader.SetTrafficClass (traf);
      ipHeader.SetFlowLabel (0);
      break;
    case SixLowPanIphc::TF_ELIDED:
      ipHeader.SetFlowLabel (0);
      ipHeader.SetTrafficClass (0);
      break;
    }

  if ( encoding.GetNh () )
    {
      // Next Header
      uint8_t dispatchRawVal = 0;
      SixLowPanDispatch::NhcDispatch_e dispatchVal;

      packet->CopyData (&dispatchRawVal, sizeof(dispatchRawVal));
      dispatchVal = SixLowPanDispatch::GetNhcDispatchType (dispatchRawVal);

      if (dispatchVal == SixLowPanDispatch::LOWPAN_UDPNHC)
        {
          ipHeader.SetNextHeader (Ipv6Header::IPV6_UDP);
          DecompressLowPanUdpNhc (packet, ipHeader.GetSourceAddress (), ipHeader.GetDestinationAddress ());
        }
      else
        {
          ipHeader.SetNextHeader (DecompressLowPanNhc (packet, src, dst, ipHeader.GetSourceAddress (), ipHeader.GetDestinationAddress ()));
        }
    }
  else
    {
      ipHeader.SetNextHeader (encoding.GetNextHeader ());
    }

  ipHeader.SetPayloadLength (packet->GetSize ());

  packet->AddHeader (ipHeader);

  NS_LOG_DEBUG ( "Rebuilt packet: " << *packet << " Size " << packet->GetSize () );

}

uint32_t
SixLowPanNetDevice::CompressLowPanNhc (Ptr<Packet> packet, uint8_t headerType, Address const &src, Address const &dst)
{
  NS_LOG_FUNCTION (this << *packet << int(headerType));

  SixLowPanNhcExtension nhcHeader;
  uint32_t size = 0;
  Buffer blob;

  if (headerType == Ipv6Header::IPV6_EXT_HOP_BY_HOP)
    {
      Ipv6ExtensionHopByHopHeader hopHeader;
      packet->PeekHeader (hopHeader);
      if (hopHeader.GetLength () >= 0xff)
        {
          NS_LOG_DEBUG ("LOWPAN_NHC MUST NOT be used to encode IPv6 Extension Headers "
                        "that have more than 255 octets following the Length field after compression. "
                        "Packet uncompressed.");
          return 0;
        }

      size += packet->RemoveHeader (hopHeader);
      nhcHeader.SetEid (SixLowPanNhcExtension::EID_HOPBYHOP_OPTIONS_H);

      // recursively compress other headers
      uint8_t nextHeader = hopHeader.GetNextHeader ();
      if (CanCompressLowPanNhc (nextHeader))
        {
          if (nextHeader == Ipv6Header::IPV6_UDP)
            {
              nhcHeader.SetNh (true);
              size += CompressLowPanUdpNhc (packet, m_omitUdpChecksum);
            }
          else if (nextHeader == Ipv6Header::IPV6_IPV6)
            {
              nhcHeader.SetNh (true);
              size += CompressLowPanIphc (packet, src, dst);
            }
          else
            {
              uint32_t sizeNhc = CompressLowPanNhc (packet, nextHeader, src, dst);
              // the compression might fail due to Extension header size.
              if (sizeNhc)
                {
                  nhcHeader.SetNh (true);
                  size += sizeNhc;
                }
              else
                {
                  nhcHeader.SetNh (false);
                  nhcHeader.SetNextHeader (nextHeader);
                }
            }
        }
      else
        {
          nhcHeader.SetNh (false);
          nhcHeader.SetNextHeader (nextHeader);
        }

      uint32_t blobSize = hopHeader.GetSerializedSize ();
      blob.AddAtStart (blobSize);
      hopHeader.Serialize (blob.Begin ());
      blob.RemoveAtStart (2);
      blobSize = blob.GetSize ();
      nhcHeader.SetBlob (blob.PeekData (), blobSize);
    }
  else if (headerType == Ipv6Header::IPV6_EXT_ROUTING)
    {
      Ipv6ExtensionRoutingHeader routingHeader;
      packet->PeekHeader (routingHeader);
      if (routingHeader.GetLength () >= 0xff)
        {
          NS_LOG_DEBUG ("LOWPAN_NHC MUST NOT be used to encode IPv6 Extension Headers "
                        "that have more than 255 octets following the Length field after compression. "
                        "Packet uncompressed.");
          return 0;
        }

      size += packet->RemoveHeader (routingHeader);
      nhcHeader.SetEid (SixLowPanNhcExtension::EID_ROUTING_H);

      // recursively compress other headers
      uint8_t nextHeader = routingHeader.GetNextHeader ();
      if (CanCompressLowPanNhc (nextHeader))
        {
          if (nextHeader == Ipv6Header::IPV6_UDP)
            {
              nhcHeader.SetNh (true);
              size += CompressLowPanUdpNhc (packet, m_omitUdpChecksum);
            }
          else if (nextHeader == Ipv6Header::IPV6_IPV6)
            {
              nhcHeader.SetNh (true);
              size += CompressLowPanIphc (packet, src, dst);
            }
          else
            {
              uint32_t sizeNhc = CompressLowPanNhc (packet, nextHeader, src, dst);
              // the compression might fail due to Extension header size.
              if (sizeNhc)
                {
                  nhcHeader.SetNh (true);
                  size += sizeNhc;
                }
              else
                {
                  nhcHeader.SetNh (false);
                  nhcHeader.SetNextHeader (nextHeader);
                }
            }
        }
      else
        {
          nhcHeader.SetNh (false);
          nhcHeader.SetNextHeader (nextHeader);
        }

      uint32_t blobSize = routingHeader.GetSerializedSize ();
      blob.AddAtStart (blobSize);
      routingHeader.Serialize (blob.Begin ());
      blob.RemoveAtStart (2);
      blobSize = blob.GetSize ();
      nhcHeader.SetBlob (blob.PeekData (), blobSize);
    }
  else if (headerType == Ipv6Header::IPV6_EXT_FRAGMENTATION)
    {
      Ipv6ExtensionFragmentHeader fragHeader;
      packet->PeekHeader (fragHeader);
      if (fragHeader.GetLength () >= 0xff)
        {
          NS_LOG_DEBUG ("LOWPAN_NHC MUST NOT be used to encode IPv6 Extension Headers "
                        "that have more than 255 octets following the Length field after compression. "
                        "Packet uncompressed.");
          return 0;
        }
      size += packet->RemoveHeader (fragHeader);
      nhcHeader.SetEid (SixLowPanNhcExtension::EID_FRAGMENTATION_H);

      // recursively compress other headers
      uint8_t nextHeader = fragHeader.GetNextHeader ();
      if (CanCompressLowPanNhc (nextHeader))
        {
          if (nextHeader == Ipv6Header::IPV6_UDP)
            {
              nhcHeader.SetNh (true);
              size += CompressLowPanUdpNhc (packet, m_omitUdpChecksum);
            }
          else if (nextHeader == Ipv6Header::IPV6_IPV6)
            {
              nhcHeader.SetNh (true);
              size += CompressLowPanIphc (packet, src, dst);
            }
          else
            {
              uint32_t sizeNhc = CompressLowPanNhc (packet, nextHeader, src, dst);
              // the compression might fail due to Extension header size.
              if (sizeNhc)
                {
                  nhcHeader.SetNh (true);
                  size += sizeNhc;
                }
              else
                {
                  nhcHeader.SetNh (false);
                  nhcHeader.SetNextHeader (nextHeader);
                }
            }
        }
      else
        {
          nhcHeader.SetNh (false);
          nhcHeader.SetNextHeader (nextHeader);
        }

      uint32_t blobSize = fragHeader.GetSerializedSize ();
      blob.AddAtStart (blobSize);
      fragHeader.Serialize (blob.Begin ());
      blob.RemoveAtStart (2);
      blobSize = blob.GetSize ();
      nhcHeader.SetBlob (blob.PeekData (), blobSize);
    }
  else if (headerType == Ipv6Header::IPV6_EXT_DESTINATION)
    {
      Ipv6ExtensionDestinationHeader destHeader;
      packet->PeekHeader (destHeader);
      if (destHeader.GetLength () >= 0xff)
        {
          NS_LOG_DEBUG ("LOWPAN_NHC MUST NOT be used to encode IPv6 Extension Headers "
                        "that have more than 255 octets following the Length field after compression. "
                        "Packet uncompressed.");
          return 0;
        }
      size += packet->RemoveHeader (destHeader);
      nhcHeader.SetEid (SixLowPanNhcExtension::EID_DESTINATION_OPTIONS_H);

      // recursively compress other headers
      uint8_t nextHeader = destHeader.GetNextHeader ();
      if (CanCompressLowPanNhc (nextHeader))
        {
          if (nextHeader == Ipv6Header::IPV6_UDP)
            {
              nhcHeader.SetNh (true);
              size += CompressLowPanUdpNhc (packet, m_omitUdpChecksum);
            }
          else if (nextHeader == Ipv6Header::IPV6_IPV6)
            {
              nhcHeader.SetNh (true);
              size += CompressLowPanIphc (packet, src, dst);
            }
          else
            {
              uint32_t sizeNhc = CompressLowPanNhc (packet, nextHeader, src, dst);
              // the compression might fail due to Extension header size.
              if (sizeNhc)
                {
                  nhcHeader.SetNh (true);
                  size += sizeNhc;
                }
              else
                {
                  nhcHeader.SetNh (false);
                  nhcHeader.SetNextHeader (nextHeader);
                }
            }
        }
      else
        {
          nhcHeader.SetNh (false);
          nhcHeader.SetNextHeader (nextHeader);
        }

      uint32_t blobSize = destHeader.GetSerializedSize ();
      blob.AddAtStart (blobSize);
      destHeader.Serialize (blob.Begin ());
      blob.RemoveAtStart (2);
      blobSize = blob.GetSize ();
      nhcHeader.SetBlob (blob.PeekData (), blobSize);
    }
  else if (headerType == Ipv6Header::IPV6_EXT_MOBILITY)
    {
      // \todo: IPv6 Mobility Header is not supported in ns-3
      NS_ABORT_MSG ("IPv6 Mobility Header is not supported in ns-3 yet");
      return 0;
    }
  else
    {
      NS_ABORT_MSG ("Unexpected Extension Header");
    }

  NS_LOG_DEBUG ("NHC Compression - NHC header size = " << nhcHeader.GetSerializedSize () );
  NS_LOG_DEBUG ("NHC Compression - packet size = " << packet->GetSize () );

  packet->AddHeader (nhcHeader);

  NS_LOG_DEBUG ("Packet after NHC compression: " << *packet);
  return size;
}

uint8_t
SixLowPanNetDevice::DecompressLowPanNhc (Ptr<Packet> packet, Address const &src, Address const &dst, Ipv6Address srcAddress, Ipv6Address dstAddress)
{
  NS_LOG_FUNCTION (this << *packet);

  SixLowPanNhcExtension encoding;

  uint32_t ret = packet->RemoveHeader (encoding);
  NS_LOG_DEBUG ("removed " << ret << " bytes - pkt is " << *packet);
  NS_UNUSED (ret);

  Ipv6ExtensionHopByHopHeader hopHeader;
  Ipv6ExtensionRoutingHeader routingHeader;
  Ipv6ExtensionFragmentHeader fragHeader;
  Ipv6ExtensionDestinationHeader destHeader;

  uint32_t blobSize;
  uint8_t blobData[260];
  blobSize = encoding.CopyBlob (blobData + 2, 260);
  uint8_t paddingSize = 0;

  uint8_t actualEncodedHeaderType = encoding.GetEid ();
  uint8_t actualHeaderType;
  Buffer blob;

  switch (actualEncodedHeaderType)
    {
    case SixLowPanNhcExtension::EID_HOPBYHOP_OPTIONS_H:
      actualHeaderType = Ipv6Header::IPV6_EXT_HOP_BY_HOP;
      if ( encoding.GetNh () )
        {
          // Next Header
          uint8_t dispatchRawVal = 0;
          SixLowPanDispatch::NhcDispatch_e dispatchVal;

          packet->CopyData (&dispatchRawVal, sizeof(dispatchRawVal));
          dispatchVal = SixLowPanDispatch::GetNhcDispatchType (dispatchRawVal);

          if (dispatchVal == SixLowPanDispatch::LOWPAN_UDPNHC)
            {
              blobData [0] = Ipv6Header::IPV6_UDP;
              DecompressLowPanUdpNhc (packet, srcAddress, dstAddress);
            }
          else
            {
              blobData [0] = DecompressLowPanNhc (packet, src, dst, srcAddress, dstAddress);
            }
        }
      else
        {
          blobData [0] = encoding.GetNextHeader ();
        }

      // manually add some padding if needed
      if ((blobSize + 2) % 8 > 0)
        {
          paddingSize = 8 - (blobSize + 2) % 8;
        }
      if (paddingSize == 1)
        {
          blobData[blobSize + 2] = 0;
        }
      else if (paddingSize > 1)
        {
          blobData[blobSize + 2] = 1;
          blobData[blobSize + 2 + 1] = paddingSize - 2;
          for (uint8_t i = 0; i < paddingSize - 2; i++)
            {
              blobData[blobSize + 2 + 2 + i] = 0;
            }
        }
      blobData [1] = ((blobSize + 2 + paddingSize) >> 3) - 1;
      blob.AddAtStart (blobSize + 2 + paddingSize);
      blob.Begin ().Write (blobData, blobSize + 2 + paddingSize);
      hopHeader.Deserialize (blob.Begin ());

      packet->AddHeader (hopHeader);
      break;

    case SixLowPanNhcExtension::EID_ROUTING_H:
      actualHeaderType = Ipv6Header::IPV6_EXT_ROUTING;
      if ( encoding.GetNh () )
        {
          // Next Header
          uint8_t dispatchRawVal = 0;
          SixLowPanDispatch::NhcDispatch_e dispatchVal;

          packet->CopyData (&dispatchRawVal, sizeof(dispatchRawVal));
          dispatchVal = SixLowPanDispatch::GetNhcDispatchType (dispatchRawVal);

          if (dispatchVal == SixLowPanDispatch::LOWPAN_UDPNHC)
            {
              blobData [0] = Ipv6Header::IPV6_UDP;
              DecompressLowPanUdpNhc (packet, srcAddress, dstAddress);
            }
          else
            {
              blobData [0] = DecompressLowPanNhc (packet, src, dst, srcAddress, dstAddress);
            }
        }
      else
        {
          blobData [0] = encoding.GetNextHeader ();
        }
      blobData [1] = ((blobSize + 2) >> 3) - 1;
      blob.AddAtStart (blobSize + 2);
      blob.Begin ().Write (blobData, blobSize + 2);
      routingHeader.Deserialize (blob.Begin ());
      packet->AddHeader (routingHeader);
      break;

    case SixLowPanNhcExtension::EID_FRAGMENTATION_H:
      actualHeaderType = Ipv6Header::IPV6_EXT_FRAGMENTATION;
      if ( encoding.GetNh () )
        {
          // Next Header
          uint8_t dispatchRawVal = 0;
          SixLowPanDispatch::NhcDispatch_e dispatchVal;

          packet->CopyData (&dispatchRawVal, sizeof(dispatchRawVal));
          dispatchVal = SixLowPanDispatch::GetNhcDispatchType (dispatchRawVal);

          if (dispatchVal == SixLowPanDispatch::LOWPAN_UDPNHC)
            {
              blobData [0] = Ipv6Header::IPV6_UDP;
              DecompressLowPanUdpNhc (packet, srcAddress, dstAddress);
            }
          else
            {
              blobData [0] = DecompressLowPanNhc (packet, src, dst, srcAddress, dstAddress);
            }
        }
      else
        {
          blobData [0] = encoding.GetNextHeader ();
        }
      blobData [1] = 0;
      fragHeader.Deserialize (blob.Begin ());
      packet->AddHeader (fragHeader);
      break;

    case SixLowPanNhcExtension::EID_DESTINATION_OPTIONS_H:
      actualHeaderType = Ipv6Header::IPV6_EXT_DESTINATION;
      if ( encoding.GetNh () )
        {
          // Next Header
          uint8_t dispatchRawVal = 0;
          SixLowPanDispatch::NhcDispatch_e dispatchVal;

          packet->CopyData (&dispatchRawVal, sizeof(dispatchRawVal));
          dispatchVal = SixLowPanDispatch::GetNhcDispatchType (dispatchRawVal);

          if (dispatchVal == SixLowPanDispatch::LOWPAN_UDPNHC)
            {
              blobData [0] = Ipv6Header::IPV6_UDP;
              DecompressLowPanUdpNhc (packet, srcAddress, dstAddress);
            }
          else
            {
              blobData [0] = DecompressLowPanNhc (packet, src, dst, srcAddress, dstAddress);
            }
        }
      else
        {
          blobData [0] = encoding.GetNextHeader ();
        }

      // manually add some padding if needed
      if ((blobSize + 2) % 8 > 0)
        {
          paddingSize = 8 - (blobSize + 2) % 8;
        }
      if (paddingSize == 1)
        {
          blobData[blobSize + 2] = 0;
        }
      else if (paddingSize > 1)
        {
          blobData[blobSize + 2] = 1;
          blobData[blobSize + 2 + 1] = paddingSize - 2;
          for (uint8_t i = 0; i < paddingSize - 2; i++)
            {
              blobData[blobSize + 2 + 2 + i] = 0;
            }
        }
      blobData [1] = ((blobSize + 2 + paddingSize) >> 3) - 1;
      blob.AddAtStart (blobSize + 2 + paddingSize);
      blob.Begin ().Write (blobData, blobSize + 2 + paddingSize);
      destHeader.Deserialize (blob.Begin ());

      packet->AddHeader (destHeader);
      break;
    case SixLowPanNhcExtension::EID_MOBILITY_H:
      // \todo: IPv6 Mobility Header is not supported in ns-3
      NS_ABORT_MSG ("IPv6 Mobility Header is not supported in ns-3 yet");
      break;
    case SixLowPanNhcExtension::EID_IPv6_H:
      actualHeaderType = Ipv6Header::IPV6_IPV6;
      DecompressLowPanIphc (packet, src, dst);
      break;
    default:
      NS_ABORT_MSG ("Trying to decode unknown Extension Header");
      break;
    }

  NS_LOG_DEBUG ( "Rebuilt packet: " << *packet << " Size " << packet->GetSize () );
  return actualHeaderType;
}

uint32_t
SixLowPanNetDevice::CompressLowPanUdpNhc (Ptr<Packet> packet, bool omitChecksum)
{
  NS_LOG_FUNCTION (this << *packet << int(omitChecksum));

  UdpHeader udpHeader;
  SixLowPanUdpNhcExtension udpNhcHeader;
  uint32_t size = 0;

  NS_ASSERT_MSG (packet->PeekHeader (udpHeader) != 0, "UDP header not found, abort");

  size += packet->RemoveHeader (udpHeader);

  // Set the C field and checksum
  udpNhcHeader.SetC (false);
  uint16_t checksum = udpHeader.GetChecksum ();
  udpNhcHeader.SetChecksum (checksum);

  if (omitChecksum && udpHeader.IsChecksumOk ())
    {
      udpNhcHeader.SetC (true);
    }

  // Set the value of the ports
  udpNhcHeader.SetSrcPort (udpHeader.GetSourcePort ());
  udpNhcHeader.SetDstPort (udpHeader.GetDestinationPort ());

  //Set the P field
  if ( (udpHeader.GetSourcePort () >> 4 ) == 0xf0b && (udpHeader.GetDestinationPort () >> 4 ) == 0xf0b )
    {
      udpNhcHeader.SetPorts (SixLowPanUdpNhcExtension::PORTS_LAST_SRC_LAST_DST);
    }
  else if ( (udpHeader.GetSourcePort () >> 8 ) == 0xf0 && (udpHeader.GetDestinationPort () >> 8 ) != 0xf0 )
    {
      udpNhcHeader.SetPorts (SixLowPanUdpNhcExtension::PORTS_LAST_SRC_ALL_DST);
    }
  else if ( (udpHeader.GetSourcePort () >> 8 ) != 0xf0 && (udpHeader.GetDestinationPort () >> 8 ) == 0xf0 )
    {
      udpNhcHeader.SetPorts (SixLowPanUdpNhcExtension::PORTS_ALL_SRC_LAST_DST);
    }
  else
    {
      udpNhcHeader.SetPorts (SixLowPanUdpNhcExtension::PORTS_INLINE);
    }

  NS_LOG_DEBUG ("UDP_NHC Compression - UDP_NHC header size = " << udpNhcHeader.GetSerializedSize () );
  NS_LOG_DEBUG ("UDP_NHC Compression - packet size = " << packet->GetSize () );

  packet->AddHeader (udpNhcHeader);

  NS_LOG_DEBUG ("Packet after UDP_NHC compression: " << *packet);

  return size;
}

void
SixLowPanNetDevice::DecompressLowPanUdpNhc (Ptr<Packet> packet, Ipv6Address saddr, Ipv6Address daddr)
{
  NS_LOG_FUNCTION (this << *packet);

  UdpHeader udpHeader;
  SixLowPanUdpNhcExtension encoding;

  uint32_t ret = packet->RemoveHeader (encoding);
  NS_LOG_DEBUG ("removed " << ret << " bytes - pkt is " << *packet);
  NS_UNUSED (ret);

  // Set the value of the ports
  switch ( encoding.GetPorts () )
    {
      uint16_t temp;
    case SixLowPanUdpNhcExtension::PORTS_INLINE:
      udpHeader.SetSourcePort (encoding.GetSrcPort ());
      udpHeader.SetDestinationPort (encoding.GetDstPort ());
      break;
    case SixLowPanUdpNhcExtension::PORTS_ALL_SRC_LAST_DST:
      udpHeader.SetSourcePort (encoding.GetSrcPort ());
      temp = 0xf0;
      temp |= (temp << 8) | encoding.GetDstPort ();
      udpHeader.SetDestinationPort (temp);
      break;
    case SixLowPanUdpNhcExtension::PORTS_LAST_SRC_ALL_DST:
      temp = 0xf0;
      temp |= (temp << 8) | encoding.GetSrcPort ();
      udpHeader.SetSourcePort (temp);
      udpHeader.SetDestinationPort (encoding.GetDstPort ());
      break;
    case SixLowPanUdpNhcExtension::PORTS_LAST_SRC_LAST_DST:
      temp = 0xf0b;
      temp |= (temp << 4) | encoding.GetSrcPort ();
      udpHeader.SetSourcePort (temp);
      temp = 0xf0b;
      temp |= (temp << 4) | encoding.GetDstPort ();
      udpHeader.SetDestinationPort (temp);
      break;
    }

  // Get the C field and checksum
  if (Node::ChecksumEnabled ())
    {
      if ( encoding.GetC () )
        {
          NS_LOG_LOGIC ("Recalculating UDP Checksum");
          udpHeader.EnableChecksums ();
          udpHeader.InitializeChecksum (saddr,
                                        daddr,
                                        UdpL4Protocol::PROT_NUMBER);
          packet->AddHeader (udpHeader);
        }
      else
        {
          NS_LOG_LOGIC ("Forcing UDP Checksum to " << encoding.GetChecksum ());
          udpHeader.ForceChecksum (encoding.GetChecksum ());
          packet->AddHeader (udpHeader);
          NS_LOG_LOGIC ("UDP checksum is ok ? " << udpHeader.IsChecksumOk ());
        }
    }
  else
    {
      packet->AddHeader (udpHeader);
    }

  NS_LOG_DEBUG ( "Rebuilt packet: " << *packet << " Size " << packet->GetSize () );
}

void SixLowPanNetDevice::DoFragmentation (Ptr<Packet> packet,
                                          uint32_t origPacketSize,
                                          uint32_t origHdrSize,
                                          std::list<Ptr<Packet> >& listFragments)
{
  NS_LOG_FUNCTION (this << *packet);

  Ptr<Packet> p = packet->Copy ();

  uint16_t offsetData = 0;
  uint16_t offset = 0;
  uint16_t l2Mtu = m_netDevice->GetMtu ();
  uint32_t packetSize = packet->GetSize ();
  uint32_t compressedHeaderSize = packetSize - (origPacketSize - origHdrSize);

  uint16_t tag = uint16_t (m_rng->GetValue (0, 65535));
  NS_LOG_LOGIC ("random tag " << tag << " - test " << packetSize );

  // first fragment
  SixLowPanFrag1 frag1Hdr;
  frag1Hdr.SetDatagramTag (tag);

  uint32_t size;
  NS_ASSERT_MSG ( l2Mtu > frag1Hdr.GetSerializedSize (),
                  "6LoWPAN: can not fragment, 6LoWPAN headers are bigger than MTU");

  size = l2Mtu - frag1Hdr.GetSerializedSize () - compressedHeaderSize;
  size -= size % 8;
  size += compressedHeaderSize;

  frag1Hdr.SetDatagramSize (origPacketSize);

  Ptr<Packet> fragment1 = p->CreateFragment (offsetData, size);
  offset += size + origHdrSize - compressedHeaderSize;
  offsetData += size;

  fragment1->AddHeader (frag1Hdr);
  listFragments.push_back (fragment1);

  bool moreFrag = true;
  do
    {
      SixLowPanFragN fragNHdr;
      fragNHdr.SetDatagramTag (tag);
      fragNHdr.SetDatagramSize (origPacketSize);
      fragNHdr.SetDatagramOffset ((offset) >> 3);

      size = l2Mtu - fragNHdr.GetSerializedSize ();
      size -= size % 8;

      if ( (offsetData + size) > packetSize )
        {
          size = packetSize - offsetData;
          moreFrag = false;
        }

      NS_LOG_LOGIC ("Fragment creation - " << offset << ", " << offset );
      Ptr<Packet> fragment = p->CreateFragment (offsetData, size);
      NS_LOG_LOGIC ("Fragment created - " << offset << ", " << fragment->GetSize () );

      offset += size;
      offsetData += size;

      fragment->AddHeader (fragNHdr);
      listFragments.push_back (fragment);

    }
  while (moreFrag);

  return;
}

bool SixLowPanNetDevice::ProcessFragment (Ptr<Packet>& packet, Address const &src, Address const &dst, bool isFirst)
{
  NS_LOG_FUNCTION ( this << *packet );
  SixLowPanFrag1 frag1Header;
  SixLowPanFragN fragNHeader;
  FragmentKey key;
  uint16_t packetSize;
  key.first = std::pair<Address, Address> (src, dst);

  Ptr<Packet> p = packet->Copy ();
  uint16_t offset = 0;

  /* Implementation note:
   *
   * The fragment offset is relative to the *uncompressed* packet.
   * On the other hand, the packet can not be uncompressed correctly without all
   * its fragments, as the UDP checksum can not be computed otherwise.
   *
   * As a consequence we must uncompress the packet twice, and save its first
   * fragment for the final one.
   */

  if ( isFirst )
    {
      uint8_t dispatchRawValFrag1 = 0;
      SixLowPanDispatch::Dispatch_e dispatchValFrag1;

      p->RemoveHeader (frag1Header);
      packetSize = frag1Header.GetDatagramSize ();
      p->CopyData (&dispatchRawValFrag1, sizeof(dispatchRawValFrag1));
      dispatchValFrag1 = SixLowPanDispatch::GetDispatchType (dispatchRawValFrag1);
      NS_LOG_DEBUG ( "Dispatches: " << int(dispatchRawValFrag1) << " - " << int(dispatchValFrag1) );
      NS_LOG_DEBUG ( "Packet: " << *p );

      switch ( dispatchValFrag1 )
        {
        case SixLowPanDispatch::LOWPAN_IPv6:
          {
            SixLowPanIpv6 uncompressedHdr;
            p->RemoveHeader(uncompressedHdr);
          }
         break;
        case SixLowPanDispatch::LOWPAN_HC1:
          DecompressLowPanHc1 (p, src, dst);
          break;
        case SixLowPanDispatch::LOWPAN_IPHC:
          DecompressLowPanIphc (p, src, dst);
          break;
        default:
          NS_FATAL_ERROR ("Unsupported 6LoWPAN encoding, exiting.");
          break;
        }

      key.second = std::pair<uint16_t, uint16_t> (frag1Header.GetDatagramSize (), frag1Header.GetDatagramTag ());
    }
  else
    {
      p->RemoveHeader (fragNHeader);
      packetSize = fragNHeader.GetDatagramSize ();
      offset = fragNHeader.GetDatagramOffset () << 3;
      key.second = std::pair<uint16_t, uint16_t> (fragNHeader.GetDatagramSize (), fragNHeader.GetDatagramTag ());
    }

  Ptr<Fragments> fragments;

  MapFragments_t::iterator it = m_fragments.find (key);
  if (it == m_fragments.end ())
    {
      // erase the oldest packet.
      if ( m_fragmentReassemblyListSize && (m_fragments.size () >= m_fragmentReassemblyListSize) )
        {
          MapFragmentsTimers_t::iterator iter;
          MapFragmentsTimers_t::iterator iterFound = m_fragmentsTimers.begin ();
          for ( iter = m_fragmentsTimers.begin (); iter != m_fragmentsTimers.end (); iter++)
            {
              if ( iter->second.GetTs () < iterFound->second.GetTs () )
                {
                  iterFound = iter;
                }
            }
          FragmentKey oldestKey = iterFound->first;

          std::list< Ptr<Packet> > storedFragments = m_fragments[oldestKey]->GetFraments ();
          for (std::list< Ptr<Packet> >::iterator fragIter = storedFragments.begin ();
               fragIter != storedFragments.end (); fragIter++)
            {
              m_dropTrace (DROP_FRAGMENT_BUFFER_FULL, *fragIter, m_node->GetObject<SixLowPanNetDevice> (), GetIfIndex ());
            }

          m_fragmentsTimers[oldestKey].Cancel ();
          m_fragmentsTimers.erase (oldestKey);
          m_fragments[oldestKey] = 0;
          m_fragments.erase (oldestKey);

        }
      fragments = Create<Fragments> ();
      fragments->SetPacketSize (packetSize);
      m_fragments.insert (std::make_pair (key, fragments));
      uint32_t ifIndex = GetIfIndex ();
      m_fragmentsTimers[key] = Simulator::Schedule (m_fragmentExpirationTimeout,
                                                    &SixLowPanNetDevice::HandleFragmentsTimeout, this,
                                                    key, ifIndex);
    }
  else
    {
      fragments = it->second;
    }

  fragments->AddFragment (p, offset);

  // add the very first fragment so we can correctly decode the packet once is rebuilt.
  // this is needed because otherwise the UDP header length and checksum can not be calculated.
  if ( isFirst )
    {
      fragments->AddFirstFragment (packet);
    }

  if ( fragments->IsEntire () )
    {
      packet = fragments->GetPacket ();
      NS_LOG_LOGIC ("Reconstructed packet: " << *packet);

      SixLowPanFrag1 frag1Header;
      packet->RemoveHeader (frag1Header);

      NS_LOG_LOGIC ("Rebuilt packet. Size " << packet->GetSize () << " - " << *packet);
      fragments = 0;
      m_fragments.erase (key);
      if (m_fragmentsTimers[key].IsRunning ())
        {
          NS_LOG_LOGIC ("Stopping 6LoWPAN WaitFragmentsTimer at " << Simulator::Now ().GetSeconds () << " due to complete packet");
          m_fragmentsTimers[key].Cancel ();
        }
      m_fragmentsTimers.erase (key);
      return true;
    }

  return false;
}

SixLowPanNetDevice::Fragments::Fragments ()
{
  NS_LOG_FUNCTION (this);
  m_packetSize = 0;
}

SixLowPanNetDevice::Fragments::~Fragments ()
{
  NS_LOG_FUNCTION (this);
}

void SixLowPanNetDevice::Fragments::AddFragment (Ptr<Packet> fragment, uint16_t fragmentOffset)
{
  NS_LOG_FUNCTION (this << fragmentOffset << *fragment);

  std::list<std::pair<Ptr<Packet>, uint16_t> >::iterator it;
  bool duplicate = false;

  for (it = m_fragments.begin (); it != m_fragments.end (); it++)
    {
      if (it->second > fragmentOffset)
        {
          break;
        }
      if (it->second == fragmentOffset)
        {
          duplicate = true;
          NS_ASSERT_MSG (fragment->GetSize () == it->first->GetSize (), "Duplicate fragment size differs. Aborting.");
          break;
        }
    }
  if (!duplicate)
    {
      m_fragments.insert (it, std::make_pair (fragment, fragmentOffset));
    }
}

void SixLowPanNetDevice::Fragments::AddFirstFragment (Ptr<Packet> fragment)
{
  NS_LOG_FUNCTION (this << *fragment);

  m_firstFragment = fragment;
}

bool SixLowPanNetDevice::Fragments::IsEntire () const
{
  NS_LOG_FUNCTION (this);

  bool ret = m_fragments.size () > 0;
  uint16_t lastEndOffset = 0;

  if (ret)
    {
      for (std::list<std::pair<Ptr<Packet>, uint16_t> >::const_iterator it = m_fragments.begin (); it != m_fragments.end (); it++)
        {
          // overlapping fragments should not exist
          NS_LOG_LOGIC ("Checking overlaps " << lastEndOffset << " - " << it->second );

          if (lastEndOffset < it->second)
            {
              ret = false;
              break;
            }
          // fragments might overlap in strange ways
          uint16_t fragmentEnd = it->first->GetSize () + it->second;
          lastEndOffset = std::max ( lastEndOffset, fragmentEnd );
        }
    }

  if ( ret && (lastEndOffset == m_packetSize))
    {
      return true;
    }
  return false;
}

Ptr<Packet> SixLowPanNetDevice::Fragments::GetPacket () const
{
  NS_LOG_FUNCTION (this);

  std::list<std::pair<Ptr<Packet>, uint16_t> >::const_iterator it = m_fragments.begin ();

  Ptr<Packet> p = Create<Packet> ();
  uint16_t lastEndOffset = 0;

  p->AddAtEnd (m_firstFragment);
  it = m_fragments.begin ();
  lastEndOffset = it->first->GetSize ();

  for ( it++; it != m_fragments.end (); it++)
    {
      if ( lastEndOffset > it->second )
        {
          NS_ABORT_MSG ("Overlapping fragments found, forbidden condition");
        }
      else
        {
          NS_LOG_LOGIC ("Adding: " << *(it->first) );
          p->AddAtEnd (it->first);
        }
      lastEndOffset += it->first->GetSize ();
    }

  return p;
}

void SixLowPanNetDevice::Fragments::SetPacketSize (uint32_t packetSize)
{
  NS_LOG_FUNCTION (this << packetSize);
  m_packetSize = packetSize;
}

std::list< Ptr<Packet> > SixLowPanNetDevice::Fragments::GetFraments () const
{
  std::list< Ptr<Packet> > fragments;
  std::list<std::pair<Ptr<Packet>, uint16_t> >::const_iterator iter;
  for ( iter = m_fragments.begin (); iter != m_fragments.end (); iter ++)
    {
      fragments.push_back (iter->first);
    }
  return fragments;
}

void SixLowPanNetDevice::HandleFragmentsTimeout (FragmentKey key, uint32_t iif)
{
  NS_LOG_FUNCTION (this);

  MapFragments_t::iterator it = m_fragments.find (key);
  std::list< Ptr<Packet> > storedFragments = it->second->GetFraments ();
  for (std::list< Ptr<Packet> >::iterator fragIter = storedFragments.begin ();
       fragIter != storedFragments.end (); fragIter++)
    {
      m_dropTrace (DROP_FRAGMENT_TIMEOUT, *fragIter, m_node->GetObject<SixLowPanNetDevice> (), iif);
    }
  // clear the buffers
  it->second = 0;

  m_fragments.erase (key);
  m_fragmentsTimers.erase (key);
}

Ipv6Address SixLowPanNetDevice::MakeLinkLocalAddressFromMac (Address const &addr)
{
  Ipv6Address ipv6Addr = Ipv6Address::GetAny ();

  if (m_forceEtherType && Mac48Address::IsMatchingType (addr))
    {
      ipv6Addr = Ipv6Address::MakeAutoconfiguredLinkLocalAddress (Mac48Address::ConvertFrom (addr));
    }
  else
    {
      if (Mac64Address::IsMatchingType (addr))
        {
          ipv6Addr = Ipv6Address::MakeAutoconfiguredLinkLocalAddress (Mac64Address::ConvertFrom (addr));
        }
      else if (Mac16Address::IsMatchingType (addr))
        {
          ipv6Addr = Ipv6Address::MakeAutoconfiguredLinkLocalAddress (Mac16Address::ConvertFrom (addr));
        }
    }
  if (ipv6Addr.IsAny ())
    {
      NS_ABORT_MSG ("Unknown address type");
    }
  return ipv6Addr;
}

Ipv6Address SixLowPanNetDevice::MakeGlobalAddressFromMac (Address const &addr, Ipv6Address prefix)
{
  Ipv6Address ipv6Addr = Ipv6Address::GetAny ();

  if (m_forceEtherType && Mac48Address::IsMatchingType (addr))
    {
      ipv6Addr = Ipv6Address::MakeAutoconfiguredAddress (Mac48Address::ConvertFrom (addr), Ipv6Address (prefix) );
    }
  else
    {
      if (Mac64Address::IsMatchingType (addr))
        {
          ipv6Addr = Ipv6Address::MakeAutoconfiguredAddress (Mac64Address::ConvertFrom (addr), Ipv6Address (prefix));
        }
      else if (Mac16Address::IsMatchingType (addr))
        {
          ipv6Addr = Ipv6Address::MakeAutoconfiguredAddress (Mac16Address::ConvertFrom (addr), Ipv6Address (prefix) );
        }
    }
  if (ipv6Addr.IsAny ())
    {
      NS_ABORT_MSG ("Unknown address type");
    }
  return ipv6Addr;
}

}

// namespace ns3
