/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "random-position.h"
#include "ns3/random-variable.h"
#include "ns3/default-value.h"
#include "ns3/random-variable-default-value.h"
#include "ns3/log.h"
#include <cmath>

NS_LOG_COMPONENT_DEFINE ("RandomPosition");

namespace ns3 {

static RandomVariableDefaultValue
g_rectangleX ("RandomRectanglePositionX",
	      "A random variable which represents the x position of a position in a random rectangle.",
	      "Uniform:0:200");

static RandomVariableDefaultValue
g_rectangleY ("RandomRectanglePositionY",
	      "A random variable which represents the y position of a position in a random rectangle.",
	      "Uniform:0:200");

static RandomVariableDefaultValue
g_discTheta ("RandomDiscPositionTheta",
	     "A random variable which represents the angle (gradients) of a position in a random disc.",
	     "Uniform:0:6.2830");

static RandomVariableDefaultValue
g_discRho ("RandomDiscPositionRho",
	   "A random variable which represents the radius of a position in a random disc.",
	   "Uniform:0:200");

static NumericDefaultValue<double>
g_discX ("RandomDiscPositionX",
	 "The x coordinate of the center of the random position disc.",
	 0.0);

static NumericDefaultValue<double>
g_discY ("RandomDiscPositionY",
	 "The y coordinate of the center of the random position disc.",
	 0.0);

NS_OBJECT_ENSURE_REGISTERED (RandomPosition);

TypeId 
RandomPosition::GetTypeId (void)
{
  static TypeId tid = TypeId ("RandomPosition")
    .SetParent<Object> ();
  return tid;
}

RandomPosition::RandomPosition ()
{
}

RandomPosition::~RandomPosition ()
{}

NS_OBJECT_ENSURE_REGISTERED (RandomRectanglePosition);

TypeId
RandomRectanglePosition::GetTypeId (void)
{
  static TypeId tid = TypeId ("RandomRectanglePosition")
    .SetParent<RandomPosition> ()
    .AddConstructor<RandomRectanglePosition> ()
    .AddConstructor<RandomRectanglePosition, const RandomVariable &, const RandomVariable &> ();
  return tid;
}

RandomRectanglePosition::RandomRectanglePosition ()
  : m_x (g_rectangleX.Get ()),
    m_y (g_rectangleY.Get ())
{}
RandomRectanglePosition::RandomRectanglePosition (const RandomVariable &x,
						  const RandomVariable &y)
  : m_x (x),
    m_y (y)
{}
RandomRectanglePosition::~RandomRectanglePosition ()
{}
Vector
RandomRectanglePosition::Get (void) const
{
  double x = m_x.GetValue ();
  double y = m_y.GetValue ();
  return Vector (x, y, 0.0);
}

NS_OBJECT_ENSURE_REGISTERED (RandomDiscPosition);

TypeId
RandomDiscPosition::GetTypeId (void)
{
  static TypeId tid = TypeId ("RandomDiscPosition")
    .SetParent<RandomPosition> ()
    .AddConstructor<RandomDiscPosition> ();
  return tid;
}   

RandomDiscPosition::RandomDiscPosition ()
  : m_theta (g_discTheta.Get ()),
    m_rho (g_discRho.Get ()),
    m_x (g_discX.GetValue ()),
    m_y (g_discY.GetValue ())
{}
RandomDiscPosition::RandomDiscPosition (const RandomVariable &theta,
					const RandomVariable &rho,
					double x, double y)
  : m_theta (theta),
    m_rho (rho),
    m_x (0.0),
    m_y (0.0)
{}
RandomDiscPosition::~RandomDiscPosition ()
{}
Vector
RandomDiscPosition::Get (void) const
{
  double theta = m_theta.GetValue ();
  double rho = m_rho.GetValue ();
  double x = m_x + std::cos (theta) * rho;
  double y = m_y + std::sin (theta) * rho;
  NS_LOG_DEBUG ("Disc position x=" << x << ", y=" << y);
  return Vector (x, y, 0.0);
}


} // namespace ns3 
