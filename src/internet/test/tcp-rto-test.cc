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
#include "tcp-error-model.h"

#include "ns3/node.h"
#include "ns3/log.h"
#include "ns3/tcp-westwood.h"
#include "ns3/simple-channel.h"
#include "tcp-general-test.h"
#include "tcp-error-model.h"

NS_LOG_COMPONENT_DEFINE ("TcpRtoTest");

using namespace ns3;

/**
 * \ingroup internet-test
 * \ingroup tests
 *
 * \brief Testing the moments after an RTO expiration
 *
 * The scope of this test is to be sure that, after an RTO expiration,
 * the TCP implementation set the correct state in the ACK state machine,
 * and marks the lost segment as lost; then, after the retransmission, the
 * state is fully recovered. This is the base check, where only one segment
 * (the first) is lost and retransmitted.
 *
 */
class TcpRtoTest : public TcpGeneralTest
{
public:
  /**
   * \brief Constructor.
   * \param congControl Congestion control type.
   * \param msg Test description.
   */
  TcpRtoTest (TypeId &congControl, const std::string &msg);

protected:

  virtual Ptr<TcpSocketMsgBase> CreateSenderSocket (Ptr<Node> node);
  virtual void AfterRTOExpired (const Ptr<const TcpSocketState> tcb, SocketWho who);
  virtual void RcvAck      (const Ptr<const TcpSocketState> tcb,
                            const TcpHeader& h, SocketWho who);
  virtual void ProcessedAck (const Ptr<const TcpSocketState> tcb,
                             const TcpHeader& h, SocketWho who);
  virtual void FinalChecks ();
  virtual void ConfigureProperties ();
  virtual void ConfigureEnvironment ();

private:
  bool m_afterRTOExpired;      //!< True if RTO is expired.
  bool m_segmentReceived; //!< True if segments have been received.
};

TcpRtoTest::TcpRtoTest (TypeId &congControl, const std::string &desc)
  : TcpGeneralTest (desc),
    m_afterRTOExpired (false),
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
TcpRtoTest::AfterRTOExpired (const Ptr<const TcpSocketState> tcb, SocketWho who)
{
  // In this test, the RTO fires for the first segment (and no more).
  // This function is called after the management of the RTO expiration,
  // and because of this we must check all the involved variables.
  NS_TEST_ASSERT_MSG_EQ (m_afterRTOExpired, false,
                         "Second RTO expired");
  NS_TEST_ASSERT_MSG_EQ (GetCongStateFrom (tcb), TcpSocketState::CA_LOSS,
                         "Ack state machine not in LOSS state after a loss");

  m_afterRTOExpired = true;
}

void
TcpRtoTest::RcvAck (const Ptr<const TcpSocketState> tcb, const TcpHeader& h,
                    SocketWho who)
{
  // Called after the first ack is received (the lost segment has been
  // successfully retransmitted. We must check on the sender that variables
  // are in the same state as they where after AfterRTOExpired if it is the first
  // ACK after the loss; in every other case, all must be OPEN and the counter
  // set to 0.

  if (m_afterRTOExpired && who == SENDER)
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
      m_afterRTOExpired = false;
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


/**
 * \ingroup internet-test
 * \ingroup tests
 *
 * \brief Testing the timing of RTO
 *
 * Checking if RTO is doubled ONLY after a retransmission.
 */
class TcpTimeRtoTest : public TcpGeneralTest
{
public:
  /**
   * \brief Constructor.
   * \param congControl Congestion control type.
   * \param msg Test description.
   */
  TcpTimeRtoTest (TypeId &congControl, const std::string &msg);

protected:
  virtual Ptr<TcpSocketMsgBase> CreateSenderSocket (Ptr<Node> node);
  virtual Ptr<ErrorModel> CreateReceiverErrorModel ();
  virtual void ErrorClose  (SocketWho who);
  virtual void AfterRTOExpired (const Ptr<const TcpSocketState> tcb, SocketWho who);
  virtual void Tx (const Ptr<const Packet> p, const TcpHeader&h, SocketWho who);
  virtual void FinalChecks ();

  virtual void ConfigureEnvironment ();

  /**
   * \brief Called when a packet has been dropped.
   * \param ipH IPv4 header.
   * \param tcpH TCP header.
   * \param p The packet.
   */
  void PktDropped (const Ipv4Header &ipH, const TcpHeader& tcpH, Ptr<const Packet> p);

private:
  uint32_t m_senderSentSegments;  //!< Number of segments sent.
  Time m_previousRTO;             //!< Previous RTO.
  bool m_closed;                  //!< True if the connection is closed.
};


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
TcpTimeRtoTest::AfterRTOExpired (const Ptr<const TcpSocketState> tcb, SocketWho who)
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


/**
 * \ingroup internet-test
 * \ingroup tests
 *
 * \brief TCP RTO TestSuite
 */
class TcpRtoTestSuite : public TestSuite
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
};

static TcpRtoTestSuite g_TcpRtoTestSuite; //!< Static variable for test initialization


