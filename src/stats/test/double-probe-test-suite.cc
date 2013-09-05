/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

// Include a header file from your module to test.
#include "ns3/double-probe.h"
#include "ns3/test.h"
#include "ns3/core-module.h"

using namespace ns3;

class SampleEmitter : public Object
{
public:
  static TypeId GetTypeId (void);
  SampleEmitter ()
  {
  }
  virtual ~SampleEmitter ()
  {
  }
  void Start ()
  {
    Reschedule ();
  }
  void Reschedule ()
  {
    m_time = m_var.GetValue ();
    Simulator::Schedule (Seconds (m_time), &SampleEmitter::Report, this);
    m_time += Simulator::Now ().GetSeconds ();
  }
  double GetTime ()
  {
    return m_time;
  }
  double GetValue ()
  {
    return aux;
  }
private:
  void Report ()
  {
    aux = m_var.GetValue ();
    m_trace = aux;
    Reschedule ();
  }
  ExponentialVariable m_var;
  double m_time;
  TracedValue<double> m_trace;
  double aux;
};

Ptr<SampleEmitter> s = CreateObject<SampleEmitter> ();

TypeId
SampleEmitter::GetTypeId (void)
{
  static TypeId tid = TypeId ("SampleEmitter")
    .SetParent<Object> ()
    .AddTraceSource ("Emitter", "XX", MakeTraceSourceAccessor (&SampleEmitter::m_trace))
  ;
  return tid;
}

class ProbeTestCase1 : public TestCase
{
public:
  ProbeTestCase1 ();
  virtual ~ProbeTestCase1 ();

private:
  virtual void DoRun (void);
  void TraceSink (std::string context, double oldValue, double newValue);
  uint32_t m_objectProbed;
  uint32_t m_pathProbed;
};

ProbeTestCase1::ProbeTestCase1 ()
  : TestCase ("basic probe test case"),
    m_objectProbed (0),
    m_pathProbed (0)
{
}

ProbeTestCase1::~ProbeTestCase1 ()
{
}

void
ProbeTestCase1::TraceSink (std::string context, double oldValue, double newValue)
{
  NS_TEST_ASSERT_MSG_GT (Simulator::Now (), Seconds (100), "Probed a value outside of the time window");
  NS_TEST_ASSERT_MSG_LT (Simulator::Now (), Seconds (200), "Probed a value outside of the time window");

  NS_TEST_ASSERT_MSG_EQ_TOL (s->GetValue (), newValue, 0.00001, "Value probed different than value in the variable");

  if (context == "testProbe")
    {
      m_objectProbed++;
    }
  else if (context == "testProbe2")
    {
      m_pathProbed++;
    }
}

void
ProbeTestCase1::DoRun (void)
{
  // Test that all instances of probe data are between time window specified
  // Check also that probes can be hooked to sources by Object and by path

  Ptr<DoubleProbe> p = CreateObject<DoubleProbe> ();
  p->SetName ("testProbe");

  Simulator::Schedule (Seconds (1), &SampleEmitter::Start, s);
  p->SetAttribute ("Start", TimeValue (Seconds (100.0)));
  p->SetAttribute ("Stop", TimeValue (Seconds (200.0)));
  Simulator::Stop (Seconds (300));

  // Register our emitter object so we can fetch it by using the Config
  // namespace
  Names::Add ("/Names/SampleEmitter", s);

  // Hook probe to the emitter.
  p->ConnectByObject ("Emitter", s);

  // Hook our test function to the probe trace source
  p->TraceConnect ("Output", p->GetName (), MakeCallback (&ProbeTestCase1::TraceSink, this));

  // Repeat but hook the probe to the object this time using the Config
  // name set above
  Ptr<DoubleProbe> p2 = CreateObject<DoubleProbe> ();
  p2->SetName ("testProbe2");
  p2->SetAttribute ("Start", TimeValue (Seconds (100.0)));
  p2->SetAttribute ("Stop", TimeValue (Seconds (200.0)));

  // Hook probe to the emitter.
  p2->ConnectByPath ("/Names/SampleEmitter/Emitter");

  // Hook our test function to the  probe trace source
  p2->TraceConnect ("Output", p2->GetName (), MakeCallback (&ProbeTestCase1::TraceSink, this));

  Simulator::Run ();

  // Check that each trace sink was called
  NS_TEST_ASSERT_MSG_GT (m_objectProbed, 0, "Trace sink for object probe never called");
  NS_TEST_ASSERT_MSG_GT (m_pathProbed, 0, "Trace sink for path probe never called");
  Simulator::Destroy ();
}


class ProbeTestSuite : public TestSuite
{
public:
  ProbeTestSuite ();
};

ProbeTestSuite::ProbeTestSuite ()
  : TestSuite ("double-probe", UNIT)
{
  AddTestCase (new ProbeTestCase1, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static ProbeTestSuite probeTestSuite;

