/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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

#ifndef UAN_NOISE_MODEL_DEFAULT_H
#define UAN_NOISE_MODEL_DEFAULT_H

#include "ns3/uan-noise-model.h"
#include "ns3/attribute.h"
#include "ns3/object.h"

namespace ns3 {

/**
 * \class UanNoiseModelDefault
 *
 * Standard ambient acoustic noise model.  See attributes for parameters
 *
 * This class returns ambient noise by following the algorithm given in
 * Harris, A. F. and Zorzi, M. 2007. Modeling the underwater acoustic channel in ns2.
 * In Proceedings of the 2nd international Conference on Performance Evaluation
 * Methodologies and Tools (Nantes, France, October 22 - 27, 2007). ValueTools,
 * vol. 321. ICST (Institute for Computer Sciences Social-Informatics and
 * Telecommunications Engineering), ICST, Brussels, Belgium, 1-8.
 *
 * Which uses the noise model also given in the book
 * "Principles of Underwater Sound" by Urick
 */
class UanNoiseModelDefault : public UanNoiseModel
{
public:
  UanNoiseModelDefault ();
  virtual ~UanNoiseModelDefault ();

  static TypeId GetTypeId (void);
  /**
   * \returns Noise power in dB re 1uPa/Hz
   * \param fKhz Frequency in kHz
   */
  virtual double GetNoiseDbHz (double fKhz) const;

private:
  double m_wind;
  double m_shipping;

};

}

#endif /* UAN_NOISE_MODEL_DEFAULT_H */
