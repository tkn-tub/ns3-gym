/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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

#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>

#include "ns3/core-module.h"

using namespace ns3;


bool g_debug = false;

std::string g_me;
#define LOG(x)   std::cout << x << std::endl
    #define LOGME(x) LOG (g_me << x)
    #define DEB(x) if (g_debug) { LOGME (x); }

// Output field width
int g_fwidth = 6;

/// Bench class
class Bench
{
public:
  /**
   * constructor
   * \param population the population
   * \param total the total
   */
  Bench (const uint32_t population, const uint32_t total)
    : m_population (population),
      m_total (total),
      m_count (0)
  {
  }

  /**
   * Set random stream
   * \param stream the random variable stream
   */
  void SetRandomStream (Ptr<RandomVariableStream> stream)
  {
    m_rand = stream;
  }

  /**
   * Set population function
   * \param population the population
   */
  void SetPopulation (const uint32_t population)
  {
    m_population = population;
  }

  /**
   * Set total function
   * \param total
   */
  void SetTotal (const uint32_t total)
  {
    m_total = total;
  }

  /// Run function
  void RunBench (void);
private:
  /// callback function
  void Cb (void);

  Ptr<RandomVariableStream> m_rand; ///< random variable
  uint32_t m_population; ///< population
  uint32_t m_total; ///< total
  uint32_t m_count; ///< count 
};

void
Bench::RunBench (void)
{
  SystemWallClockMs time;
  double init, simu;

  DEB ("initializing");
  m_count = 0;


  time.Start ();
  for (uint32_t i = 0; i < m_population; ++i)
    {
      Time at = NanoSeconds (m_rand->GetValue ());
      Simulator::Schedule (at, &Bench::Cb, this);
    }
  init = time.End ();
  init /= 1000;
  DEB ("initialization took " << init << "s");

  DEB ("running");
  time.Start ();
  Simulator::Run ();
  simu = time.End ();
  simu /= 1000;
  DEB ("run took " << simu << "s");

  LOG (std::setw (g_fwidth) << init <<
       std::setw (g_fwidth) << (m_population / init) <<
       std::setw (g_fwidth) << (init / m_population) <<
       std::setw (g_fwidth) << simu <<
       std::setw (g_fwidth) << (m_count / simu) <<
       std::setw (g_fwidth) << (simu / m_count));

}

void
Bench::Cb (void)
{
  if (m_count >= m_total)
    {
      return;
    }
  DEB ("event at " << Simulator::Now ().GetSeconds () << "s");

  Time after = NanoSeconds (m_rand->GetValue ());
  Simulator::Schedule (after, &Bench::Cb, this);
  ++m_count;
}


Ptr<RandomVariableStream>
GetRandomStream (std::string filename)
{
  Ptr<RandomVariableStream> stream = 0;

  if (filename == "")
    {
      LOGME ("using default exponential distribution");
      Ptr<ExponentialRandomVariable> erv = CreateObject<ExponentialRandomVariable> ();
      erv->SetAttribute ("Mean", DoubleValue (100));
      stream = erv;
    }
  else
    {
      std::istream *input;

      if (filename == "-")
        {
          LOGME ("using event distribution from stdin");
          input = &std::cin;
        }
      else
        {
          LOGME ("using event distribution from " << filename);
          input = new std::ifstream (filename.c_str ());
        }

      double value;
      std::vector<double> nsValues;

      while (!input->eof ())
        {
          if (*input >> value)
            {
              uint64_t ns = (uint64_t) (value * 1000000000);
              nsValues.push_back (ns);
            }
          else
            {
              input->clear ();
              std::string line;
              *input >> line;
            }
        }
      LOGME ("found " << nsValues.size () << " entries");
      Ptr<DeterministicRandomVariable> drv = CreateObject<DeterministicRandomVariable> ();
      drv->SetValueArray (&nsValues[0], nsValues.size ());
      stream = drv;
    }

  return stream;
}



int main (int argc, char *argv[])
{

  bool schedCal  = false;
  bool schedHeap = false;
  bool schedList = false;
  bool schedMap  = true;

  uint32_t pop   =  100000;
  uint32_t total = 1000000;
  uint32_t runs  =       1;
  std::string filename = "";

  CommandLine cmd;
  cmd.Usage ("Benchmark the simulator scheduler.\n"
             "\n"
             "Event intervals are taken from one of:\n"
             "  an exponential distribution, with mean 100 ns,\n"
             "  an ascii file, given by the --file=\"<filename>\" argument,\n"
             "  or standard input, by the argument --file=\"-\"\n"
             "In the case of either --file form, the input is expected\n"
             "to be ascii, giving the relative event times in ns.");
  cmd.AddValue ("cal",   "use CalendarSheduler",          schedCal);
  cmd.AddValue ("heap",  "use HeapScheduler",             schedHeap);
  cmd.AddValue ("list",  "use ListSheduler",              schedList);
  cmd.AddValue ("map",   "use MapScheduler (default)",    schedMap);
  cmd.AddValue ("debug", "enable debugging output",       g_debug);
  cmd.AddValue ("pop",   "event population size (default 1E5)",         pop);
  cmd.AddValue ("total", "total number of events to run (default 1E6)", total);
  cmd.AddValue ("runs",  "number of runs (default 1)",    runs);
  cmd.AddValue ("file",  "file of relative event times",  filename);
  cmd.AddValue ("prec",  "printed output precision",      g_fwidth);
  cmd.Parse (argc, argv);
  g_me = cmd.GetName () + ": ";
  g_fwidth += 6;  // 5 extra chars in '2.000002e+07 ': . e+0 _

  ObjectFactory factory ("ns3::MapScheduler");
  if (schedCal)
    {
      factory.SetTypeId ("ns3::CalendarScheduler");
    }
  if (schedHeap)
    {
      factory.SetTypeId ("ns3::HeapScheduler");
    }
  if (schedList)
    {
      factory.SetTypeId ("ns3::ListScheduler");
    }
  Simulator::SetScheduler (factory);

  LOGME (std::setprecision (g_fwidth - 6));
  DEB ("debugging is ON");

  LOGME ("scheduler: " << factory.GetTypeId ().GetName ());
  LOGME ("population: " << pop);
  LOGME ("total events: " << total);
  LOGME ("runs: " << runs);

  Bench *bench = new Bench (pop, total);
  bench->SetRandomStream (GetRandomStream (filename));

  // table header
  LOG ("");
  LOG (std::left << std::setw (g_fwidth) << "Run #" <<
       std::left << std::setw (3 * g_fwidth) << "Inititialization:" <<
       std::left << std::setw (3 * g_fwidth) << "Simulation:");
  LOG (std::left << std::setw (g_fwidth) << "" <<
       std::left << std::setw (g_fwidth) << "Time (s)" <<
       std::left << std::setw (g_fwidth) << "Rate (ev/s)" <<
       std::left << std::setw (g_fwidth) << "Per (s/ev)" <<
       std::left << std::setw (g_fwidth) << "Time (s)" <<
       std::left << std::setw (g_fwidth) << "Rate (ev/s)" <<
       std::left << std::setw (g_fwidth) << "Per (s/ev)" );
  LOG (std::setfill ('-') <<
       std::right << std::setw (g_fwidth) << " " <<
       std::right << std::setw (g_fwidth) << " " <<
       std::right << std::setw (g_fwidth) << " " <<
       std::right << std::setw (g_fwidth) << " " <<
       std::right << std::setw (g_fwidth) << " " <<
       std::right << std::setw (g_fwidth) << " " <<
       std::right << std::setw (g_fwidth) << " " <<
       std::setfill (' ')
       );

  // prime
  DEB ("priming");
  std::cout << std::left << std::setw (g_fwidth) << "(prime)";
  bench->RunBench ();

  bench->SetPopulation (pop);
  bench->SetTotal (total);
  for (uint32_t i = 0; i < runs; i++)
    {
      std::cout << std::setw (g_fwidth) << i;

      bench->RunBench ();
    }

  LOG ("");
  Simulator::Destroy ();
  delete bench;
  return 0;
}
