/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Fraunhofer FKIE
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author:
 *  Sascha Alexander Jopen <jopen@cs.uni-bonn.de>
 */
#ifndef LR_WPAN_LQI_TAG_H
#define LR_WPAN_LQI_TAG_H

#include <ns3/tag.h>

namespace ns3 {

class Tag;

class LrWpanLqiTag : public Tag
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  /**
   * Create a LrWpanLqiTag with the default LQI 0 .
   */
  LrWpanLqiTag (void);

  /**
   * Create a LrWpanLqiTag with the given LQI value.
   */
  LrWpanLqiTag (uint8_t lqi);

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual void Print (std::ostream &os) const;

  /**
   * Set the LQI to the given value.
   *
   * @param lqi the value of the LQI to set
   */
  void Set (uint8_t lqi);
  /**
   * Get the LQI value.
   *
   * @return the LQI value
   */
  uint8_t Get (void) const;
private:
  uint8_t m_lqi;
};


}
#endif /* LR_WPAN_LQI_TAG_H */
