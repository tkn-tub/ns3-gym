/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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

#include <ctime>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <fstream>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/abort.h"

using namespace ns3;

static const uint64_t US_PER_NS = (uint64_t)1000;
static const uint64_t US_PER_SEC = (uint64_t)1000000;
static const uint64_t NS_PER_SEC = (uint64_t)1000000000;

uint64_t
GetRealtimeInNs (void)
{
  struct timeval tv;
  gettimeofday (&tv, NULL);

  uint64_t nsResult = tv.tv_sec * NS_PER_SEC + tv.tv_usec * US_PER_NS;
  return nsResult;
}

void
PerfFile (FILE *file, uint32_t n, const char *buffer, uint32_t size)
{
  for (uint32_t i = 0; i < n; ++i)
    {
      if (std::fwrite (buffer, 1, size, file) !=  size)
        {
          NS_ABORT_MSG ("PerfFile():  fwrite error");
        }
    }
}

void
PerfStream (ostream &stream, uint32_t n, const char *buffer, uint32_t size)
{

  for (uint32_t i = 0; i < n; ++i)
    {
      stream.write (buffer, size);
    }
}

int 
main (int argc, char *argv[])
{
  uint32_t n = 100000;
  uint32_t iter = 50;
  bool doStream = false;
  bool binmode = true;
 

  CommandLine cmd;
  cmd.AddValue ("n", "How many times to write (defaults to 100000", n);
  cmd.AddValue ("iter", "How many times to run the test looking for a min (defaults to 50)", iter);
  cmd.AddValue ("doStream", "Run the C++ I/O benchmark otherwise the C I/O ", doStream);
  cmd.AddValue ("binmode", "Select binary mode for the C++ I/O benchmark (defaults to true)", binmode);
  cmd.Parse (argc, argv);

  uint64_t result = std::numeric_limits<uint64_t>::max ();

  char buffer[1024];

  if (doStream)
    {
      //
      // This will probably run on a machine doing other things.  Run it some
      // relatively large number of times and try to find a minimum, which
      // will hopefully represent a time when it runs free of interference.
      //
      for (uint32_t i = 0; i < iter; ++i)
        {
          ofstream stream;
          if (binmode)
            {
              stream.open ("streamtest", std::ios_base::binary | std::ios_base::out);
            }
          else
            {
              stream.open ("streamtest", std::ios_base::out);
            }

          uint64_t start = GetRealtimeInNs ();
          PerfStream (stream, n, buffer, 1024);
          uint64_t et = GetRealtimeInNs () - start;
          result = min (result, et);
          stream.close ();
          std::cout << "."; std::cout.flush ();
        }
      cout << std::endl;
    }
  else
    {
      //
      // This will probably run on a machine doing other things.  Run it some
      // relatively large number of times and try to find a minimum, which
      // will hopefully represent a time when it runs free of interference.
      //
      for (uint32_t i = 0; i < iter; ++i)
        {
          FILE *file = fopen ("filetest", "w");

          uint64_t start = GetRealtimeInNs ();
          PerfFile (file, n, buffer, 1024);
          uint64_t et = GetRealtimeInNs () - start;
          result = std::min (result, et);
          fclose (file);
          file = 0;
          std::cout << "."; std::cout.flush ();
        }
      std::cout << std::endl;
    }
  std::cout << argv[0] << ": " << result << "ns" << std::endl;
}
