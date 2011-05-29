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
#ifndef WPAN_SPECTRUM_PROPAGATION_LOSS_MODEL_H
#define WPAN_SPECTRUM_PROPAGATION_LOSS_MODEL_H

#include <ns3/spectrum-propagation-loss-model.h>
#include <ns3/propagation-loss-model.h>

namespace ns3 {

class MobilityModel;

/**
 * \brief propagation loss model for WPAN that uses simple PropagationLossModel
 */
class WpanSpectrumPropagationLossModel : public SpectrumPropagationLossModel
{

public:
  WpanSpectrumPropagationLossModel ();
  ~WpanSpectrumPropagationLossModel ();

  static TypeId GetTypeId ();


  virtual Ptr<SpectrumValue> DoCalcRxPowerSpectralDensity (Ptr<const SpectrumValue> txPsd,
                                                           Ptr<const MobilityModel> a,
                                                           Ptr<const MobilityModel> b) const;


private:
  double CalculateLossFromSimpleLossModel (Ptr<const MobilityModel> a,
                                           Ptr<const MobilityModel> b) const;
  Ptr<PropagationLossModel> m_simpleLossModel;

};

}

#endif /* WPAN_SPECTRUM_PROPAGATION_LOSS_MODEL_H */
