/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#include <iostream>
#include <cassert>
#include <math.h>

#include "ns3/random-variable.h"
#include "ns3/simulator.h"

#include "dcf.h"
#include "mac-parameters.h"



#define noDCF_TRACE 1

#ifdef DCF_TRACE
#  include <iostream>
#  define TRACE(x) \
   std::cout << "DCF " << x << std::endl;
#else /* DCF_TRACE */
# define TRACE(x)
#endif /* DCF_TRACE */

namespace ns3 {


DcfAccessListener::DcfAccessListener ()
{}
DcfAccessListener::~DcfAccessListener ()
{}



Dcf::Dcf ()
  : m_accessTimerEvent (),
    m_backoffStart (MicroSeconds (0)),
    m_backoffLeft (MicroSeconds (0)),
    m_lastNavStart (MicroSeconds (0)),
    m_lastNavDuration (MicroSeconds (0)),
    m_lastRxStart (MicroSeconds (0)),
    m_lastRxDuration (MicroSeconds (0)),
    m_lastRxReceivedOk (true),
    m_lastRxEnd (MicroSeconds (0)),
    m_lastTxStart (MicroSeconds (0)),
    m_lastTxDuration (MicroSeconds (0)),
    m_lastBusyStart (MicroSeconds (0)),
    m_lastBusyDuration (MicroSeconds (0)),
    m_rxing (false)
{
  ResetCw ();
  m_random = new UniformVariable ();
}

Dcf::~Dcf ()
{
  delete m_random;
}

void 
Dcf::ResetRng (uint32_t seed)
{
  // XXX
  // m_random->Reset (seed);
}
void
Dcf::SetParameters (MacParameters const*parameters)
{
  m_parameters = parameters;
}

void 
Dcf::SetDifs (Time difs)
{
  m_difs = difs;
}
void 
Dcf::SetEifs (Time eifs)
{
  m_eifs = eifs;
}
void 
Dcf::SetCwBounds (uint32_t min, uint32_t max)
{
  m_cwMin = min;
  m_cwMax = max;
  m_cw = min;
}
void 
Dcf::RegisterAccessListener (DcfAccessListener *listener)
{
  m_listener = listener;
}


/***************************************************************
 *     public API.
 ***************************************************************/ 

void 
Dcf::RequestAccess (void)
{
  Time delayUntilAccessGranted = GetDelayUntilAccessGranted (Now ());
  if (m_listener->AccessingAndWillNotify ()) 
    {
      /* don't do anything. We will start a backoff and maybe
       * a timer when the txop notifies us of the end-of-access.
       */
      TRACE ("accessing. will be notified.");
    } 
  else if (m_accessTimerEvent.IsRunning ()) 
    {
      /* we don't need to do anything because we have an access
       * timer which will expire soon.
       */
      TRACE ("access timer running. will be notified");
    } 
  else if (IsBackoffNotCompleted (Now ()) && m_accessTimerEvent.IsExpired ()) 
    {
      /* start timer for ongoing backoff.
       */
      TRACE ("request access X delayed for="<<delayUntilAccessGranted);
      m_accessTimerEvent = Simulator::Schedule (delayUntilAccessGranted, 
                                                &Dcf::AccessTimeout, this);
    } 
  else if (IsPhyBusy ()) 
    {
      /* someone else has accessed the medium.
       * generate a backoff, start timer.
       */
      StartBackoff ();
    } 
  else if (delayUntilAccessGranted.IsStrictlyPositive ()) 
    {
      /* medium is IDLE, we have no backoff running but we 
       * need to wait a bit before accessing the medium.
       */
      TRACE ("request access Y delayed for="<< delayUntilAccessGranted);
      assert (m_accessTimerEvent.IsExpired ());
      m_accessTimerEvent = Simulator::Schedule (delayUntilAccessGranted, 
                                                &Dcf::AccessTimeout, this);
    } 
  else 
    {
      /* we can access the medium now.
       */
      TRACE ("access granted immediatly");
      m_listener->AccessGrantedNow ();
    }
}

/***************************************************************
 *     Timeout method. Notifies when Access is Granted.
 ***************************************************************/ 


void 
Dcf::AccessTimeout ()
{
  UpdateBackoff (Now ());
  if (m_backoffLeft.IsZero ()) 
    {
      TRACE ("timeout access granted");
      m_listener->AccessGrantedNow ();
    } 
  else 
    {
      Time delayUntilAccessGranted  = GetDelayUntilAccessGranted (Now ());
      TRACE ("timeout access delayed for "<< delayUntilAccessGranted);
      m_accessTimerEvent = Simulator::Schedule (delayUntilAccessGranted, 
                                                &Dcf::AccessTimeout, this);
    }
}


/***************************************************************
 *     Random trivial helper methods.
 ***************************************************************/ 

Time
Dcf::PickBackoffDelay (void)
{
  // XXX
  //uint32_t pickedCw = m_random->GetSingleValue (0, m_cw);
  uint32_t pickedCw = 0;
  TRACE ("cw="<<GetCwMin ()<<
         "<"<<m_cw<<"<"<<GetCwMax ()<<
         ", picked="<<pickedCw); 
  Time delay = Scalar (pickedCw) * m_parameters->GetSlotTime ();
  return delay;
}
void
Dcf::ResetCw (void)
{
  m_cw = GetCwMin ();
}
void
Dcf::UpdateFailedCw (void)
{
  uint32_t cw = m_cw;
  cw *= 2;
  if (cw > GetCwMax ()) 
    {
      cw = GetCwMax ();
    }
  m_cw = cw;
}

Time
Dcf::MostRecent (Time a, Time b) const
{
  return Max (a, b);
}
Time
Dcf::MostRecent (Time a, Time b, Time c) const
{
  Time retval;
  retval = Max (a, b);
  retval = Max (retval, c);
  return retval;
}
Time
Dcf::MostRecent (Time a, Time b, Time c, Time d) const
{
  Time e = Max (a, b);
  Time f = Max (c, d);
  Time retval = Max (e, f);
  return retval;
}

Time
Dcf::GetDifs (void) const
{
  return m_difs;
}
Time
Dcf::GetEifs (void) const
{
  return m_eifs;
}
uint32_t 
Dcf::GetCwMin (void) const
{
  return m_cwMin;
}
uint32_t 
Dcf::GetCwMax (void) const
{
  return m_cwMax;
}

/***************************************************************
 *     Complicated timekeeping backoff methods.
 ***************************************************************/ 

bool 
Dcf::IsPhyBusy (void)
{
  if (m_rxing) 
    {
      return true;
    }
  Time lastTxEnd = m_lastTxStart + m_lastTxDuration;
  if (lastTxEnd > Simulator::Now ()) 
    {
      return true;
    }
  return false;
}

void
Dcf::StartBackoff (void)
{
  Time backoffStart = Now ();
  Time backoffDuration = PickBackoffDelay ();
  m_backoffTrace (backoffDuration);
  assert (m_backoffStart <= backoffStart);
  m_backoffStart = backoffStart;
  m_backoffLeft = backoffDuration;
  if (m_listener->AccessNeeded () && m_accessTimerEvent.IsExpired ()) 
    {
      Time delayUntilAccessGranted  = GetDelayUntilAccessGranted (Now ());
      if (delayUntilAccessGranted.IsStrictlyPositive ()) 
        {
          TRACE ("start at "<<backoffStart<<", for "<<backoffDuration);
          m_accessTimerEvent = Simulator::Schedule (delayUntilAccessGranted,
                                                    &Dcf::AccessTimeout, this);
        } 
      else 
        {
          TRACE ("access granted now");
          m_listener->AccessGrantedNow ();
        }
    } 
  else 
    {
      if (m_accessTimerEvent.IsRunning ()) 
        {
          TRACE ("no access needed because timer running.");
        } 
      if (!m_listener->AccessNeeded ()) 
        {
          TRACE ("no access needed.");
        }
      TRACE ("no access needed for now.");
    }
}
Time
Dcf::GetAccessGrantedStart (void) const
{
  /* This method evaluates the time where access to the
   * medium is allowed. The return value could be 
   * somewhere in the past or in the future.
   */
  Time rxAccessStart;
  if (m_lastRxEnd >= m_lastRxStart) 
    {
      if (m_lastRxReceivedOk) 
        {
          rxAccessStart = m_lastRxEnd + GetDifs ();
        } 
      else
        {
          rxAccessStart = m_lastRxEnd + GetEifs ();
        }
    } 
  else 
    {
      rxAccessStart = m_lastRxStart + m_lastRxDuration + GetDifs ();
    }
  Time busyAccessStart = m_lastBusyStart + m_lastBusyDuration + GetDifs ();
  Time txAccessStart = m_lastTxStart + m_lastTxDuration + GetDifs ();
  Time navAccessStart = m_lastNavStart + m_lastNavDuration + GetDifs ();
  Time accessGrantedStart = MostRecent (rxAccessStart, 
                                        busyAccessStart,
                                        txAccessStart, 
                                        navAccessStart);
  return accessGrantedStart;
}

bool
Dcf::IsBackoffNotCompleted (Time now)
{
  UpdateBackoff (now);
  if (m_backoffLeft.IsStrictlyPositive ()) 
    {
      return true;
    } 
  else 
    {
      return false;
    }
}


Time
Dcf::GetDelayUntilAccessGranted (Time now)
{
  Time deltaTo = GetAccessGrantedStart () - now;
  Time retval = Max (deltaTo, Seconds (0));
  UpdateBackoff (now);
  retval += m_backoffLeft;
  return retval;
}
void
Dcf::UpdateBackoff (Time time)
{
  if (m_backoffLeft.IsZero ()) 
    {
      return;
    }
  
  //TRACE ("time: %f, backoffstart: %f\n", time, m_backoffStart);
  assert (time >= m_backoffStart);

  Time mostRecentEvent = MostRecent (m_backoffStart,
                                     GetAccessGrantedStart ());
  if (mostRecentEvent < time) 
    {
      Time newBackoffLeft = m_backoffLeft - (time - mostRecentEvent);
      m_backoffLeft = Max (newBackoffLeft, Seconds (0)); 
      TRACE ("at="<<time<<", left="<< m_backoffLeft);
      m_backoffStart = time;
    }
}

/***************************************************************
 *     Notification methods.
 ***************************************************************/ 
void
Dcf::NotifyNavReset (Time navStart, Time duration)
{
  m_lastNavStart = navStart;
  m_lastNavDuration = duration;
  Time navEnd = navStart + duration;
  Time newDelayUntilAccessGranted = GetDelayUntilAccessGranted (navEnd);
  assert (newDelayUntilAccessGranted.IsStrictlyPositive ());
  /* This is quite unfortunate but we need to cancel the access timer
   * because this nav reset might have brought the time of
   * possible access closer to us than expected.
   */
  if (m_accessTimerEvent.IsRunning ()) 
    {
      m_accessTimerEvent.Cancel ();
      m_accessTimerEvent = Simulator::Schedule (newDelayUntilAccessGranted,
                                                &Dcf::AccessTimeout, this);
    }
}
void
Dcf::NotifyNavStart (Time navStart, Time duration)
{
  assert (m_lastNavStart < navStart);
  TRACE ("nav start at="<<navStart<<", for="<<duration);
  UpdateBackoff (navStart);
  m_lastNavStart = navStart;
  m_lastNavDuration = duration;
}
void
Dcf::NotifyNavContinue (Time navStart, Time duration)
{
  NotifyNavStart (navStart, duration);
}

void 
Dcf::NotifyRxStartNow (Time duration)
{
  Time now = Now ();
  TRACE ("rx start at="<<now<<", for="<<duration);
  UpdateBackoff (now);
  m_lastRxStart = now;
  m_lastRxDuration = duration;
  m_rxing = true;
}
void 
Dcf::NotifyRxEndOkNow (void)
{
  Time now = Now ();
  TRACE ("rx end ok at="<<now);
  m_lastRxEnd = now;
  m_lastRxReceivedOk = true;
  m_rxing = false;
}
void 
Dcf::NotifyRxEndErrorNow (void)
{
  Time now = Now ();
  TRACE ("rx end error at=");
  m_lastRxEnd = now;
  m_lastRxReceivedOk = false;
  m_rxing = false;
}
void 
Dcf::NotifyTxStartNow (Time duration)
{
  Time now = Now ();
  TRACE ("tx start at="<<now<<" for "<<duration);
  UpdateBackoff (now);
  m_lastTxStart = now;
  m_lastTxDuration = duration;
}

void 
Dcf::NotifyCcaBusyStartNow (Time duration)
{
  Time now = Now ();
  TRACE ("busy start at="<<now<<" for "<<duration);
  UpdateBackoff (now);
  m_lastBusyStart = now;
  m_lastBusyDuration = duration;
}

} // namespace ns3

#ifdef RUN_SELF_TESTS
#include "ns3/test.h"
#include <list>

namespace ns3 {

class DcfTest : public Test {
public:
  DcfTest ();
  virtual bool RunTests (void);

  // callback from DcfListener
  void AccessGrantedNow (void);
  bool AccessNeeded (void);
  bool AccessingAndWillNotify (void);
private:

  void AddRxOkEvt (uint64_t at, uint64_t duration);
  void AddRxErrorEvt (uint64_t at, uint64_t duration);
  void AddTxEvt (uint64_t at, uint64_t duration);
  void AddNavReset (uint64_t at, uint64_t start, uint64_t duration);
  void AddNavStart (uint64_t at, uint64_t start, uint64_t duration);
  void AddNavContinue (uint64_t at, uint64_t start, uint64_t duration);
  void AddAccessRequest (uint64_t time);
  void AddAccessError (uint64_t time);
  void AddAccessErrorButOk (uint64_t time);
  void AddAccessOk (uint64_t time);

  void ExpectAccessGranted (uint64_t time);
  
  // callback to forward to DCF
  void AccessError (uint64_t time);
  void AccessErrorButOk (uint64_t time);
  void AccessOk (uint64_t time);

  void StartTest (void);
  void EndTest (void);

  Dcf *m_dcf;
  MacParameters *m_parameters;
  class TestAccessListener *m_listener;
  std::list<uint64_t> m_accessGrantedExpected;
  bool m_failed;
};

class TestAccessListener : public DcfAccessListener {
public:
  TestAccessListener (DcfTest *test)
    : m_test (test) {}
  virtual ~TestAccessListener () {}
  virtual void AccessGrantedNow (void) {
    m_test->AccessGrantedNow ();
  }
  virtual bool AccessNeeded (void) {
    return m_test->AccessNeeded ();
  }
  virtual bool AccessingAndWillNotify (void) {
    return m_test->AccessingAndWillNotify ();
  }
private:
  DcfTest *m_test;
};



DcfTest::DcfTest ()
  : Test ("Dcf") {}

void 
DcfTest::AccessGrantedNow (void)
{
  if (m_accessGrantedExpected.empty ()) 
    {
      Failure () << "DCF "
                 << "Failure: unexpected access granted at="<<Simulator::Now ()
                 << std::endl;
      m_failed = true;
      return;
    }
  uint64_t expected = m_accessGrantedExpected.front ();
  uint64_t actual = Simulator::Now ().GetMicroSeconds ();
  if (expected != actual) 
    {
      Failure () << "DCF "
                 << "Failure: access granted at=" << Simulator::Now ()
                 << ", expected at="<<expected<<"us"
                 << std::endl;
      m_failed = true;
      return;
    }
  m_accessGrantedExpected.erase (m_accessGrantedExpected.begin ());
}
bool 
DcfTest::AccessNeeded (void)
{
  return true;
}
bool 
DcfTest::AccessingAndWillNotify (void)
{
  return false;
}

void 
DcfTest::AddRxOkEvt (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (), 
                       &Dcf::NotifyRxStartNow, m_dcf, 
                       MicroSeconds (duration));
  Simulator::Schedule (MicroSeconds (at+duration) - Now (), 
                       &Dcf::NotifyRxEndOkNow, m_dcf);
}
void 
DcfTest::AddRxErrorEvt (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (), 
                       &Dcf::NotifyRxStartNow, m_dcf, 
                       MicroSeconds (duration));
  Simulator::Schedule (MicroSeconds (at+duration) - Now (), 
                       &Dcf::NotifyRxEndErrorNow, m_dcf);
}
void 
DcfTest::AddTxEvt (uint64_t at, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (), 
                       &Dcf::NotifyTxStartNow, m_dcf, 
                       MicroSeconds (duration));
}
void 
DcfTest::AddNavReset (uint64_t at, uint64_t start, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (), 
                       &Dcf::NotifyNavReset, m_dcf, 
                       MicroSeconds (start), 
                       MicroSeconds (duration));
}
void 
DcfTest::AddNavStart (uint64_t at, uint64_t start, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (), 
                       &Dcf::NotifyNavStart, m_dcf, 
                       MicroSeconds (start), MicroSeconds (duration));
}
void 
DcfTest::AddNavContinue (uint64_t at, uint64_t start, uint64_t duration)
{
  Simulator::Schedule (MicroSeconds (at) - Now (), 
                       &Dcf::NotifyNavContinue, m_dcf, 
                       MicroSeconds (start), 
                       MicroSeconds (duration));
}
void 
DcfTest::AddAccessRequest (uint64_t time)
{
  Simulator::Schedule (MicroSeconds (time) - Now (), 
                       &Dcf::RequestAccess, m_dcf);
}
void 
DcfTest::AddAccessError (uint64_t time)
{
  Simulator::Schedule (MicroSeconds (time) - Now (), 
                       &DcfTest::AccessError, this, 
                       time);
}
void 
DcfTest::AddAccessErrorButOk (uint64_t time)
{
  Simulator::Schedule (MicroSeconds (time) - Now (), 
                       &DcfTest::AccessErrorButOk, this, 
                       time);
}
void 
DcfTest::AddAccessOk (uint64_t time)
{
  Simulator::Schedule (MicroSeconds (time) - Now (), 
                       &DcfTest::AccessOk, this, 
                       time);
}

void 
DcfTest::AccessError (uint64_t time)
{
  m_dcf->UpdateFailedCw ();
  m_dcf->StartBackoff ();
}
void 
DcfTest::AccessErrorButOk (uint64_t time)
{
  m_dcf->ResetCw ();
  m_dcf->StartBackoff ();
}
void 
DcfTest::AccessOk (uint64_t time)
{
  m_dcf->ResetCw ();
  m_dcf->StartBackoff ();
}

void 
DcfTest::ExpectAccessGranted (uint64_t time)
{
  m_accessGrantedExpected.push_back (time);
}

void 
DcfTest::StartTest (void)
{
  m_dcf = new Dcf ();
  m_dcf->ResetRng (0);
  m_parameters = new MacParameters ();
  m_listener = new TestAccessListener (this);
  m_dcf->SetParameters (m_parameters);
  m_dcf->RegisterAccessListener (m_listener);  

  m_parameters->SetSlotTime (MicroSeconds (1));
  m_dcf->SetDifs (MicroSeconds (3));
  m_dcf->SetEifs (MicroSeconds (4));
  m_dcf->SetCwBounds (8, 64);
}
void 
DcfTest::EndTest (void)
{
  if (!m_accessGrantedExpected.empty ()) 
    {
      Failure () << "DCF: access not granted as expected"
                 << std::endl;
    }
  m_accessGrantedExpected.erase (m_accessGrantedExpected.begin (),
                                 m_accessGrantedExpected.end ());
  Simulator::Destroy ();
  delete m_dcf;
  delete m_parameters;
  delete m_listener;
}

bool
DcfTest::RunTests (void)
{
  m_failed = false;

  StartTest ();
  AddRxOkEvt (10, 20);
  AddNavStart (30, 30, 2+8);
  AddRxOkEvt (32, 5);
  AddAccessRequest (15);
  AddAccessRequest (16);
  AddAccessRequest (20);
  ExpectAccessGranted (51);
  Simulator::Run ();
  EndTest ();

  StartTest ();
  AddRxOkEvt (10, 20);
  AddNavStart (30, 30, 2+5);
  AddRxOkEvt (32, 7);
  AddAccessRequest (15);
  AddAccessRequest (16);
  AddAccessRequest (20);
  ExpectAccessGranted (50);
  Simulator::Run ();
  EndTest ();

  StartTest ();
  AddAccessRequest (10);
  ExpectAccessGranted (10);
  Simulator::Run ();
  EndTest ();

  StartTest ();
  AddRxOkEvt (10, 20);
  AddNavStart (30, 30, 2+8);
  AddRxOkEvt (32, 7);
  AddAccessRequest (40);
  ExpectAccessGranted (43);
  Simulator::Run ();
  EndTest ();

  StartTest ();
  AddRxOkEvt (10, 20);
  AddNavStart (30, 30, 2+8);
  AddRxOkEvt (32, 7);
  AddAccessRequest (41);
  ExpectAccessGranted (43);
  Simulator::Run ();
  EndTest ();

  StartTest ();
  AddRxOkEvt (10, 20);
  AddNavStart (30, 30, 2+8);
  AddRxOkEvt (32, 7);
  AddAccessRequest (43);
  ExpectAccessGranted (43);
  Simulator::Run ();
  EndTest ();

  StartTest ();
  AddRxErrorEvt (10, 20);
  AddRxOkEvt (31, 7);
  AddAccessRequest (39);
  ExpectAccessGranted (41);
  Simulator::Run ();
  EndTest ();

  StartTest ();
  AddRxErrorEvt (10, 20);
  AddRxErrorEvt (31, 7);
  AddAccessRequest (39);
  ExpectAccessGranted (42);
  Simulator::Run ();
  EndTest ();


  StartTest ();
  AddRxOkEvt (10, 20);
  AddNavStart (30, 30, 200);
  AddRxOkEvt (35, 10);
  AddNavReset (45, 45, 0);
  AddAccessRequest (32);
  ExpectAccessGranted (48);
  Simulator::Run ();
  EndTest ();

  StartTest ();
  AddRxOkEvt (10, 20);
  AddNavStart (30, 30, 200);
  AddRxOkEvt (35, 10);
  AddNavReset (45, 45, 0);
  Simulator::Run ();
  EndTest ();

  StartTest ();
  AddRxOkEvt (10, 20);
  AddNavStart (30, 30, 200);
  AddRxOkEvt (35, 10);
  AddNavReset (45, 45, 0);
  AddAccessRequest (49);
  ExpectAccessGranted (49);
  Simulator::Run ();
  EndTest ();


  return !m_failed;
}

static DcfTest gDcfTest;

} // namespace ns3


#endif /* RUN_SELF_TESTS */
