#include "ns3/simulator.h"
#include "ns3/rectangle.h"
#include "static-speed-helper.h"

namespace ns3 {

StaticSpeedHelper::StaticSpeedHelper ()
{}
void 
StaticSpeedHelper::InitializePosition (const Position &position)
{
  m_position = position;
  m_speed.dx = 0.0;
  m_speed.dy = 0.0;
  m_lastUpdate = Simulator::Now ();
  m_pauseEnd = Simulator::Now ();
}

void
StaticSpeedHelper::Reset (const Speed &speed, const Time &pauseDelay)
{
  Reset (speed);
  m_pauseEnd = Simulator::Now () + pauseDelay;
}

Position 
StaticSpeedHelper::GetCurrentPosition (void) const
{
  Update ();
  return m_position;
}

Speed 
StaticSpeedHelper::GetCurrentSpeed (void) const
{
  return m_speed;
}

void
StaticSpeedHelper::Update (void) const
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
  m_position.z += m_speed.dz * deltaS;
}

void 
StaticSpeedHelper::Reset (const Speed &speed)
{
  Update ();
  m_speed = speed;
  m_pauseEnd = Simulator::Now ();
}
Position
StaticSpeedHelper::IntersectCurrentDirection (const Rectangle &bounds)
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
      return Position (bounds.xMax, xMaxY, 0.0);
    }
  else if (xMinOk && m_speed.dx <= 0)
    {
      return Position (bounds.xMin, xMinY, 0.0);
    }
  else if (yMaxOk && m_speed.dy >= 0)
    {
      return Position (yMaxX, bounds.yMax, 0.0);
    }
  else if (yMinOk && m_speed.dy <= 0)
    {
      return Position (yMinX, bounds.yMin, 0.0);
    }
  else
    {
      NS_ASSERT (false);
      // quiet compiler
      return Position (0.0, 0.0, 0.0);
    }
}
Time 
StaticSpeedHelper::GetDelayToNextPosition (const Rectangle &bounds, Time delayLeft)
{
  UpdateFull (bounds);
  Position nextPosition = m_position;
  nextPosition.x += m_speed.dx * delayLeft.GetSeconds ();
  nextPosition.y += m_speed.dy * delayLeft.GetSeconds ();
  if (bounds.IsInside (nextPosition))
    {
      return delayLeft;
    }
  nextPosition = IntersectCurrentDirection (bounds);
  Time delay = Seconds ((nextPosition.x - m_position.x) / m_speed.dx);
  return delay;
}
void 
StaticSpeedHelper::Rebound (const Rectangle &bounds)
{
  UpdateFull (bounds);
  double xMinDist = m_position.x - bounds.xMin;
  double xMaxDist = bounds.xMax - m_position.x;
  double yMinDist = m_position.y - bounds.yMin;
  double yMaxDist = bounds.yMax - m_position.y;
  double minX = std::min (xMinDist, xMaxDist);
  double minY = std::min (yMinDist, yMaxDist);
  if (minY < minX)
    {
      m_speed.dy = - m_speed.dy;
    }
  else
    {
      m_speed.dx = - m_speed.dx;
    }
}

void
StaticSpeedHelper::UpdateFull (const Rectangle &bounds) const
{
  Update ();
  m_position.x = std::min (bounds.xMax, m_position.x);
  m_position.x = std::max (bounds.xMin, m_position.x);
  m_position.y = std::min (bounds.yMax, m_position.y);
  m_position.y = std::max (bounds.yMin, m_position.y);
}

Position 
StaticSpeedHelper::GetCurrentPosition (const Rectangle &bounds) const
{
  UpdateFull (bounds);
  return m_position;
}


} // namespace ns3
