/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 Georgia Tech Research Corporation
 * Copyright (c) 2011 Mathieu Lacage
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
 * Authors: Rajib Bhattacharjea<raj.b@gatech.edu>
 *          Hadi Arbabi<marbabi@cs.odu.edu>
 *          Mathieu Lacage <mathieu.lacage@gmail.com>
 *
 * Modified by Mitch Watrous <watrous@u.washington.edu>
 *
 */
#include "random-variable-stream.h"
#include "assert.h"
#include "boolean.h"
#include "double.h"
#include "integer.h"
#include "string.h"
#include "pointer.h"
#include "log.h"
#include "rng-stream.h"
#include "rng-seed-manager.h"
#include <cmath>
#include <iostream>

NS_LOG_COMPONENT_DEFINE ("RandomVariableStream");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (RandomVariableStream);

TypeId 
RandomVariableStream::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RandomVariableStream")
    .SetParent<Object> ()
    .AddAttribute("Stream",
		  "The stream number for this RNG stream. -1 means \"allocate a stream automatically\". "
		  "Note that if -1 is set, Get will return -1 so that it is not possible to know which "
		  "value was automatically allocated.",
		  IntegerValue(-1),
		  MakeIntegerAccessor(&RandomVariableStream::SetStream,
				      &RandomVariableStream::GetStream),
		  MakeIntegerChecker<int64_t>())
    .AddAttribute("Antithetic", "Set this RNG stream to generate antithetic values",
		  BooleanValue (false),
		  MakeBooleanAccessor(&RandomVariableStream::SetAntithetic,
				      &RandomVariableStream::IsAntithetic),
		  MakeBooleanChecker())
    ;
  return tid;
}

RandomVariableStream::RandomVariableStream()
  : m_rng (0)
{}
RandomVariableStream::~RandomVariableStream()
{
  delete m_rng;
}

void
RandomVariableStream::SetAntithetic(bool isAntithetic)
{
  m_isAntithetic = isAntithetic;
}
bool
RandomVariableStream::IsAntithetic(void) const
{
  return m_isAntithetic;
}
void
RandomVariableStream::SetStream (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);
  // negative values are not legal.
  NS_ASSERT (stream >= -1);
  delete m_rng;
  if (stream == -1)
    {
      // The first 2^63 streams are reserved for automatic stream
      // number assignment.
      uint64_t nextStream = RngSeedManager::GetNextStreamIndex ();
      NS_ASSERT(nextStream <= ((1ULL)<<63));
      m_rng = new RngStream (RngSeedManager::GetSeed (),
                             nextStream,
                             RngSeedManager::GetRun ());
    }
  else
    {
      // The last 2^63 streams are reserved for deterministic stream
      // number assignment.
      uint64_t base = ((1ULL)<<63);
      uint64_t target = base + stream;
      m_rng = new RngStream (RngSeedManager::GetSeed (),
                             target,
                             RngSeedManager::GetRun ());
    }
  m_stream = stream;
}
int64_t
RandomVariableStream::GetStream(void) const
{
  return m_stream;
}

RngStream *
RandomVariableStream::Peek(void) const
{
  return m_rng;
}

NS_OBJECT_ENSURE_REGISTERED(UniformRandomVariable);

TypeId 
UniformRandomVariable::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UniformRandomVariable")
    .SetParent<RandomVariableStream>()
    .AddConstructor<UniformRandomVariable> ()
    .AddAttribute("Min", "The lower bound on the values returned by this RNG stream.",
		  DoubleValue(0),
		  MakeDoubleAccessor(&UniformRandomVariable::m_min),
		  MakeDoubleChecker<double>())
    .AddAttribute("Max", "The upper bound on the values returned by this RNG stream.",
		  DoubleValue(1.0),
		  MakeDoubleAccessor(&UniformRandomVariable::m_max),
		  MakeDoubleChecker<double>())
    ;
  return tid;
}
UniformRandomVariable::UniformRandomVariable ()
{
  // m_min and m_max are initialized after constructor by attributes
}

double 
UniformRandomVariable::GetMin (void) const
{
  return m_min;
}
double 
UniformRandomVariable::GetMax (void) const
{
  return m_max;
}

double 
UniformRandomVariable::GetValue (double min, double max)
{
  double v = min + Peek ()->RandU01 () * (max - min);
  if (IsAntithetic ())
    {
      v = min + (max - v);
    }
  return v;
}
uint32_t 
UniformRandomVariable::GetInteger (uint32_t min, uint32_t max)
{
  NS_ASSERT (min <= max);
  return static_cast<uint32_t> ( GetValue (min, max + 1) );
}

double 
UniformRandomVariable::GetValue (void)
{
  return GetValue (m_min, m_max);
}
uint32_t 
UniformRandomVariable::GetInteger (void)
{
  return (uint32_t)GetValue (m_min, m_max + 1);
}

} // namespace ns3
