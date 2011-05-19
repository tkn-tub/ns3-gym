/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 */

#ifndef SHADOWING_LOSS_MODEL_H
#define SHADOWING_LOSS_MODEL_H


#include "discrete-time-loss-model.h"
#include <ns3/random-variable.h>

namespace ns3 {


/**
 * \ingroup lte
 *
 * \brief This class models the propagation loss model due to the shadowing
 */
class ShadowingLossModel : public DiscreteTimeLossModel
{

public:
  ShadowingLossModel ();

  /**
   * \param mu mu parameter of the lognormal distribution
   * \param sigma sigma parameter of the lognormal distribution
   * \param samplingPeriod the interval every time the model should be updated
   */
  ShadowingLossModel (double mu, double sigma, double samplingPeriod);

  virtual ~ShadowingLossModel ();

  static TypeId GetTypeId (void);

  /**
   * Set the value of the shadowing loss model, expressed in dB
   * \param sh the shadowing loss value
   */
  void SetValue (double sh);


  /**
   * Get the value of the shadowing loss model, expressed in dB
   * \return the value of the shadowing loss model
   */
  double GetValue (void);

private:
  LogNormalVariable m_randVariable;
  double m_shadowingValue;
};

}

#endif /* SHADOWING_LOSS_MODEL_H */
