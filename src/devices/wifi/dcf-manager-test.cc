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
  void StartTest (uint64_t slotTime, uint64_t ackTxDuration);
  void AddDcfState (uint32_t cwMin, uint32_t cwMax, uint32_t aifsn);
  void EndTest (void);
  void ExpectAccessGranted (uint64_t time, uint32_t from);
  void ExpectInternalCollision (uint64_t time, uint32_t from);
  void ExpectCollision (uint64_t time, uint32_t from);
  void AddRxOkEvt (uint64_t at, uint64_t duration);
  void AddRxErrorEvt (uint64_t at, uint64_t duration);
  void AddTxEvt (uint64_t at, uint64_t duration);
  void AddNavReset (uint64_t at, uint64_t duration);
  void AddNavStart (uint64_t at, uint64_t duration);
  void AddAccessRequest (uint64_t time, uint32_t from);
  
  typedef std::vector<DcfStateTest *> DcfStates;
  typedef std::list<std::pair<uint64_t, uint32_t> > ExpectedEvent;

  DcfManager *m_dcfManager;
  MacParameters *m_parameters;
  DcfStates m_dcfStates;
  ExpectedEvent m_expectedAccessGranted;
  ExpectedEvent m_expectedInternalCollision;
  ExpectedEvent m_expectedCollision;
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
  UpdateFailedCw ();
  StartBackoffNow (0);
  m_test->NotifyInternalCollision (m_i);
}
void 
DcfStateTest::NotifyCollision (void)
{
  UpdateFailedCw ();
  StartBackoffNow (0);
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
  NS_TEST_ASSERT_EQUAL (MicroSeconds (expected.first), Simulator::Now ());
  NS_TEST_ASSERT_EQUAL (expected.second, i);
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
  std::pair<uint64_t, uint32_t> expected = m_expectedInternalCollision.front ();
  m_expectedInternalCollision.pop_front ();
  NS_TEST_ASSERT_EQUAL (MicroSeconds (expected.first), Simulator::Now ());
  NS_TEST_ASSERT_EQUAL (expected.second, i);
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
  std::pair<uint64_t, uint32_t> expected = m_expectedCollision.front ();
  m_expectedCollision.pop_front ();
  NS_TEST_ASSERT_EQUAL (MicroSeconds (expected.first), Simulator::Now ());
  NS_TEST_ASSERT_EQUAL (expected.second, i);
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
DcfManagerTest::ExpectInternalCollision (uint64_t time, uint32_t from)
{
  m_expectedInternalCollision.push_back (std::make_pair (time, from));
}
void 
DcfManagerTest::ExpectCollision (uint64_t time, uint32_t from)
{
  m_expectedCollision.push_back (std::make_pair (time, from));
}

void
DcfManagerTest::StartTest (uint64_t slotTime, uint64_t ackTxDuration)
{
  m_dcfManager = new DcfManager ();
  m_parameters = new MacParameters ();
  m_parameters->SetSlotTime (MicroSeconds (slotTime));
  m_dcfManager->SetParameters (m_parameters);
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
  NS_TEST_ASSERT (!m_expectedAccessGranted.empty ());
  NS_TEST_ASSERT (!m_expectedInternalCollision.empty ());
  NS_TEST_ASSERT (!m_expectedCollision.empty ());
  m_expectedAccessGranted.clear ();
  m_expectedInternalCollision.clear ();
  m_expectedCollision.clear ();
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

  StartTest (1 /* slot time */, 10 /* ack tx dur */);
  AddDcfState (8 /* cwmin */, 64 /* cwmax */, 1 /* aifsn */);
  AddAccessRequest (10 /* at */ , 0 /* from */);
  ExpectAccessGranted (10 /* at */, 0 /* from */);
  EndTest ();

  return m_result;
}



static DcfManagerTest g_dcf_manager_test;

} // namespace ns3

#endif /* RUN_SELF_TESTS */
