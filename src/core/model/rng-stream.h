/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
//  Copyright (C) 2001  Pierre L'Ecuyer (lecuyer@iro.umontreal.ca)
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
// Modified for ns-3 by: Rajib Bhattacharjea<raj.b@gatech.edu>

#ifndef RNGSTREAM_H
#define RNGSTREAM_H
#include <string>
#include <stdint.h>

namespace ns3 {

/**
 * \ingroup randomvariable 
 *
 * \brief Combined Multiple-Recursive Generator MRG32k3a
 *
 * This class is the combined multiple-recursive random number
 * generator called MRG32k3a.  The ns3::RandomVariableBase class
 * holds a static instance of this class.  The details of this
 * class are explained in:
 * http://www.iro.umontreal.ca/~lecuyer/myftp/papers/streams00.pdf
 */
class RngStream
{
public:
  RngStream (uint32_t seed, uint64_t stream, uint64_t substream);
  RngStream (const RngStream&);
  /**
   * Generate the next random number for this stream.
   * Uniformly distributed between 0 and 1.
   */
  double RandU01 (void);

private:
  void AdvanceNthBy (uint64_t nth, int by, double state[6]);

  double m_currentState[6];
};

} // namespace ns3

#endif
 

