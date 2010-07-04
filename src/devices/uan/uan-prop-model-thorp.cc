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
#include "uan-prop-model-thorp.h"
#include "uan-tx-mode.h"
#include "ns3/double.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("UanPropModelThorp");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (UanPropModelThorp);

UanPropModelThorp::UanPropModelThorp ()
{
}

UanPropModelThorp::~UanPropModelThorp ()
{
}

TypeId
UanPropModelThorp::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UanPropModelThorp")
    .SetParent<Object> ()
    .AddConstructor<UanPropModelThorp> ()
    .AddAttribute ("SpreadCoef",
                   "Spreading coefficient used in calculation of Thorp's approximation",
                   DoubleValue (1.5),
                   MakeDoubleAccessor (&UanPropModelThorp::m_SpreadCoef),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

double
UanPropModelThorp::GetPathLossDb (Ptr<MobilityModel> a, Ptr<MobilityModel> b, UanTxMode mode)
{
  double distKyd = a->GetDistanceFrom (b) / 1.093613298; // Convert from dB/km to dB/kyd

  return m_SpreadCoef * 10.0 * log10 (distKyd)
         + distKyd * GetAttenDbKyd (mode.GetCenterFreqHz () / 1000.0);
}

UanPdp
UanPropModelThorp::GetPdp (Ptr<MobilityModel> a, Ptr<MobilityModel> b, UanTxMode mode)
{
  return UanPdp::CreateImpulsePdp ();
}

Time
UanPropModelThorp::GetDelay (Ptr<MobilityModel> a, Ptr<MobilityModel> b, UanTxMode mode)
{
  return Seconds (a->GetDistanceFrom (b) / 1500.0);
}

double
UanPropModelThorp::GetAttenDbKyd (double freqKhz)
{

  double fsq = freqKhz * freqKhz;
  double atten;

  if (freqKhz < 0.4)
    {
      atten = 0.11 * fsq / (1 + fsq) + 44 * fsq / (4100 + freqKhz)
        + 2.75 * 0.0001 * fsq + 0.003;
    }
  else
    {
      atten = 0.002 + 0.11 * (freqKhz / (1 + freqKhz)) + 0.011 * freqKhz;
    }

  return atten;
}

} // namespace ns3
