/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017 Lawrence Livermore National Laboratory
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
 * Author: Gustavo Carneiro <gjc@inescporto.pt>
 * Author: Peter D. Barnes, Jr. <pdbarnes@llnl.gov>
 */

/**
 * \file
 * \ingroup core-examples
 * \ingroup logging
 * Example program that demonstrates ShowProgress.
 */

/**
 * Example program that demonstrates ShowProgress.
 *
 */

#include "ns3/core-module.h"

#include <ctime>
#include <iomanip>
#include <string>


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SampleShowProgress");

namespace {

class Timestamp
{
 public:
  Timestamp () :
    last(0),
    diff(0)
  {
    stamp ();
  }
  
  void stamp () {
    time_t seconds  = time (NULL);
    diff = seconds - last;
    last  = seconds;
  }

  std::string string () {
    std::string now = ctime ( &last );
    now.resize(now.length () - 1);  // trim trailing newline
    return now;
  }

  time_t last;
  time_t diff;

};  // class Timestamp

  
/**
 * Execute a function periodically.
 */
class Hold : public SimpleRefCount<Hold>
{
public:
  
  Hold (Time wait, Time interval)
  {
    m_wait = wait;
    m_interval = interval;

    m_rng = CreateObject<ExponentialRandomVariable> ();
    m_rng->SetAttribute ("Mean", DoubleValue (wait.GetSeconds ()));
  }

  Hold (Ptr<RandomVariableStream> rng)
    : m_rng (rng)
  {
  }

  void Event (void)
  {
    double delta = m_rng->GetValue ();
    Time delay = Seconds (delta);
    NS_LOG_LOGIC ("event delay: " << delay);
    
    Simulator::Schedule (delay, &Hold::Event, this);

    // Switch work load every 10 * m_interval of simulation time
    int64x64_t ratio = (Simulator::Now () / m_interval) / 10;
    bool even = (ratio.GetHigh () % 2);
    Time work = m_wait * (even ? 3 : 1);
    m_condition.TimedWait ( work.GetNanoSeconds () );
    
  }
  
private:
  Ptr<RandomVariableStream> m_rng;
  SystemCondition m_condition;
  Time m_wait;
  Time m_interval;

};  // class HOLD

}  // unnamed namespace


int
main (int argc, char ** argv)
{
  Time stop = Seconds (100);
  Time interval = Seconds (1);
  Time wait = MilliSeconds (10);
  bool verbose = false;

  CommandLine cmd;
  cmd.AddValue ("stop", "Simulation duration in virtual time.", stop);
  cmd.AddValue ("interval", "Approximate reporting interval, in wall clock time.", interval);
  cmd.AddValue ("wait", "Wallclock time to burn on each event.", wait);
  cmd.AddValue ("verbose", "Turn on verbose progress message.", verbose);
  cmd.Parse (argc, argv);

  std::cout << "\n"
            << cmd.GetName () << ":\n"
            << "\n"
            << "verbose progress message:  " << (verbose ? "on\n" : "off\n")
            << "target reporting interval: " << interval.As (Time::S) << "\n"
            << "average event sleep time:  " << wait.As (Time::MS)    << "\n"
            << "total simulatino run time: " << stop.As (Time::S)
            << std::endl;
  
  Ptr<Hold> h = Create<Hold> (wait, interval);
  h->Event ();

  Simulator::Stop (stop);
  ShowProgress spinner (interval);
  spinner.SetVerbose (verbose);

  Timestamp ts;
  ts.stamp ();

  std::cout << std::endl;
  std::cout << "Start wall clock:   " << ts.string ()
            << " (" << ts.last << ")"
            << std::endl;
  
  Simulator::Run ();

  ts.stamp ();
  std::cout << "Elapsed wall clock: " << ts.diff << "s" << std::endl;

  
  Simulator::Destroy ();

}
