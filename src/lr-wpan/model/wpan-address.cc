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


#include "ns3/wpan-address.h"

namespace ns3 {

WpanAddress::WpanAddress ()
{

}
WpanAddress::WpanAddress (uint16_t addr)
{
  SetAddress (addr);
}

WpanAddress::WpanAddress (uint64_t addr)
{
  SetAddress (addr);
}

WpanAddress::~WpanAddress ()
{

}

void WpanAddress::SetAddress (uint64_t addr)
{
  m_addr64 = addr;
}

void WpanAddress::SetAddress (uint16_t addr)
{
  m_addr16 = addr;
}



uint16_t WpanAddress::GetShortAddress (void) const
{
  return(m_addr16);
}

uint64_t WpanAddress::GetExtAddress (void) const
{
  return(m_addr64);
}

}  // namespace ns3
