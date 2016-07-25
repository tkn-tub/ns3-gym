/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 Sébastien Deronne
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
 * Author: Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#ifndef DSSS_PARAMETER_SET_H
#define DSSS_PARAMETER_SET_H

#include <stdint.h>
#include <ostream>
#include "ns3/buffer.h"
#include "ns3/wifi-information-element.h"

namespace ns3 {

/**
 * \brief The DSSS Parameter Set
 * \ingroup wifi
 *
 * This class knows how to serialise and deserialise the DSSS Parameter Set.
 */
class DsssParameterSet : public WifiInformationElement
{
public:
  DsssParameterSet ();
  void SetDsssSupported (uint8_t DsssSupported);
  
  /**
   * Set the Current Channel field in the DsssParameterSet information element.
   *
   * \param qosInfo the CurrentChannel field in the DsssParameterSet information element
   */
  void SetCurrentChannel (uint8_t currentChannel);
  
  /*
   * Return the Current Channel field in the DsssParameterSet information element.
   *
   * \return the Current Channel field in the DsssParameterSet information element
   */
  uint8_t GetCurrentChannel (void) const;
  
  WifiInformationElementId ElementId () const;
  uint8_t GetInformationFieldSize () const;
  void SerializeInformationField (Buffer::Iterator start) const;
  uint8_t DeserializeInformationField (Buffer::Iterator start, uint8_t length);
    
  /**
   * This information element is a bit special in that it is only
   * included if the STA does support DSSS. To support this we
   * override the Serialize and GetSerializedSize methods of
   * WifiInformationElement.
   *
   * \param start
   *
   * \return an iterator
   */
  Buffer::Iterator Serialize (Buffer::Iterator start) const;
  /**
   * Return the serialized size of this DSSS Parameter Set.
   *
   * \return the serialized size of this DSSS Parameter Set
   */
  uint16_t GetSerializedSize () const;

private:
  uint8_t m_currentChannel;

  //This is used to decide whether this element should be added to the frame or not
  bool m_dsssSupported;
};

std::ostream &operator << (std::ostream &os, const DsssParameterSet &dsssParameterSet);
std::istream &operator >> (std::istream &is, DsssParameterSet &dsssParameterSet);

ATTRIBUTE_HELPER_HEADER (DsssParameterSet);

} //namespace ns3

#endif /* DSSS_PARAMETER_SET_H */
