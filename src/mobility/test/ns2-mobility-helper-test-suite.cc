/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 *               2009,2010 Contributors
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
 * Contributors: Thomas Waldecker <twaldecker@rocketmail.com>
 *               Martín Giachino <martin.giachino@gmail.com>
 *
 * Brief description: Implementation of a ns2 movement trace file reader.
 *
 * This implementation is based on the ns2 movement documentation of ns2
 * as described in http://www.isi.edu/nsnam/ns/doc/node174.html
 *
 * Valid trace files use the following ns2 statements:
 *
 * $node set X_ x1
 * $node set Y_ y1
 * $node set Z_ z1
 * $ns at $time $node setdest x2 y2 speed
 * $ns at $time $node set X_ x1
 * $ns at $time $node set Y_ Y1
 * $ns at $time $node set Z_ Z1
 *
 */

#include <algorithm>
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/node-list.h"
#include "ns3/node.h"
#include "ns3/constant-velocity-mobility-model.h"
#include "ns3/test.h"
#include "ns3/node-container.h"
#include "ns3/names.h"
#include "ns3/config.h"
#include "ns3/ns2-mobility-helper.h"

namespace ns3 {

// -----------------------------------------------------------------------------
// Testing
// -----------------------------------------------------------------------------
bool operator== (Vector const & a, Vector const & b)
{
  return (a.x == b.x && a.y == b.y && a.z == b.z);
}
/**
 * Every test case is supposed to:
 *  1. Generate short mobility trace file
 *  2. Read it back using Ns2MobilityHelper
 *  3. Check initial node positions and speeds.
 *  4. Run simulation listening for all CourseChange events and compare actual mobility with the reference
 */
class Ns2MobilityHelperTest : public TestCase
{
public:
  /// Single record in mobility reference
  struct ReferencePoint
  {
    std::string node;       ///< node ID as string, e.g. "1"
    Time time;              ///< timestamp
    Vector pos;             ///< reference position
    Vector vel;             ///< reference velocity

    ReferencePoint (std::string const & id, Time t, Vector const & p, Vector const & v)
      : node (id),
        time (t),
        pos (p),
        vel (v)
    {
    }
    /// Sort by timestamp
    bool operator< (ReferencePoint const & o) const
    {
      return (time < o.time);
    }
  };
  /**
   * Create new test case. To make it useful SetTrace () and AddReferencePoint () must be called
   *
   * \param name        Short description
   * \param nodes       Number of nodes used in the test trace, 1 by default
   */
  Ns2MobilityHelperTest (std::string const & name, Time timeLimit, uint32_t nodes = 1)
    : TestCase (name),
      m_timeLimit (timeLimit),
      m_nodeCount (nodes),
      m_nextRefPoint (0)
  {
  }
  /// Empty
  virtual ~Ns2MobilityHelperTest ()
  {
  }
  /// Set NS-2 trace to read as single large string (don't forget to add \n and quote ")
  void SetTrace (std::string const & trace)
  {
    m_trace = trace;
  }
  /// Add next reference point
  void AddReferencePoint (ReferencePoint const & r)
  {
    m_reference.push_back (r);
  }
  /// Sugar
  void AddReferencePoint (const char * id, double sec, Vector const & p, Vector const & v)
  {
    AddReferencePoint (ReferencePoint (id, Seconds (sec), p, v));
  }

private:
  /// Test time limit
  Time m_timeLimit;
  /// Number of nodes used in the test
  uint32_t m_nodeCount;
  /// Trace as string
  std::string m_trace;
  /// Reference mobility
  std::vector<ReferencePoint> m_reference;
  /// Next reference point to be checked
  size_t m_nextRefPoint;
  /// TMP trace file name
  std::string m_traceFile;

private:
  /// Dump NS-2 trace to tmp file
  bool WriteTrace ()
  {
    m_traceFile = GetTempDir () + "Ns2MobilityHelperTest.tcl";
    std::ofstream of (m_traceFile.c_str ());
    NS_TEST_ASSERT_MSG_EQ_RETURNS_BOOL (of.is_open (), true, "Need to write tmp. file");
    of << m_trace;
    of.close ();
    return false; // no errors
  }
  /// Create and name nodes
  void CreateNodes ()
  {
    NodeContainer nodes;
    nodes.Create (m_nodeCount);
    for (uint32_t i = 0; i < m_nodeCount; ++i)
      {
        std::ostringstream os;
        os << i;
        Names::Add (os.str (), nodes.Get (i));
      }
  }
  /// Check that all initial positions are correct
  bool CheckInitialPositions ()
  {
    std::stable_sort (m_reference.begin (), m_reference.end ());
    while (m_nextRefPoint < m_reference.size () && m_reference[m_nextRefPoint].time == Seconds (0))
      {
        ReferencePoint const & rp = m_reference[m_nextRefPoint];
        Ptr<Node> node = Names::Find<Node> (rp.node);
        NS_TEST_ASSERT_MSG_NE_RETURNS_BOOL (node, 0, "Can't find node with id " << rp.node);
        Ptr<MobilityModel> mob = node->GetObject<MobilityModel> ();
        NS_TEST_ASSERT_MSG_NE_RETURNS_BOOL (mob, 0, "Can't find mobility for node " << rp.node);

        NS_TEST_EXPECT_MSG_EQ (rp.pos, mob->GetPosition (), "Initial position mismatch for node " << rp.node);
        NS_TEST_EXPECT_MSG_EQ (rp.vel, mob->GetVelocity (), "Initial velocity mismatch for node " << rp.node);

        m_nextRefPoint++;
      }
    return GetErrorStatus ();
  }
  /// Listen for course change events
  void CourseChange (std::string context, Ptr<const MobilityModel> mobility)
  {
    Time time = Simulator::Now ();
    Ptr<Node> node = mobility->GetObject<Node> ();
    NS_ASSERT (node);
    std::string id = Names::FindName (node);
    NS_ASSERT (!id.empty ());
    Vector pos = mobility->GetPosition ();
    Vector vel = mobility->GetVelocity ();

    NS_TEST_EXPECT_MSG_LT (m_nextRefPoint, m_reference.size (), "Not enough reference points");
    if (m_nextRefPoint >= m_reference.size ())
      {
        return;
      }

    ReferencePoint const & ref = m_reference [m_nextRefPoint++];
    NS_TEST_EXPECT_MSG_EQ (time, ref.time, "Time mismatch");
    NS_TEST_EXPECT_MSG_EQ (id, ref.node, "Node ID mismatch at time " << time.GetSeconds () << " s");
    NS_TEST_EXPECT_MSG_EQ (pos, ref.pos, "Position mismatch at time " << time.GetSeconds () << " s for node " << id);
    NS_TEST_EXPECT_MSG_EQ (vel, ref.vel, "Velocity mismatch at time " << time.GetSeconds () << " s for node " << id);
  }
  /// Go
  void DoRun ()
  {
    NS_TEST_ASSERT_MSG_EQ (m_trace.empty (), false, "Need trace");
    NS_TEST_ASSERT_MSG_EQ (m_reference.empty (), false, "Need reference");

    if (WriteTrace ())
      {
        return;
      }
    CreateNodes ();
    Ns2MobilityHelper mobility (m_traceFile);
    mobility.Install ();
    if (CheckInitialPositions ())
      {
        return;
      }
    Config::Connect ("/NodeList/*/$ns3::MobilityModel/CourseChange",
                     MakeCallback (&Ns2MobilityHelperTest::CourseChange, this));
    Simulator::Stop (m_timeLimit);
    Simulator::Run ();
    Names::Clear ();
    std::remove (m_traceFile.c_str ());
    Simulator::Destroy ();
  }
};

/// The test suite
class Ns2MobilityHelperTestSuite : public TestSuite
{
public:
  Ns2MobilityHelperTestSuite () : TestSuite ("mobility-ns2-trace-helper", UNIT)
  {
    // to be used as temporary variable for test cases.
    // Note that test suite takes care of deleting all test cases.
    Ns2MobilityHelperTest * t (0);

    // Initial position
    t = new Ns2MobilityHelperTest ("initial position", Seconds (1));
    t->SetTrace ("$node_(0) set X_ 1.0\n"
                 "$node_(0) set Y_ 2.0\n"
                 "$node_(0) set Z_ 3.0\n"
                 );
    t->AddReferencePoint ("0", 0, Vector (1, 2, 3), Vector (0, 0, 0));
    AddTestCase (t);

    // Check parsing comments, empty lines and no EOF at the end of file
    t = new Ns2MobilityHelperTest ("comments", Seconds (1));
    t->SetTrace ("# comment\n"
                 "\n\n" // empty lines
                 "$node_(0) set X_ 1.0 # comment \n"
                 "$node_(0) set Y_ 2.0 ### \n"
                 "$node_(0) set Z_ 3.0 # $node_(0) set Z_ 3.0\n"
                 "#$node_(0) set Z_ 100 #"
                 );
    t->AddReferencePoint ("0", 0, Vector (1, 2, 3), Vector (0, 0, 0));
    AddTestCase (t);

    // Simple setdest. Arguments are interpreted as x, y, speed by default
    t = new Ns2MobilityHelperTest ("simple setdest", Seconds (10));
    t->SetTrace ("$ns_ at 1.0 \"$node_(0) setdest 25 0 5\"");
    //                     id  t  position         velocity
    t->AddReferencePoint ("0", 0, Vector (0, 0, 0), Vector (0, 0, 0));
    t->AddReferencePoint ("0", 1, Vector (0, 0, 0), Vector (5, 0, 0));
    t->AddReferencePoint ("0", 6, Vector (25, 0, 0), Vector (0, 0, 0));
    AddTestCase (t);

    // Several set and setdest. Arguments are interpreted as x, y, speed by default
    t = new Ns2MobilityHelperTest ("square setdest", Seconds (6));
    t->SetTrace ("$node_(0) set X_ 0.0\n"
                 "$node_(0) set Y_ 0.0\n"
                 "$ns_ at 1.0 \"$node_(0) setdest 5  0  5\"\n"
                 "$ns_ at 2.0 \"$node_(0) setdest 5  5  5\"\n"
                 "$ns_ at 3.0 \"$node_(0) setdest 0  5  5\"\n"
                 "$ns_ at 4.0 \"$node_(0) setdest 0  0  5\"\n"
                 );
    //                     id  t  position         velocity
    t->AddReferencePoint ("0", 0, Vector (0, 0, 0), Vector (0,  0, 0));
    t->AddReferencePoint ("0", 1, Vector (0, 0, 0), Vector (5,  0, 0));
    t->AddReferencePoint ("0", 2, Vector (5, 0, 0), Vector (0,  0, 0));
    t->AddReferencePoint ("0", 2, Vector (5, 0, 0), Vector (0,  5, 0));
    t->AddReferencePoint ("0", 3, Vector (5, 5, 0), Vector (0,  0, 0));
    t->AddReferencePoint ("0", 3, Vector (5, 5, 0), Vector (-5, 0, 0));
    t->AddReferencePoint ("0", 4, Vector (0, 5, 0), Vector (0, 0, 0));
    t->AddReferencePoint ("0", 4, Vector (0, 5, 0), Vector (0, -5, 0));
    t->AddReferencePoint ("0", 5, Vector (0, 0, 0), Vector (0,  0, 0));
    AddTestCase (t);

    // Scheduled set position
    t = new Ns2MobilityHelperTest ("scheduled set position", Seconds (2));
    t->SetTrace ("$ns_ at 1.0 \"$node_(0) set X_ 10\"\n"
                 "$ns_ at 1.0 \"$node_(0) set Z_ 10\"\n"
                 "$ns_ at 1.0 \"$node_(0) set Y_ 10\"");
    //                     id  t  position         velocity
    t->AddReferencePoint ("0", 1, Vector (10, 0, 0), Vector (0, 0, 0));
    t->AddReferencePoint ("0", 1, Vector (10, 0, 10), Vector (0, 0, 0));
    t->AddReferencePoint ("0", 1, Vector (10, 10, 10), Vector (0, 0, 0));
    AddTestCase (t);

    // Malformed lines
    t = new Ns2MobilityHelperTest ("malformed lines", Seconds (2));
    t->SetTrace ("$node() set X_ 1 # node id is not present\n"
                 "$node # incoplete line\"\n"
                 "$node this line is not correct\n"
                 "$node_(0) set X_ 1 # line OK \n"
                 "$node_(0) set Y_ 2 # line OK \n"
                 "$node_(0) set Z_ 3 # line OK \n"
                 "$ns_ at  \"$node_(0) setdest 4 4 4\" # time not present\n"
                 "$ns_ at 1 \"$node_(0) setdest 2 2 1   \" # line OK \n");
    //                     id  t  position         velocity
    t->AddReferencePoint ("0", 0, Vector (1, 2, 3), Vector (0, 0, 0));
    t->AddReferencePoint ("0", 1, Vector (1, 2, 3), Vector (1, 0, 0));
    t->AddReferencePoint ("0", 2, Vector (2, 2, 3), Vector (0, 0, 0));
    AddTestCase (t);

    // Non possible values
    t = new Ns2MobilityHelperTest ("non possible values", Seconds (2));
    t->SetTrace ("$node_(0) set X_ 1 # line OK \n"
                 "$node_(0) set Y_ 2 # line OK \n"
                 "$node_(0) set Z_ 3 # line OK \n"
                 "$node_(-22) set Y_ 3 # node id not correct\n"
                 "$node_(3.3) set Y_ 1111 # node id not correct\n"
                 "$ns_ at sss \"$node_(0) setdest 5 5 5\" # time is not a number\n"
                 "$ns_ at 1 \"$node_(0) setdest 2 2 1\" # line OK \n"
                 "$ns_ at 1 \"$node_(0) setdest 2 2 -1\" # negative speed is not correct\n"
                 "$ns_ at 1 \"$node_(0) setdest 2 2 sdfs\"    # speed is not a number\n"
                 "$ns_ at 1 \"$node_(0) setdest 2 2 s232dfs\" # speed is not a number\n"
                 "$ns_ at 1 \"$node_(0) setdest 233 2.. s232dfs\"   # more than one non numbers\n"
                 "$ns_ at -12 \"$node_(0) setdest 11 22 33\" # time should not be negative\n");
    //                     id  t  position         velocity
    t->AddReferencePoint ("0", 0, Vector (1, 2, 3), Vector (0, 0, 0));
    t->AddReferencePoint ("0", 1, Vector (1, 2, 3), Vector (1, 0, 0));
    t->AddReferencePoint ("0", 2, Vector (2, 2, 3), Vector (0, 0, 0));
    AddTestCase (t);

    // More than one node
    t = new Ns2MobilityHelperTest ("few nodes, combinations of set and setdest", Seconds (10), 3);
    t->SetTrace ("$node_(0) set X_ 1.0\n"
                 "$node_(0) set Y_ 2.0\n"
                 "$node_(0) set Z_ 3.0\n"
                 "$ns_ at 1.0 \"$node_(1) setdest 25 0 5\"\n"
                 "$node_(2) set X_ 0.0\n"
                 "$node_(2) set Y_ 0.0\n"
                 "$ns_ at 1.0 \"$node_(2) setdest 5  0  5\"\n"
                 "$ns_ at 2.0 \"$node_(2) setdest 5  5  5\"\n"
                 "$ns_ at 3.0 \"$node_(2) setdest 0  5  5\"\n"
                 "$ns_ at 4.0 \"$node_(2) setdest 0  0  5\"\n");
    //                     id  t  position         velocity
    t->AddReferencePoint ("0", 0, Vector (1, 2, 3), Vector (0, 0, 0));
    t->AddReferencePoint ("1", 0, Vector (0, 0, 0), Vector (0, 0, 0));
    t->AddReferencePoint ("1", 1, Vector (0, 0, 0), Vector (5, 0, 0));
    t->AddReferencePoint ("1", 6, Vector (25, 0, 0), Vector (0, 0, 0));
    t->AddReferencePoint ("2", 0, Vector (0, 0, 0), Vector (0,  0, 0));
    t->AddReferencePoint ("2", 1, Vector (0, 0, 0), Vector (5,  0, 0));
    t->AddReferencePoint ("2", 2, Vector (5, 0, 0), Vector (0,  0, 0));
    t->AddReferencePoint ("2", 2, Vector (5, 0, 0), Vector (0,  5, 0));
    t->AddReferencePoint ("2", 3, Vector (5, 5, 0), Vector (0,  0, 0));
    t->AddReferencePoint ("2", 3, Vector (5, 5, 0), Vector (-5, 0, 0));
    t->AddReferencePoint ("2", 4, Vector (0, 5, 0), Vector (0, 0, 0));
    t->AddReferencePoint ("2", 4, Vector (0, 5, 0), Vector (0, -5, 0));
    t->AddReferencePoint ("2", 5, Vector (0, 0, 0), Vector (0,  0, 0));
    AddTestCase (t);

    // Test for Speed == 0, that acts as stop the node.
    t = new Ns2MobilityHelperTest ("setdest with speed cero", Seconds (10));
    t->SetTrace ("$ns_ at 1.0 \"$node_(0) setdest 25 0 5\"\n"
                 "$ns_ at 7.0 \"$node_(0) setdest 11  22  0\"\n");
    //                     id  t  position         velocity
    t->AddReferencePoint ("0", 0, Vector (0, 0, 0), Vector (0, 0, 0));
    t->AddReferencePoint ("0", 1, Vector (0, 0, 0), Vector (5, 0, 0));
    t->AddReferencePoint ("0", 6, Vector (25, 0, 0), Vector (0, 0, 0));
    t->AddReferencePoint ("0", 7, Vector (25, 0, 0), Vector (0, 0, 0));
    AddTestCase (t);


    // Test negative positions
    t = new Ns2MobilityHelperTest ("test negative positions", Seconds (10));
    t->SetTrace ("$node_(0) set X_ -1.0\n"
                 "$node_(0) set Y_ 0\n"
                 "$ns_ at 1.0 \"$node_(0) setdest 0 0 1\"\n"
                 "$ns_ at 2.0 \"$node_(0) setdest 0  -1  1\"\n");
    //                     id  t  position         velocity
    t->AddReferencePoint ("0", 0, Vector (-1, 0, 0), Vector (0, 0, 0));
    t->AddReferencePoint ("0", 1, Vector (-1, 0, 0), Vector (1, 0, 0));
    t->AddReferencePoint ("0", 2, Vector (0, 0, 0), Vector (0, 0, 0));
    t->AddReferencePoint ("0", 2, Vector (0, 0, 0), Vector (0, -1, 0));
    t->AddReferencePoint ("0", 3, Vector (0, -1, 0), Vector (0, 0, 0));
    AddTestCase (t);

    // Sqare setdest with values in the form 1.0e+2
    t = new Ns2MobilityHelperTest ("Foalt numbers in 1.0e+2 format", Seconds (6));
    t->SetTrace ("$node_(0) set X_ 0.0\n"
                 "$node_(0) set Y_ 0.0\n"
                 "$ns_ at 1.0 \"$node_(0) setdest 1.0e+2  0       1.0e+2\"\n"
                 "$ns_ at 2.0 \"$node_(0) setdest 1.0e+2  1.0e+2  1.0e+2\"\n"
                 "$ns_ at 3.0 \"$node_(0) setdest 0       1.0e+2  1.0e+2\"\n"
                 "$ns_ at 4.0 \"$node_(0) setdest 0       0       1.0e+2\"\n");
    //                     id  t  position         velocity
    t->AddReferencePoint ("0", 0, Vector (0, 0, 0), Vector (0,  0, 0));
    t->AddReferencePoint ("0", 1, Vector (0, 0, 0), Vector (100,  0, 0));
    t->AddReferencePoint ("0", 2, Vector (100, 0, 0), Vector (0,  0, 0));
    t->AddReferencePoint ("0", 2, Vector (100, 0, 0), Vector (0,  100, 0));
    t->AddReferencePoint ("0", 3, Vector (100, 100, 0), Vector (0,  0, 0));
    t->AddReferencePoint ("0", 3, Vector (100, 100, 0), Vector (-100, 0, 0));
    t->AddReferencePoint ("0", 4, Vector (0, 100, 0), Vector (0, 0, 0));
    t->AddReferencePoint ("0", 4, Vector (0, 100, 0), Vector (0, -100, 0));
    t->AddReferencePoint ("0", 5, Vector (0, 0, 0), Vector (0,  0, 0));
    AddTestCase (t);

  }
} g_ns2TransmobilityHelperTestSuite;


} // namespace ns3
