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
 * \ingroup core
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
class RngStream {
public:  //public api
  RngStream ();
  RngStream (const RngStream&);
  void InitializeStream(); // Separate initialization
  void ResetStartStream ();
  void ResetStartSubstream ();
  void ResetNextSubstream ();
  void ResetNthSubstream(uint32_t N);
  void SetAntithetic (bool a);
  void IncreasedPrecis (bool incp);
  bool SetSeeds (const uint32_t seed[6]);
  void AdvanceState (int32_t e, int32_t c);
  void GetState (uint32_t seed[6]) const;
  double RandU01 ();
  int32_t RandInt (int32_t i, int32_t j);
public: //public static api
  static bool SetPackageSeed (uint32_t seed);
  static bool SetPackageSeed (const uint32_t seed[6]);
  static void GetPackageSeed (uint32_t seed[6]);
  static void SetPackageRun (uint32_t run);
  static uint32_t GetPackageRun (void);
  static bool CheckSeed(const uint32_t seed[6]);
  static bool CheckSeed(uint32_t seed);
private: //members
  double Cg[6], Bg[6], Ig[6];
  bool anti, incPrec;
  double U01 ();
  double U01d ();
  static uint32_t EnsureGlobalInitialized (void);
private: //static data
  static double nextSeed[6];
};

} //namespace ns3

#endif
 

