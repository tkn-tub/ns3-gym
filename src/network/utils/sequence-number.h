/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2008-2010 INESC Porto
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
// Author: Gustavo J. A. M. Carneiro  <gjc@inescporto.pt> <gjcarneiro@gmail.com>
//

#ifndef __NS3_SEQ_NUM_H__
#define __NS3_SEQ_NUM_H__

#include <limits>
#include <iostream>
#include <stdint.h>

namespace ns3 {

/**
 * \brief Generic "sequence number" class
 *
 * This class can be used to handle sequence numbers.  In networking
 * protocols, sequence numbers are fixed precision integer numbers
 * that are used to order events relative to each other.  A sequence
 * number is expected to increase over time but, since it has a
 * limited number of bits, the number will "wrap around" from the
 * maximum value that can represented with the given number of bits
 * back to zero.  For this reason, comparison of two sequence numbers,
 * and subtraction, is non-trivial.  The SequenceNumber class behaves
 * like a number, with the usual arythmetic operators implemented, but
 * knows how to correctly compare and subtract sequence numbers.
 *
 * This is a templated class.  To use it you need to supply two
 * fundamental types as template parameters: NUMERIC_TYPE and
 * SIGNED_TYPE.  For instance, SequenceNumber<uint32_t, int32_t> gives
 * you a 32-bit sequence number, while SequenceNumber<uint16_t,
 * int16_t> is a 16-bit one.  For your convenience, these are
 * typedef'ed as SequenceNumber32 and SequenceNumber16, respectively.
 */
template<typename NUMERIC_TYPE, typename SIGNED_TYPE>
class SequenceNumber
{
public:
  SequenceNumber ()
    : m_value (0)
  {}

  /**
   * \brief Constructs a SequenceNumber with the given value
   * \param value the sequence number value
   */ 
  explicit SequenceNumber (NUMERIC_TYPE value)
    : m_value (value)
  {}

  // copy contructor
  SequenceNumber (SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> const &value)
    : m_value (value.m_value)
  {}

  // assignment from a plain number
  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE>& operator= (NUMERIC_TYPE value)
  {
    m_value = value;
    return *this;
  }

  // assignment from a sequence number
  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE>& operator= (SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> const &value)
  {
    m_value = value.m_value;
    return *this;
  }

#if 0
  // a SequenceNumber implicitly converts to a plain number, but not the other way around
  operator NUMERIC_TYPE () const
  {
    return m_value;
  }
#endif

  /**
   * \brief Extracts the numeric value of the sequence number
   * \returns the sequence number value
   */ 
  NUMERIC_TYPE GetValue () const
  {
    return m_value;
  }

  // prefix ++
  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> operator++ ()
  {
    m_value++;
    return *this;
  }

  // postfix ++
  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> operator++ (int)
  {
    SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> retval (m_value);
    m_value++;
    return retval;
  }

  // prefix --
  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> operator-- ()
  {
    m_value--;
    return *this;
  }

  // postfix --
  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> operator-- (int)
  {
    SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> retval (m_value);
    m_value--;
    return retval;
  }

  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE>& operator+= (SIGNED_TYPE value)
  {
    m_value += value;
    return *this;
  }

  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE>& operator-= (SIGNED_TYPE value)
  {
    m_value -= value;
    return *this;
  }

  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> operator + (const SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> &other) const
  {
    return SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> (m_value + other.m_value);
  }

  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> operator + (SIGNED_TYPE delta) const
  {
    return SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> (m_value + delta);
  }

  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> operator - (SIGNED_TYPE delta) const
  {
    return SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> (m_value - delta);
  }

  SIGNED_TYPE operator - (const SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> &other) const
  {
    static const NUMERIC_TYPE maxValue = std::numeric_limits<NUMERIC_TYPE>::max ();
    static const NUMERIC_TYPE halfMaxValue = std::numeric_limits<NUMERIC_TYPE>::max () / 2;
    if (m_value > other.m_value)
      {
        NUMERIC_TYPE diff = m_value - other.m_value;
        if (diff < halfMaxValue)
          {
            return static_cast<SIGNED_TYPE> (diff);
          }
        else
          {
            //      |------------|------------|
            //       ====                  ===
            //          ^                  ^
            //       other.m_value      m_value
            return -(static_cast<SIGNED_TYPE> (maxValue - m_value + 1 + other.m_value));
          }
      }
    else
      {
        NUMERIC_TYPE diff = other.m_value - m_value;
        if (diff < halfMaxValue)
          {
            //      |------------|------------|
            //          ========
            //          ^      ^
            //     m_value   other.m_value
            return -(static_cast<SIGNED_TYPE> (diff));
          }
        else
          {
            //      |------------|------------|
            //       ====                  ===
            //          ^                  ^
            //       m_value      other.m_value
            return static_cast<SIGNED_TYPE> (maxValue - other.m_value + 1 + m_value);
          }
      }
  }


  // Here is the critical part, how the comparison is made taking into
  // account wrap-around.  From RFC 3626:
  //
  //   The sequence number S1 is said to be "greater than" the sequence
  //    number S2 if:
  //
  //           S1 > S2 AND S1 - S2 <= MAXVALUE/2 OR
  //
  //           S2 > S1 AND S2 - S1 > MAXVALUE/2
  bool operator > (const SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> &other) const
  {
    static const NUMERIC_TYPE halfMaxValue = std::numeric_limits<NUMERIC_TYPE>::max () / 2;

    return (((m_value > other.m_value) && (m_value - other.m_value) <= halfMaxValue)
            || ((other.m_value > m_value) && (other.m_value - m_value) > halfMaxValue));
  }

  bool operator == (const SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> &other) const
  {
    return (m_value == other.m_value);
  }

  bool operator != (const SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> &other) const
  {
    return (m_value != other.m_value);
  }

  bool operator <= (const SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> &other) const
  {
    return (!this->operator> (other));
  }

  bool operator >= (const SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> &other) const
  {
    return (this->operator> (other) || this->operator== (other));
  }

  bool operator < (const SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> &other) const
  {
    return !this->operator> (other) && m_value != other.m_value;
  }


  template<typename NUMERIC_TYPE2, typename SIGNED_TYPE2>
  friend std::ostream & operator<< (std::ostream& os, const SequenceNumber<NUMERIC_TYPE2, SIGNED_TYPE2> &val);

  template<typename NUMERIC_TYPE2, typename SIGNED_TYPE2>
  friend std::istream & operator >> (std::istream &is, const SequenceNumber<NUMERIC_TYPE2, SIGNED_TYPE2> &val);

private: // unimplemented operators
  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE>& operator+= (SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> const &value);
  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE>& operator-= (SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> const &value);
  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> operator* (const SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE>& b) const;
  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> operator/ (const SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE>& b) const;
  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> operator% (const SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE>& b) const;
  bool operator ! () const;
  bool operator && (const SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE>& b) const;
  bool operator || (const SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE>& b) const;
  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> operator~ () const;
  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> operator& (const SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE>& b) const;
  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> operator| (const SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE>& b) const;
  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> operator^ (const SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE>& b) const;
  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> operator<< (const SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE>& b) const;
  SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> operator>> (const SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE>& b) const;
  int operator* ();
  //SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE>* operator& ();

private:
  NUMERIC_TYPE m_value;
};


template<typename NUMERIC_TYPE, typename SIGNED_TYPE>
std::ostream &
operator<< (std::ostream& os, const SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> &val)
{
  os << val.m_value;
  return os;
}

template<typename NUMERIC_TYPE, typename SIGNED_TYPE>
std::istream & operator >> (std::istream &is, const SequenceNumber<NUMERIC_TYPE, SIGNED_TYPE> &val)
{
  is >> val.m_value;
  return is;
}


typedef SequenceNumber<uint32_t, int32_t> SequenceNumber32;
typedef SequenceNumber<uint16_t, int16_t> SequenceNumber16;

} // namespace ns3

#endif


