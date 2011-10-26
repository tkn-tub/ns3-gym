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
 * Author: Marco Miozzo <marco.miozzo@cttc.es>
 *        remove dependencies from Phy and Mobility models
 */

#ifndef MULTIPATH_LOSS_MODEL_H
#define MULTIPATH_LOSS_MODEL_H


//#include "discrete-time-loss-model.h"
#include <ns3/object.h>
#include <ns3/nstime.h>
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
class JakesFadingLossModel : public Object
{

public:
  JakesFadingLossModel ();
  virtual ~JakesFadingLossModel ();

  static TypeId GetTypeId (void);

  
  /**
  * \brief Get the value for a particular sub channel and a given speed
  * \param subChannel the sub channel for which a value is requested
  * \param speed the relative speed of the two devices
  * \return the loss for a particular sub channel
  */
  double GetValue (int subChannel, double speed);

private:
  
  void LoadTrace (std::string filename);


  
  /**
   * Vector with fading samples in time domain (for a fixed RB)
   */
  typedef std::vector<double> FadingTraceSample;
  /**
   * Vector collecting the time fading traces in the frequency domain (per RB)
   */
  typedef std::vector<FadingTraceSample> FadingTrace;


  
  std::string m_traceFile;
  
  FadingTrace m_fadingTrace;

  
  Time m_traceLength;
  uint32_t m_samplesNum;
  Time m_windowSize;
  int m_windowOffset; 
  
  UniformVariable* m_startJakes;
  
  uint8_t m_rbNum;
  Time m_lastWindowUpdate;

};

}

#endif /* MULTIPATH_LOSS_MODEL_H */
