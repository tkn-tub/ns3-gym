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

#include "ns3/test.h"
#include "ns3/simulator.h"
#include "ns3/channel-access-manager.h"
#include "ns3/txop.h"
#include "ns3/mac-low.h"

using namespace ns3;

class ChannelAccessManagerTest;

/**
 * \ingroup wifi-test
 * \ingroup tests
 *
 * \brief TxopTest Txop Test
 */
class TxopTest : public Txop
{
public:
  /**
   * Constructor
   *
   * \param test the test DCF manager
   * \param i the DCF state
   */
  TxopTest (ChannelAccessManagerTest *test, uint32_t i);

  /**
   * Queue transmit function
   * \param txTime the transmit time
   * \param expectedGrantTime the expected grant time
   */
  void QueueTx (uint64_t txTime, uint64_t expectedGrantTime);

private:
  /// allow ChannelAccessManagerTest class access
  friend class ChannelAccessManagerTest;

  typedef std::pair<uint64_t,uint64_t> ExpectedGrant; //!< the expected grant typedef
  typedef std::list<ExpectedGrant> ExpectedGrants; //!< the collection of expected grants typedef
  /// ExpectedCollision structure
  struct ExpectedCollision
  {
    uint64_t at; //!< at
    uint32_t nSlots; //!< number of slots
  };
  typedef std::list<struct ExpectedCollision> ExpectedCollisions; //!< expected collisions typedef

  ExpectedCollisions m_expectedInternalCollision; //!< expected internal collisions
  ExpectedCollisions m_expectedCollision; //!< expected collision
  ExpectedGrants m_expectedGrants; //!< expected grants

  bool IsAccessRequested (void) const;
  void NotifyAccessRequested (void);
  void NotifyAccessGranted (void);
  void NotifyInternalCollision (void);
  void NotifyCollision (void);
  void NotifyChannelSwitching (void);
  void NotifySleep (void);
  void NotifyWakeUp (void);
  void DoDispose (void);

  ChannelAccessManagerTest *m_test; //!< the test DCF manager
  uint32_t m_i; //!< the DCF state
  bool m_accessRequested; //!< true if access requested
};

/**
 * \ingroup wifi-test
 * \ingroup tests
 *
 * \brief Mac Low Stub
 */
class MacLowStub : public MacLow
{
public:
  MacLowStub ()
  {
  }
  /**
   * This function indicates whether it is the CF period.
   */
  bool IsCfPeriod (void) const
  {
    return false;
  }
};

/**
 * \ingroup wifi-test
 * \ingroup tests
 *
 * \brief Dcf Manager Test
 */
class ChannelAccessManagerTest : public TestCase
{
public:
  ChannelAccessManagerTest ();
  virtual void DoRun (void);

  /**
   * Notify access granted function
   * \param i the DCF state
   */
  void NotifyAccessGranted (uint32_t i);
  /**
   * Notify internal collision function
   * \param i the DCF state
   */
  void NotifyInternalCollision (uint32_t i);
  /**
   * Notify collision function
   * \param i the DCF state
   */
  void NotifyCollision (uint32_t i);
  /**
   * Notify channel switching function
   * \param i the DCF state
   */
  void NotifyChannelSwitching (uint32_t i);


private:
  /**
   * Start test function
   * \param slotTime the slot time
   * \param sifs the SIFS
   * \param eifsNoDifsNoSifs the EIFS no DIFS no SIFS
   * \param ackTimeoutValue the ack timeout value
   */
  void StartTest (uint64_t slotTime, uint64_t sifs, uint64_t eifsNoDifsNoSifs, uint32_t ackTimeoutValue = 20);
  /**
   * Add DCF state function
   * \param aifsn the AIFSN
   */
  void AddDcfState (uint32_t aifsn);
  /// End test function
  void EndTest (void);
  /**
   * Expect internal collision function
   * \param time the expectedtime
   * \param nSlots the number of slots
   * \param from the expected from
   */
  void ExpectInternalCollision (uint64_t time, uint32_t nSlots, uint32_t from);
  /**
   * Expect internal collision function
   * \param time the expectedtime
   * \param nSlots the number of slots
   * \param from the expected from
   */
  void ExpectCollision (uint64_t time, uint32_t nSlots, uint32_t from);
  /**
   * Add expect collision function
   * \param at
   * \param duration the duration
   */
  void AddRxOkEvt (uint64_t at, uint64_t duration);
  /**
   * Add receive error event function
   * \param at the event time
   * \param duration the duration
   */
  void AddRxErrorEvt (uint64_t at, uint64_t duration);
  /**
   * Add receive inside SIFS event function
   * \param at the event time
   * \param duration the duration
   */
  void AddRxInsideSifsEvt (uint64_t at, uint64_t duration);
  /**
   * Add transmit event function
   * \param at the event time
   * \param duration the duration
   */
  void AddTxEvt (uint64_t at, uint64_t duration);
  /**
   * Add NAV reset function
   * \param at the event time
   * \param duration the duration
   */
  void AddNavReset (uint64_t at, uint64_t duration);
  /**
   * Add NAV start function
   * \param at the event time
   * \param duration the duration
   */
  void AddNavStart (uint64_t at, uint64_t duration);
  /**
   * Add ack timeout reset function
   * \param at the event time
   */
  void AddAckTimeoutReset (uint64_t at);
  /**
   * Add access function
   * \param at the event time
   * \param txTime the transmit time
   * \param expectedGrantTime the expected grant time
   * \param from
   */
  void AddAccessRequest (uint64_t at, uint64_t txTime,
                         uint64_t expectedGrantTime, uint32_t from);
  /**
   * Add access request with ack timeout
   * \param at time to schedule DoAccessRequest event
   * \param txTime DoAccessRequest txTime
   * \param expectedGrantTime DoAccessRequest expectedGrantTime
   * \param from DoAccessRequest TxopTest
   */
  void AddAccessRequestWithAckTimeout (uint64_t at, uint64_t txTime,
                                       uint64_t expectedGrantTime, uint32_t from);
  /**
   * Add access request with successful ack
   * \param at time to schedule DoAccessRequest event
   * \param txTime DoAccessRequest txTime
   * \param expectedGrantTime DoAccessRequest expectedGrantTime
   * \param ackDelay is delay of the ack after txEnd
   * \param from DoAccessRequest TxopTest
   */
  void AddAccessRequestWithSuccessfullAck (uint64_t at, uint64_t txTime,
                                           uint64_t expectedGrantTime, uint32_t ackDelay, uint32_t from);
  /**
   * Add access request with successful ack
   * \param txTime DoAccessRequest txTime
   * \param expectedGrantTime DoAccessRequest expectedGrantTime
   * \param state TxopTest
   */
  void DoAccessRequest (uint64_t txTime, uint64_t expectedGrantTime, Ptr<TxopTest> state);
  /**
   * Add CCA busy event function
   * \param at the event time
   * \param duration the duration
   */
  void AddCcaBusyEvt (uint64_t at, uint64_t duration);
  /**
   * Add switching event function
   * \param at the event time
   * \param duration the duration
   */
  void AddSwitchingEvt (uint64_t at, uint64_t duration);
  /**
   * Add receive start event function
   * \param at the event time
   * \param duration the duration
   */
  void AddRxStartEvt (uint64_t at, uint64_t duration);

  typedef std::vector<Ptr<TxopTest> > TxopTests; //!< the TXOP tests typedef

  Ptr<MacLowStub> m_low; //!< the MAC low stubbed
  Ptr<ChannelAccessManager> m_ChannelAccessManager; //!< the DCF manager
  TxopTests m_txop; //!< the TXOP
  uint32_t m_ackTimeoutValue; //!< the ack timeout value
};

void
TxopTest::QueueTx (uint64_t txTime, uint64_t expectedGrantTime)
{
  m_expectedGrants.push_back (std::make_pair (txTime, expectedGrantTime));
}

TxopTest::TxopTest (ChannelAccessManagerTest *test, uint32_t i)
  : m_test (test),
    m_i (i),
    m_accessRequested (false)
{
}

void
TxopTest::DoDispose (void)
{
  m_test = 0;
  Txop::DoDispose ();
}

bool
TxopTest::IsAccessRequested (void) const
{
  return m_accessRequested;
}

void
TxopTest::NotifyAccessRequested (void)
{
  m_accessRequested = true;
}

void
TxopTest::NotifyAccessGranted (void)
{
  m_accessRequested = false;
  m_test->NotifyAccessGranted (m_i);
}

void
TxopTest::NotifyInternalCollision (void)
{
  m_test->NotifyInternalCollision (m_i);
}

void
TxopTest::NotifyCollision (void)
{
  m_test->NotifyCollision (m_i);
}

void
TxopTest::NotifyChannelSwitching (void)
{
  m_test->NotifyChannelSwitching (m_i);
}

void
TxopTest::NotifySleep (void)
{
}

void
TxopTest::NotifyWakeUp (void)
{
}

ChannelAccessManagerTest::ChannelAccessManagerTest ()
  : TestCase ("ChannelAccessManager")
{
}

void
ChannelAccessManagerTest::NotifyAccessGranted (uint32_t i)
{
  Ptr<TxopTest> state = m_txop[i];
  NS_TEST_EXPECT_MSG_EQ (state->m_expectedGrants.empty (), false, "Have expected grants");
  if (!state->m_expectedGrants.empty ())
    {
      std::pair<uint64_t, uint64_t> expected = state->m_expectedGrants.front ();
      state->m_expectedGrants.pop_front ();
      NS_TEST_EXPECT_MSG_EQ (Simulator::Now (), MicroSeconds (expected.second), "Expected access grant is now");
      m_ChannelAccessManager->NotifyTxStartNow (MicroSeconds (expected.first));
      m_ChannelAccessManager->NotifyAckTimeoutStartNow (MicroSeconds (m_ackTimeoutValue + expected.first));
    }
}

void
ChannelAccessManagerTest::AddTxEvt (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &ChannelAccessManager::NotifyTxStartNow, m_ChannelAccessManager,
                       MicroSeconds (duration));
}

void
ChannelAccessManagerTest::NotifyInternalCollision (uint32_t i)
{
  Ptr<TxopTest> state = m_txop[i];
  NS_TEST_EXPECT_MSG_EQ (state->m_expectedInternalCollision.empty (), false, "Have expected internal collisions");
  if (!state->m_expectedInternalCollision.empty ())
    {
      struct TxopTest::ExpectedCollision expected = state->m_expectedInternalCollision.front ();
      state->m_expectedInternalCollision.pop_front ();
      NS_TEST_EXPECT_MSG_EQ (Simulator::Now (), MicroSeconds (expected.at), "Expected internal collision time is now");
      state->StartBackoffNow (expected.nSlots);
    }
}

void
ChannelAccessManagerTest::NotifyCollision (uint32_t i)
{
  Ptr<TxopTest> state = m_txop[i];
  NS_TEST_EXPECT_MSG_EQ (state->m_expectedCollision.empty (), false, "Have expected collisions");
  if (!state->m_expectedCollision.empty ())
    {
      struct TxopTest::ExpectedCollision expected = state->m_expectedCollision.front ();
      state->m_expectedCollision.pop_front ();
      NS_TEST_EXPECT_MSG_EQ (Simulator::Now (), MicroSeconds (expected.at), "Expected collision is now");
      state->StartBackoffNow (expected.nSlots);
    }
}

void
ChannelAccessManagerTest::NotifyChannelSwitching (uint32_t i)
{
  Ptr<TxopTest> state = m_txop[i];
  if (!state->m_expectedGrants.empty ())
    {
      std::pair<uint64_t, uint64_t> expected = state->m_expectedGrants.front ();
      state->m_expectedGrants.pop_front ();
      NS_TEST_EXPECT_MSG_EQ (Simulator::Now (), MicroSeconds (expected.second), "Expected grant is now");
    }
  state->m_accessRequested = false;
}

void
ChannelAccessManagerTest::ExpectInternalCollision (uint64_t time, uint32_t nSlots, uint32_t from)
{
  Ptr<TxopTest> state = m_txop[from];
  struct TxopTest::ExpectedCollision col;
  col.at = time;
  col.nSlots = nSlots;
  state->m_expectedInternalCollision.push_back (col);
}

void
ChannelAccessManagerTest::ExpectCollision (uint64_t time, uint32_t nSlots, uint32_t from)
{
  Ptr<TxopTest> state = m_txop[from];
  struct TxopTest::ExpectedCollision col;
  col.at = time;
  col.nSlots = nSlots;
  state->m_expectedCollision.push_back (col);
}

void
ChannelAccessManagerTest::StartTest (uint64_t slotTime, uint64_t sifs, uint64_t eifsNoDifsNoSifs, uint32_t ackTimeoutValue)
{
  m_ChannelAccessManager = CreateObject<ChannelAccessManager> ();
  m_low = CreateObject<MacLowStub> ();
  m_ChannelAccessManager->SetupLow (m_low);
  m_ChannelAccessManager->SetSlot (MicroSeconds (slotTime));
  m_ChannelAccessManager->SetSifs (MicroSeconds (sifs));
  m_ChannelAccessManager->SetEifsNoDifs (MicroSeconds (eifsNoDifsNoSifs + sifs));
  m_ackTimeoutValue = ackTimeoutValue;
}

void
ChannelAccessManagerTest::AddDcfState (uint32_t aifsn)
{
  Ptr<TxopTest> txop = CreateObject<TxopTest> (this, m_txop.size ());
  txop->SetAifsn (aifsn);
  m_txop.push_back (txop);
  m_ChannelAccessManager->Add (txop);
}

void
ChannelAccessManagerTest::EndTest (void)
{
  Simulator::Run ();
  Simulator::Destroy ();

  for (TxopTests::const_iterator i = m_txop.begin (); i != m_txop.end (); i++)
    {
      Ptr<TxopTest> state = *i;
      NS_TEST_EXPECT_MSG_EQ (state->m_expectedGrants.empty (), true, "Have no expected grants");
      NS_TEST_EXPECT_MSG_EQ (state->m_expectedInternalCollision.empty (), true, "Have no internal collisions");
      NS_TEST_EXPECT_MSG_EQ (state->m_expectedCollision.empty (), true, "Have no expected collisions");
      state = 0;
    }
  m_txop.clear ();

  for (TxopTests::const_iterator i = m_txop.begin (); i != m_txop.end (); i++)
    {
      Ptr<TxopTest> txop = *i;
      txop->Dispose ();
      txop = 0;
    }
  m_txop.clear ();

  m_ChannelAccessManager = 0;
  m_low = 0;
}

void
ChannelAccessManagerTest::AddRxOkEvt (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &ChannelAccessManager::NotifyRxStartNow, m_ChannelAccessManager,
                       MicroSeconds (duration));
  Simulator::Schedule (MicroSeconds (at + duration) - Now (),
                       &ChannelAccessManager::NotifyRxEndOkNow, m_ChannelAccessManager);
}

void
ChannelAccessManagerTest::AddRxInsideSifsEvt (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &ChannelAccessManager::NotifyRxStartNow, m_ChannelAccessManager,
                       MicroSeconds (duration));
}

void
ChannelAccessManagerTest::AddRxErrorEvt (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &ChannelAccessManager::NotifyRxStartNow, m_ChannelAccessManager,
                       MicroSeconds (duration));
  Simulator::Schedule (MicroSeconds (at + duration) - Now (),
                       &ChannelAccessManager::NotifyRxEndErrorNow, m_ChannelAccessManager);
}

void
ChannelAccessManagerTest::AddNavReset (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &ChannelAccessManager::NotifyNavResetNow, m_ChannelAccessManager,
                       MicroSeconds (duration));
}

void
ChannelAccessManagerTest::AddNavStart (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &ChannelAccessManager::NotifyNavStartNow, m_ChannelAccessManager,
                       MicroSeconds (duration));
}

void
ChannelAccessManagerTest::AddAckTimeoutReset (uint64_t at)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &ChannelAccessManager::NotifyAckTimeoutResetNow, m_ChannelAccessManager);
}

void
ChannelAccessManagerTest::AddAccessRequest (uint64_t at, uint64_t txTime,
                                            uint64_t expectedGrantTime, uint32_t from)
{
  AddAccessRequestWithSuccessfullAck (at, txTime, expectedGrantTime, 0, from);
}

void
ChannelAccessManagerTest::AddAccessRequestWithAckTimeout (uint64_t at, uint64_t txTime,
                                                          uint64_t expectedGrantTime, uint32_t from)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &ChannelAccessManagerTest::DoAccessRequest, this,
                       txTime, expectedGrantTime, m_txop[from]);
}

void
ChannelAccessManagerTest::AddAccessRequestWithSuccessfullAck (uint64_t at, uint64_t txTime,
                                                              uint64_t expectedGrantTime, uint32_t ackDelay, uint32_t from)
{
  NS_ASSERT (ackDelay < m_ackTimeoutValue);
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &ChannelAccessManagerTest::DoAccessRequest, this,
                       txTime, expectedGrantTime, m_txop[from]);
  AddAckTimeoutReset (expectedGrantTime + txTime + ackDelay);
}

void
ChannelAccessManagerTest::DoAccessRequest (uint64_t txTime, uint64_t expectedGrantTime, Ptr<TxopTest> state)
{
  state->QueueTx (txTime, expectedGrantTime);
  m_ChannelAccessManager->RequestAccess (state);
}

void
ChannelAccessManagerTest::AddCcaBusyEvt (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &ChannelAccessManager::NotifyMaybeCcaBusyStartNow, m_ChannelAccessManager,
                       MicroSeconds (duration));
}

void
ChannelAccessManagerTest::AddSwitchingEvt (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &ChannelAccessManager::NotifySwitchingStartNow, m_ChannelAccessManager,
                       MicroSeconds (duration));
}

void
ChannelAccessManagerTest::AddRxStartEvt (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &ChannelAccessManager::NotifyRxStartNow, m_ChannelAccessManager,
                       MicroSeconds (duration));
}

void
ChannelAccessManagerTest::DoRun (void)
{
  // Bug 2369 addresses this case
  //  0      3       4    5      8       9  10   12
  //  | sifs | aifsn | tx | sifs | aifsn |   | tx |
  //
  StartTest (1, 3, 10);
  AddDcfState (1);
  AddAccessRequest (1, 1, 4, 0);
  // Generate backoff when the request is within SIFS
  ExpectCollision (1, 0, 0); // 0 slots
  AddAccessRequest (10, 2, 10, 0);
  EndTest ();
  // Bug 2369 addresses this case
  //  0      3       5    6      9       11  12   13
  //  | sifs | aifsn | tx | sifs | aifsn |   | tx |
  //
  StartTest (1, 3, 10);
  AddDcfState (2);
  AddAccessRequest (4, 1, 5, 0);
  // Generate backoff when the request is within AIFSN
  ExpectCollision (4, 0, 0); // 0 slots
  AddAccessRequest (12, 2, 12, 0);
  EndTest ();
  // Check that receiving inside SIFS shall be cancelled properly:
  //  0      3       4    5      8     9     12       13 14
  //  | sifs | aifsn | tx | sifs | ack | sifs | aifsn |  |tx |
  //

  StartTest (1, 3, 10);
  AddDcfState (1);
  AddAccessRequest (1, 1, 4, 0);
  ExpectCollision (1, 0, 0);
  AddRxInsideSifsEvt (6, 10);
  AddTxEvt (8, 1);
  AddAccessRequest (14, 2, 14, 0);
  EndTest ();
  // The test below mainly intends to test the case where the medium
  // becomes busy in the middle of a backoff slot: the backoff counter
  // must not be decremented for this backoff slot. This is the case
  // below for the backoff slot starting at time 78us.
  //
  //  20          60     66      70        74        78  80    100     106      110      114      118   120
  //   |    rx     | sifs | aifsn | bslot0  | bslot1  |   | rx   | sifs  |  aifsn | bslot2 | bslot3 | tx  |
  //        |
  //       30 request access. backoff slots: 4

  StartTest (4, 6, 10);
  AddDcfState (1);
  AddRxOkEvt (20, 40);
  AddRxOkEvt (80, 20);
  AddAccessRequest (30, 2, 118, 0);
  ExpectCollision (30, 4, 0); //backoff: 4 slots
  EndTest ();
  // Test the case where the backoff slots is zero.
  //
  //  20          60     66      70   72
  //   |    rx     | sifs | aifsn | tx |
  //        |
  //       30 request access. backoff slots: 0

  StartTest (4, 6, 10);
  AddDcfState (1);
  AddRxOkEvt (20, 40);
  AddAccessRequest (30, 2, 70, 0);
  ExpectCollision (30, 0, 0); // backoff: 0 slots
  EndTest ();
  // Test shows when two frames are received without interval between
  // them:
  //  20          60         100   106     110  112
  //   |    rx     |    rx     |sifs | aifsn | tx |
  //        |
  //       30 request access. backoff slots: 0

  StartTest (4, 6, 10);
  AddDcfState (1);
  AddRxOkEvt (20, 40);
  AddRxOkEvt (60, 40);
  AddAccessRequest (30, 2, 110, 0);
  ExpectCollision (30, 0, 0); //backoff: 0 slots
  EndTest ();

  // Bug 2369.  Test case of requesting access within SIFS interval
  //
  //  20    60     66      70         74
  //   | rx  | sifs | aifsn | backoff | tx |
  //           |
  //          62 request access.
  //
  StartTest (4, 6, 10);
  AddDcfState (1);
  AddRxOkEvt (20, 40);
  AddAccessRequest (62, 2, 74, 0);
  ExpectCollision (62, 1, 0); //backoff: 1 slots
  EndTest ();

  // Bug 2369.  Test case of requesting access after DIFS (no backoff)
  //
  //  20    60     66      70
  //   | rx  | sifs | aifsn | tx |
  //                        |
  //                       70 request access.
  //
  StartTest (4, 6, 10);
  AddDcfState (1);
  AddRxOkEvt (20, 40);
  AddAccessRequest (70, 2, 70, 0);
  EndTest ();

  // Test an EIFS
  //
  //  20          60     66           76             86       90       94       98       102   106
  //   |    rx     | sifs | acktxttime | sifs + aifsn | bslot0 | bslot1 | bslot2 | bslot3 | tx |
  //        |      | <------eifs------>|
  //       30 request access. backoff slots: 4
  StartTest (4, 6, 10);
  AddDcfState (1);
  AddRxErrorEvt (20, 40);
  AddAccessRequest (30, 2, 102, 0);
  ExpectCollision (30, 4, 0); //backoff: 4 slots
  EndTest ();

  // Test an EIFS which is interrupted by a successful transmission.
  //
  //  20          60      66  69     75     81      85       89       93       97      101  103
  //   |    rx     | sifs  |   |  rx  | sifs | aifsn | bslot0 | bslot1 | bslot2 | bslot3 | tx |
  //        |      | <--eifs-->|
  //       30 request access. backoff slots: 4
  StartTest (4, 6, 10);
  AddDcfState (1);
  AddRxErrorEvt (20, 40);
  AddAccessRequest (30, 2, 101, 0);
  ExpectCollision (30, 4, 0); //backoff: 4 slots
  AddRxOkEvt (69, 6);
  EndTest ();

  // Test two DCFs which suffer an internal collision. the first DCF has a higher
  // priority than the second DCF.
  //
  //      20          60      66      70       74       78    88
  // DCF0  |    rx     | sifs  | aifsn | bslot0 | bslot1 | tx  |
  // DCF1  |    rx     | sifs  | aifsn | aifsn  | aifsn  |     | sifs | aifsn | aifsn | aifsn | bslot |  tx  |
  //                                                                 94      98     102     106     110    112
  StartTest (4, 6, 10);
  AddDcfState (1); //high priority DCF
  AddDcfState (3); //low priority DCF
  AddRxOkEvt (20, 40);
  AddAccessRequest (30, 10, 78, 0);
  ExpectCollision (30, 2, 0); //backoff: 2 slot
  AddAccessRequest (40, 2, 110, 1);
  ExpectCollision (40, 0, 1); //backoff: 0 slot
  ExpectInternalCollision (78, 1, 1); //backoff: 1 slot
  EndTest ();

  // Test of AckTimeout handling: First queue requests access and ack procedure fails,
  // inside the ack timeout second queue with higher priority requests access.
  //
  //            20           40      50     60  66      76
  // DCF0 - low  |     tx     | ack timeout |sifs|       |
  // DCF1 - high |                    |     |sifs|  tx   |
  //                                  ^ request access
  StartTest (4, 6, 10);
  AddDcfState (2); //high priority DCF
  AddDcfState (0); //low priority DCF
  AddAccessRequestWithAckTimeout (20, 20, 20, 0);
  AddAccessRequest (50, 10, 66, 1);
  ExpectCollision (50, 0, 1);
  EndTest ();

  // Test of AckTimeout handling:
  //
  // First queue requests access and ack is 2 us delayed (got ack interval at the picture),
  // inside this interval second queue with higher priority requests access.
  //
  //            20           40  41   42    48      58
  // DCF0 - low  |     tx     |got ack |sifs|       |
  // DCF1 - high |                |    |sifs|  tx   |
  //                              ^ request access
  StartTest (4, 6, 10);
  AddDcfState (2); //high priority DCF
  AddDcfState (0); //low priority DCF
  AddAccessRequestWithSuccessfullAck (20, 20, 20, 2, 0);
  AddAccessRequest (41, 10, 48, 1);
  ExpectCollision (41, 0, 1);
  EndTest ();

  //Repeat the same but with one queue:
  //            20           40  41   42    48      58
  // DCF0 - low  |     tx     |got ack |sifs|       |
  //                              ^ request access
  StartTest (4, 6, 10);
  AddDcfState (2);
  AddAccessRequestWithSuccessfullAck (20, 20, 20, 2, 0);
  AddAccessRequest (41, 10, 56, 0);
  ExpectCollision (41, 0, 0);
  EndTest ();

  // test simple NAV count. This scenario modelizes a simple DATA+ACK handshake
  // where the data rate used for the ACK is higher than expected by the DATA source
  // so, the data exchange completes before the end of nav.
  StartTest (4, 6, 10);
  AddDcfState (1);
  AddRxOkEvt (20, 40);
  AddNavStart (60, 15);
  AddRxOkEvt (66, 5);
  AddNavStart (71, 0);
  AddAccessRequest (30, 10, 93, 0);
  ExpectCollision (30, 2, 0); //backoff: 2 slot
  EndTest ();

  // test more complex NAV handling by a CF-poll. This scenario modelizes a
  // simple DATA+ACK handshake interrupted by a CF-poll which resets the
  // NAV counter.
  StartTest (4, 6, 10);
  AddDcfState (1);
  AddRxOkEvt (20, 40);
  AddNavStart (60, 15);
  AddRxOkEvt (66, 5);
  AddNavReset (71, 2);
  AddAccessRequest (30, 10, 91, 0);
  ExpectCollision (30, 2, 0); //backoff: 2 slot
  EndTest ();


  StartTest (4, 6, 10);
  AddDcfState (2);
  AddRxOkEvt (20, 40);
  AddAccessRequest (80, 10, 80, 0);
  EndTest ();


  StartTest (4, 6, 10);
  AddDcfState (2);
  AddRxOkEvt (20, 40);
  AddRxOkEvt (78, 8);
  AddAccessRequest (30, 50, 108, 0);
  ExpectCollision (30, 3, 0); //backoff: 3 slots
  EndTest ();


  // Channel switching tests

  //  0          20     23      24   25
  //  | switching | sifs | aifsn | tx |
  //                |
  //               21 access request.
  StartTest (1, 3, 10);
  AddDcfState (1);
  AddSwitchingEvt (0,20);
  AddAccessRequest (21, 1, 24, 0);
  ExpectCollision (21, 0, 0);
  EndTest ();

  //  20          40       50     53      54       55        56   57
  //   | switching |  busy  | sifs | aifsn | bslot0 | bslot 1 | tx |
  //         |          |
  //        30 busy.   45 access request.
  //
  StartTest (1, 3, 10);
  AddDcfState (1);
  AddSwitchingEvt (20,20);
  AddCcaBusyEvt (30,20);
  ExpectCollision (45, 2, 0); //backoff: 2 slots
  AddAccessRequest (45, 1, 56, 0);
  EndTest ();

  //  20     30          50     53      54   55
  //   |  rx  | switching | sifs | aifsn | tx |
  //                        |
  //                       51 access request.
  //
  StartTest (1, 3, 10);
  AddDcfState (1);
  AddRxStartEvt (20,40);
  AddSwitchingEvt (30,20);
  AddAccessRequest (51, 1, 54, 0);
  ExpectCollision (51, 0, 0);
  EndTest ();

  //  20     30          50     53      54   55
  //   | busy | switching | sifs | aifsn | tx |
  //                        |
  //                       51 access request.
  //
  StartTest (1, 3, 10);
  AddDcfState (1);
  AddCcaBusyEvt (20,40);
  AddSwitchingEvt (30,20);
  AddAccessRequest (51, 1, 54, 0);
  ExpectCollision (51, 0, 0);
  EndTest ();

  //  20      30          50     53      54   55
  //   |  nav  | switching | sifs | aifsn | tx |
  //                        |
  //                       51 access request.
  //
  StartTest (1, 3, 10);
  AddDcfState (1);
  AddNavStart (20,40);
  AddSwitchingEvt (30,20);
  AddAccessRequest (51, 1, 54, 0);
  ExpectCollision (51, 0, 0);
  EndTest ();

  //  20      40             50          55     58      59   60
  //   |  tx   | ack timeout  | switching | sifs | aifsn | tx |
  //                  |                     |
  //                 45 access request.    56 access request.
  //
  StartTest (1, 3, 10);
  AddDcfState (1);
  AddAccessRequestWithAckTimeout (20, 20, 20, 0);
  AddAccessRequest (45, 1, 50, 0);
  ExpectCollision (45, 0, 0);
  AddSwitchingEvt (50,5);
  AddAccessRequest (56, 1, 59, 0);
  ExpectCollision (56, 0, 0);
  EndTest ();

  //  20         60     66      70       74       78  80         100    106     110  112
  //   |    rx    | sifs | aifsn | bslot0 | bslot1 |   | switching | sifs | aifsn | tx |
  //        |                                                        |
  //       30 access request.                                      101 access request.
  //
  StartTest (4, 6, 10);
  AddDcfState (1);
  AddRxOkEvt (20,40);
  AddAccessRequest (30, 2, 80, 0);
  ExpectCollision (30, 4, 0); //backoff: 4 slots
  AddSwitchingEvt (80,20);
  AddAccessRequest (101, 2, 110, 0);
  ExpectCollision (101, 0, 0); //backoff: 0 slots
  EndTest ();
}


/**
 * \ingroup wifi-test
 * \ingroup tests
 *
 * \brief Dcf Test Suite
 */
class DcfTestSuite : public TestSuite
{
public:
  DcfTestSuite ();
};

DcfTestSuite::DcfTestSuite ()
  : TestSuite ("devices-wifi-dcf", UNIT)
{
  AddTestCase (new ChannelAccessManagerTest, TestCase::QUICK);
}

static DcfTestSuite g_dcfTestSuite;
