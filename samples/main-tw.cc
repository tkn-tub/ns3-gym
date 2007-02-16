/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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
 */

#include <string>
#include "ns3/debug.h"
#include "ns3/trace-writer.h"

using namespace ns3;

int main (int argc, char *argv[])
{
  NS3_TRACEALL("TraceWriter Test")

  TraceWriter writer1;
  writer1.Open("trace-writer-test.txt");
  writer1.Write("writer1.char*\n");
  writer1.Write(std::string("writer1.string\n"));
  writer1.Close();

  TraceWriter writer2;
  writer2.Open(std::string("trace-writer-test.txt"));
  writer2.Write("writer2.char*\n");
  writer2.Write(std::string("writer2.string\n"));
  writer2.Close();

  TraceWriter writer3("trace-writer-test.txt");
  writer3.Write("writer3.char*\n");
  writer3.Write(std::string("writer3.string\n"));
  writer3.Close();

  TraceWriter writer4(std::string("trace-writer-test.txt"));
  writer4.Write("writer4.char*\n");
  writer4.Write(std::string("writer4.string\n"));
  writer4.Close();

  return 0;
}
