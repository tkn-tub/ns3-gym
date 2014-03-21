/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 */
#include "error-rate-model.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (ErrorRateModel);

TypeId ErrorRateModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ErrorRateModel")
    .SetParent<Object> ()
  ;
  return tid;
}

double
ErrorRateModel::CalculateSnr (WifiMode txMode, double ber) const
{
  // This is a very simple binary search.
  double low, high, precision;
  low = 1e-25;
  high = 1e25;
  precision = 1e-12;
  while (high - low > precision)
    {
      NS_ASSERT (high >= low);
      double middle = low + (high - low) / 2;
      if ((1 - GetChunkSuccessRate (txMode, middle, 1)) > ber)
        {
          low = middle;
        }
      else
        {
          high = middle;
        }
    }
  return low;
}

} // namespace ns3
