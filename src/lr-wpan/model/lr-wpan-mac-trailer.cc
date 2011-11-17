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
 * Author: kwong yin <kwong-sang.yin@boeing.com>
 */

#include "lr-wpan-mac-trailer.h"
#include "ns3/object.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LrWpanMacTrailer);

LrWpanMacTrailer::LrWpanMacTrailer ()
{

}

LrWpanMacTrailer::~LrWpanMacTrailer ()
{

}

TypeId LrWpanMacTrailer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LrWpanMacTrailer")
    .SetParent<Trailer> ()
    .AddConstructor<LrWpanMacTrailer> ()
  ;
  return tid;
}

TypeId LrWpanMacTrailer::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void LrWpanMacTrailer::Print (std::ostream &os) const
{

}

uint32_t LrWpanMacTrailer::GetSerializedSize (void) const
{
  return LRWPAN_MAC_FCS_LENGTH;
}

void LrWpanMacTrailer::Serialize (Buffer::Iterator start) const
{
  start.Prev (LRWPAN_MAC_FCS_LENGTH);
  start.WriteU16 (0);
}

uint32_t LrWpanMacTrailer::Deserialize (Buffer::Iterator start)
{
  return LRWPAN_MAC_FCS_LENGTH;
}

} //namespace ns3
