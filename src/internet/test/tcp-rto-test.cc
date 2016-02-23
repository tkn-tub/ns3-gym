/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Natale Patriciello <natale.patriciello@gmail.com>
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
 */
#include "tcp-rto-test.h"
#include "tcp-error-model.h"

#include "ns3/node.h"
#include "ns3/log.h"
#include "ns3/tcp-westwood.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TcpRtoTest");

TcpRtoTest::TcpRtoTest (TypeId &congControl, const std::string &desc)
  : TcpGeneralTest (desc),
    m_rtoExpired (false),
    m_segmentReceived (false)
{
  m_congControlTypeId = congControl;
}

void
TcpRtoTest::ConfigureEnvironment ()
{
  TcpGeneralTest::ConfigureEnvironment ();
  SetAppPktCount (100);
}

void
TcpRtoTest::ConfigureProperties ()
{
  TcpGeneralTest::ConfigureProperties ();
  SetInitialSsThresh (SENDER, 0);
}

Ptr<TcpSocketMsgBase>
TcpRtoTest::CreateSenderSocket (Ptr<Node> node)
{
  // Get a really low RTO, and let them fire as soon as possible since
  // we are interested only in what happen after it expires
  Ptr<TcpSocketMsgBase> socket = TcpGeneralTest::CreateSenderSocket (node);
  socket->SetAttribute ("MinRto", TimeValue (Seconds (0.5)));

  return socket;
}

void
TcpRtoTest::RTOExpired (const Ptr<const TcpSocketState> tcb, SocketWho who)
{
  // In this test, the RTO fires for the first segment (and no more).
  // This function is called after the management of the RTO expiration,
  // and because of this we must check all the involved variables.
  NS_TEST_ASSERT_MSG_EQ (m_rtoExpired, false,
                         "Second RTO expired");
  NS_TEST_ASSERT_MSG_EQ (GetCongStateFrom (tcb), TcpSocketState::CA_LOSS,
                         "Ack state machine not in LOSS state after a loss");

  m_rtoExpired = true;
}

void
TcpRtoTest::RcvAck (const Ptr<const TcpSocketState> tcb, const TcpHeader& h,
                    SocketWho who)
{
  // Called after the first ack is received (the lost segment has been
  // successfully retransmitted. We must check on the sender that variables
  // are in the same state as they where after RTOExpired if it is the first
  // ACK after the loss; in every other case, all must be OPEN and the counter
  // set to 0.

  if (m_rtoExpired && who == SENDER)
    {
      NS_TEST_ASSERT_MSG_EQ (GetCongStateFrom (tcb), TcpSocketState::CA_LOSS,
                             "Ack state machine not in LOSS state after a loss");
    }
  else
    {
      NS_TEST_ASSERT_MSG_EQ (GetCongStateFrom (tcb), TcpSocketState::CA_OPEN,
                             "Ack state machine not in OPEN state after recovering "
                             "from loss");
    }
}

void
TcpRtoTest::ProcessedAck (const Ptr<const TcpSocketState> tcb, const TcpHeader &h,
                          SocketWho who)
{
  // Called after the ACK processing. Every time we should be in OPEN state,
  // without any packet lost or marked as retransmitted, in both the sockets

  NS_TEST_ASSERT_MSG_EQ (GetCongStateFrom (tcb), TcpSocketState::CA_OPEN,
                         "Ack state machine not in OPEN state after recovering "
                         "from loss");

  if (who == SENDER)
    {
      m_rtoExpired = false;
      m_segmentReceived = true;
    }
}

void
TcpRtoTest::FinalChecks ()
{
  // At least one time we should process an ACK; otherwise, the segment
  // has not been retransmitted, and this is bad

  NS_TEST_ASSERT_MSG_EQ (m_segmentReceived, true,
                         "Retransmission has not been done");
}

// TcpTimeRtoTest

TcpTimeRtoTest::TcpTimeRtoTest (TypeId &congControl, const std::string &desc)
  :   TcpGeneralTest (desc),
    m_senderSentSegments (0),
    m_closed (false)
{
  m_congControlTypeId = congControl;
}

void
TcpTimeRtoTest::ConfigureEnvironment ()
{
  TcpGeneralTest::ConfigureEnvironment ();
  SetAppPktCount (100);
}


Ptr<TcpSocketMsgBase>
TcpTimeRtoTest::CreateSenderSocket (Ptr<Node> node)
{
  Ptr<TcpSocketMsgBase> s = TcpGeneralTest::CreateSenderSocket (node);
  s->SetAttribute ("DataRetries", UintegerValue (6));

  return s;
}

Ptr<ErrorModel>
TcpTimeRtoTest::CreateReceiverErrorModel ()
{
  Ptr<TcpSeqErrorModel> errorModel = CreateObject<TcpSeqErrorModel> ();

  // Drop packet for 7 times. At the 7th, the connection should be dropped.
  for (uint32_t i = 0; i<7; ++i)
    {
      errorModel->AddSeqToKill (SequenceNumber32 (1));
    }

  errorModel->SetDropCallback (MakeCallback (&TcpTimeRtoTest::PktDropped, this));

  return errorModel;
}

void
TcpTimeRtoTest::Tx (const Ptr<const Packet> p, const TcpHeader&h, SocketWho who)
{
  NS_LOG_FUNCTION (this << p << h << who);

  if (who == SENDER)
    {
      ++m_senderSentSegments;
      NS_LOG_INFO (Simulator::Now ().GetSeconds () << "\tMeasured RTO:" <<
                   GetRto (SENDER).GetSeconds ());

      if (h.GetFlags () & TcpHeader::SYN)
        {
          NS_ASSERT (m_senderSentSegments == 1);

          Time s_rto = GetRto (SENDER);
          NS_TEST_ASSERT_MSG_EQ (s_rto, GetConnTimeout (SENDER),
                                 "SYN packet sent without respecting "
                                 "ConnTimeout attribute");
        }
      else
        {
          NS_LOG_INFO (Simulator::Now ().GetSeconds () << "\tTX: " << h <<
                       m_senderSentSegments);

          NS_TEST_ASSERT_MSG_EQ (h.GetSequenceNumber ().GetValue (), 1,
                                 "First packet has been correctly sent");

          // Remember, from RFC:
          // m_rto = Max (m_rtt->GetEstimate () +
          //              Max (m_clockGranularity, m_rtt->GetVariation ()*4), m_minRto);

          if (m_senderSentSegments == 2)
            { // ACK of SYN-ACK, rto set for the first time, since now we have
              // an estimation of RTT

              Ptr<RttEstimator> rttEstimator = GetRttEstimator (SENDER);
              Time clockGranularity = GetClockGranularity (SENDER);
              m_previousRTO = rttEstimator->GetEstimate ();

              if (clockGranularity > rttEstimator->GetVariation ()*4)
                {
                  m_previousRTO += clockGranularity;
                }
              else
                {
                  m_previousRTO += rttEstimator->GetVariation ()*4;
                }

              m_previousRTO = Max (m_previousRTO, GetMinRto (SENDER));

              NS_TEST_ASSERT_MSG_EQ_TOL (GetRto (SENDER), m_previousRTO, Seconds (0.01),
                                         "RTO value differs from calculation");
            }
          else if (m_senderSentSegments == 3)
            { // First data packet. RTO should be the same as before

              NS_TEST_ASSERT_MSG_EQ_TOL (GetRto (SENDER), m_previousRTO, Seconds (0.01),
                                         "RTO value has changed unexpectedly");

            }
        }
    }
  else if (who == RECEIVER)
    {

    }
}

void
TcpTimeRtoTest::ErrorClose  (SocketWho who)
{
  m_closed = true;
}

void
TcpTimeRtoTest::RTOExpired (const Ptr<const TcpSocketState> tcb, SocketWho who)
{
  NS_TEST_ASSERT_MSG_EQ (who, SENDER, "RTO in Receiver. That's unexpected");

  Time actualRto = GetRto (SENDER);

  if (actualRto < Seconds (60))
    {
      NS_TEST_ASSERT_MSG_EQ_TOL (actualRto, m_previousRTO+m_previousRTO, Seconds (0.01),
                                 "RTO has not doubled after an expiration");
      m_previousRTO += m_previousRTO;
    }
  else
    {
      NS_TEST_ASSERT_MSG_EQ (actualRto, Seconds (60),
                             "RTO goes beyond 60 second limit");
    }
}

void
TcpTimeRtoTest::PktDropped (const Ipv4Header &ipH, const TcpHeader& tcpH,
                            Ptr<const Packet> p)
{
  NS_LOG_INFO (Simulator::Now ().GetSeconds () << "\tDROPPED! " << tcpH);
}

void
TcpTimeRtoTest::FinalChecks ()
{
  NS_TEST_ASSERT_MSG_EQ (m_closed, true,
                         "Socket has not been closed after retrying data retransmissions");
}

//-----------------------------------------------------------------------------

static class TcpRtoTestSuite : public TestSuite
{
public:
  TcpRtoTestSuite () : TestSuite ("tcp-rto-test", UNIT)
  {
    std::list<TypeId> types;
    types.insert (types.begin (), TcpNewReno::GetTypeId ());
    types.insert (types.begin (), TcpWestwood::GetTypeId ());

    for (std::list<TypeId>::iterator it = types.begin (); it != types.end (); ++it)
      {
        AddTestCase (new TcpRtoTest ((*it), "RTO retransmit testing"), TestCase::QUICK);
        AddTestCase (new TcpTimeRtoTest ((*it), "RTO timing testing"), TestCase::QUICK);
      }
  }
} g_TcpRtoTestSuite;

} // namespace ns3
