/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 CTTC
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


#include <ns3/log.h>
#include <ns3/double.h>
#include <cmath>

#include "antenna-model.h"
#include "cosine-antenna-model.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("CosineAntennaModel");

NS_OBJECT_ENSURE_REGISTERED (CosineAntennaModel);


TypeId 
CosineAntennaModel::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::CosineAntennaModel")
    .SetParent<AntennaModel> ()
    .SetGroupName("Antenna")
    .AddConstructor<CosineAntennaModel> ()
    .AddAttribute ("Beamwidth",
                   "The 3dB beamwidth (degrees)",
                   DoubleValue (60),
                   MakeDoubleAccessor (&CosineAntennaModel::SetBeamwidth,
                                       &CosineAntennaModel::GetBeamwidth),
                   MakeDoubleChecker<double> (0, 180))
    .AddAttribute ("Orientation",
                   "The angle (degrees) that expresses the orientation of the antenna on the x-y plane relative to the x axis",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&CosineAntennaModel::SetOrientation,
                                       &CosineAntennaModel::GetOrientation),
                   MakeDoubleChecker<double> (-360, 360))
    .AddAttribute ("MaxGain",
                   "The gain (dB) at the antenna boresight (the direction of maximum gain)",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&CosineAntennaModel::m_maxGain),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

void 
CosineAntennaModel::SetBeamwidth (double beamwidthDegrees)
{ 
  NS_LOG_FUNCTION (this << beamwidthDegrees);
  m_beamwidthRadians = DegreesToRadians (beamwidthDegrees);
  m_exponent = -3.0 / (20 * std::log10 (std::cos (m_beamwidthRadians / 4.0)));
  NS_LOG_LOGIC (this << " m_exponent = " << m_exponent);
}

double
CosineAntennaModel::GetBeamwidth () const
{
  return RadiansToDegrees (m_beamwidthRadians);
}

void 
CosineAntennaModel::SetOrientation (double orientationDegrees)
{
  NS_LOG_FUNCTION (this << orientationDegrees);
  m_orientationRadians = DegreesToRadians (orientationDegrees);
}

double
CosineAntennaModel::GetOrientation () const
{
  return RadiansToDegrees (m_orientationRadians);
}

double 
CosineAntennaModel::GetGainDb (Angles a)
{
  NS_LOG_FUNCTION (this << a);
  // azimuth angle w.r.t. the reference system of the antenna
  double phi = a.phi - m_orientationRadians;

  // make sure phi is in (-pi, pi]
  while (phi <= -M_PI)
    {
      phi += M_PI+M_PI;
    }
  while (phi > M_PI)
    {
      phi -= M_PI+M_PI;
    }

  NS_LOG_LOGIC ("phi = " << phi );

  // element factor: amplitude gain of a single antenna element in linear units
  double ef = std::pow (std::cos (phi / 2.0), m_exponent);

  // the array factor is not considered. Note that if we did consider
  // the array factor, the actual beamwidth would change, and in
  // particular it would be different from the one specified by the
  // user. Hence it is not desirable to use the array factor, for the
  // ease of use of this model.

  double gainDb = 20 * std::log10 (ef);
  NS_LOG_LOGIC ("gain = " << gainDb << " + " << m_maxGain << " dB");
  return gainDb + m_maxGain;
}


}

