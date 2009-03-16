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

#include <stdint.h>
#include <list>
#include "ns3/buffer.h"
#include "ns3/nstime.h"
#include "ns3/dot11s-codes.h"
namespace ns3 {
/**
 * \ingroup mesh
 */
class WifiBeaconTimingElementUnit : public RefCountBase
{
public:
  WifiBeaconTimingElementUnit();
  void SetAID(uint8_t aid);
  void SetLastBeacon(uint16_t last_beacon);
  void SetBeaconInterval(uint16_t beacon_interval);

  uint8_t GetAID();
  uint16_t GetLastBeacon();
  uint16_t GetBeaconInterval();
private:
  //Least significant octet of AID:
  uint8_t AID;
  //Last time we received a beacon in accordance with a
  //local TSF measured in 256 microseconds unit:
  uint16_t LastBeacon;
  //Beacon interval of remote mesh point:
  uint16_t BeaconInterval;
};
/**
 * \ingroup mesh
 * This type is a list of timing elements obtained from neigbours with their beacons:
 */
typedef Ptr<WifiBeaconTimingElementUnit>  WifiBeaconTimingElementPointer;

/**
 * \ingroup mesh
 */
typedef std::list<WifiBeaconTimingElementPointer> NeighboursTimingUnitsList;

/**
 * \ingroup mesh
 */
class WifiBeaconTimingElement
{
public:
  WifiBeaconTimingElement();
  //This methods are needed for beacon collision
  //avoidance module:
  NeighboursTimingUnitsList
  GetNeighboursTimingElementsList();
  //The arguments of the following methods are different
  //from internalBeaconTimingElementUnint. This was made
  //for better communication with peer manager.
  //BeaconTimingElement class should convert it into
  //proper types:
  void   AddNeighboursTimingElementUnit(
    uint16_t aid,
    Time  last_beacon, //MicroSeconds!
    Time  beacon_interval //MicroSeconds!
  );
  void   DelNeighboursTimingElementUnit(
    uint16_t aid,
    Time  last_beacon, //MicroSeconds!
    Time  beacon_interval //MicroSeconds!
  );
  void   ClearTimingElement();
  uint16_t TIMESTAMP_TO_U16(Time x)
  {
    return ((uint16_t)((x.GetMicroSeconds() >> 8)&0xffff));
  };
  uint16_t BEACON_INTERVAL_TO_U16(Time x)
  {
    return ((uint16_t)(x.GetMicroSeconds() >>10)&0xffff);
  };
  uint8_t AID_TO_U8(uint16_t x)
  {
    return (uint8_t)(x&0xff);
  };
  //Serialize-deserialize methods:
  uint32_t  GetSerializedSize () const;
  Buffer::Iterator Serialize (Buffer::Iterator i) const;
  Buffer::Iterator Deserialize (Buffer::Iterator i);
private:
  static uint8_t ElementId() {
    return (uint8_t)IE11S_BEACON_TIMING;
  }
  NeighboursTimingUnitsList  m_neighbours;
  //The maximum size of this element:
  const static uint16_t DEFAULT_MAX_SIZE = 255*5 +2;
  uint16_t  m_maxSize;
  uint16_t  m_numOfUnits;
};
}//namespace ns3
#endif
