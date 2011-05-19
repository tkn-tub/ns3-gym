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

#ifndef DISCRETE_TIME_LOSS_MODEL_H
#define DISCRETE_TIME_LOSS_MODEL_H


#include <ns3/nstime.h>
#include "ns3/object.h"


namespace ns3 {


/**
 * \ingroup lte
 *
 * \brief The DiscreteTimeLossModel class offers a basic implementation
 * for all propagation loss models used for LTE networks
 */
class DiscreteTimeLossModel : public Object
{

public:
  DiscreteTimeLossModel ();
  virtual ~DiscreteTimeLossModel ();

  static TypeId GetTypeId (void);

  /**
   * \brief Set the time in which the model has been
   * updated.
   * Each model can be updated every Sampling interval. Every time
   * the model is updated, the variable m_lastUpdate will be set to
   * the current simulation time.
   */
  void SetLastUpdate (void);

  /**
   * \brief Get the time in which the model has been
   * updated
   *
   * \return the time instant in which the model have been updated
   */
  Time GetLastUpdate (void);

  /**
   * \brief Set the time interval every time the model
   * should be updated
   *
   * \param sp the time interval
   */
  void SetSamplingPeriod (double sp);

  /**
   * \brief Get the time interval every time the model
   * should be updated
   *
   * \return the time interval
   */
  double GetSamplingPeriod (void);

  /**
   * \brief Check if the model should be updated
   */
  bool NeedForUpdate (void);


private:
  Time m_lastUpdate;
  double m_samplingPeriod;
};


}

#endif /* DISCRETE_TIME_LOSS_MODEL_H */
