/*
 * lr-wpan-mac-trailer.h
 *
 *  Created on: Mar 4, 2011
 *      Author: kwong
 */

#ifndef LRWPANMACTRAILER_H_
#define LRWPANMACTRAILER_H_

#include "ns3/trailer.h"
#include <stdint.h>

namespace ns3 {

/**
 * The length in octets of the IEEE 802.15.4 MAC FCS field
 */

static const uint16_t LRWPAN_MAC_FCS_LENGTH = 2;

class LrWpanMacTrailer : public Trailer
{
public:
  LrWpanMacTrailer ();
  ~LrWpanMacTrailer ();

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
};
} // namespace ns3
#endif /* LRWPANMACTRAILER_H_ */
