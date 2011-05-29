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
 * Authors:
 *  Gary Pei <guangyu.pei@boeing.com>
 */

#include <ns3/mobility-model.h>
#include <ns3/wpan-spectrum-propagation-loss-model.h>
#include <math.h>

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (WpanSpectrumPropagationLossModel);

WpanSpectrumPropagationLossModel::WpanSpectrumPropagationLossModel ()
{
  m_simpleLossModel = 0;
}

WpanSpectrumPropagationLossModel::~WpanSpectrumPropagationLossModel ()
{
  m_simpleLossModel = 0;
}

TypeId
WpanSpectrumPropagationLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WpanSpectrumPropagationLossModel")
    .SetParent<SpectrumPropagationLossModel> ()
    .AddConstructor<WpanSpectrumPropagationLossModel> ()
  ;
  return tid;
}

Ptr<SpectrumValue>
WpanSpectrumPropagationLossModel::DoCalcRxPowerSpectralDensity (Ptr<const SpectrumValue> txPsd,
                                                                Ptr<const MobilityModel> a,
                                                                Ptr<const MobilityModel> b) const
{
  Ptr<SpectrumValue> rxPsd = Copy<SpectrumValue> (txPsd);
  Values::iterator vit = rxPsd->ValuesBegin ();
  Bands::const_iterator fit = rxPsd->ConstBandsBegin ();

  NS_ASSERT (a);
  NS_ASSERT (b);

  //double d = a->GetDistanceFrom (b);

  while (vit != rxPsd->ValuesEnd ())
    {
      NS_ASSERT (fit != rxPsd->ConstBandsEnd ());
      *vit /= CalculateLossFromSimpleLossModel (a, b); // Prx = Ptx / loss
      ++vit;
      ++fit;
    }
  return rxPsd;
}

double
WpanSpectrumPropagationLossModel::CalculateLossFromSimpleLossModel (Ptr<const MobilityModel> a,
                                                                    Ptr<const MobilityModel> b) const
{
  double loss = 1.0;

  if (m_simpleLossModel)
    {
      loss = m_simpleLossModel->CalcRxPower (0.0, ConstCast<MobilityModel> (a), ConstCast<MobilityModel> (b));
      loss = pow (10.0, loss / 10.0);
      loss = 1.0 / loss;
    }

  return loss;
}


}
