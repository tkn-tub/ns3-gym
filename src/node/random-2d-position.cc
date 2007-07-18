#include "random-2d-position.h"
#include "ns3/random-variable.h"
#include "ns3/random-variable-default-value.h"
#include <cmath>

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
g_discTheta ("RandomRectanglePositionTheta",
	     "A random variable which represents the angle (gradients) of a position in a random disc.",
	     "Uniform:0:3.1415");

static RandomVariableDefaultValue
g_discRho ("RandomRectanglePositionRho",
	   "A random variable which represents the radius of a position in a random disc.",
	   "Uniform:0:200");

const InterfaceId Random2dPosition::iid = MakeInterfaceId ("Random2dPosition", Object::iid);

const ClassId RandomRectanglePosition::cid = 
  MakeClassId<RandomRectanglePosition> ("RandomRectanglePosition", 
					Random2dPosition::iid);
const ClassId RandomDiscPosition::cid = 
  MakeClassId<RandomDiscPosition> ("RandomDiscPosition", 
				   Random2dPosition::iid);

Random2dPosition::Random2dPosition ()
{
  Object::SetInterfaceId (Random2dPosition::iid);
}

Random2dPosition::~Random2dPosition ()
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
Position2d
RandomRectanglePosition::Get (void) const
{
  double x = m_x->GetValue ();
  double y = m_y->GetValue ();
  return Position2d (x, y);
}

RandomDiscPosition::RandomDiscPosition ()
  : m_theta (g_discTheta.GetCopy ()),
    m_rho (g_discRho.GetCopy ())
{}
RandomDiscPosition::RandomDiscPosition (const RandomVariable &theta,
					const RandomVariable &rho)
  : m_theta (theta.Copy ()),
    m_rho (rho.Copy ())
{}
RandomDiscPosition::~RandomDiscPosition ()
{
  delete m_theta;
  delete m_rho;
  m_theta = 0;
  m_rho = 0;
}
Position2d
RandomDiscPosition::Get (void) const
{
  double theta = m_theta->GetValue ();
  double rho = m_rho->GetValue ();
  double x = std::cos (theta) * rho;
  double y = std::sin (theta) * rho;
  return Position2d (x, y);
}


} // namespace ns3 
