/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#ifndef RANDOM_STREAM_H
#define RANDOM_STREAM_H

#include <stdint.h>
#include <list>
#include "ns3/random-variable-stream.h"

namespace ns3 {

/**
 * A simple wrapper around RngStream to make testing
 * of the code easier.
 */
class RandomStream
{
public:
  virtual ~RandomStream ();
  /**
   * Get integer between min and max (including min and max).
   *
   * \param min lower bound (inclusive)
   * \param max upper bound (inclusive)
   * \return a random number between min and max (including min and max)
   */
  virtual uint32_t GetNext (uint32_t min, uint32_t max) = 0;

 /**
  * Assign a fixed random variable stream number to the random variables
  * used by this model.  Return the number of streams (possibly zero) that
  * have been assigned.
  *
  * \param stream first stream index to use
  * \return the number of stream indices assigned by this model
  */
  virtual int64_t AssignStreams (int64_t stream) = 0;
};

class RealRandomStream : public RandomStream
{
public:
  RealRandomStream ();
  virtual uint32_t GetNext (uint32_t min, uint32_t max);

 /**
  * Assign a fixed random variable stream number to the random variables
  * used by this model.  Return the number of streams (possibly zero) that
  * have been assigned.
  *
  * \param stream first stream index to use
  * \return the number of stream indices assigned by this model
  */
  virtual int64_t AssignStreams (int64_t stream);

private:
  /// Provides uniform random variables.
  Ptr<UniformRandomVariable> m_stream;
};

class TestRandomStream : public RandomStream
{
public:
  /**
   * Add the given value to the list.
   *
   * \param v
   */
  void AddNext (uint32_t v);
  virtual uint32_t GetNext (uint32_t min, uint32_t max);

 /**
  * Assign a fixed random variable stream number to the random variables
  * used by this model.  Return the number of streams (possibly zero) that
  * have been assigned.
  *
  * \param stream first stream index to use
  * \return the number of stream indices assigned by this model
  */
  virtual int64_t AssignStreams (int64_t stream);

private:
  std::list<uint32_t> m_nexts;
};

} // namespace ns3

#endif /* RANDOM_STREAM_H */
