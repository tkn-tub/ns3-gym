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
#include "ns3/dcf-state.h"
#include "ns3/dcf-manager.h"
#include "ns3/dca-txop.h"

using namespace ns3;

class DcfManagerTest;

/**
 * \ingroup wifi-test
 * \ingroup tests
 *
 * \brief Dcf State Test
 */
class DcfStateTest : public DcfState
{
public:
  /**
   * Constructor
   *
   * \param dca the DCA TXOP
   */
  DcfStateTest (Ptr<DcaTxop> dca);
  /**
   * Queue transmit function
   * \param txTime the transmit time
   * \param expectedGrantTime the expected grant time
   */
  void QueueTx (uint64_t txTime, uint64_t expectedGrantTime);


private:
  /// allow DcfManagerTest class access
  friend class DcfManagerTest;

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
};


/**
 * \ingroup wifi-test
 * \ingroup tests
 *
 * \brief Dca Txop Test
 */
class DcaTxopTest : public DcaTxop
{
public:
  /**
   * Constructor
   *
   * \param test the test DCF manager
   * \param i the DCF state
   */
  DcaTxopTest (DcfManagerTest *test, uint32_t i);


private:
  void NotifyAccessGranted (void);
  void NotifyInternalCollision (void);
  void NotifyCollision (void);
  void NotifyChannelSwitching (void);
  void NotifySleep (void);
  void NotifyWakeUp (void);
  void DoDispose (void);

  DcfManagerTest *m_test; //!< the test DCF manager
  uint32_t m_i; //!< the DCF state
};


/**
 * \ingroup wifi-test
 * \ingroup tests
 *
 * \brief Dcf Manager Test
 */
class DcfManagerTest : public TestCase
{
public:
  DcfManagerTest ();
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
   * \param from DoAccessRequest DcfStateTest
   */
  void AddAccessRequestWithAckTimeout (uint64_t at, uint64_t txTime,
                                       uint64_t expectedGrantTime, uint32_t from);
  /**
   * Add access request with successful ack
   * \param at time to schedule DoAccessRequest event
   * \param txTime DoAccessRequest txTime
   * \param expectedGrantTime DoAccessRequest expectedGrantTime
   * \param ackDelay is delay of the ack after txEnd
   * \param from DoAccessRequest DcfStateTest
   */
  void AddAccessRequestWithSuccessfullAck (uint64_t at, uint64_t txTime,
                                           uint64_t expectedGrantTime, uint32_t ackDelay, uint32_t from);
  /**
   * Add access request with successful ack
   * \param txTime DoAccessRequest txTime
   * \param expectedGrantTime DoAccessRequest expectedGrantTime
   * \param state DcfStateTest
   */
  void DoAccessRequest (uint64_t txTime, uint64_t expectedGrantTime, Ptr<DcfStateTest> state);
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

  typedef std::vector<Ptr<DcfStateTest> > DcfStates; //!< the DCF test states typedef
  typedef std::vector<Ptr<DcaTxopTest> > Dca; //!< the DCA TXOP tests typedef

  Ptr<DcfManager> m_dcfManager; //!< the DCF manager
  DcfStates m_dcfStates; //!< the DCF states
  Dca m_dca; //!< the DCA
  uint32_t m_ackTimeoutValue; //!< the ack timeout value
};

DcfStateTest::DcfStateTest (Ptr<DcaTxop> dca)
  : DcfState (dca)
{
}

void
DcfStateTest::QueueTx (uint64_t txTime, uint64_t expectedGrantTime)
{
  m_expectedGrants.push_back (std::make_pair (txTime, expectedGrantTime));
}

DcaTxopTest::DcaTxopTest (DcfManagerTest *test, uint32_t i)
  : m_test (test),
    m_i (i)
{
}

void
DcaTxopTest::DoDispose (void)
{
  m_test = 0;
  DcaTxop::DoDispose ();
}

void
DcaTxopTest::NotifyAccessGranted (void)
{
  m_test->NotifyAccessGranted (m_i);
}

void
DcaTxopTest::NotifyInternalCollision (void)
{
  m_test->NotifyInternalCollision (m_i);
}

void
DcaTxopTest::NotifyCollision (void)
{
  m_test->NotifyCollision (m_i);
}

void
DcaTxopTest::NotifyChannelSwitching (void)
{
  m_test->NotifyChannelSwitching (m_i);
}

void
DcaTxopTest::NotifySleep (void)
{
}

void
DcaTxopTest::NotifyWakeUp (void)
{
}

DcfManagerTest::DcfManagerTest ()
  : TestCase ("DcfManager")
{
}

void
DcfManagerTest::NotifyAccessGranted (uint32_t i)
{
  Ptr<DcfStateTest> state = m_dcfStates[i];
  NS_TEST_EXPECT_MSG_EQ (state->m_expectedGrants.empty (), false, "Have expected grants");
  if (!state->m_expectedGrants.empty ())
    {
      std::pair<uint64_t, uint64_t> expected = state->m_expectedGrants.front ();
      state->m_expectedGrants.pop_front ();
      NS_TEST_EXPECT_MSG_EQ (Simulator::Now (), MicroSeconds (expected.second), "Expected access grant is now");
      m_dcfManager->NotifyTxStartNow (MicroSeconds (expected.first));
      m_dcfManager->NotifyAckTimeoutStartNow (MicroSeconds (m_ackTimeoutValue + expected.first));
    }
}

void
DcfManagerTest::AddTxEvt (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &DcfManager::NotifyTxStartNow, m_dcfManager,
                       MicroSeconds (duration));
}

void
DcfManagerTest::NotifyInternalCollision (uint32_t i)
{
  Ptr<DcfStateTest> state = m_dcfStates[i];
  NS_TEST_EXPECT_MSG_EQ (state->m_expectedInternalCollision.empty (), false, "Have expected internal collisions");
  if (!state->m_expectedInternalCollision.empty ())
    {
      struct DcfStateTest::ExpectedCollision expected = state->m_expectedInternalCollision.front ();
      state->m_expectedInternalCollision.pop_front ();
      NS_TEST_EXPECT_MSG_EQ (Simulator::Now (), MicroSeconds (expected.at), "Expected internal collision time is now");
      state->StartBackoffNow (expected.nSlots);
    }
}

void
DcfManagerTest::NotifyCollision (uint32_t i)
{
  Ptr<DcfStateTest> state = m_dcfStates[i];
  NS_TEST_EXPECT_MSG_EQ (state->m_expectedCollision.empty (), false, "Have expected collisions");
  if (!state->m_expectedCollision.empty ())
    {
      struct DcfStateTest::ExpectedCollision expected = state->m_expectedCollision.front ();
      state->m_expectedCollision.pop_front ();
      NS_TEST_EXPECT_MSG_EQ (Simulator::Now (), MicroSeconds (expected.at), "Expected collision is now");
      state->StartBackoffNow (expected.nSlots);
    }
}

void
DcfManagerTest::NotifyChannelSwitching (uint32_t i)
{
  Ptr<DcfStateTest> state = m_dcfStates[i];
  if (!state->m_expectedGrants.empty ())
    {
      std::pair<uint64_t, uint64_t> expected = state->m_expectedGrants.front ();
      state->m_expectedGrants.pop_front ();
      NS_TEST_EXPECT_MSG_EQ (Simulator::Now (), MicroSeconds (expected.second), "Expected grant is now");
    }
}

void
DcfManagerTest::ExpectInternalCollision (uint64_t time, uint32_t nSlots, uint32_t from)
{
  Ptr<DcfStateTest> state = m_dcfStates[from];
  struct DcfStateTest::ExpectedCollision col;
  col.at = time;
  col.nSlots = nSlots;
  state->m_expectedInternalCollision.push_back (col);
}

void
DcfManagerTest::ExpectCollision (uint64_t time, uint32_t nSlots, uint32_t from)
{
  Ptr<DcfStateTest> state = m_dcfStates[from];
  struct DcfStateTest::ExpectedCollision col;
  col.at = time;
  col.nSlots = nSlots;
  state->m_expectedCollision.push_back (col);
}

void
DcfManagerTest::StartTest (uint64_t slotTime, uint64_t sifs, uint64_t eifsNoDifsNoSifs, uint32_t ackTimeoutValue)
{
  m_dcfManager = CreateObject<DcfManager> ();
  m_dcfManager->SetSlot (MicroSeconds (slotTime));
  m_dcfManager->SetSifs (MicroSeconds (sifs));
  m_dcfManager->SetEifsNoDifs (MicroSeconds (eifsNoDifsNoSifs + sifs));
  m_ackTimeoutValue = ackTimeoutValue;
}

void
DcfManagerTest::AddDcfState (uint32_t aifsn)
{
  Ptr<DcaTxopTest> dca = CreateObject<DcaTxopTest> (this, m_dcfStates.size ());
  m_dca.push_back (dca);
  Ptr<DcfStateTest> state = CreateObject<DcfStateTest> (dca);
  state->SetAifsn (aifsn);
  m_dcfStates.push_back (state);
  m_dcfManager->Add (state);
}

void
DcfManagerTest::EndTest (void)
{
  Simulator::Run ();
  Simulator::Destroy ();

  for (DcfStates::const_iterator i = m_dcfStates.begin (); i != m_dcfStates.end (); i++)
    {
      Ptr<DcfStateTest> state = *i;
      NS_TEST_EXPECT_MSG_EQ (state->m_expectedGrants.empty (), true, "Have no expected grants");
      NS_TEST_EXPECT_MSG_EQ (state->m_expectedInternalCollision.empty (), true, "Have no internal collisions");
      NS_TEST_EXPECT_MSG_EQ (state->m_expectedCollision.empty (), true, "Have no expected collisions");
      state = 0;
    }
  m_dcfStates.clear ();

  for (Dca::const_iterator i = m_dca.begin (); i != m_dca.end (); i++)
    {
      Ptr<DcaTxopTest> dca = *i;
      dca->Dispose ();
      dca = 0;
    }
  m_dca.clear ();

  m_dcfManager = 0;
}

void
DcfManagerTest::AddRxOkEvt (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &DcfManager::NotifyRxStartNow, m_dcfManager,
                       MicroSeconds (duration));
  Simulator::Schedule (MicroSeconds (at + duration) - Now (),
                       &DcfManager::NotifyRxEndOkNow, m_dcfManager);
}

void
DcfManagerTest::AddRxInsideSifsEvt (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &DcfManager::NotifyRxStartNow, m_dcfManager,
                       MicroSeconds (duration));
}

void
DcfManagerTest::AddRxErrorEvt (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &DcfManager::NotifyRxStartNow, m_dcfManager,
                       MicroSeconds (duration));
  Simulator::Schedule (MicroSeconds (at + duration) - Now (),
                       &DcfManager::NotifyRxEndErrorNow, m_dcfManager);
}

void
DcfManagerTest::AddNavReset (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &DcfManager::NotifyNavResetNow, m_dcfManager,
                       MicroSeconds (duration));
}

void
DcfManagerTest::AddNavStart (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &DcfManager::NotifyNavStartNow, m_dcfManager,
                       MicroSeconds (duration));
}

void
DcfManagerTest::AddAckTimeoutReset (uint64_t at)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &DcfManager::NotifyAckTimeoutResetNow, m_dcfManager);
}

void
DcfManagerTest::AddAccessRequest (uint64_t at, uint64_t txTime,
                                  uint64_t expectedGrantTime, uint32_t from)
{
  AddAccessRequestWithSuccessfullAck (at, txTime, expectedGrantTime, 0, from);
}

void
DcfManagerTest::AddAccessRequestWithAckTimeout (uint64_t at, uint64_t txTime,
                                                uint64_t expectedGrantTime, uint32_t from)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &DcfManagerTest::DoAccessRequest, this,
                       txTime, expectedGrantTime, m_dcfStates[from]);
}

void
DcfManagerTest::AddAccessRequestWithSuccessfullAck (uint64_t at, uint64_t txTime,
                                                    uint64_t expectedGrantTime, uint32_t ackDelay, uint32_t from)
{
  NS_ASSERT (ackDelay < m_ackTimeoutValue);
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &DcfManagerTest::DoAccessRequest, this,
                       txTime, expectedGrantTime, m_dcfStates[from]);
  AddAckTimeoutReset (expectedGrantTime + txTime + ackDelay);
}

void
DcfManagerTest::DoAccessRequest (uint64_t txTime, uint64_t expectedGrantTime, Ptr<DcfStateTest> state)
{
  state->QueueTx (txTime, expectedGrantTime);
  m_dcfManager->RequestAccess (state);
}

void
DcfManagerTest::AddCcaBusyEvt (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &DcfManager::NotifyMaybeCcaBusyStartNow, m_dcfManager,
                       MicroSeconds (duration));
}

void
DcfManagerTest::AddSwitchingEvt (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &DcfManager::NotifySwitchingStartNow, m_dcfManager,
                       MicroSeconds (duration));
}

void
DcfManagerTest::AddRxStartEvt (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (),
                       &DcfManager::NotifyRxStartNow, m_dcfManager,
                       MicroSeconds (duration));
}

void
DcfManagerTest::DoRun (void)
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

  // Test an EIFS which is interupted by a successfull transmission.
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
  AddTestCase (new DcfManagerTest, TestCase::QUICK);
}

static DcfTestSuite g_dcfTestSuite;
