/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INESC Porto
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

#include "ns3/assert.h"

#include "olsr-header.h"

#define IPV4_ADDRESS_SIZE 4
#define OLSR_MSG_HEADER_SIZE 12
#define OLSR_PKT_HEADER_SIZE 4

namespace ns3 {
namespace olsr {

/// Scaling factor used in RFC 3626.
#define OLSR_C 0.0625

///
/// \brief Converts a decimal number of seconds to the mantissa/exponent format.
///
/// \param seconds decimal number of seconds we want to convert.
/// \return the number of seconds in mantissa/exponent format.
///
uint8_t
SecondsToEmf (double seconds)
{
  int a, b = 0;
  
  // find the largest integer 'b' such that: T/C >= 2^b
  for (b = 0; (seconds/OLSR_C) >= (1 << b); ++b)
    ;
  NS_ASSERT ((seconds/OLSR_C) < (1 << b));
  b--;
  NS_ASSERT ((seconds/OLSR_C) >= (1 << b));

  // compute the expression 16*(T/(C*(2^b))-1), which may not be a integer
  double tmp = 16*(seconds/(OLSR_C*(1<<b))-1);

  // round it up.  This results in the value for 'a'
  a = (int) ceil (tmp);

  // if 'a' is equal to 16: increment 'b' by one, and set 'a' to 0
  if (a == 16)
    {
      b += 1;
      a = 0;
    }

  // now, 'a' and 'b' should be integers between 0 and 15,
  NS_ASSERT (a >= 0 && a < 16);
  NS_ASSERT (b >= 0 && b < 16);

  // the field will be a byte holding the value a*16+b
  return (uint8_t) ((a << 4) | b);
}

///
/// \brief Converts a number of seconds in the mantissa/exponent format to a decimal number.
///
/// \param olsr_format number of seconds in mantissa/exponent format.
/// \return the decimal number of seconds.
///
double
EmfToSeconds (uint8_t olsrFormat)
{
  int a = (olsrFormat >> 4);
  int b = (olsrFormat & 0xf);
  // value = C*(1+a/16)*2^b [in seconds]
  return OLSR_C * (1 + a/16.0) * (1 << b);
}



// ---------------- OLSR Packet -------------------------------

PacketHeader::PacketHeader ()
{}

PacketHeader::~PacketHeader ()
{}

uint32_t
PacketHeader::GetUid (void)
{
  static uint32_t uid = AllocateUid<PacketHeader>
    ("PacketHeader.nsnam.org");
  return uid;
}

uint32_t 
PacketHeader::GetSerializedSize (void) const
{
  return OLSR_PKT_HEADER_SIZE;
}

void 
PacketHeader::Print (std::ostream &os) const
{
  // TODO
}

void
PacketHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteHtonU16 (m_packetLength);
  i.WriteHtonU16 (m_packetSequenceNumber);
}

uint32_t
PacketHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_packetLength  = i.ReadNtohU16 ();
  m_packetSequenceNumber = i.ReadNtohU16 ();
  return GetSerializedSize ();
}


// ---------------- OLSR Message -------------------------------

MessageHeader::MessageHeader ()
  : m_messageType (MessageHeader::MessageType (0))
{}

MessageHeader::~MessageHeader ()
{}

uint32_t
MessageHeader::GetUid (void)
{
  static uint32_t uid = AllocateUid<MessageHeader>
    ("MessageHeader.nsnam.org");
  return uid;
}

uint32_t
MessageHeader::GetSerializedSize (void) const
{
  uint32_t size = OLSR_MSG_HEADER_SIZE;
  switch (m_messageType)
    {
    case MID_MESSAGE:
      size += m_message.mid.GetSerializedSize ();
      break;
    case HELLO_MESSAGE:
      size += m_message.hello.GetSerializedSize ();
      break;
    case TC_MESSAGE:
      size += m_message.tc.GetSerializedSize ();
      break;
    case HNA_MESSAGE:
      size += m_message.hna.GetSerializedSize ();
      break;
    default:
      NS_ASSERT (false);
    }
  return size;
}

void 
MessageHeader::Print (std::ostream &os) const
{
  // TODO
}

void
MessageHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteU8 (m_messageType);
  i.WriteU8 (m_vTime);
  i.WriteHtonU16 (GetSerializedSize () - OLSR_MSG_HEADER_SIZE);
  i.WriteHtonU32 (m_originatorAddress.GetHostOrder ());
  i.WriteU8 (m_timeToLive);
  i.WriteU8 (m_hopCount);
  i.WriteHtonU16 (m_messageSequenceNumber);

  switch (m_messageType)
    {
    case MID_MESSAGE:
      m_message.mid.Serialize (i);
      break;
    case HELLO_MESSAGE:
      m_message.hello.Serialize (i);
      break;
    case TC_MESSAGE:
      m_message.tc.Serialize (i);
      break;
    case HNA_MESSAGE:
      m_message.hna.Serialize (i);
      break;
    default:
      NS_ASSERT (false);
    }

}

uint32_t
MessageHeader::Deserialize (Buffer::Iterator start)
{
  uint32_t size;
  Buffer::Iterator i = start;
  m_messageType  = (MessageType) i.ReadU8 ();
  NS_ASSERT (m_messageType >= HELLO_MESSAGE && m_messageType <= HNA_MESSAGE);
  m_vTime  = i.ReadU8 ();
  m_messageSize  = i.ReadNtohU16 ();
  m_originatorAddress = Ipv4Address (i.ReadNtohU32 ());
  m_timeToLive  = i.ReadU8 ();
  m_hopCount  = i.ReadU8 ();
  m_messageSequenceNumber = i.ReadNtohU16 ();
  size = OLSR_MSG_HEADER_SIZE;
  switch (m_messageType)
    {
    case MID_MESSAGE:
      size += m_message.mid.Deserialize (i, m_messageSize);
      break;
    case HELLO_MESSAGE:
      size += m_message.hello.Deserialize (i, m_messageSize);
      break;
    case TC_MESSAGE:
      size += m_message.tc.Deserialize (i, m_messageSize);
      break;
    case HNA_MESSAGE:
      size += m_message.hna.Deserialize (i, m_messageSize);
      break;
    default:
      NS_ASSERT (false);
    }
  return size;
}


// ---------------- OLSR MID Message -------------------------------

uint32_t 
MessageHeader::Mid::GetSerializedSize (void) const
{
  return this->interfaceAddresses.size () * IPV4_ADDRESS_SIZE;
}

void 
MessageHeader::Mid::Print (std::ostream &os) const
{
  // TODO
}

void
MessageHeader::Mid::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  for (std::vector<Ipv4Address>::const_iterator iter = this->interfaceAddresses.begin ();
       iter != this->interfaceAddresses.end (); iter++)
    {
      i.WriteHtonU32 (iter->GetHostOrder ());
    }
}

uint32_t
MessageHeader::Mid::Deserialize (Buffer::Iterator start, uint32_t messageSize)
{
  Buffer::Iterator i = start;

  this->interfaceAddresses.clear ();
  NS_ASSERT (messageSize >= 0);
  NS_ASSERT (messageSize % IPV4_ADDRESS_SIZE == 0);
  
  int numAddresses = messageSize / IPV4_ADDRESS_SIZE;
  this->interfaceAddresses.erase (this->interfaceAddresses.begin(),
                                  this->interfaceAddresses.end ());
  for (int n = 0; n < numAddresses; ++n)
      this->interfaceAddresses.push_back (Ipv4Address (i.ReadNtohU32 ()));
  return GetSerializedSize ();
}



// ---------------- OLSR HELLO Message -------------------------------

uint32_t 
MessageHeader::Hello::GetSerializedSize (void) const
{
  uint32_t size = 4;
  for (std::vector<LinkMessage>::const_iterator iter = this->linkMessages.begin ();
       iter != this->linkMessages.end (); iter++)
    {
      const LinkMessage &lm = *iter;
      size += 4;
      size += IPV4_ADDRESS_SIZE * lm.neighborInterfaceAddresses.size ();
    }
  return size;
}

void 
MessageHeader::Hello::Print (std::ostream &os) const
{
  // TODO
}

void
MessageHeader::Hello::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU16 (0); // Reserved
  i.WriteU8 (this->hTime);
  i.WriteU8 (this->willingness);
  
  for (std::vector<LinkMessage>::const_iterator iter = this->linkMessages.begin ();
       iter != this->linkMessages.end (); iter++)
    {
      const LinkMessage &lm = *iter;

      i.WriteU8 (lm.linkCode);
      i.WriteU8 (0); // Reserved

      // The size of the link message, counted in bytes and measured
      // from the beginning of the "Link Code" field and until the
      // next "Link Code" field (or - if there are no more link types
      // - the end of the message).
      i.WriteHtonU16 (4 + lm.neighborInterfaceAddresses.size () * IPV4_ADDRESS_SIZE);
      
      for (std::vector<Ipv4Address>::const_iterator neigh_iter = lm.neighborInterfaceAddresses.begin ();
           neigh_iter != lm.neighborInterfaceAddresses.end (); neigh_iter++)
        {
          i.WriteHtonU32 (neigh_iter->GetHostOrder ());
        }
    }
}

uint32_t
MessageHeader::Hello::Deserialize (Buffer::Iterator start, uint32_t messageSize)
{
  Buffer::Iterator i = start;

  NS_ASSERT (messageSize >= 4);

  this->linkMessages.clear ();
  
  uint16_t helloSizeLeft = messageSize;
  
  i.ReadNtohU16 (); // Reserved
  this->hTime = i.ReadU8 ();
  this->willingness = i.ReadU8 ();
  
  helloSizeLeft -= 4;

  while (helloSizeLeft)
    {
      LinkMessage lm;
      NS_ASSERT (helloSizeLeft >= 4);
      lm.linkCode = i.ReadU8 ();
      i.ReadU8 (); // Reserved
      uint16_t lmSize = i.ReadNtohU16 ();
      NS_ASSERT ((lmSize - 4) % IPV4_ADDRESS_SIZE == 0);
      for (int n = (lmSize - 4) / IPV4_ADDRESS_SIZE; n; --n)
        {
          lm.neighborInterfaceAddresses.push_back (Ipv4Address (i.ReadNtohU32 ()));
        }
      helloSizeLeft -= lmSize;
      this->linkMessages.push_back (lm);
    }

  return messageSize;
}



// ---------------- OLSR TC Message -------------------------------

uint32_t 
MessageHeader::Tc::GetSerializedSize (void) const
{
  return 4 + this->neighborAddresses.size () * IPV4_ADDRESS_SIZE;
}

void 
MessageHeader::Tc::Print (std::ostream &os) const
{
  // TODO
}

void
MessageHeader::Tc::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteHtonU16 (this->ansn);
  i.WriteHtonU16 (0); // Reserved

  for (std::vector<Ipv4Address>::const_iterator iter = this->neighborAddresses.begin ();
       iter != this->neighborAddresses.end (); iter++)
    {
      i.WriteHtonU32 (iter->GetHostOrder ());
    }
}

uint32_t
MessageHeader::Tc::Deserialize (Buffer::Iterator start, uint32_t messageSize)
{
  Buffer::Iterator i = start;

  this->neighborAddresses.clear ();
  NS_ASSERT (messageSize >= 4);

  this->ansn = i.ReadNtohU16 ();
  i.ReadNtohU16 (); // Reserved
  
  NS_ASSERT ((messageSize - 4) % IPV4_ADDRESS_SIZE == 0);
  int numAddresses = (messageSize - 4) / IPV4_ADDRESS_SIZE;
  this->neighborAddresses.clear ();
  for (int n = 0; n < numAddresses; ++n)
    this->neighborAddresses.push_back (Ipv4Address (i.ReadNtohU32 ()));

  return messageSize;
}


// ---------------- OLSR HNA Message -------------------------------

uint32_t 
MessageHeader::Hna::GetSerializedSize (void) const
{
  return 2*this->associations.size () * IPV4_ADDRESS_SIZE;
}

void 
MessageHeader::Hna::Print (std::ostream &os) const
{
  // TODO
}

void
MessageHeader::Hna::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  for (size_t n = 0; n < this->associations.size (); ++n)
    {
      i.WriteHtonU32 (this->associations[n].address.GetHostOrder ());
      i.WriteHtonU32 (this->associations[n].mask.GetHostOrder ());
    }
}

uint32_t
MessageHeader::Hna::Deserialize (Buffer::Iterator start, uint32_t messageSize)
{
  Buffer::Iterator i = start;

  NS_ASSERT (messageSize % (IPV4_ADDRESS_SIZE*2) == 0);
  int numAddresses = messageSize / IPV4_ADDRESS_SIZE / 2;
  this->associations.clear ();
  for (int n = 0; n < numAddresses; ++n)
    {
      Ipv4Address address (i.ReadNtohU32 ());
      Ipv4Mask mask (i.ReadNtohU32 ());
      this->associations.push_back ((Association) {address, mask});
    }
  return messageSize;
}



}} // namespace olsr, ns3


#ifdef RUN_SELF_TESTS


#include "ns3/test.h"
#include "ns3/packet.h"
#include <math.h>


namespace ns3 {

class OlsrHeaderTest : public ns3::Test {
private:
public:
  OlsrHeaderTest ();
  virtual bool RunTests (void);


};

OlsrHeaderTest::OlsrHeaderTest ()
  : ns3::Test ("OlsrHeader")
{}


bool 
OlsrHeaderTest::RunTests (void)
{
  bool result = true;

  // Testing packet header + message header + MID message
  {
    Packet packet;

    {
      olsr::PacketHeader hdr;
      olsr::MessageHeader msg1;
      olsr::MessageHeader::Mid &mid1 = msg1.GetMid ();
      olsr::MessageHeader msg2;
      olsr::MessageHeader::Mid &mid2 = msg2.GetMid ();
    
      // MID message #1
      {
        std::vector<Ipv4Address> &addresses = mid1.interfaceAddresses;
        addresses.clear ();
        addresses.push_back (Ipv4Address ("1.2.3.4"));
        addresses.push_back (Ipv4Address ("1.2.3.5"));
      }

      msg1.SetTimeToLive (255);
      msg1.SetOriginatorAddress (Ipv4Address ("11.22.33.44"));
      msg1.SetVTime (Seconds (9));
      msg1.SetMessageSequenceNumber (7);

      // MID message #2
      {
        std::vector<Ipv4Address> &addresses = mid2.interfaceAddresses;
        addresses.clear ();
        addresses.push_back (Ipv4Address ("2.2.3.4"));
        addresses.push_back (Ipv4Address ("2.2.3.5"));
      }

      msg2.SetTimeToLive (254);
      msg2.SetOriginatorAddress (Ipv4Address ("12.22.33.44"));
      msg2.SetVTime (Seconds (10));
      msg2.SetMessageType (olsr::MessageHeader::MID_MESSAGE);
      msg2.SetMessageSequenceNumber (7);

      // Build an OLSR packet header
      hdr.SetPacketLength (hdr.GetSerializedSize () + msg1.GetSerializedSize () + msg2.GetSerializedSize ());
      hdr.SetPacketSequenceNumber (123);
    

      // Now add all the headers in the correct order
      packet.AddHeader (msg2);
      packet.AddHeader (msg1);
      packet.AddHeader (hdr);
    }    

    {
      olsr::PacketHeader hdr;
      packet.RemoveHeader (hdr);
      NS_TEST_ASSERT_EQUAL (hdr.GetPacketSequenceNumber (), 123);
      uint32_t sizeLeft = hdr.GetPacketLength () - hdr.GetSerializedSize ();
      {
        olsr::MessageHeader msg1;

        packet.RemoveHeader (msg1);

        NS_TEST_ASSERT_EQUAL (msg1.GetTimeToLive (),  255);
        NS_TEST_ASSERT_EQUAL (msg1.GetOriginatorAddress (), Ipv4Address ("11.22.33.44"));
        NS_TEST_ASSERT_EQUAL (msg1.GetVTime (), Seconds (9));
        NS_TEST_ASSERT_EQUAL (msg1.GetMessageType (), olsr::MessageHeader::MID_MESSAGE);
        NS_TEST_ASSERT_EQUAL (msg1.GetMessageSequenceNumber (), 7);

        olsr::MessageHeader::Mid &mid1 = msg1.GetMid ();
        NS_TEST_ASSERT_EQUAL (mid1.interfaceAddresses.size (), 2);
        NS_TEST_ASSERT_EQUAL (*mid1.interfaceAddresses.begin (), Ipv4Address ("1.2.3.4"));

        sizeLeft -= msg1.GetSerializedSize ();
        NS_TEST_ASSERT (sizeLeft > 0);
      }
      {
        // now read the second message
        olsr::MessageHeader msg2;

        packet.RemoveHeader (msg2);

        NS_TEST_ASSERT_EQUAL (msg2.GetTimeToLive (),  254);
        NS_TEST_ASSERT_EQUAL (msg2.GetOriginatorAddress (), Ipv4Address ("12.22.33.44"));
        NS_TEST_ASSERT_EQUAL (msg2.GetVTime (), Seconds (10));
        NS_TEST_ASSERT_EQUAL (msg2.GetMessageType (), olsr::MessageHeader::MID_MESSAGE);
        NS_TEST_ASSERT_EQUAL (msg2.GetMessageSequenceNumber (), 7);

        olsr::MessageHeader::Mid mid2 = msg2.GetMid ();
        NS_TEST_ASSERT_EQUAL (mid2.interfaceAddresses.size (), 2);
        NS_TEST_ASSERT_EQUAL (*mid2.interfaceAddresses.begin (), Ipv4Address ("2.2.3.4"));

        sizeLeft -= msg2.GetSerializedSize ();
        NS_TEST_ASSERT_EQUAL (sizeLeft, 0);
      }
    }
  }
  
  // Test the HELLO message
  {
    Packet packet;
    olsr::MessageHeader msgIn;
    olsr::MessageHeader::Hello &helloIn = msgIn.GetHello ();

    helloIn.SetHTime (Seconds (7));
    helloIn.willingness = 66;

    {
      olsr::MessageHeader::Hello::LinkMessage lm1;
      lm1.linkCode = 2;
      lm1.neighborInterfaceAddresses.push_back (Ipv4Address ("1.2.3.4"));
      lm1.neighborInterfaceAddresses.push_back (Ipv4Address ("1.2.3.5"));
      helloIn.linkMessages.push_back (lm1);

      olsr::MessageHeader::Hello::LinkMessage lm2;
      lm2.linkCode = 3;
      lm2.neighborInterfaceAddresses.push_back (Ipv4Address ("2.2.3.4"));
      lm2.neighborInterfaceAddresses.push_back (Ipv4Address ("2.2.3.5"));
      helloIn.linkMessages.push_back (lm2);
    }

    packet.AddHeader (msgIn);

    olsr::MessageHeader msgOut;
    packet.RemoveHeader (msgOut);
    olsr::MessageHeader::Hello &helloOut = msgOut.GetHello ();
    
    NS_TEST_ASSERT_EQUAL (helloOut.GetHTime (), Seconds (7));
    NS_TEST_ASSERT_EQUAL (helloOut.willingness, 66);
    NS_TEST_ASSERT_EQUAL (helloOut.linkMessages.size (), 2);

    NS_TEST_ASSERT_EQUAL (helloOut.linkMessages[0].linkCode, 2);
    NS_TEST_ASSERT_EQUAL (helloOut.linkMessages[0].neighborInterfaceAddresses[0],
                          Ipv4Address ("1.2.3.4"));
    NS_TEST_ASSERT_EQUAL (helloOut.linkMessages[0].neighborInterfaceAddresses[1],
                          Ipv4Address ("1.2.3.5"));

    NS_TEST_ASSERT_EQUAL (helloOut.linkMessages[1].linkCode, 3);
    NS_TEST_ASSERT_EQUAL (helloOut.linkMessages[1].neighborInterfaceAddresses[0],
                          Ipv4Address ("2.2.3.4"));
    NS_TEST_ASSERT_EQUAL (helloOut.linkMessages[1].neighborInterfaceAddresses[1],
                          Ipv4Address ("2.2.3.5"));

    // check that all bytes of the message were read
    NS_TEST_ASSERT_EQUAL (packet.GetSize (), 0);
  }

  // Test the TC message
  {
    Packet packet;
    olsr::MessageHeader msgIn;
    olsr::MessageHeader::Tc &tcIn = msgIn.GetTc ();

    tcIn.ansn = 0x1234;
    tcIn.neighborAddresses.push_back (Ipv4Address ("1.2.3.4"));
    tcIn.neighborAddresses.push_back (Ipv4Address ("1.2.3.5"));
    packet.AddHeader (msgIn);

    olsr::MessageHeader msgOut;
    packet.RemoveHeader (msgOut);
    olsr::MessageHeader::Tc &tcOut = msgOut.GetTc ();
    
    NS_TEST_ASSERT_EQUAL (tcOut.ansn, 0x1234);
    NS_TEST_ASSERT_EQUAL (tcOut.neighborAddresses.size (), 2);

    NS_TEST_ASSERT_EQUAL (tcOut.neighborAddresses[0],
                          Ipv4Address ("1.2.3.4"));
    NS_TEST_ASSERT_EQUAL (tcOut.neighborAddresses[1],
                          Ipv4Address ("1.2.3.5"));

    // check that all bytes of the message were read
    NS_TEST_ASSERT_EQUAL (packet.GetSize (), 0);
  }

  // Test the HNA message
  {
    Packet packet;
    olsr::MessageHeader msgIn;
    olsr::MessageHeader::Hna &hnaIn = msgIn.GetHna ();

    hnaIn.associations.push_back ((olsr::MessageHeader::Hna::Association)
                                  { Ipv4Address ("1.2.3.4"), Ipv4Mask ("255.255.255.0")});
    hnaIn.associations.push_back ((olsr::MessageHeader::Hna::Association)
                                  {Ipv4Address ("1.2.3.5"), Ipv4Mask ("255.255.0.0")});
    packet.AddHeader (msgIn);

    olsr::MessageHeader msgOut;
    packet.RemoveHeader (msgOut);
    olsr::MessageHeader::Hna &hnaOut = msgOut.GetHna ();
    
    NS_TEST_ASSERT_EQUAL (hnaOut.associations.size (), 2);

    NS_TEST_ASSERT_EQUAL (hnaOut.associations[0].address,
                          Ipv4Address ("1.2.3.4"));
    NS_TEST_ASSERT_EQUAL (hnaOut.associations[0].mask,
                          Ipv4Mask ("255.255.255.0"));

    NS_TEST_ASSERT_EQUAL (hnaOut.associations[1].address,
                          Ipv4Address ("1.2.3.5"));
    NS_TEST_ASSERT_EQUAL (hnaOut.associations[1].mask,
                          Ipv4Mask ("255.255.0.0"));

    // check that all bytes of the message were read
    NS_TEST_ASSERT_EQUAL (packet.GetSize (), 0);
  }

  for (int time = 1; time <= 30; time++)
    {
      uint8_t emf = olsr::SecondsToEmf (time);
      double seconds = olsr::EmfToSeconds (emf);
      if (seconds < 0 || fabs (seconds - time) > 0.1)
        {
          result = false;
          Failure () << "In " << time << " out " << seconds << std::endl;
        }
    }

  return result;
}

static OlsrHeaderTest gOlsrHeaderTest;

}; // namespace


#endif /* RUN_SELF_TESTS */
