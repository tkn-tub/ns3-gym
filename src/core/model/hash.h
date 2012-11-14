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

#ifndef HASH_H
#define HASH_H

#include <string>

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"

namespace ns3 {

class HashImplementation;

/**
 *  \ingroup hash
 *
 *  \brief Generic Hash function interface
 *
 *  This class provides a generic interface for computing hashes
 *  of buffers.  Various getters return hashes of different lengths.
 *  The choice of hash function can be made at construction by
 *  passing a Ptr<> to the desired HashImplementation.
 *  The available implementations are documented in group hash.
 *  The default implementation is Murmur3.  FNV is also available.
 *
 *  In addition to this class interface, global functions are
 *  defined which use the default hash implementation.
 *
 * \internal
 *
 *  Would be nice to offer longer hashes.  \c uint128_t looks doable,
 *  except that our fallback \c int64x64_t implementation doesn't
 *  offer \c unsigned.
 *
 *  Longer hashes require returning a byte buffer of some sort,
 *  but our \ref buffer class seems a bit overkill for this case.
 *
 */
class Hash
{
public:
  typedef uint32_t Hash32_t;
  typedef uint64_t Hash64_t;

  /**
   * Constructor using the default implementation
   */
  Hash ();
  /**
   * Constructor using the supplied implementation
   *
   * \param [in] hp Ptr<HashImplementation> to the desired implementation
   */
  Hash (Ptr<HashImplementation> hp);
  /**
   * Compute 32-bit hash of a byte buffer
   *
   * \param [in] buffer pointer to the beginning of the buffer
   * \param [in] size length of the buffer, in bytes
   * \return 32-bit hash of the buffer
   */
  Hash32_t  GetHash32  (const char * buffer, const size_t size);
  /**
   * Compute 64-bit hash of a byte buffer
   *
   * \param [in] buffer pointer to the beginning of the buffer
   * \param [in] size length of the buffer, in bytes
   * \return 64-bit hash of the buffer
   */
  Hash64_t  GetHash64  (const char * buffer, const size_t size);

  /**
   * Compute 32-bit hash of a string
   *
   * \param [in] s string to hash
   * \return 32-bit hash of the string
   */
  Hash32_t  GetHash32  (const std::string s);
  /**
   * Compute 64-bit hash of a string
   *
   * \param [in] s string to hash
   * \return 64-bit hash of the string
   */
  Hash64_t  GetHash64  (const std::string s);
  
private:
  Ptr<HashImplementation> m_impl;    /** Hash implementation */
  
};  // Hash

/*************************************************
 **  Global functions
 ************************************************/

/**
 * \ingroup hash
 *
 * Compute 32-bit hash of a byte buffer, using the default hash function
 *
 * \param [in] buffer pointer to the beginning of the buffer
 * \param [in] size length of the buffer, in bytes
 * \return 32-bit hash of the buffer
 */
Hash::Hash32_t Hash32 (const char * buffer, const size_t size);
/**
 * \ingroup hash
 *
 * Compute 64-bit hash of a byte buffer, using the default hash function
 *
 * \param [in] buffer pointer to the beginning of the buffer
 * \param [in] size length of the buffer, in bytes
 * \return 64-bit hash of the buffer
 */
Hash::Hash64_t Hash64 (const char * buffer, const size_t size);

/**
 * \ingroup hash
 *
 * Compute 32-bit hash of a string, using the default hash function
 *
 * \param [in] s string to hash
 * \return 32-bit hash of the string
 */
Hash::Hash32_t Hash32 (const std::string s);
/**
 * \ingroup hash
 *
 * Compute 64-bit hash of a string, using the default hash function
 *
 * \param [in] s string to hash
 * \return 64-bit hash of the string
 */
Hash::Hash64_t Hash64 (const std::string s);


/*************************************************
 **  Hash function implementation class
 ************************************************/
  
/**
 *  \ingroup hash
 *
 *  \brief Hash function implementation base class
 */
#ifndef HASHIMPLEMENTATION_C
#define HASHIMPLEMENTATION_C
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
  virtual Hash::Hash32_t  GetHash32  (const char * buffer, const size_t size) = 0;
  /**
   * Compute 64-bit hash of a byte buffer.
   *
   * Default implementation returns 32-bit hash, with a warning.
   *
   * \param [in] buffer pointer to the beginning of the buffer
   * \param [in] size length of the buffer, in bytes
   * \return 64-bit hash of the buffer
   */
  virtual Hash::Hash64_t  GetHash64  (const char * buffer, const size_t size);
  /*
   * Destructor
   */
  virtual ~HashImplementation () {} ;
};  // HashImplementation
#endif /* HASHIMPLEMENTATION_C */

/**
 *
 * \ingroup hash
 *
 * \brief Basic hash function typedefs.
 *
 * See Hash32Implementation<> or Hash64Implementation<>
 */
typedef Hash::Hash32_t (*Hash32Function_ptr) (const char *, const size_t);
typedef Hash::Hash64_t (*Hash64Function_ptr) (const char *, const size_t);


/**
 * \ingroup hash
 *
 * \brief Template for HashImplementations from 32-bit hash functions
 */
template <Hash32Function_ptr hp>
class Hash32Implementation : public HashImplementation
{
  Hash::Hash32_t GetHash32 (const char * buffer, const size_t size)
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
  Hash::Hash64_t GetHash64 (const char * buffer, const size_t size)
  {
    return (*hp) (buffer, size);
  }
  Hash::Hash32_t GetHash32 (const char * buffer, const size_t size)
  {
    Hash::Hash64_t hash = GetHash64(buffer, size);
    return (Hash::Hash32_t *)(&hash);
  }
};  // Hash32Implementation<HashFunction>

}  // namespace ns3



/*************************************************
 **  Real Hash function implementations
 ************************************************/

#include "ns3/hash-murmur3.h"
#include "ns3/hash-fnv.h"


/*************************************************
 **  Inline implementations for rvo
 ************************************************/

namespace ns3 {
  
/*************************************************
 class Hash implementation, inlined for rvo
*/

inline
Hash::Hash32_t
Hash::GetHash32  (const char * buffer, const size_t size)
{
  return m_impl->GetHash32  (buffer, size);
}

inline
Hash::Hash64_t
Hash::GetHash64  (const char * buffer, const size_t size)
{
  return m_impl->GetHash64  (buffer, size);
}

inline
Hash::Hash32_t
Hash::GetHash32  (const std::string s)
{
  return m_impl->GetHash32  (s.c_str (), s.size ());
}

inline
Hash::Hash64_t
Hash::GetHash64  (const std::string s)
{
  return m_impl->GetHash64  (s.c_str (), s.size ());
}


/*************************************************
  Global hash functions, inlined for rvo
*/

inline
Hash::Hash32_t
Hash32 (const char * buffer, const size_t size)
{
  return Hash().GetHash32 (buffer, size);
}

inline
Hash::Hash64_t
Hash64 (const char * buffer, const size_t size)
{
  return Hash().GetHash64 (buffer, size);
}

inline
Hash::Hash32_t
Hash32 (const std::string s)
{
  return Hash().GetHash32 (s);
}

inline
Hash::Hash64_t
Hash64 (const std::string s)
{
  return Hash().GetHash64 (s);
}


}  // namespace ns3

#endif /* HASH_H */
