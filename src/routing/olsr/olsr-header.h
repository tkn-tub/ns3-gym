/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INESC Porto
 * All rights reserved.
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
 * Author: Gustavo J. A. M. Carneiro  <gjc@inescporto.pt>
 */

#ifndef OLSR_HEADER_H
#define OLSR_HEADER_H

#include <stdint.h>
#include <vector>
#include "ns3/header.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"


namespace ns3 {


double OlsrEmfToSeconds (uint8_t emf);
uint8_t OlsrSecondsToEmf (double seconds);

// 3.3.  Packet Format
//
//    The basic layout of any packet in OLSR is as follows (omitting IP and
//    UDP headers):
//
//        0                   1                   2                   3
//        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |         Packet Length         |    Packet Sequence Number     |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |  Message Type |     Vtime     |         Message Size          |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |                      Originator Address                       |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |  Time To Live |   Hop Count   |    Message Sequence Number    |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |                                                               |
//       :                            MESSAGE                            :
//       |                                                               |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |  Message Type |     Vtime     |         Message Size          |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |                      Originator Address                       |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |  Time To Live |   Hop Count   |    Message Sequence Number    |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |                                                               |
//       :                            MESSAGE                            :
//       |                                                               |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       :                                                               :
//                (etc.)
class OlsrPacketHeader : public Header
{
public:
  OlsrPacketHeader ();
  virtual ~OlsrPacketHeader ();

  void SetPacketLength (uint16_t length)
  {
    m_packetLength = length;
  }
  uint16_t GetPacketLength () const
  {
    return m_packetLength;
  }

  void SetPacketSequenceNumber (uint16_t seqnum)
  {
    m_packetSequenceNumber = seqnum;
  }
  uint16_t GetPacketSequenceNumber () const
  {
    return m_packetSequenceNumber;
  }

private:
  uint16_t m_packetLength;
  uint16_t m_packetSequenceNumber;

public:  
  static uint32_t GetUid (void);
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual std::string GetName (void) const { return "OlsrPacket"; }
};


//        0                   1                   2                   3
//        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |  Message Type |     Vtime     |         Message Size          |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |                      Originator Address                       |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |  Time To Live |   Hop Count   |    Message Sequence Number    |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
class OlsrMessageHeader : public Header
{
public:

  enum MessageType {
    HELLO_MESSAGE = 1,
    TC_MESSAGE    = 2,
    MID_MESSAGE   = 3,
    HNA_MESSAGE   = 4,
  };

  OlsrMessageHeader ();
  virtual ~OlsrMessageHeader ();

  void SetMessageType (MessageType messageType)
  {
    m_messageType = messageType;
  }
  MessageType GetMessageType () const
  {
    return m_messageType;
  }

  void SetVTime (Time time)
  {
    m_vTime = OlsrSecondsToEmf (time.GetSeconds ());
  }
  Time GetVTime () const
  {
    return Seconds (OlsrEmfToSeconds (m_vTime));
  }

  void SetOriginatorAddress (Ipv4Address originatorAddress)
  {
    m_originatorAddress = originatorAddress;
  }
  Ipv4Address GetOriginatorAddress () const
  {
    return m_originatorAddress;
  }

  void SetTimeToLive (uint8_t timeToLive)
  {
    m_timeToLive = timeToLive;
  }
  uint8_t GetTimeToLive () const
  {
    return m_timeToLive;
  }

  void SetHopCount (uint8_t hopCount)
  {
    m_hopCount = hopCount;
  }
  uint8_t GetHopCount () const
  {
    return m_hopCount;
  }

  void SetMessageSequenceNumber (uint16_t messageSequenceNumber)
  {
    m_messageSequenceNumber = messageSequenceNumber;
  }
  uint16_t GetMessageSequenceNumber () const
  {
    return m_messageSequenceNumber;
  }

  void SetMessageSize (uint16_t messageSize)
  {
    m_messageSize = messageSize;
  }
  uint16_t GetMessageSize () const
  {
    return m_messageSize;
  }
  
private:
  MessageType m_messageType;
  uint8_t m_vTime;
  Ipv4Address m_originatorAddress;
  uint8_t m_timeToLive;
  uint8_t m_hopCount;
  uint16_t m_messageSequenceNumber;
  uint16_t m_messageSize;

public:  
  static uint32_t GetUid (void);
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual std::string GetName (void) const { return "OlsrMessage"; }
};

// 5.1.  MID Message Format
//
//    The proposed format of a MID message is as follows:
//
//        0                   1                   2                   3
//        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |                    OLSR Interface Address                     |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |                    OLSR Interface Address                     |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |                              ...                              |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
class OlsrMidMessageHeader : public Header
{
public:

  OlsrMidMessageHeader ();
  virtual ~OlsrMidMessageHeader ();

  void SetMessageSize (uint32_t messageSize) {
    m_messageSize = messageSize;
  }
  uint32_t GetMessageSize () const {
    return m_messageSize;
  }
  
  const std::vector<Ipv4Address> & GetInterfaceAddresses () const
  {
    return m_interfaceAddresses;
  }
  void SetInterfaceAddresses (const std::vector<Ipv4Address> &addresses)
  {
    m_interfaceAddresses = addresses;
  }
  
  
private:
  std::vector<Ipv4Address> m_interfaceAddresses;
  uint32_t m_messageSize; // has to be manually set before deserialization

public:  
  static uint32_t GetUid (void);
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual std::string GetName (void) const { return "OlsrMidMessage"; }
};

// 6.1.  HELLO Message Format
//
//        0                   1                   2                   3
//        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |          Reserved             |     Htime     |  Willingness  |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |   Link Code   |   Reserved    |       Link Message Size       |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |                  Neighbor Interface Address                   |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |                  Neighbor Interface Address                   |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       :                             .  .  .                           :
//       :                                                               :
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |   Link Code   |   Reserved    |       Link Message Size       |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |                  Neighbor Interface Address                   |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |                  Neighbor Interface Address                   |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       :                                                               :
//       :                                       :
//    (etc.)
class OlsrHelloMessageHeader : public Header
{
public:

  OlsrHelloMessageHeader ();
  virtual ~OlsrHelloMessageHeader ();

  
  struct LinkMessage {
    uint8_t linkCode;
    std::vector<Ipv4Address> neighborInterfaceAddresses;
  };


  void SetMessageSize (uint32_t messageSize) {
    m_messageSize = messageSize;
  }
  uint32_t GetMessageSize () const {
    return m_messageSize;
  }

  void SetWillingness (uint8_t willingness)
  {
    m_willingness = willingness;
  }
  uint8_t GetWillingness () const
  {
    return m_willingness;
  }

  void SetHTime (Time time)
  {
    m_hTime = OlsrSecondsToEmf (time.GetSeconds ());
  }
  Time GetHTime () const
  {
    return Seconds (OlsrEmfToSeconds (m_hTime));
  }

  const std::vector<LinkMessage> & GetLinkMessages () const
  {
    return m_linkMessages;
  }
  void SetLinkMessages (const std::vector<LinkMessage> &linkMessages)
  {
    m_linkMessages = linkMessages;
  }
  
private:
  uint8_t m_hTime;
  uint8_t m_willingness;
  uint32_t m_messageSize; // has to be manually set before deserialization
  std::vector<LinkMessage> m_linkMessages;

public:  
  static uint32_t GetUid (void);
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual std::string GetName (void) const { return "OlsrHelloMessage"; }
};

// 9.1.  TC Message Format
//
//    The proposed format of a TC message is as follows:
//
//        0                   1                   2                   3
//        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |              ANSN             |           Reserved            |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |               Advertised Neighbor Main Address                |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |               Advertised Neighbor Main Address                |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |                              ...                              |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

class OlsrTcMessageHeader : public Header
{
public:

  OlsrTcMessageHeader ();
  virtual ~OlsrTcMessageHeader ();

  void SetMessageSize (uint32_t messageSize) {
    m_messageSize = messageSize;
  }
  uint32_t GetMessageSize () const {
    return m_messageSize;
  }

  const std::vector<Ipv4Address> & GetNeighborAddresses () const
  {
    return m_neighborAddresses;
  }
  void SetNeighborAddresses (const std::vector<Ipv4Address> &addresses)
  {
    m_neighborAddresses = addresses;
  }

  void SetAnsn (uint16_t ansn)
  {
    m_ansn = ansn;
  }
  uint16_t GetAnsn () const
  {
    return m_ansn;
  }
  
private:
  std::vector<Ipv4Address> m_neighborAddresses;
  uint16_t m_ansn;

  uint32_t m_messageSize; // has to be manually set before deserialization

public:  
  static uint32_t GetUid (void);
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual std::string GetName (void) const { return "OlsrTcMessage"; }
};


// 12.1.  HNA Message Format
//
//    The proposed format of an HNA-message is:
//
//        0                   1                   2                   3
//        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |                         Network Address                       |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |                             Netmask                           |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |                         Network Address                       |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |                             Netmask                           |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//       |                              ...                              |
//       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

// Note: HNA stands for Host Network Association
class OlsrHnaMessageHeader : public Header
{
public:

  OlsrHnaMessageHeader ();
  virtual ~OlsrHnaMessageHeader ();

  struct Association
  {
    Ipv4Address address;
    Ipv4Mask mask;
  };

  void SetMessageSize (uint32_t messageSize)
  {
    m_messageSize = messageSize;
  }
  uint32_t GetMessageSize () const
  {
    return m_messageSize;
  }

  const std::vector<Association> & GetAssociations () const
  {
    return m_associations;
  }
  void SetAssociations (const std::vector<Association> &associations)
  {
    m_associations = associations;
  }
  
private:
  std::vector<Association> m_associations;
  uint32_t m_messageSize; // has to be manually set before deserialization

public:  
  static uint32_t GetUid (void);
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual std::string GetName (void) const { return "OlsrHnaMessage"; }
};


}; // namespace ns3

#endif /* OLSR_HEADER_H */

