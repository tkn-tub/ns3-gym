/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 The Georgia Institute of Technology 
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
 * Authors: Josh Pelkey <jpelkey@gatech.edu>
 */

#include "ns3/log.h"
#include "ns3/fatal-error.h"

#include "nix-vector.h"

NS_LOG_COMPONENT_DEFINE ("NixVector");

namespace ns3 {

typedef std::vector<uint32_t> NixBits_t;

NixVector::NixVector ()
  : m_nixVector (0),
    m_used (0),
    m_currentVectorBitSize (0),
    m_totalBitSize (0)
{
  NS_LOG_FUNCTION_NOARGS ();

  m_nixVector.push_back (0);
}

NixVector::~NixVector ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

NixVector::NixVector (const NixVector &o)
  : m_nixVector (o.m_nixVector),
    m_used (o.m_used),
    m_currentVectorBitSize (o.m_currentVectorBitSize),
    m_totalBitSize (o.m_totalBitSize)
{
}

NixVector &
NixVector::operator = (const NixVector &o)
{
  if (this == &o)
    {
      return *this;
    }
  m_nixVector = o.m_nixVector;
  m_used = o.m_used;
  m_currentVectorBitSize = o.m_currentVectorBitSize;
  m_totalBitSize = o.m_totalBitSize;
  return *this;
}

Ptr<NixVector>
NixVector::Copy (void) const
{
  // we need to invoke the copy constructor directly
  // rather than calling Create because the copy constructor
  // is private.
  return Ptr<NixVector> (new NixVector (*this), false);
}

std::ostream & operator << (std::ostream &os, const NixVector &nix)
{
  nix.DumpNixVector (os); 
  return os;
}

void
NixVector::AddNeighborIndex (uint32_t newBits, uint32_t numberOfBits)
{
  NS_LOG_FUNCTION_NOARGS ();

  if (numberOfBits > 32)
    {
      NS_FATAL_ERROR ("Can't add more than 32 bits to a nix-vector at one time");
    }

  // Check to see if the number
  // of new bits forces the creation of 
  // a new entry into the NixVector vector
  // i.e., we will overflow int o.w.
  if (m_currentVectorBitSize + numberOfBits > 32) 
    {
      if (m_currentVectorBitSize == 32)
        {
          // can't add any more to this vector, so 
          // start a new one
          m_nixVector.push_back (newBits);

          // also reset number of bits in
          // m_currentVectorBitSize
          // because we are working with a new 
          // entry in the vector
          m_currentVectorBitSize = numberOfBits;
          m_totalBitSize += numberOfBits;
        }
      else
        {
          // Put what we can in the remaining portion of the 
          // vector entry
          uint32_t tempBits = newBits;
          tempBits = newBits << m_currentVectorBitSize;
          tempBits |= m_nixVector.back ();
          m_nixVector.back () = tempBits;

          // Now start a new vector entry
          // and push the remaining bits 
          // there
          newBits = newBits >> (32 - m_currentVectorBitSize);
          m_nixVector.push_back (newBits);

          // also reset number of bits in
          // m_currentVectorBitSize
          // because we are working with a new 
          // entry in the vector
          m_currentVectorBitSize = (numberOfBits - (32 - m_currentVectorBitSize));
          m_totalBitSize += numberOfBits;
        }
    }
  else
    {
      // Shift over the newbits by the
      // number of current bits.  This allows 
      // us to logically OR with the present 
      // NixVector, resulting in the new 
      // NixVector
      newBits = newBits << m_currentVectorBitSize;
      newBits |= m_nixVector.back ();

      // Now insert the new NixVector and 
      // increment number of bits for
      // m_currentVectorBitSize and m_totalBitSize
      // accordingly 
      m_nixVector.back () = newBits;
      m_currentVectorBitSize += numberOfBits;
      m_totalBitSize += numberOfBits;
    }
}

uint32_t
NixVector::ExtractNeighborIndex (uint32_t numberOfBits)
{
  NS_LOG_FUNCTION_NOARGS ();

  if (numberOfBits > 32)
    {
      NS_FATAL_ERROR ("Can't extract more than 32 bits to a nix-vector at one time");
    }

  uint32_t vectorIndex = 0;
  uint32_t extractedBits = 0;
  uint32_t totalRemainingBits = GetRemainingBits ();

  if (numberOfBits > totalRemainingBits)
    {
      NS_FATAL_ERROR ("You've tried to extract too many bits of the Nix-vector, " << this << ". NumberBits: " 
                      << numberOfBits << " Remaining: " << totalRemainingBits);
    }

  if (numberOfBits <= 0)
    {
      NS_FATAL_ERROR ("You've specified a number of bits for Nix-vector <= 0!");
    }

  // First determine where in the NixVector 
  // vector we need to extract which depends
  // on the number of used bits and the total
  // number of bits
  vectorIndex = ((totalRemainingBits-1) / 32);

  // Next, determine if this extraction will
  // span multiple vector entries
  if (vectorIndex > 0) // we could span more than one
    {
      if ((numberOfBits-1) > ((totalRemainingBits-1) % 32)) // we do span more than one
        {
          extractedBits = m_nixVector.at (vectorIndex) << (32 - (totalRemainingBits % 32));
          extractedBits = extractedBits >> ((32 - (totalRemainingBits % 32)) 
                                            - (numberOfBits - (totalRemainingBits % 32)));
          extractedBits |= (m_nixVector.at (vectorIndex-1) 
                            >> (32 - (numberOfBits - (totalRemainingBits % 32))));
          m_used += numberOfBits;
          return extractedBits;
        }
    }

  // we don't span more than one
  extractedBits = m_nixVector.at (vectorIndex) << (32 - (totalRemainingBits % 32));
  extractedBits = extractedBits >> (32 - (numberOfBits));
  m_used += numberOfBits;
  return extractedBits;
}

uint32_t
NixVector::GetSerializedSize (void) const
{
  uint32_t totalSizeInBytes = 0;
  totalSizeInBytes = sizeof (m_used) + sizeof (m_currentVectorBitSize) + 
    sizeof (m_totalBitSize) + (4 * m_nixVector.size ());

  return totalSizeInBytes;
}

uint32_t
NixVector::Serialize (uint32_t* buffer, uint32_t maxSize) const
{
  NS_LOG_FUNCTION (this);
  uint32_t* p = buffer;
  uint32_t size = 0;

  if (size + 4 <= maxSize)
    {
      size += 4;
      // grab number of used bits
      *p++ = m_used;
    }
  else
    {
      return 0;
    }

  if (size + 4 <= maxSize)
    {
      size += 4;
      // grab number of current used bits
      // for the front vector
      *p++ = m_currentVectorBitSize;
    }
  else
    {
      return 0;
    }

  if (size + 4 <= maxSize)
    {
      size += 4;
      // grab total bit size
      *p++ = m_totalBitSize;
    }
  else 
    {
      return 0;
    }
  for (uint32_t j = 0; j < m_nixVector.size (); j++)
    {
      if (size + 4 <= maxSize)
        {
          size += 4;
          *p++ = m_nixVector.at (j);
        }
      else
        {
          return 0;
        }
    }

  // Serialized successfully
  return 1;
}

uint32_t
NixVector::Deserialize (const uint32_t* buffer, uint32_t size)
{
  NS_LOG_FUNCTION (this);
  const uint32_t* p = buffer;
  uint32_t sizeCheck = size - 4;

  NS_ASSERT (sizeCheck >= 4);
  m_used = *p++;
  sizeCheck -= 4;

  NS_ASSERT (sizeCheck >= 4);
  m_currentVectorBitSize = *p++;
  sizeCheck -= 4;

  NS_ASSERT (sizeCheck >= 4);
  m_totalBitSize = *p++;
  sizeCheck -= 4;

  // make sure the nix-vector
  // is empty
  m_nixVector.clear ();
  while (sizeCheck > 0)
    {
      NS_ASSERT (sizeCheck >= 4);
      uint32_t nix = *p++;
      m_nixVector.push_back (nix);
      sizeCheck -= 4;
    }

  NS_ASSERT (sizeCheck == 0);

  // return zero if an entire nix-vector was 
  // not deserialized
  return (sizeCheck != 0) ? 0 : 1;
}

void
NixVector::DumpNixVector (std::ostream &os) const
{
  NS_LOG_FUNCTION_NOARGS ();
  uint32_t i = m_nixVector.size ();
  std::vector<uint32_t>::const_reverse_iterator rIter;
  for (rIter = m_nixVector.rbegin (); rIter != m_nixVector.rend (); rIter++)
    {
      uint32_t numBits = BitCount (*rIter);

      // all this work just to get the nix 
      // vector to print out neat

      // if it's not the first entry in the vector, 
      // we may have to add some zeros and fill 
      // out the vector
      if (m_totalBitSize > ((sizeof (uint32_t)*8) * i))
        {
          PrintDec2BinNixFill (*rIter,numBits,os);
        }
      else if (m_totalBitSize%32 == 0)
        {
          PrintDec2BinNix (*rIter,32,os);
        }
      else
        {
          PrintDec2BinNix (*rIter,m_totalBitSize%32,os);
        }

      i--;

      if (i > 0)
        {
          os << "--";
        }
    }
}

uint32_t 
NixVector::GetRemainingBits (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  return (m_totalBitSize - m_used);
}

uint32_t
NixVector::BitCount (uint32_t numberOfNeighbors) const
{
  NS_LOG_FUNCTION_NOARGS ();

  // Given the numberOfNeighbors, return the number 
  // of bits needed (essentially, log2(numberOfNeighbors-1)
  uint32_t bitCount = 0;

  if (numberOfNeighbors < 2) 
    {
      return 1;
    }
  else
    {
      for (numberOfNeighbors -= 1; numberOfNeighbors != 0; numberOfNeighbors >>= 1)
        {
          bitCount++;
        }
      return bitCount;
    }
}

void
NixVector::PrintDec2BinNix (uint32_t decimalNum, uint32_t bitCount, std::ostream &os) const
{
  if(decimalNum == 0)
    {
      for (; bitCount > 0; bitCount--)
        {
          os << 0;
        }
      return;
    }
  if(decimalNum == 1)
    {
      for (; bitCount > 1; bitCount--)
        {
          os << 0;
        }
      os << 1;
    }
  else
    {
      PrintDec2BinNix (decimalNum / 2,bitCount-1, os);
      os << decimalNum % 2;
    }
}

void
NixVector::PrintDec2BinNixFill (uint32_t decimalNum, uint32_t bitCount, std::ostream &os) const
{
  if(decimalNum == 0)
    {
      os << 0;
      return;
    }
  if(decimalNum == 1)
    {
      // check to see if we need to 
      // print out some zeros at the 
      // beginning of the nix vector
      if ((uint32_t)(sizeof (uint32_t)*8) > bitCount)
        {
          for (uint32_t i = ((sizeof (uint32_t)*8)-bitCount); i > 0; i--)
            {
              os << 0;
            }
        }
      os << 1;
    }
  else
    {
      PrintDec2BinNixFill (decimalNum / 2, bitCount, os);
      os << decimalNum % 2;
    }
}

} // namespace ns3
