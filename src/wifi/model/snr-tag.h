/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
 * Copyright (c) 2009 MIRKO BANCHI
 * Copyright (c) 2013 University of Surrey
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 * Author: Mirko Banchi <mk.banchi@gmail.com>
 * Author: Konstantinos Katsaros <dinos.katsaros@gmail.com>
 */

#ifndef SNR_TAG_H
#define SNR_TAG_H

#include "ns3/packet.h"

namespace ns3 {

class Tag;

class SnrTag : public Tag
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  /**
   * Create a SnrTag with the default snr 0 
   */
  SnrTag();

  /**
   * Create a SnrTag with the given snr value
   * \param snr the given SNR value
   */
  SnrTag(double snr);

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual void Print (std::ostream &os) const;

  /**
   * Set the SNR to the given value.
   *
   * \param snr the value of the snr to set
   */
  void Set (double snr);
  /**
   * Return the SNR value.
   *
   * \return the SNR value
   */
  double Get (void) const;
private:
  double m_snr;  //!< SNR value
};


}
#endif /* SNR_TAG_H */
