/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Lawrence Livermore National Laboratory
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
 * Author: Peter D. Barnes, Jr. <pdbarnes@llnl.gov>
 */

#include <iostream>  // boolalpha
#include <string>

#include "ns3/core-module.h"


using namespace ns3;


std::string g_val4="val4 default";

bool SetVal4 (std::string val)
{
  g_val4 = val;
  return true;
}


int main (int argc, char *argv[])
{

  int         val1 = 1;
  bool        val2 = false;
  std::string val3 = "val3 default";
  
  CommandLine cmd;
  cmd.Usage ("CommandLine example program.\n"
             "\n"
             "This little program demonstrates how to use CommandLine.");
  cmd.AddValue ("val1", "an int argument",          val1);
  cmd.AddValue ("val2", "a bool argument",          val2);
  cmd.AddValue ("val3", "a string argument",        val3);
  cmd.AddValue ("val4", "a string via callback", MakeCallback (SetVal4));
  cmd.Parse (argc, argv);

  std::cout << "val1:\t" << val1 << std::endl;
  std::cout << "val2:\t"   << std::boolalpha << val2
            << std::noboolalpha << std::endl;
  std::cout << "val3:\t\"" << val3 << "\"" << std::endl;
  std::cout << "val4:\t\"" << g_val4 << "\"" << std::endl;

  return 0;
}
