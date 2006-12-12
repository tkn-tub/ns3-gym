/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
 * All rights reserved.
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

#include "ns3/simulator.h"
#include "ns3/system-wall-clock-ms.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace ns3;


bool g_debug = false;

class Bench 
{
public:
  void ReadDistribution (std::istream &istream);
  void SetTotal (uint32_t total);
  void RunBench (void);
private:
  void Cb (void);
  std::vector<uint64_t> m_distribution;
  std::vector<uint64_t>::const_iterator m_current;
  uint32_t m_n;
  uint32_t m_total;
};

void 
Bench::SetTotal (uint32_t total)
{
  m_total = total;
}

void
Bench::ReadDistribution (std::istream &input)
{
  double data;
  while (!input.eof ()) 
    {
      if (input >> data) 
        {
          uint64_t ns = (uint64_t) (data * 1000000000);
          m_distribution.push_back (ns);
        } 
      else 
        {
          input.clear ();
          std::string line;
          input >> line;
        }
    }
}

void
Bench::RunBench (void) 
{
  SystemWallClockMs time;
  double init, simu;
  time.Start ();
  for (std::vector<uint64_t>::const_iterator i = m_distribution.begin ();
       i != m_distribution.end (); i++) 
    {
      Simulator::Schedule (NanoSeconds (*i), &Bench::Cb, this);
    }
  init = time.End ();

  m_current = m_distribution.begin ();

  time.Start ();
  Simulator::Run ();
  simu = time.End ();

  std::cout <<
      "init n=" << m_distribution.size () << ", time=" << init << "s" << std::endl <<
      "simu n=" << m_n << ", time=" <<simu << "s" << std::endl <<
      "init " << ((double)m_distribution.size ()) / init << " insert/s, avg insert=" <<
      init / ((double)m_distribution.size ())<< "s" << std::endl <<
      "simu " << ((double)m_n) / simu<< " hold/s, avg hold=" << 
      simu / ((double)m_n) << "s" << std::endl
      ;
}

void
Bench::Cb (void)
{
  if (m_n > m_total) 
    {
      return;
    }
  if (m_current == m_distribution.end ()) 
    {
      m_current = m_distribution.begin ();
    }
  if (g_debug) 
    {
      std::cerr << "event at " << Simulator::Now ().GetSeconds () << "s" << std::endl;
    }
  Simulator::Schedule (NanoSeconds (*m_current), &Bench::Cb, this);
  m_current++;
  m_n++;
}

void
PrintHelp (void)
{
  std::cout << "bench-simulator filename [options]"<<std::endl;
  std::cout << "  filename: a string which identifies the input distribution. \"-\" represents stdin." << std::endl;
  std::cout << "  Options:"<<std::endl;
  std::cout << "      --list: use std::list scheduler"<<std::endl;
  std::cout << "      --map: use std::map cheduler"<<std::endl;
  std::cout << "      --heap: use Binary Heap scheduler"<<std::endl;
  std::cout << "      --log=filename: log scheduler events for the event replay utility."<<std::endl;
  std::cout << "      --debug: enable some debugging"<<std::endl;
}

int main (int argc, char *argv[])
{
  char const *filename = argv[1];
  std::istream *input;
  if (argc == 1)
    {
      PrintHelp ();
      return 0;
    }
  argc-=2;
  argv+= 2;
  if (strcmp (filename, "-") == 0) 
    {
      input = &std::cin;
    } 
  else 
    {
      input = new std::ifstream (filename);
    }
  while (argc > 0) 
    {
      if (strcmp ("--list", argv[0]) == 0) 
        {
          Simulator::SetLinkedList ();
        } 
      else if (strcmp ("--heap", argv[0]) == 0) 
        {
          Simulator::SetBinaryHeap ();
        } 
      else if (strcmp ("--map", argv[0]) == 0) 
        {
          Simulator::SetStdMap ();
        } 
      else if (strcmp ("--debug", argv[0]) == 0) 
        {
          g_debug = true;
        } 
      else if (strncmp ("--log=", argv[0],strlen ("--log=")) == 0) 
        {
          char const *filename = argv[0] + strlen ("--log=");
          Simulator::EnableLogTo (filename);
        }
      argc--;
      argv++;
  }
  Bench *bench = new Bench ();
  bench->ReadDistribution (*input);
  bench->SetTotal (20000);
  bench->RunBench ();

  return 0;
}
