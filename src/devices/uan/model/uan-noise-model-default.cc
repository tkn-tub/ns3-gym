/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 * Author: Leonard Tracy <lentracy@gmail.com>
 */

#include "uan-noise-model-default.h"
#include "ns3/double.h"

#include <cmath>

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (UanNoiseModelDefault);

UanNoiseModelDefault::UanNoiseModelDefault ()
{

}

UanNoiseModelDefault::~UanNoiseModelDefault ()
{
}

TypeId
UanNoiseModelDefault::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UanNoiseModelDefault")
    .SetParent<Object> ()
    .AddConstructor<UanNoiseModelDefault> ()
    .AddAttribute ("Wind", "Wind speed in m/s",
                   DoubleValue (1),
                   MakeDoubleAccessor (&UanNoiseModelDefault::m_wind),
                   MakeDoubleChecker<double> (0))
    .AddAttribute ("Shipping", "Shipping contribution to noise between 0 and 1",
                   DoubleValue (0),
                   MakeDoubleAccessor (&UanNoiseModelDefault::m_shipping),
                   MakeDoubleChecker<double> (0,1))
  ;
  return tid;
}

// Common acoustic noise formulas.  These can be found
// in "Priniciples of Underwater Sound" by Robert J. Urick
double
UanNoiseModelDefault::GetNoiseDbHz (double fKhz) const
{
  double turb, win, ship, thermal, noise;
  turb = 17.0 - 30.0 * log10 (fKhz);
  turb = pow (10.0, turb * 0.1);

  ship = 40.0 + 20.0 * (m_shipping - 0.5) + 26.0 * log10 (fKhz) - 60.0 * log10 (fKhz + 0.03);
  ship = pow (10.0, (ship * 0.1));

  win = 50.0 + 7.5 * pow (m_wind, 0.5) + 20.0 * log10 (fKhz) - 40.0 * log10 (fKhz + 0.4);
  win = pow (10.0, m_wind * 0.1);

  thermal = -15 + 20 * log10 (fKhz);
  thermal = pow (10, thermal * 0.1);

  noise = 10 * log10 (turb + ship + win + thermal);

  return noise;
}

} // namespace ns3
