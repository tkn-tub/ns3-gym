/*
 * lr-wpan-mac-trailer.cc
 *
 *  Created on: Mar 4, 2011
 *      Author: kwong
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
