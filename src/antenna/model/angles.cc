/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011, 2012 CTTC
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
#include <cmath>
#include "angles.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Angles");

double DegreesToRadians (double degrees)
{
  return degrees * M_PI / 180.0;

}

double RadiansToDegrees (double radians)
{
  return radians * 180.0 / M_PI;
}

std::ostream& operator<< (std::ostream& os, const Angles& a)
{
  os << "(" << a.phi << ", " << a.theta << ")";
  return os;
}

std::istream &operator >> (std::istream &is, Angles &a)
{
  char c;
  is >> a.phi >> c >> a.theta;
  if (c != ':')
    {
      is.setstate (std::ios_base::failbit);
    }
  return is;
}


Angles::Angles ()
  : phi (0),
    theta (0)
{
}


Angles::Angles (double p, double t)
  : phi (p),
    theta (t)
{
}


Angles::Angles (Vector v)
  : phi (std::atan2 (v.y, v.x)),
    theta (std::acos (v.z / sqrt (v.x*v.x + v.y*v.y + v.z*v.z)))
{
}

Angles::Angles (Vector v, Vector o)
  : phi (std::atan2 (v.y - o.y, v.x - o.x)),
    theta (std::acos ((v.z - o.z) / CalculateDistance (v, o)))
{
}


}

