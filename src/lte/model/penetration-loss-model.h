/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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

#ifndef PENETRATION_LOSS_MODEL_H
#define PENETRATION_LOSS_MODEL_H


#include "discrete-time-loss-model.h"


namespace ns3 {

/**
 * \ingroup lte
 *
 * \brief This class models the propagation loss model due to the penetration loss
 */
class PenetrationLossModel : public DiscreteTimeLossModel
{

public:
  PenetrationLossModel ();
  virtual ~PenetrationLossModel ();

  static TypeId GetTypeId (void);

  /**
   * Set the value of the penetration loss model, expressed in dB
   * \param pnl the penetration loss value
   */
  void SetValue (double pnl);

  /**
   * Get the value of the penetration loss model, expressed in dB
   * \return the value of the penetration loss model
   */
  double GetValue (void);

private:
  double m_pnl;
};

}

#endif /* PENETRATION_LOSS_MODEL_H */
