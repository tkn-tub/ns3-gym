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

#ifndef PATH_LOSS_MODEL_H
#define PATH_LOSS_MODEL_H


#include "discrete-time-loss-model.h"


namespace ns3 {

class MobilityModel;

/**
 * \ingroup lte
 *
 * \brief This class models the propagation loss model due to the path loss
 */
class PathLossModel : public DiscreteTimeLossModel
{

public:
  PathLossModel ();
  virtual ~PathLossModel ();

  static TypeId GetTypeId (void);

  /**
   * Set the value of the path loss model, expressed in dB
   * \param pl the path loss value
   */
  void SetValue (double pl);


  /**
   * Get the value of the path loss model, expressed in dB
   * \param a sender mobility
   * \param b receiver mobility
   * \return the value of the path loss
   */
  double GetValue (Ptr<const MobilityModel> a, Ptr<const MobilityModel> b);

private:
  double m_pl;
};

}

#endif /* PATH_LOSS_MODEL_H */
