/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */


#ifndef WIFI_TIMING_ELEMENT_H
#define WIFI_TIMING_ELEMENT_H

#include <vector>
#include "ns3/nstime.h"
#include "ns3/wifi-information-element.h"

namespace ns3 {
namespace dot11s {
/**
 * \ingroup dot11s
 * \brief Describes one unit of beacon timing element
 */
class IeBeaconTimingUnit : public RefCountBase
{
public:
  IeBeaconTimingUnit ();
  void SetAid (uint8_t aid);
  void SetLastBeacon (uint16_t last_beacon);
  void SetBeaconInterval (uint16_t beacon_interval);

  uint8_t GetAid () const;
  uint16_t GetLastBeacon () const;
  uint16_t GetBeaconInterval () const;
  /**
   * \brief Least significant octet of AID:
   */
  uint8_t m_aid;
  /**
   * \brief Last time we received a beacon in accordance with a
   * local TSF measured in 256 microseconds unit:
   */
  uint16_t m_lastBeacon;
  /**
   * \brief Beacon interval of remote mesh point:
   */
  uint16_t m_beaconInterval;
  friend bool operator== (const IeBeaconTimingUnit & a, const IeBeaconTimingUnit & b);
};

/**
 * \ingroup dot11s
 * \brief See 7.3.2.89 of 802.11s draft 2.07
 */
class IeBeaconTiming : public WifiInformationElement
{
public:
  /**
   * \ingroup dot11s
   * This type is a list of timing elements obtained from neigbours with their beacons:
   */
  typedef std::vector< Ptr<IeBeaconTimingUnit> > NeighboursTimingUnitsList;

  IeBeaconTiming ();
  /**
   * This methods are needed for beacon collision
   * avoidance module:
   */
  NeighboursTimingUnitsList GetNeighboursTimingElementsList ();
  void AddNeighboursTimingElementUnit (
    uint16_t aid,
    Time last_beacon,
    Time beacon_interval
  );
  void   DelNeighboursTimingElementUnit (
    uint16_t aid,
    Time  last_beacon,
    Time  beacon_interval
  );
  void   ClearTimingElement ();
private:
  WifiElementId ElementId () const {
    return IE11S_BEACON_TIMING;
  }
  uint8_t  GetInformationSize () const;
  void SerializeInformation (Buffer::Iterator i) const;
  uint8_t DeserializeInformation (Buffer::Iterator i, uint8_t length);
  void PrintInformation (std::ostream& os) const; 
  /**
   * Converters:
   */
  static uint16_t TimestampToU16 (Time x);
  static uint16_t BeaconIntervalToU16 (Time x);
  static uint8_t AidToU8 (uint16_t x);

  NeighboursTimingUnitsList  m_neighbours;
  /**
   * Timing element parameters:
   */
  uint16_t  m_numOfUnits;
  friend bool operator== (const IeBeaconTiming & a, const IeBeaconTiming & b);
};
bool operator== (const IeBeaconTiming & a, const IeBeaconTiming & b);
bool operator== (const IeBeaconTimingUnit & a, const IeBeaconTimingUnit & b);
} // namespace dot11s
} //namespace ns3
#endif
