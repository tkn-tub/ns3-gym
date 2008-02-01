/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2006 Georgia Tech Research Corporation
// All rights reserved.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: Rajib Bhattacharjea<raj.b@gatech.edu>
//


// This is a port of Data PDU Headers from:
// Georgia Tech Network Simulator
// George F. Riley.  Georgia Tech, Spring 2002

#include <iostream>
#include <algorithm>

#include <string.h>

#include "pending-data.h"
#include "ns3/packet.h"
#include "ns3/fatal-error.h"
namespace ns3
{

namespace Serializable
{
  uint8_t* GetSize (uint8_t* b, uint32_t& r, uint32_t& s)
  { // Get the size of the next size field
    if (sizeof(s) > r)
    {
      NS_FATAL_ERROR ("Serialization error; remaining " << r
           << " thissize " << sizeof(s) << std::endl);
    }
    r -= sizeof (s); // Reduce remaining for next time
    memcpy (&s, b, sizeof(s));
    return b + sizeof (s);
  }
}

PendingData::PendingData () : size (0), data (0),
               msgSize (0), responseSize (0)
{
}

PendingData::PendingData (uint32_t s, uint8_t* d, uint32_t msg, uint32_t resp)
  : size (s), data (0), msgSize (msg), responseSize (resp)
{ // Make a copy of the data
  if (d)
    {
      data = new uint8_t[s];
      memcpy (data, d, s);
    }
}

PendingData::PendingData(const std::string& s) 
  : size (s.length () + 1), data ((uint8_t*)strdup(s.c_str ())),
    msgSize (0), responseSize (0)
{
}

PendingData::PendingData(const PendingData& c)
  : size (c.Size ()), data (0),
    msgSize (c.msgSize), responseSize (c.responseSize)
{ // Copy constructor
  if (c.Contents())
    { // Has data
      data = new uint8_t[Size ()];
      memcpy(data, c.Contents (), Size ());
    }
}

PendingData::~PendingData()
{ // destructor
  if (data)
    {
	  delete [] data;
    }
}

PendingData* PendingData::Copy () const
{
  return new PendingData (*this);
};

PendingData* PendingData::CopyS (uint32_t s)
{ // Copy, but with new size (assumes no associated data);
  return new PendingData (s, 0, msgSize, responseSize);
}

PendingData* PendingData::CopySD (uint32_t s, uint8_t* d)
{ // Copy, but with new size (assumes no associated data);
  return new PendingData (s, d, msgSize, responseSize);
}

void PendingData::Clear ()
{ // Remove all pending data
  if (data)
    {
      delete [] data; // Free memory if used
    }
  data = 0;
  size = 0;
}

void PendingData::Add (uint32_t s, const uint8_t* d)
{
  if (data)
    { // PendingData exists, realloc and copy
      uint8_t* n = new uint8_t[Size () + s];
      memcpy(n, data, Size ());
      if (d)
        { // New data specified
          memcpy(n + Size (), d, s); // Append the new data
        }
      else
        {
          memset(n + Size (), 0, s); // Apend zeros
        }
      delete [] data;           // Delete the old data
      data = n;                 // Points to new one
    }
  else
    { // No existing data, see if new data
      if (d)
        {
          data = new uint8_t[s];
          memcpy (data, d, s);
        }
    }
  size += s;
}

void PendingData::Remove(uint32_t s)
{
  uint32_t r = s > Size () ? Size () : s;

  size -= r;          // Reduce size from current
  if (data)
    { // data actually exists, realloc and copy
      if (size)
        {
          uint8_t* d = new uint8_t[Size ()];
          memcpy(d, data, Size ());
          delete [] data;
          data = d;
        }
      else
        { // Zero size, so don't need the data anymore
          delete [] data;
          data = NULL;
        }
    }
}

uint32_t PendingData::SizeFromSeq (const SequenceNumber& f, const SequenceNumber& o)
{
  uint32_t o1 = OffsetFromSeq (f,o); // Offset to start of unused data
  return SizeFromOffset (o1);      // Amount of data after offset
}

uint32_t PendingData::SizeFromOffset (uint32_t o)
{ // Find out how much data is available from offset
  if (o > size) return 0;     // No data at requested offset
  return size - o;            // Available data after offset
}

uint32_t PendingData::OffsetFromSeq (const SequenceNumber& f, const SequenceNumber& o)
{ // f is the first sequence number in this data, o is offset sequence
  if (o < f) 
    {
      return 0; // HuH?  Shouldn't happen
    }
  return o - f;
}

PendingData* PendingData::CopyFromOffset (uint32_t s, uint32_t o)
{ // Make a copy of data from starting position "o" for "s" bytes
  // Return NULL if results in zero length data
  uint32_t s1 = std::min (s, SizeFromOffset (o)); // Insure not beyond end of data
  if (s1 == 0)
    {
      return NULL;   // No data requested
    }
  if (data)
    { // Actual data exists, make copy and return it
      uint8_t* d1 = new uint8_t[s1];  // Allocate memory for the copy
      memcpy (d1, &data[o], s1); // Copy the data
      PendingData* d = new PendingData (s1, d1, msgSize, responseSize);  // Return copy
      return d;
    }
  else
    { // No actual data, just return non-data pdu of correct size
      return new PendingData (s1, 0, msgSize, responseSize);
    }
}

PendingData* PendingData::CopyFromSeq (uint32_t s, const SequenceNumber& f, const SequenceNumber& o)
{
  PendingData* d = CopyFromOffset (s, OffsetFromSeq(f,o));
  return d;
}

}//namepsace ns3      
  
  




