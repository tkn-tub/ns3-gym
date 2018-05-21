/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015
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
 * Authors: Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#ifndef CF_PARAMETER_SET_H
#define CF_PARAMETER_SET_H

#include "ns3/wifi-information-element.h"

namespace ns3 {

/**
 * \brief The CF Parameter Set
 * \ingroup wifi
 *
 * This class knows how to serialise and deserialise the CF Parameter Set.
 */
class CfParameterSet : public WifiInformationElement
{
public:
  CfParameterSet ();
  /**
   * Set PCF supported function
   * \param pcfSupported the PCF supported indicator
   */
  void SetPcfSupported (uint8_t pcfSupported);

  /**
   * Element ID function
   * \returns the wifi information element ID
   */
  WifiInformationElementId ElementId () const;
  /**
   * Get information field size function
   * \returns the information field size
   */
  uint8_t GetInformationFieldSize () const;
  /**
   * Serialize information field function
   * \param start the iterator
   */
  void SerializeInformationField (Buffer::Iterator start) const;
  /**
   * Deserialize information field function
   * \param start the iterator
   * \param length the length
   * \returns the size
   */
  uint8_t DeserializeInformationField (Buffer::Iterator start, uint8_t length);

  /**
   * Return the CFP Count in DTIM frames unit.
   *
   * \return the CFP Count in DTIM frames unit
   */
  uint8_t GetCFPCount (void) const;
  /**
   * Return the CFP Period in DTIM frames unit.
   *
   * \return the CFP Period in DTIM frames unit
   */
  uint8_t GetCFPPeriod (void) const;
  /**
   * Return the CFP MaxDuration in microseconds.
   *
   * \return the CFP MaxDuration in microseconds.
   */
  uint64_t GetCFPMaxDurationUs (void) const;
  /**
   * Return the CFP DurRemaining in microseconds.
   *
   * \return the CFP DurRemaining in microseconds
   */
  uint64_t GetCFPDurRemainingUs (void) const;

  /**
   * Set the CFP Count in DTIM frames unit.
   *
   * \param cfpcount the CFP Count in DTIM frames unit
   */
  void SetCFPCount (uint8_t cfpcount);
  /**
   * Set the CFP Period in DTIM frames unit.
   *
   * \param cfpperiod the CFP Period in DTIM frames unit
   */
  void SetCFPPeriod (uint8_t cfpperiod);
  /**
   * Set the CFP MaxDuration in microseconds.
   *
   * \param cfpmaxduration the CFP MaxDuration in microseconds
   */
  void SetCFPMaxDurationUs (uint64_t cfpmaxduration);
  /**
   * Set the CFP MaxDuration in microseconds.
   *
   * \param cfpdurremaining the CFP DurRemaining in microseconds
   */
  void SetCFPDurRemainingUs (uint64_t cfpdurremaining);

  /**
   * This information element is a bit special in that it is only
   * included if the STA is a QoS STA. To support this we
   * override the Serialize and GetSerializedSize methods of
   * WifiInformationElement.
   *
   * \param start
   *
   * \return an iterator
   */
  Buffer::Iterator Serialize (Buffer::Iterator start) const;
  /**
   * Return the serialized size of this EDCA Parameter Set.
   *
   * \return the serialized size of this EDCA Parameter Set
   */
  uint16_t GetSerializedSize () const;


private:
  uint8_t m_CFPCount;                 ///< CFP Count
  uint8_t m_CFPPeriod;                ///< CFP Period
  uint64_t m_CFPMaxDuration;          ///< CFP MaxDuration
  uint64_t m_CFPDurRemaining;         ///< CFP DurRemaining

  /// This is used to decide if this element should be added to the frame or not
  uint8_t m_pcfSupported;
};

std::ostream &operator << (std::ostream &os, const CfParameterSet &cfParameterSet);

} //namespace ns3

#endif /* CF_PARAMETER_SET_H */
