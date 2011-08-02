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

#ifndef MULTIPATH_LOSS_MODEL_H
#define MULTIPATH_LOSS_MODEL_H


#include "discrete-time-loss-model.h"
#include <list>
#include <ns3/random-variable.h>

namespace ns3 {

class LtePhy;

/**
 * \ingroup lte
 *
 * \brief JakesFadingLossModel class implements a loss model due to the fast fading.
 * In particular, the fast fading is modeled using a Jakes Model
 */
class JakesFadingLossModel : public DiscreteTimeLossModel
{

public:
  JakesFadingLossModel ();
  virtual ~JakesFadingLossModel ();

  static TypeId GetTypeId (void);

  /**
   * \brief Set the value of the considered loss model
   */
  void SetValue (void);
  /**
   * \brief Get the value for a particular sub channel
   * \param subChannel the sub channel for which a value is requested
   * \return the loss for a particular sub channel
   */
  double GetValue (int subChannel);

  /**
   * \brief Set the physical layer 
   * \param phy the physical layer
   */
  void SetPhy (Ptr<LtePhy> phy);
  /**
   * \brief Get the physical layer 
   * \return the pointer to the physical layer
   */
  Ptr<LtePhy> GetPhy (void);


  /*
   * In order to avoid to execute every TTI the Jakes Model, the value
   * of the multipath loss is stored into a matrix (MultipathForFrequencyDomain)
   * frequency x time.
   *
   * A MultipathForFrequencyDomain element is build in a way that
   * m_multipath.at(i).at(j) represents the loss at the frequency i and time j.
   *
   * The model is udated every samplingInterval (the default value is 0.5 ms)
   */

  /**
   * brief a list of multipath values for the time domain 
   */
  typedef std::vector<double> MultipathForTimeDomain;
  /**
   * brief a list of multipath values for the frequency domain 
   */
  typedef std::vector<MultipathForTimeDomain> MultipathForFrequencyDomain;

private:

  MultipathForFrequencyDomain m_multipath;

  UniformVariable m_nbOfPaths;
  UniformVariable m_startJakes;

  Ptr<LtePhy> m_phy;
};

}

#endif /* MULTIPATH_LOSS_MODEL_H */
