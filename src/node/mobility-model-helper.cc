#include <cmath>
#include "ns3/simulator.h"
#include "mobility-model-helper.h"

namespace ns3 {

Position2D::Position2D (double _x, double _y)
  : x (_x),
    y (_y)
{}
Position2D::Position2D ()
  : x (0.0),
    y (0.0)
{}

Speed2D::Speed2D (double _dx, double _dy)  
  : dx (_dx),
    dy (_dy)
{}

Speed2D::Speed2D ()  
  : dx (0.0),
    dy (0.0)
{}

MobilityModelHelper::MobilityModelHelper ()
{}
void 
MobilityModelHelper::InitializePosition (const struct Position2D &position)
{
  m_position = position;
  m_speed.dx = 0.0;
  m_speed.dy = 0.0;
  m_lastUpdate = Simulator::Now ();
  m_pauseEnd = Simulator::Now ();
}
void
MobilityModelHelper::Update (const struct AreaBounds &bounds) const
{
  Time now = Simulator::Now ();
  if (m_pauseEnd > now)
    {
      return;
    }
  Time last = std::max (now, m_pauseEnd);
  if (m_lastUpdate >= last)
    {
      return;
    }
  Time deltaTime = now - last;
  m_lastUpdate = now;
  double deltaS = deltaTime.GetSeconds ();
  m_position.x += m_speed.dx * deltaS;
  m_position.y += m_speed.dy * deltaS;
  m_position.x = std::min (m_position.x, bounds.xMax);
  m_position.y = std::min (m_position.y, bounds.yMax);
  m_position.x = std::max (m_position.x, bounds.xMin);
  m_position.y = std::max (m_position.y, bounds.yMin);
}
Position2D 
MobilityModelHelper::CalculateIntersection (const struct AreaBounds &bounds) const
{
  double xMaxY = m_position.y + (bounds.xMax - m_position.x) / m_speed.dx * m_speed.dy;
  double xMinY = m_position.y + (bounds.xMin - m_position.x) / m_speed.dx * m_speed.dy;
  double yMaxX = m_position.x + (bounds.yMax - m_position.y) / m_speed.dy * m_speed.dx;
  double yMinX = m_position.x + (bounds.yMin - m_position.y) / m_speed.dy * m_speed.dx;
  bool xMaxOk = xMaxY <= bounds.yMax && xMaxY >= bounds.yMin;
  bool xMinOk = xMinY <= bounds.yMax && xMinY >= bounds.yMin;
  bool yMaxOk = yMaxX <= bounds.xMax && yMaxX >= bounds.xMin;
  bool yMinOk = yMinX <= bounds.xMax && yMinX >= bounds.xMin;
  if (xMaxOk && m_speed.dx >= 0)
    {
      return Position2D (bounds.xMax, xMaxY);
    }
  else if (xMinOk && m_speed.dx <= 0)
    {
      return Position2D (bounds.xMin, xMinY);
    }
  else if (yMaxOk && m_speed.dy >= 0)
    {
      return Position2D (yMaxX, bounds.yMax);
    }
  else if (yMinOk && m_speed.dy <= 0)
    {
      return Position2D (yMinX, bounds.yMin);
    }
  else
    {
      NS_ASSERT (false);
      // quiet compiler
      return Position2D (0.0, 0.0);
    }
}
double 
MobilityModelHelper::Distance (const Position2D &a, const Position2D &b) const
{
  double dx = a.x - b.x;
  double dy = a.y - b.y;
  return sqrt (dx * dx + dy * dy);
}
			       
Time
MobilityModelHelper::Reset (const struct AreaBounds &bounds,
			    double direction,
			    double speed,
			    const Time &maxMovementDelay,
			    const Time &pauseDelay)
{
  Update (bounds);
  m_pauseEnd = Simulator::Now () + pauseDelay;
  m_speed.dx = std::cos (direction) * speed;
  m_speed.dy = std::sin (direction) * speed;
  Position2D intersection = CalculateIntersection (bounds);
  Time seconds = Seconds (Distance (intersection, m_position) / speed);
  seconds = std::min (seconds, maxMovementDelay);
  return seconds;
}
Time
MobilityModelHelper::Reset (const struct AreaBounds &bounds,
			    double direction,
			    double speed,
			    double maxDistance,
			    const Time &pauseDelay)
{
  Update (bounds);
  m_pauseEnd = Simulator::Now () + pauseDelay;
  m_speed.dx = std::cos (direction) * speed;
  m_speed.dy = std::sin (direction) * speed;
  Position2D intersection = CalculateIntersection (bounds);
  double distance = Distance (intersection, m_position);
  distance = std::min (distance, maxDistance);
  double seconds = distance / speed;
  return Seconds (seconds);
}
Time
MobilityModelHelper::Reset (const struct AreaBounds &bounds,
			    double direction,
			    double speed,
			    const Time &pauseDelay)
{
  Update (bounds);
  m_pauseEnd = Simulator::Now () + pauseDelay;
  m_speed.dx = std::cos (direction) * speed;
  m_speed.dy = std::sin (direction) * speed;
  Position2D intersection = CalculateIntersection (bounds);
  double seconds = Distance (intersection, m_position) / speed;
  return Seconds (seconds);
}

Position2D 
MobilityModelHelper::GetCurrentPosition (const struct AreaBounds &bounds) const
{
  Update (bounds);
  return m_position;
}

Speed2D 
MobilityModelHelper::GetSpeed (void) const
{
  return m_speed;
}

enum MobilityModelHelper::Side 
MobilityModelHelper::GetSide (const struct AreaBounds &bounds) const
{
  if (m_position.x == bounds.xMin)
    {
      return MobilityModelHelper::LEFT;
    }
  if (m_position.x == bounds.xMax)
    {
      return MobilityModelHelper::RIGHT;
    }
  if (m_position.y == bounds.yMin)
    {
      return MobilityModelHelper::BOTTOM;
    }
  if (m_position.y == bounds.yMax)
    {
      return MobilityModelHelper::TOP;
    }
  return MobilityModelHelper::NONE;
}

} // namespace ns3
