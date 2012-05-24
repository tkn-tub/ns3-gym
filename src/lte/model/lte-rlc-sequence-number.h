/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */

#ifndef LTE_RLC_SEQUENCE_NUMBER_H
#define LTE_RLC_SEQUENCE_NUMBER_H

#include <limits>
#include <iostream>
#include <stdint.h>

// #include "ns3/lte-rlc.h"

namespace ns3 {


class SequenceNumber10
{
public:
  SequenceNumber10 ()
    : m_value (0),
      m_modulusBase (0)
  {}

  explicit SequenceNumber10 (uint16_t value)
    : m_value (value % 1024),
      m_modulusBase (0)
  {}

  SequenceNumber10 (SequenceNumber10 const &value)
    : m_value (value.m_value),
      m_modulusBase (value.m_modulusBase)
  {}

  SequenceNumber10& operator= (uint16_t value)
  {
    m_value = value % 1024;
    return *this;
  }


  /**
   * \brief Extracts the numeric value of the sequence number
   * \returns the sequence number value
   */
  uint16_t GetValue () const
  {
    return m_value;
  }

  void SetModulusBase (SequenceNumber10 modulusBase)
  {
    m_modulusBase = modulusBase.m_value;
  }

  void SetModulusBase (uint16_t modulusBase)
  {
    m_modulusBase = modulusBase;
  }

   // postfix ++
  SequenceNumber10 operator++ (int)
  {
    SequenceNumber10 retval (m_value);
    m_value = (m_value + 1) % 1024;
    return retval;
  }

  SequenceNumber10 operator + (uint16_t delta) const
  {
    SequenceNumber10 ret ((m_value + delta) % 1024);
    ret.SetModulusBase (m_modulusBase);
    return ret;
  }

  SequenceNumber10 operator - (uint16_t delta) const
  {
    SequenceNumber10 ret ((m_value - delta) % 1024);
    ret.SetModulusBase (m_modulusBase);
    return ret;
  }

  uint16_t operator - (const SequenceNumber10 &other) const
  {
    uint16_t diff = m_value - other.m_value;
    return (diff);
  }

  bool operator > (const SequenceNumber10 &other) const
  {
    SequenceNumber10 v1 ((m_value - m_modulusBase) % 1024);
    SequenceNumber10 v2 ((other.m_value - other.m_modulusBase) % 1024);
    return ( v1.GetValue () > v2.GetValue () );
  }

  bool operator == (const SequenceNumber10 &other) const
  {
    return (m_value == other.m_value);
  }

  bool operator != (const SequenceNumber10 &other) const
  {
    return (m_value != other.m_value);
  }

  bool operator <= (const SequenceNumber10 &other) const
  {
    return (!this->operator> (other));
  }

  bool operator >= (const SequenceNumber10 &other) const
  {
    return (this->operator> (other) || this->operator== (other));
  }

  bool operator < (const SequenceNumber10 &other) const
  {
    return !this->operator> (other) && m_value != other.m_value;
  }


  friend std::ostream & operator<< (std::ostream& os, const SequenceNumber10 &val);

private:
  uint16_t m_value;
  uint16_t m_modulusBase;
};


} // namespace ns3

#endif // LTE_RLC_SEQUENCE_NUMBER_H
