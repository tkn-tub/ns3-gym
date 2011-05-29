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
#include "ns3/lr-wpan-error-rate-model.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("LrWpanErrorRateModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LrWpanErrorRateModel);

TypeId
LrWpanErrorRateModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LrWpanErrorRateModel")
    .SetParent<Object> ()
    .AddConstructor<LrWpanErrorRateModel> ()
  ;
  return tid;
}

LrWpanErrorRateModel::LrWpanErrorRateModel ()
{
}

double
LrWpanErrorRateModel::GetChunkSuccessRate (double snr, uint32_t nbits) const
{
  double ber = 0.0;

  for (uint32_t k = 2; k <= 16; k++)
    {
      ber += pow (-1.0, (double) k) * BinomialCoefficient (k,16) * exp (20.0 * snr * (1.0 / k - 1.0));
    }

  ber = ber * 8.0 / 15.0 / 16.0;

  ber = std::min (ber, 1.0);
  double retval = pow (1.0 - ber, nbits);
  return retval;
}

uint64_t
LrWpanErrorRateModel::Factorial (uint32_t k) const
{
  uint64_t fact = 1;
  while (k > 0)
    {
      fact *= k;
      k--;
    }
  return fact;
}

uint32_t LrWpanErrorRateModel::BinomialCoefficient (uint32_t k, uint32_t n) const
{
  uint32_t retval = Factorial (n) / Factorial (k) / Factorial (n - k);
  return retval;
}


} // namespace ns3
