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
#include <math.h>
#include "antenna-model.h"


NS_LOG_COMPONENT_DEFINE ("AntennaModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (AntennaModel);



double DegreesToRadians (double degrees)
{
  return degrees * M_PI / 180.0;

}

double RadiansToDegrees (double radians)
{
  return radians * 180.0 / M_PI;
}

std::ostream& operator<< (std::ostream& os, const AntennaModel::Angles& a)
{
  os << "(" << a.phi << ", " << a.theta << ")";
  return os;
}

AntennaModel::AntennaModel ()
{
}
  
AntennaModel::~AntennaModel ()
{
}

TypeId 
AntennaModel::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::AntennaModel")
    .SetParent<Object> ()
    ;
  return tid;
}

  
  
AntennaModel::Angles::Angles ()
  : phi (0),
    theta (0)
{
}


AntennaModel::Angles::Angles (double p, double t)
  : phi (p),
    theta (t)
{
}


AntennaModel::Angles::Angles (Vector v)
  : phi (atan2 (v.y, v.x)),
    theta (acos (v.z / sqrt (v.x*v.x + v.y*v.y + v.z*v.z)))
{
}

AntennaModel::Angles::Angles (Vector v, Vector o)
  : phi (atan2 (v.y - o.y, v.x - o.x)),
    theta (acos ((v.z - o.z) / CalculateDistance (v, o)))
{
}


}

