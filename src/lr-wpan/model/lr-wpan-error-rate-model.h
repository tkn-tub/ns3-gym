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
 * Author: Gary Pei <guangyu.pei@boeing.com>
 */
#ifndef LR_WPAN_ERROR_RATE_MODEL_H
#define LR_WPAN_ERROR_RATE_MODEL_H

#include <stdint.h>
#include <math.h>
#include "ns3/object.h"

namespace ns3 {

/**
 * Model the error rate for IEEE 802.15.4 2.4 GHz AWGN channel for OQPSK
 * the model description can be found in IEEE Std 802.15.4-2006, section
 * E.4.1.7
 */
class LrWpanErrorRateModel : public Object
{
public:
  static TypeId GetTypeId (void);

  LrWpanErrorRateModel ();

  double GetChunkSuccessRate (double snr, uint32_t nbits) const;

private:
  uint64_t Factorial (uint32_t k) const;
  uint32_t BinomialCoefficient (uint32_t k, uint32_t n) const;
};


} // namespace ns3

#endif /* LR_WPAN_ERROR_RATE_MODEL_H */
