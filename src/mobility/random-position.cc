#include "random-position.h"
#include "ns3/random-variable.h"
#include "ns3/default-value.h"
#include "ns3/random-variable-default-value.h"
#include "ns3/debug.h"
#include <cmath>

NS_DEBUG_COMPONENT_DEFINE ("RandomPosition");

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

const InterfaceId RandomPosition::iid = MakeInterfaceId ("RandomPosition", Object::iid);

const ClassId RandomRectanglePosition::cid = 
  MakeClassId<RandomRectanglePosition> ("RandomRectanglePosition", 
					RandomPosition::iid);
const ClassId RandomDiscPosition::cid = 
  MakeClassId<RandomDiscPosition> ("RandomDiscPosition", 
				   RandomPosition::iid);

RandomPosition::RandomPosition ()
{
  Object::SetInterfaceId (RandomPosition::iid);
}

RandomPosition::~RandomPosition ()
{}

RandomRectanglePosition::RandomRectanglePosition ()
  : m_x (g_rectangleX.GetCopy ()),
    m_y (g_rectangleY.GetCopy ())
{}
RandomRectanglePosition::RandomRectanglePosition (const RandomVariable &x,
						  const RandomVariable &y)
  : m_x (x.Copy ()),
    m_y (y.Copy ())
{}
RandomRectanglePosition::~RandomRectanglePosition ()
{
  delete m_x;
  delete m_y;
  m_x = 0;
  m_y = 0;
}
Position
RandomRectanglePosition::Get (void) const
{
  double x = m_x->GetValue ();
  double y = m_y->GetValue ();
  return Position (x, y, 0.0);
}

RandomDiscPosition::RandomDiscPosition ()
  : m_theta (g_discTheta.GetCopy ()),
    m_rho (g_discRho.GetCopy ()),
    m_x (g_discX.GetValue ()),
    m_y (g_discY.GetValue ())
{}
RandomDiscPosition::RandomDiscPosition (const RandomVariable &theta,
					const RandomVariable &rho,
					double x, double y)
  : m_theta (theta.Copy ()),
    m_rho (rho.Copy ()),
    m_x (0.0),
    m_y (0.0)
{}
RandomDiscPosition::~RandomDiscPosition ()
{
  delete m_theta;
  delete m_rho;
  m_theta = 0;
  m_rho = 0;
}
Position
RandomDiscPosition::Get (void) const
{
  double theta = m_theta->GetValue ();
  double rho = m_rho->GetValue ();
  double x = m_x + std::cos (theta) * rho;
  double y = m_y + std::sin (theta) * rho;
  NS_DEBUG ("Disc position x=" << x << ", y=" << y);
  return Position (x, y, 0.0);
}


} // namespace ns3 
