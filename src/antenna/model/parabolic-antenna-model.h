/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 CTTC
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */

#ifndef PARABOLIC_ANTENNA_MODEL_H
#define PARABOLIC_ANTENNA_MODEL_H


#include <ns3/object.h>
#include <ns3/antenna-model.h>

namespace ns3 {

/**
 * 
 * \brief  Antenna model based on a parabolic approximation of the main lobe radiation pattern.
 *
 * This class implements the parabolic model as described in some 3GPP document, e.g., R4-092042
 *
 * A similar model appears in 
 *
 * George Calcev and Matt Dillon, "Antenna Tilt Control in CDMA Networks"
 * in Proc. of the 2nd Annual International Wireless Internet Conference (WICON), 2006
 *
 * though the latter addresses also the elevation plane, which the present model doesn't.
 *
 *
 */
class ParabolicAntennaModel : public AntennaModel
{
public:

  // inherited from Object
  static TypeId GetTypeId ();

  // inherited from AntennaModel
  virtual double GetGainDb (Angles a);


  // attribute getters/setters
  void SetBeamwidth (double beamwidthDegrees);
  double GetBeamwidth () const;
  void SetOrientation (double orientationDegrees);
  double GetOrientation () const;

private:

  double m_beamwidthRadians;

  double m_orientationRadians;

  double m_maxAttenuation;
};



} // namespace ns3


#endif // PARABOLIC_ANTENNA_MODEL_H
