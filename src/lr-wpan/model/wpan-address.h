/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 The Boeing Company
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
 *
 *      Author: kwong yin <kwong-sang.yin@boeing.com>
 */


#ifndef WPANADDRESS_H_
#define WPANADDRESS_H_

#include <stdint.h>

namespace ns3 {

class WpanAddress
{
public:
  WpanAddress ();
  WpanAddress (uint16_t addr);
  WpanAddress (uint64_t addr);
  ~WpanAddress ();

  void SetAddress (uint64_t addr);
  void SetAddress (uint16_t addr);

  uint16_t GetShortAddress (void) const;
  uint64_t GetExtAddress (void) const;
private:
  union
  {
    uint64_t m_addr64;                 //8 octet for ext addr
    uint16_t m_addr16;                 //2 octet for short addr
  };
};

} //namespace ns3

#endif /* WPANADDRESS_H_ */
