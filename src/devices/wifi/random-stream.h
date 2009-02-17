/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
#include "ns3/random-variable.h"

namespace ns3 {

/**
 * A simple wrapper around RngStream to make testing 
 * of the code easier.
 */
class RandomStream
{
public:
  virtual ~RandomStream ();
  virtual uint32_t GetNext (uint32_t min, uint32_t max) = 0;
};

class RealRandomStream : public RandomStream
{
public:
  RealRandomStream ();
  virtual uint32_t GetNext (uint32_t min, uint32_t max);
private:
  UniformVariable m_stream;
};

class TestRandomStream : public RandomStream
{
public:
  void AddNext (uint32_t v);
  virtual uint32_t GetNext (uint32_t min, uint32_t max);
private:
  std::list<uint32_t> m_nexts;
};

} // namespace ns3

#endif /* RANDOM_STREAM_H */
