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


#include <fstream>
#include <sstream>
#include <map>
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
#include "ns2-mobility-helper.h"

NS_LOG_COMPONENT_DEFINE ("Ns2MobilityHelper");

using namespace std;

namespace ns3 {

// Constants definitions
#define  NS2_AT       "at"
#define  NS2_X_COORD  "X_"
#define  NS2_Y_COORD  "Y_"
#define  NS2_Z_COORD  "Z_"
#define  NS2_SETDEST  "setdest"
#define  NS2_SET      "set"
#define  NS2_NODEID   "$node_("
#define  NS2_NS_SCH   "$ns_"


// Type to maintain line parsed and its values
struct ParseResult
{
  vector<string> tokens; // tokens from a line
  vector<int> ivals;     // int values for each tokens
  vector<bool> has_ival; // points if a tokens has an int value
  vector<double> dvals;  // double values for each tokens
  vector<bool> has_dval; // points if a tokens has a double value
  vector<string> svals;  // string value for each token
};


// Parses a line of ns2 mobility
static ParseResult ParseNs2Line (const string& str);

// Put out blank spaces at the start and end of a line
static string TrimNs2Line (const string& str);

// Checks if a string represents a number or it has others characters than digits an point.
static bool IsNumber (const string& s);

// Check if s string represents a numeric value
template<class T>
static bool IsVal (const string& str, T& ret);

// Checks if the value between brackets is a correct nodeId number
static bool HasNodeIdNumber (string str);

// Gets nodeId number in string format from the string like $node_(4)
static string GetNodeIdFromToken (string str);

// Get node id number in int format
static int GetNodeIdInt (ParseResult pr);

// Get node id number in string format
static string GetNodeIdString (ParseResult pr);

// Add one coord to a vector position
static Vector SetOneInitialCoord (Vector actPos, string& coord, double value);

// Check if this corresponds to a line like this: $node_(0) set X_ 123
static bool IsSetInitialPos (ParseResult pr);

// Check if this corresponds to a line like this: $ns_ at 1 "$node_(0) setdest 2 3 4"
static bool IsSchedSetPos (ParseResult pr);

// Check if this corresponds to a line like this: $ns_ at 1 "$node_(0) set X_ 2"
static bool IsSchedMobilityPos (ParseResult pr);

// Set waypoints and speed for movement.
static Vector SetMovement (Ptr<ConstantVelocityMobilityModel> model, Vector lastPos, double at,
                           double xFinalPosition, double yFinalPosition, double speed);

// Set initial position for a node
static Vector SetInitialPosition (Ptr<ConstantVelocityMobilityModel> model, string coord, double coordVal);

// Schedule a set of position for a node
static Vector SetSchedPosition (Ptr<ConstantVelocityMobilityModel> model, double at, string coord, double coordVal);


Ns2MobilityHelper::Ns2MobilityHelper (std::string filename)
  : m_filename (filename)
{
}

Ptr<ConstantVelocityMobilityModel>
Ns2MobilityHelper::GetMobilityModel (std::string idString, const ObjectStore &store) const
{
  std::istringstream iss;
  iss.str (idString);
  uint32_t id (0);
  iss >> id;
  Ptr<Object> object = store.Get (id);
  if (object == 0)
    {
      return 0;
    }
  Ptr<ConstantVelocityMobilityModel> model = object->GetObject<ConstantVelocityMobilityModel> ();
  if (model == 0)
    {
      model = CreateObject<ConstantVelocityMobilityModel> ();
      object->AggregateObject (model);
    }
  return model;
}


void
Ns2MobilityHelper::ConfigNodesMovements (const ObjectStore &store) const
{
  map<int, Vector> last_pos;    // Vector containing lasts positions for each node

  std::ifstream file (m_filename.c_str (), std::ios::in);
  if (file.is_open ())
    {
      while (!file.eof () )
        {
          int         iNodeId = 0;
          std::string nodeId;
          std::string line;

          getline (file, line);

          // ignore empty lines
          if (line.empty ())
            {
              continue;
            }

          ParseResult pr = ParseNs2Line (line); // Parse line and obtain tokens

          // Check if the line corresponds with one of the three types of line
          if (pr.tokens.size () != 4 && pr.tokens.size () != 7 && pr.tokens.size () != 8)
            {
              NS_LOG_ERROR ("Line has not correct number of parameters (corrupted file?): " << line << "\n");
              continue;
            }

          // Get the node Id
          nodeId  = GetNodeIdString (pr);
          iNodeId = GetNodeIdInt (pr);
          if (iNodeId == -1)
            {
        	  NS_LOG_ERROR ("Node number couldn't be obtained (corrupted file?): " << line << "\n");
              continue;
            }

          // get mobility model of node
          Ptr<ConstantVelocityMobilityModel> model = GetMobilityModel (nodeId,store);

          // if model not exists, continue
          if (model == 0)
            {
              NS_LOG_ERROR ("Unknown node ID (corrupted file?): " << nodeId << "\n");
              continue;
            }


          /*
           * In this case a initial position is being seted
           * line like $node_(0) set X_ 151.05190721688197
           */
          if (IsSetInitialPos (pr))
            {
              //                                            coord         coord value
              last_pos[iNodeId] = SetInitialPosition (model, pr.tokens[2], pr.dvals[3]);

              // Log new position
              NS_LOG_DEBUG ("Positions after parse for node " << iNodeId << " " << nodeId <<
                            " x=" << last_pos[iNodeId].x << " y=" << last_pos[iNodeId].y << " z=" << last_pos[iNodeId].z);
            }

          else // NOW EVENTS TO BE SCHEDULED
            {

              // This is a scheduled event, so time at should be present
              double at;

              if (!IsNumber (pr.tokens[2]))
                {
                  NS_LOG_WARN ("Time is not a number: " << pr.tokens[2]);
                  continue;
                }

              at = pr.dvals[2]; // set time at

              if ( at < 0 )
                {
                  NS_LOG_WARN ("Time is less than cero: " << at);
                  continue;
                }



              /*
               * In this case a new waypoint is added
               * line like $ns_ at 1 "$node_(0) setdest 2 3 4"
               */
              if (IsSchedMobilityPos (pr))
                {
                  //                                     last position     time  X coord     Y coord      velocity
                  last_pos[iNodeId] = SetMovement (model, last_pos[iNodeId], at, pr.dvals[5], pr.dvals[6], pr.dvals[7]);

                  // Log new position
                  NS_LOG_DEBUG ("Positions after parse for node " << iNodeId << " " << nodeId <<
                                " x=" << last_pos[iNodeId].x << " y=" << last_pos[iNodeId].y << " z=" << last_pos[iNodeId].z);
                }


              /*
               * Scheduled set position
               * line like $ns_ at 4.634906291962 "$node_(0) set X_ 28.675920486450"
               */
              else if (IsSchedSetPos (pr))
                {
                  //                                         time  coordinate   coord value
                  last_pos[iNodeId] = SetSchedPosition (model, at, pr.tokens[5], pr.dvals[6]);

                  // Log new position
                  NS_LOG_DEBUG ("Positions after parse for node " << iNodeId << " " << nodeId <<
                                " x=" << last_pos[iNodeId].x << " y=" << last_pos[iNodeId].y << " z=" << last_pos[iNodeId].z);
                }
              else
                {
                  NS_LOG_WARN ("Format Line is not correct: " << line << "\n");
                }
            }
        }
      file.close ();
    }
}


ParseResult
ParseNs2Line (const string& str)
{
  ParseResult ret;
  istringstream s;
  string line;

  // ignore comments (#)
  size_t pos_sharp = str.find_first_of ('#');
  if (pos_sharp != string::npos)
    {
      line = str.substr (0, pos_sharp);
    }
  else
    {
      line = str;
    }

  line = TrimNs2Line (line);

  // If line hasn't a correct node Id
  if (!HasNodeIdNumber (line))
    {
      NS_LOG_WARN ("Line has no node Id: " << line);
      return ret;
    }

  s.str (line);

  while (!s.eof ())
    {
      string x;
      s >> x;
      ret.tokens.push_back (x);
      int ii (0);
      double d (0);
      if (HasNodeIdNumber (x))
        {
          x = GetNodeIdFromToken (x);
        }
      ret.has_ival.push_back (IsVal<int> (x, ii));
      ret.ivals.push_back (ii);
      ret.has_dval.push_back (IsVal<double> (x, d));
      ret.dvals.push_back (d);
      ret.svals.push_back (x);
    }

  size_t tokensLength   = ret.tokens.size ();                 // number of tokens in line
  size_t lasTokenLength = ret.tokens[tokensLength - 1].size (); // length of the last token

  // if it is a scheduled set _[XYZ] or a setdest I need to remove the last "
  // and re-calculate values
  if ( (tokensLength == 7 || tokensLength == 8)
       && (ret.tokens[tokensLength - 1][lasTokenLength - 1] == '"') )
    {

      // removes " from the last position
      ret.tokens[tokensLength - 1] = ret.tokens[tokensLength - 1].substr (0,lasTokenLength - 1);

      string x;
      x = ret.tokens[tokensLength - 1];

      if (HasNodeIdNumber (x))
        {
          x = GetNodeIdFromToken (x);
        }

      // Re calculate values
      int ii (0);
      double d (0);
      ret.has_ival[tokensLength - 1] = IsVal<int> (x, ii);
      ret.ivals[tokensLength - 1] = ii;
      ret.has_dval[tokensLength - 1] = IsVal<double> (x, d);
      ret.dvals[tokensLength - 1] = d;
      ret.svals[tokensLength - 1] = x;

    }
  else if ( (tokensLength == 9 && ret.tokens[tokensLength - 1] == "\"")
            || (tokensLength == 8 && ret.tokens[tokensLength - 1] == "\""))
    {
      // if the line has the " character in this way: $ns_ at 1 "$node_(0) setdest 2 2 1  "
      // or in this: $ns_ at 4 "$node_(0) set X_ 2  " we need to ignore this last token

      ret.tokens.erase (ret.tokens.begin () + tokensLength - 1);
      ret.has_ival.erase (ret.has_ival.begin () + tokensLength - 1);
      ret.ivals.erase (ret.ivals.begin () + tokensLength - 1);
      ret.has_dval.erase (ret.has_dval.begin () + tokensLength - 1);
      ret.dvals.erase (ret.dvals.begin () + tokensLength - 1);
      ret.svals.erase (ret.svals.begin () + tokensLength - 1);

    }



  return ret;
}


string
TrimNs2Line (const string& s)
{
  string ret = s;

  while (ret.size () > 0 && isblank (ret[0]))
    {
      ret.erase (0, 1);    // Removes blank spaces at the begining of the line
    }

  while (ret.size () > 0 && isblank (ret[ret.size () - 1]))
    {
      ret.erase (ret.size () - 1, 1); // Removes blank spaces from at end of line
    }

  return ret;
}


bool
IsNumber (const string& s)
{
  char *endp;
  double unused;
  unused = strtod (s.c_str (), &endp); // declared with warn_unused_result
  return endp == s.c_str () + s.size ();
}


template<class T>
bool IsVal (const string& str, T& ret)
{
  if (str.size () == 0)
    {
      return false;
    }
  else if (IsNumber (str))
    {
      string s2 = str;
      istringstream s (s2);
      s >> ret;
      return true;
    }
  else
    {
      return false;
    }
}


bool
HasNodeIdNumber (string str)
{

  // find brackets
  std::string::size_type startNodeId = str.find_first_of ("("); // index of left bracket
  std::string::size_type endNodeId   = str.find_first_of (")"); // index of right bracket

  // Get de nodeId in a string and in a int value
  std::string nodeId;     // node id

  // if no brackets, continue!
  if (startNodeId == std::string::npos || endNodeId == std::string::npos)
    {
      return false;
    }

  nodeId = str.substr (startNodeId + 1, endNodeId - (startNodeId + 1)); // set node id

  //   is number              is integer                                       is not negative
  if (IsNumber (nodeId) && (nodeId.find_first_of (".") == std::string::npos) && (nodeId[0] != '-'))
    {
      return true;
    }
  else
    {
      return false;
    }
}


string
GetNodeIdFromToken (string str)
{
  if (HasNodeIdNumber (str))
    {
      // find brackets
      std::string::size_type startNodeId = str.find_first_of ("(");     // index of left bracket
      std::string::size_type endNodeId   = str.find_first_of (")");     // index of right bracket

      return str.substr (startNodeId + 1, endNodeId - (startNodeId + 1)); // set node id
    }
  else
    {
      return "";
    }
}


int
GetNodeIdInt (ParseResult pr)
{
  int result = -1;
  switch (pr.tokens.size ())
    {
    case 4:   // line like $node_(0) set X_ 11
      result = pr.ivals[0];
      break;
    case 7:   // line like $ns_ at 4 "$node_(0) set X_ 28"
      result = pr.ivals[3];
      break;
    case 8:   // line like $ns_ at 1 "$node_(0) setdest 2 3 4"
      result = pr.ivals[3];
      break;
    default:
      result = -1;
    }
  return result;
}

// Get node id number in string format
string
GetNodeIdString (ParseResult pr)
{
  switch (pr.tokens.size ())
    {
    case 4:   // line like $node_(0) set X_ 11
      return pr.svals[0];
      break;
    case 7:   // line like $ns_ at 4 "$node_(0) set X_ 28"
      return pr.svals[3];
      break;
    case 8:   // line like $ns_ at 1 "$node_(0) setdest 2 3 4"
      return pr.svals[3];
      break;
    default:
      return "";
    }
}


Vector
SetOneInitialCoord (Vector position, string& coord, double value)
{

  // set the position for the coord.
  if (coord == NS2_X_COORD)
    {
      position.x = value;
      NS_LOG_DEBUG ("X=" << value);
    }
  else if (coord == NS2_Y_COORD)
    {
      position.y = value;
      NS_LOG_DEBUG ("Y=" << value);
    }
  else if (coord == NS2_Z_COORD)
    {
      position.z = value;
      NS_LOG_DEBUG ("Z=" << value);
    }
  return position;
}


bool
IsSetInitialPos (ParseResult pr)
{
  //        number of tokens         has $node_( ?                        has "set"           has doble for position?
  return pr.tokens.size () == 4 && HasNodeIdNumber (pr.tokens[0]) && pr.tokens[1] == NS2_SET && pr.has_dval[3]
         // coord name is X_, Y_ or Z_ ?
         && (pr.tokens[2] == NS2_X_COORD || pr.tokens[2] == NS2_Y_COORD || pr.tokens[2] == NS2_Z_COORD);

}


bool
IsSchedSetPos (ParseResult pr)
{
  //      correct number of tokens,    has $ns_                   and at
  return pr.tokens.size () == 7 && pr.tokens[0] == NS2_NS_SCH && pr.tokens[1] == NS2_AT
         && pr.tokens[4] == NS2_SET && pr.has_dval[2] && pr.has_dval[3]   // has set and double value for time and nodeid
         && ( pr.tokens[5] == NS2_X_COORD || pr.tokens[5] == NS2_Y_COORD || pr.tokens[5] == NS2_Z_COORD) // has X_, Y_ or Z_?
         && pr.has_dval[2]; // time is a number
}

bool
IsSchedMobilityPos (ParseResult pr)
{
  //     number of tokens      and    has $ns_                and    has at
  return pr.tokens.size () == 8 && pr.tokens[0] == NS2_NS_SCH && pr.tokens[1] == NS2_AT
         //    time             x coord          y coord          velocity are numbers?
         && pr.has_dval[2] && pr.has_dval[5] && pr.has_dval[6] && pr.has_dval[7]
         && pr.tokens[4] == NS2_SETDEST; // and has setdest

}

Vector
SetMovement (Ptr<ConstantVelocityMobilityModel> model, Vector last_pos, double at,
             double xFinalPosition, double yFinalPosition, double speed)
{
  Vector position;
  position.x = last_pos.x;
  position.y = last_pos.y;
  position.z = last_pos.z;

  if (speed == 0)
    {
      // We have to maintain last position, and stop the movement
      Simulator::Schedule (Seconds (at), &ConstantVelocityMobilityModel::SetVelocity, model,
                           Vector (0, 0, 0));
    }
  else if (speed > 0)
    {
      // first calculate the time; time = distance / speed
      double time = sqrt (pow (xFinalPosition - position.x, 2) + pow (yFinalPosition - position.y, 2)) / speed;
      NS_LOG_DEBUG ("at=" << at << " time=" << time);
      // now calculate the xSpeed = distance / time
      double xSpeed = (xFinalPosition - position.x) / time;
      double ySpeed = (yFinalPosition - position.y) / time; // & same with ySpeed

      // quick and dirty set zSpeed = 0
      double zSpeed = 0;

      NS_LOG_DEBUG ("Calculated Speed: X=" << xSpeed << " Y=" << ySpeed << " Z=" << zSpeed);

      // Set the Values
      Simulator::Schedule (Seconds (at), &ConstantVelocityMobilityModel::SetVelocity, model,
                           Vector (xSpeed, ySpeed, zSpeed));

      if (time >= 0)
        {
          Simulator::Schedule (Seconds (at + time), &ConstantVelocityMobilityModel::SetVelocity,
                               model, Vector (0, 0, 0));
        }

      position.x = xFinalPosition;
      position.y = yFinalPosition;
      position.z = 0;
    }

  return position;
}


Vector
SetInitialPosition (Ptr<ConstantVelocityMobilityModel> model, string coord, double coordVal)
{
  model->SetPosition (SetOneInitialCoord (model->GetPosition (), coord, coordVal));

  Vector position;
  position.x = model->GetPosition ().x;
  position.y = model->GetPosition ().y;
  position.z = model->GetPosition ().z;

  return position;
}

// Schedule a set of position for a node
Vector
SetSchedPosition (Ptr<ConstantVelocityMobilityModel> model, double at, string coord, double coordVal)
{
  // update position
  model->SetPosition (SetOneInitialCoord (model->GetPosition (), coord, coordVal));

  Vector position;
  position.x = model->GetPosition ().x;
  position.y = model->GetPosition ().y;
  position.z = model->GetPosition ().z;

  // Chedule next positions
  Simulator::Schedule (Seconds (at), &ConstantVelocityMobilityModel::SetPosition, model,position);

  return position;
}

void
Ns2MobilityHelper::Install (void) const
{
  Install (NodeList::Begin (), NodeList::End ());
}

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
    NS_TEST_ASSERT_MSG_EQ (of.is_open (), true, "Need to write tmp. file");
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
        NS_TEST_ASSERT_MSG_NE (node, 0, "Can't find node with id " << rp.node);
        Ptr<MobilityModel> mob = node->GetObject<MobilityModel> ();
        NS_TEST_ASSERT_MSG_NE (mob, 0, "Can't find mobility for node " << rp.node);

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
  bool DoRun ()
  {
    NS_TEST_ASSERT_MSG_EQ (m_trace.empty (), false, "Need trace");
    NS_TEST_ASSERT_MSG_EQ (m_reference.empty (), false, "Need reference");

    if (WriteTrace ())
      {
        return GetErrorStatus ();
      }
    CreateNodes ();
    Ns2MobilityHelper mobility (m_traceFile);
    mobility.Install ();
    if (CheckInitialPositions ())
      {
        return GetErrorStatus ();
      }
    Config::Connect ("/NodeList/*/$ns3::MobilityModel/CourseChange",
                     MakeCallback (&Ns2MobilityHelperTest::CourseChange, this));
    Simulator::Stop (m_timeLimit);
    Simulator::Run ();
    Names::Clear ();
    std::remove (m_traceFile.c_str ());
    Simulator::Destroy ();
    return GetErrorStatus ();
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
