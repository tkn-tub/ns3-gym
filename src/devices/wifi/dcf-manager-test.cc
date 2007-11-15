/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
#ifdef RUN_SELF_TESTS

#include "ns3/test.h"
#include "ns3/simulator.h"
#include "dcf-manager.h"
#include "mac-parameters.h"

namespace ns3 {

class DcfManagerTest;

class DcfStateTest : public DcfState 
{
public:
  DcfStateTest (DcfManagerTest *test, uint32_t i);
private:
  virtual bool NeedsAccess (void) const;
  virtual void NotifyAccessGranted (void);
  virtual void NotifyInternalCollision (void);
  virtual void NotifyCollision (void);

  DcfManagerTest *m_test;
  uint32_t m_i;
};


class DcfManagerTest : public Test
{
public:
  DcfManagerTest ();
  virtual bool RunTests (void);


  void NotifyAccessGranted (uint32_t i);
  void NotifyInternalCollision (uint32_t i);
  void NotifyCollision (uint32_t i);


private:
  void StartTest (uint64_t slotTime, uint64_t sifs, uint64_t ackTxDuration);
  void AddDcfState (uint32_t cwMin, uint32_t cwMax, uint32_t aifsn);
  void EndTest (void);
  void ExpectAccessGranted (uint64_t time, uint32_t from);
  void ExpectInternalCollision (uint64_t time, uint32_t from, uint32_t nSlots);
  void ExpectCollision (uint64_t time, uint32_t from, uint32_t nSlots);
  void AddRxOkEvt (uint64_t at, uint64_t duration);
  void AddRxErrorEvt (uint64_t at, uint64_t duration);
  void AddTxEvt (uint64_t at, uint64_t duration);
  void AddNavReset (uint64_t at, uint64_t duration);
  void AddNavStart (uint64_t at, uint64_t duration);
  void AddAccessRequest (uint64_t time, uint32_t from);
  
  struct ExpectedCollision {
    uint64_t at;
    uint32_t from;
    uint32_t nSlots;
  };
  typedef std::vector<DcfStateTest *> DcfStates;
  typedef std::list<std::pair<uint64_t, uint32_t> > ExpectedAccess;
  typedef std::list<struct ExpectedCollision> ExpectedCollisions;

  DcfManager *m_dcfManager;
  MacParameters *m_parameters;
  DcfStates m_dcfStates;
  ExpectedAccess m_expectedAccessGranted;
  ExpectedCollisions m_expectedInternalCollision;
  ExpectedCollisions m_expectedCollision;
  bool m_result;
};



DcfStateTest::DcfStateTest (DcfManagerTest *test, uint32_t i) 
  : m_test (test), m_i(i)
{}
bool 
DcfStateTest::NeedsAccess (void) const
{
  return true;
}
void 
DcfStateTest::NotifyAccessGranted (void)
{
  m_test->NotifyAccessGranted (m_i);
}
void
DcfStateTest::NotifyInternalCollision (void)
{
  m_test->NotifyInternalCollision (m_i);
}
void 
DcfStateTest::NotifyCollision (void)
{
  m_test->NotifyCollision (m_i);
}



DcfManagerTest::DcfManagerTest ()
  : Test ("DcfManager")
{}

void 
DcfManagerTest::NotifyAccessGranted (uint32_t i)
{
  bool result = true;
  NS_TEST_ASSERT (!m_expectedAccessGranted.empty ());
  std::pair<uint64_t, uint32_t> expected = m_expectedAccessGranted.front ();
  m_expectedAccessGranted.pop_front ();
  NS_TEST_ASSERT_EQUAL (Simulator::Now (), MicroSeconds (expected.first));
  NS_TEST_ASSERT_EQUAL (i, expected.second);
  if (!result)
    {
      m_result = result;
    }
}
void 
DcfManagerTest::NotifyInternalCollision (uint32_t i)
{
  bool result = true;
  NS_TEST_ASSERT (!m_expectedInternalCollision.empty ());
  struct ExpectedCollision expected = m_expectedInternalCollision.front ();
  m_expectedInternalCollision.pop_front ();
  NS_TEST_ASSERT_EQUAL (Simulator::Now (), MicroSeconds (expected.at));
  NS_TEST_ASSERT_EQUAL (i, expected.from);
  uint32_t k = 0;
  for (DcfStates::const_iterator j = m_dcfStates.begin (); j != m_dcfStates.end (); j++, k++)
    {
      if (i == k)
        {
          DcfState *state = *j;
          state->StartBackoffNow (expected.nSlots);
        }
    }
  if (!result)
    {
      m_result = result;
    }
}
void 
DcfManagerTest::NotifyCollision (uint32_t i)
{
  bool result = true;
  NS_TEST_ASSERT (!m_expectedCollision.empty ());
  struct ExpectedCollision expected = m_expectedCollision.front ();
  m_expectedCollision.pop_front ();
  NS_TEST_ASSERT_EQUAL (Simulator::Now (), MicroSeconds (expected.at));
  NS_TEST_ASSERT_EQUAL (i, expected.from);
  DcfState *state = m_dcfStates[i];
  state->StartBackoffNow (expected.nSlots);
  if (!result)
    {
      m_result = result;
    }
}


void 
DcfManagerTest::ExpectAccessGranted (uint64_t time, uint32_t from)
{
  m_expectedAccessGranted.push_back (std::make_pair (time, from));
}
void 
DcfManagerTest::ExpectInternalCollision (uint64_t time, uint32_t nSlots, uint32_t from)
{
  struct ExpectedCollision col;
  col.at = time;
  col.from = from;
  col.nSlots = nSlots;
  m_expectedInternalCollision.push_back (col);
}
void 
DcfManagerTest::ExpectCollision (uint64_t time, uint32_t nSlots, uint32_t from)
{
  struct ExpectedCollision col;
  col.at = time;
  col.from = from;
  col.nSlots = nSlots;
  m_expectedCollision.push_back (col);
}

void
DcfManagerTest::StartTest (uint64_t slotTime, uint64_t sifs, uint64_t ackTxDuration)
{
  m_dcfManager = new DcfManager ();
  m_dcfManager->SetSlotTime (MicroSeconds (slotTime));
  m_dcfManager->SetSifs (MicroSeconds (sifs));
  m_dcfManager->SetAckTxDuration (MicroSeconds (ackTxDuration));
}

void
DcfManagerTest::AddDcfState (uint32_t cwMin, uint32_t cwMax, uint32_t aifsn)
{
  DcfStateTest *state = new DcfStateTest (this, m_dcfStates.size ());
  state->SetCwBounds (cwMin, cwMax);
  state->SetAifsn (aifsn);
  m_dcfStates.push_back (state);
  m_dcfManager->Add (state);
}

void
DcfManagerTest::EndTest (void)
{
  bool result = true;
  Simulator::Run ();
  NS_TEST_ASSERT (m_expectedAccessGranted.empty ());
  NS_TEST_ASSERT (m_expectedInternalCollision.empty ());
  NS_TEST_ASSERT (m_expectedCollision.empty ());
  Simulator::Destroy ();
  for (DcfStates::const_iterator i = m_dcfStates.begin (); i != m_dcfStates.end (); i++)
    {
      delete *i;
    }
  m_dcfStates.clear ();
  delete m_dcfManager;
  delete m_parameters;
  if (!result)
    {
      m_result = result;
    }
}

void 
DcfManagerTest::AddRxOkEvt (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (), 
                       &DcfManager::NotifyRxStartNow, m_dcfManager, 
                       MicroSeconds (duration));
  Simulator::Schedule (MicroSeconds (at+duration) - Now (), 
                       &DcfManager::NotifyRxEndOkNow, m_dcfManager);
}
void 
DcfManagerTest::AddRxErrorEvt (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (), 
                       &DcfManager::NotifyRxStartNow, m_dcfManager, 
                       MicroSeconds (duration));
  Simulator::Schedule (MicroSeconds (at+duration) - Now (), 
                       &DcfManager::NotifyRxEndErrorNow, m_dcfManager);
}
void 
DcfManagerTest::AddTxEvt (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (), 
                       &DcfManager::NotifyTxStartNow, m_dcfManager, 
                       MicroSeconds (duration));
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
DcfManagerTest::AddAccessRequest (uint64_t time, uint32_t from)
{
  Simulator::Schedule (MicroSeconds (time) - Now (), 
                       &DcfManager::RequestAccess, 
		       m_dcfManager, m_dcfStates[from]);
}




bool 
DcfManagerTest::RunTests (void)
{
  m_result = true;

  StartTest (1 /* slot time */, 3 /* sifs */, 10 /* ack tx dur */);
  AddDcfState (8 /* cwmin */, 64 /* cwmax */, 1 /* aifsn */);
  AddAccessRequest (1 /* at */ , 0 /* from */);
  ExpectAccessGranted (4, 0);
  AddAccessRequest (10, 0);
  ExpectAccessGranted (10, 0);
  EndTest ();

  // The test below mainly intends to test the case where the medium
  // becomes busy in the middle of a backoff slot: the backoff counter
  // must not be decremented for this backoff slot. This is the case
  // below for the backoff slot starting at time 78us.
  //
  //  20          60     66      70        74        78  80    100     106      110      114      118
  //   |    rx     | sifs | aifsn | bslot0  | bslot1  |   | rx   | sifs  |  aifsn | bslot2 | bslot3 |
  //        |
  //       30 request access. backoff slots: 4
  StartTest (4, 6 , 10);
  AddDcfState (8, 64, 1);
  AddRxOkEvt (20, 40);
  AddRxOkEvt (80, 20);
  AddAccessRequest (30, 0);
  ExpectCollision (30, 4, 0); // backoff: 4 slots
  ExpectAccessGranted (118, 0);
  EndTest ();

  // The test below is subject to some discussion because I am 
  // not sure I understand the intent of the spec here.
  // i.e., what happens if you make a request to get access
  // to the medium during the difs idle time after a busy period ?
  // do you need to start a backoff ? Or do you need to wait until
  // the end of difs and access the medium ?
  // Here, we wait until the end of difs and access the medium.
  //
  //  20    60     66      70
  //   | rx  | sifs | aifsn |
  //           |
  //          62 request access.
  //
  StartTest (4, 6 , 10);
  AddDcfState (8, 64, 1);
  AddRxOkEvt (20, 40);
  AddAccessRequest (62, 0);
  ExpectAccessGranted (70, 0);
  EndTest ();

  return m_result;
}



static DcfManagerTest g_dcf_manager_test;

} // namespace ns3

#endif /* RUN_SELF_TESTS */
