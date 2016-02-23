/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 Natale Patriciello <natale.patriciello@gmail.com>
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

#include "tcp-general-test.h"
#include "ns3/node.h"
#include "ns3/log.h"
#include "tcp-error-model.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TcpBytesInFlightTestSuite");

/**
 * \brief Check the value of BytesInFlight against a home-made guess
 *
 * The guess is made wrt to segments that travel the network; we have,
 * in theory, the possibility to know the real amount of bytes in flight. However
 * this value is useless, since the sender bases its guess on the received ACK.
 *
 * \see Tx
 * \see BytesInFlightTrace
 */
class TcpBytesInFlightTest : public TcpGeneralTest
{
public:
  TcpBytesInFlightTest (const std::string &desc, std::vector<uint32_t> &toDrop);

protected:
  virtual Ptr<ErrorModel> CreateReceiverErrorModel ();
  virtual void Rx (const Ptr<const Packet> p, const TcpHeader&h, SocketWho who);
  virtual void Tx (const Ptr<const Packet> p, const TcpHeader&h, SocketWho who);
  virtual void BytesInFlightTrace (uint32_t oldValue, uint32_t newValue);

  void PktDropped (const Ipv4Header &ipH, const TcpHeader& tcpH, Ptr<const Packet> p);

  void FinalChecks ();

private:
  uint32_t m_realBytesInFlight;
  uint32_t m_guessedBytesInFlight;
  uint32_t m_dupAckRecv;
  SequenceNumber32 m_lastAckRecv;
  SequenceNumber32 m_greatestSeqSent;
  std::vector<uint32_t> m_toDrop;     // List of SequenceNumber to drop
};

TcpBytesInFlightTest::TcpBytesInFlightTest (const std::string &desc, std::vector<uint32_t> &toDrop)
  : TcpGeneralTest (desc, 500, 30, Seconds (0.01), Seconds (0.05), Seconds (2.0),
                    0xffffffff,1, 500),
    m_realBytesInFlight (0),
    m_guessedBytesInFlight (0),
    m_dupAckRecv (0),
    m_lastAckRecv (1),
    m_greatestSeqSent (0),
    m_toDrop (toDrop)
{
}

Ptr<ErrorModel>
TcpBytesInFlightTest::CreateReceiverErrorModel ()
{
  Ptr<TcpSeqErrorModel> m_errorModel = CreateObject<TcpSeqErrorModel> ();
  for (std::vector<uint32_t>::iterator it = m_toDrop.begin (); it != m_toDrop.end (); ++it)
    {
      m_errorModel->AddSeqToKill (SequenceNumber32 (*it));
    }

  m_errorModel->SetDropCallback (MakeCallback (&TcpBytesInFlightTest::PktDropped, this));

  return m_errorModel;
}

void
TcpBytesInFlightTest::PktDropped (const Ipv4Header &ipH, const TcpHeader &tcpH,
                                  Ptr<const Packet> p)
{
  NS_LOG_DEBUG ("Drop seq= " << tcpH.GetSequenceNumber () << " size " << p->GetSize ());

  // These bytes leave the world, they were not loved by anyone
  m_realBytesInFlight -= p->GetSize ();
}

void
TcpBytesInFlightTest::Rx (const Ptr<const Packet> p, const TcpHeader &h, SocketWho who)
{
  if (who == RECEIVER)
    {
      // Received has got data; bytes are not in flight anymore
      m_realBytesInFlight -= p->GetSize ();
    }
  else if (who == SENDER)
    {
      if (h.GetAckNumber () > m_lastAckRecv)
        { // New ack
          uint32_t diff = h.GetAckNumber () - m_lastAckRecv;
          NS_LOG_DEBUG ("Recv ACK=" << h.GetAckNumber ());

          if (m_dupAckRecv > 0)
            { // Previously we got some ACKs
              if (h.GetAckNumber () >= m_greatestSeqSent)
                { // This an ACK which acknowledge all the window
                  diff -= (m_dupAckRecv * GetSegSize (SENDER));

                  if (diff > m_guessedBytesInFlight)
                    {
                      // Our home-made guess is influenced also by retransmission
                      // so make sure that this does not overflow
                      diff = m_guessedBytesInFlight;
                    }

                  m_dupAckRecv = 0;
                }
              else
                {
                  // Partial ACK: Update the dupAck received count
                  m_dupAckRecv -= diff / GetSegSize (SENDER);
                }
            }

          if ((h.GetFlags () & TcpHeader::FIN) != 0
              || m_guessedBytesInFlight + 1 == diff)
            { // received the ACK for the FIN (which includes 1 spurious byte)
              diff -= 1;
            }
          m_guessedBytesInFlight -= diff;
          m_lastAckRecv = h.GetAckNumber ();
          NS_LOG_DEBUG ("Update m_guessedBytesInFlight to " <<
                        m_guessedBytesInFlight);
        }
      else if (h.GetAckNumber () == m_lastAckRecv
               && m_lastAckRecv != SequenceNumber32 (1)
               && (h.GetFlags () & TcpHeader::FIN) == 0)
        {
          // For each dupack I should guess that a segment has been received
          // Please do not count FIN and SYN/ACK as dupacks
          m_guessedBytesInFlight -= GetSegSize (SENDER);
          m_dupAckRecv++;
          NS_LOG_DEBUG ("Dupack received, Update m_guessedBytesInFlight to " <<
                        m_guessedBytesInFlight);
        }

    }
}

void
TcpBytesInFlightTest::Tx (const Ptr<const Packet> p, const TcpHeader &h, SocketWho who)
{
  if (who == SENDER)
    {
      m_realBytesInFlight += p->GetSize ();
      if (m_greatestSeqSent <= h.GetSequenceNumber ())
        { // This is not a retransmission
          m_guessedBytesInFlight += p->GetSize ();
          m_greatestSeqSent = h.GetSequenceNumber ();
        }

      // TODO: Maybe we need to account retransmission in another variable,
      // such as m_guessedRetransOut ?

      NS_LOG_DEBUG ("TX size=" << p->GetSize () << " seq=" << h.GetSequenceNumber () <<
                    " m_guessedBytesInFlight=" << m_guessedBytesInFlight);
    }
}

void
TcpBytesInFlightTest::BytesInFlightTrace (uint32_t oldValue, uint32_t newValue)
{
  NS_LOG_DEBUG ("Socket BytesInFlight=" << newValue <<
                " mine is=" << m_guessedBytesInFlight);
  NS_TEST_ASSERT_MSG_EQ (m_guessedBytesInFlight, newValue,
                         "Guessed and measured bytes in flight differs");
}

void
TcpBytesInFlightTest::FinalChecks ()
{
  NS_TEST_ASSERT_MSG_EQ (m_guessedBytesInFlight, 0,
                         "Still present bytes in flight at the end of the transmission");
}

//-----------------------------------------------------------------------------

static class TcpBytesInFlightTestSuite : public TestSuite
{
public:
  TcpBytesInFlightTestSuite () : TestSuite ("tcp-bytes-in-flight-test", UNIT)
  {
    std::vector<uint32_t> toDrop;
    AddTestCase (new TcpBytesInFlightTest ("BytesInFlight value, no drop", toDrop),
                 TestCase::QUICK);
    toDrop.push_back (4001);
    AddTestCase (new TcpBytesInFlightTest ("BytesInFlight value, one drop", toDrop),
                 TestCase::QUICK);
    toDrop.push_back (4001);
    AddTestCase (new TcpBytesInFlightTest ("BytesInFlight value, two drop of same segment", toDrop),
                 TestCase::QUICK);
    toDrop.pop_back ();
    toDrop.push_back (4501);
    AddTestCase (new TcpBytesInFlightTest ("BytesInFlight value, two drop of consecutive segments", toDrop),
                 TestCase::QUICK);
  }
} g_tcpBytesInFlightTestSuite;

} // namespace ns3
