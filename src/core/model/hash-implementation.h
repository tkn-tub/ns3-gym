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

#ifndef HASHIMPLEMENTATION_H
#define HASHIMPLEMENTATION_H

#include "simple-ref-count.h"

namespace ns3 {

/**
 * \ingroup hash
 *
 * Hash value opaque types
 */
typedef uint32_t Hash32_t;
typedef uint64_t Hash64_t;


/**
 *  \ingroup hash
 *
 *  \brief Hash function implementation base class
 */
class HashImplementation : public SimpleRefCount<HashImplementation>
{
public:
  /**
   * Compute 32-bit hash of a byte buffer
   *
   * \param [in] buffer pointer to the beginning of the buffer
   * \param [in] size length of the buffer, in bytes
   * \return 32-bit hash of the buffer
   */
  virtual Hash32_t  GetHash32  (const char * buffer, const size_t size) = 0;
  /**
   * Compute 64-bit hash of a byte buffer.
   *
   * Default implementation returns 32-bit hash, with a warning.
   *
   * \param [in] buffer pointer to the beginning of the buffer
   * \param [in] size length of the buffer, in bytes
   * \return 64-bit hash of the buffer
   */
  virtual Hash64_t  GetHash64  (const char * buffer, const size_t size);
  /**
   * Restore initial state
   */
  virtual void clear (void) = 0;
  /*
   * Destructor
   */
  virtual ~HashImplementation () {} ;
};  // HashImplementation

  
/*--------------------------------------
 *  Hash function implementation
 *  by function pointers and templates
 */

/**
 *
 * \ingroup hash
 *
 * \brief Basic hash function typedefs.
 *
 * See Hash32Implementation<> or Hash64Implementation<>
 */
typedef Hash32_t (*Hash32Function_ptr) (const char *, const size_t);
typedef Hash64_t (*Hash64Function_ptr) (const char *, const size_t);


/**
 * \ingroup hash
 *
 * \brief Template for HashImplementations from 32-bit hash functions
 */
template <Hash32Function_ptr hp>
class Hash32Implementation : public HashImplementation
{
  Hash32_t GetHash32 (const char * buffer, const size_t size)
  {
    return (*hp) (buffer, size);
  }
};  // Hash32Implementation<HashFunction>

/**
 * \ingroup hash
 *
 * \brief Template for HashImplementations from 64-bit hash functions
 */
template <Hash64Function_ptr hp>
class Hash64Implementation : public HashImplementation
{
  Hash64_t GetHash64 (const char * buffer, const size_t size)
  {
    return (*hp) (buffer, size);
  }
  Hash32_t GetHash32 (const char * buffer, const size_t size)
  {
    Hash64_t hash = GetHash64(buffer, size);
    return (Hash32_t *)(&hash);
  }
};  // Hash32Implementation<HashFunction>

}  // namespace ns3

#endif /* HASHIMPLEMENTATION_H */

