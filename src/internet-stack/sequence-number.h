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

#ifndef __seq_h__
#define __seq_h__

#include <stdint.h>

#define MAX_SEQ ((uint32_t)0xffffffff)

/**
 * \ingroup tcp
 *
 * \brief Class to manage arithmetic operations on sequence numbers (mod 2^32)
 */
class SequenceNumber {
public:
  SequenceNumber () : seq(0) { }
  SequenceNumber (const uint32_t s) : seq (s) { }

  operator uint32_t () const { return seq;}

  SequenceNumber& operator= (const uint32_t s) { seq = s; return *this;}

  SequenceNumber& operator+= (const uint32_t s) { seq += s; return *this;}
  SequenceNumber  operator++ ()              { seq++; return *this;}
  SequenceNumber  operator++ (int)           { SequenceNumber ss (seq); seq++; return ss;}
  SequenceNumber& operator-= (const uint32_t s) { seq -= s; return *this;}
  SequenceNumber  operator-- ()              { seq--; return *this;}
  SequenceNumber  operator-- (int)           { SequenceNumber ss (seq); seq--; return ss;}
public:
  uint32_t seq;
};

// Comparison operators

bool operator< (const SequenceNumber l, const SequenceNumber r);
bool operator<= (const SequenceNumber l, const SequenceNumber r); 
bool operator> (const SequenceNumber l, const SequenceNumber r);
bool operator>= (const SequenceNumber l, const SequenceNumber r);

// Non-member arithmetic operators
SequenceNumber  operator+ (const SequenceNumber l, const SequenceNumber r);
SequenceNumber  operator- (const SequenceNumber l, const SequenceNumber r);

#endif


