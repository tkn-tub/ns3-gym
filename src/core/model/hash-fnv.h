/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Lawrence Livermore National Laboratory
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
 * Author: Peter D. Barnes, Jr. <pdbarnes@llnl.gov>
 */

#ifndef HASH_FNV_H
#define HASH_FNV_H

#include "ns3/hash.h"

namespace ns3 {
  
  namespace HashImplNS {

/**
 *  \ingroup hash
 *
 *  \brief Fnv hash function implementation
 *
 */
class Fnv : public HashImplementation
{
public:
  /**
   * Compute 32-bit hash of a byte buffer
   *
   * \param [in] buffer pointer to the beginning of the buffer
   * \param [in] size length of the buffer, in bytes
   * \return 32-bit hash of the buffer
   */
  Hash::Hash32_t  GetHash32  (const char * buffer, const size_t size);
  /**
   * Compute 64-bit hash of a byte buffer.
   *
   * \param [in] buffer pointer to the beginning of the buffer
   * \param [in] size length of the buffer, in bytes
   * \return 64-bit hash of the buffer
   */
  Hash::Hash64_t  GetHash64  (const char * buffer, const size_t size);

private:
  /**
   * Seed value
   *
   * This has to be a constant for all MPI ranks to generate
   * the same hash from the same string.
   */
  enum seed
    {
      SEED = 0x8BADF00D  // Ate bad food
    };

};  // class Fnv

}  // namespace HashImplNS
  
} // namespace ns3

#endif  /* HASH_FNV_H */
