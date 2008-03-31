/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2006 Georgia Tech Research Corporation
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


// Ported from:
// Georgia Tech Network Simulator - Manage 32 bit unsigned sequence numbers
// George F. Riley.  Georgia Tech, Spring 2002

// Class to manage arithmetic operations on sequence numbers (mod 2^32)

#include "sequence-number.h"

bool operator< (const SequenceNumber l, const SequenceNumber r) 
{ // Account for 32 bit rollover
  if (l.seq > 0xc0000000 && r.seq < 0x40000000) return true; // Rollover
  return l.seq < r.seq;
}

bool operator<= (const SequenceNumber l, const SequenceNumber r) 
{ // Account for 32 bit rollover
  if (l.seq > 0xc0000000 && r.seq < 0x40000000) return true; // Rollover
  return l.seq <= r.seq;
}

bool operator> (const SequenceNumber l, const SequenceNumber r) 
{ // Account for 32 bit rollover
  if (l.seq > 0xc0000000 && r.seq < 0x40000000) return false; // Rollover
  return l.seq > r.seq;
}

bool operator>= (const SequenceNumber l, const SequenceNumber r) 
{ // Account for 32 bit rollover
  if (l.seq > 0xc0000000 && r.seq < 0x40000000) return false; // Rollover
  return l.seq >= r.seq;
}

// Non-Member Arithmetic operators
SequenceNumber  operator+ (const SequenceNumber l, const SequenceNumber r)
{
  return SequenceNumber (l.seq + r.seq);
}

SequenceNumber operator- (const SequenceNumber l, const SequenceNumber r)
{ // This assumes l is always bigger than r (allows for rollover)
  if (l.seq >= r.seq) return SequenceNumber (l.seq-r.seq);
  return SequenceNumber ((MAX_SEQ - r.seq) + l.seq + 1);  // Adjust for rollover
}

