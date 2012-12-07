/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011, 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Marco Miozzo  <marco.miozzo@cttc.es>
 *         Nicola Baldo <nbaldo@cttc.es>
 * 
 */

#ifndef ITU_R_1238_PROPAGATION_LOSS_MODEL_H
#define ITU_R_1238_PROPAGATION_LOSS_MODEL_H

#include <ns3/propagation-loss-model.h>
#include <ns3/propagation-environment.h>

namespace ns3 {

/**
 * This class implements the ITU-R 1238 propagation loss model.
 * 
 */
class ItuR1238PropagationLossModel : public PropagationLossModel
{

public:

  // inherited from Object
  static TypeId GetTypeId (void);

  /** 
   * 
   * 
   * \param a the first mobility model
   * \param b the second mobility model
   * 
   * \return the loss in dBm for the propagation between
   * the two given mobility models
   */
  double GetLoss (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const;

private:

  // inherited from PropagationLossModel
  virtual double DoCalcRxPower (double txPowerDbm,
                                Ptr<MobilityModel> a,
                                Ptr<MobilityModel> b) const;
  virtual int64_t DoAssignStreams (int64_t stream);
  
  double m_frequency; ///< frequency in MHz
  double m_lambda; ///< wavelength
  EnvironmentType m_environment;
  CitySize m_citySize;
  double m_rooftopHeight; ///< in meters
  double m_streetsOrientation; ///< in degrees [0,90]
  double m_streetsWidth; ///< in meters
  double m_buildingsExtend; ///< in meters
  double m_buildingSeparation; ///< in meters

};

} // namespace ns3


#endif // ITU_R_1238_PROPAGATION_LOSS_MODEL_H

