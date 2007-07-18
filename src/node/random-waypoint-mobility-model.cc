#include "ns3/simulator.h"
#include "ns3/random-variable.h"
#include "random-waypoint-mobility-model.h"
#include "random-position.h"

namespace ns3 {

RandomWaypointMobilityModelParameters::RandomWaypointMobilityModelParameters ()
{}
RandomWaypointMobilityModelParameters::RandomWaypointMobilityModelParameters (Ptr<RandomPosition> randomPosition,
									      const RandomVariable &speed,
									      const RandomVariable &pause)
  : m_speed (speed.Copy ()),
    m_pause (pause.Copy ()),
    m_position (randomPosition)
{}
void 
RandomWaypointMobilityModelParameters::SetWaypointPositionModel (Ptr<RandomPosition> randomPosition)
{
  m_position = randomPosition;
}
void 
RandomWaypointMobilityModelParameters::SetSpeed (const RandomVariable &speed)
{
  delete m_speed;
  m_speed = speed.Copy ();
}
void 
RandomWaypointMobilityModelParameters::SetPause (const RandomVariable &pause)
{
  delete m_pause;
  m_pause = pause.Copy ();
}
void 
RandomWaypointMobilityModelParameters::DoDispose (void)
{
  m_position = 0;
  delete m_pause;
  delete m_speed;
  m_pause = 0;
  m_speed = 0;  
}


RandomWaypointMobilityModel::RandomWaypointMobilityModel (Ptr<RandomWaypointMobilityModelParameters> parameters)
  : m_parameters (parameters)
{
  Simulator::ScheduleNow (&RandomWaypointMobilityModel::Start, this);
}

void
RandomWaypointMobilityModel::Start (void)
{
  Position m_current = m_helper.GetCurrentPosition ();
  Position destination = m_parameters->m_position->Get ();
  double speed = m_parameters->m_speed->GetValue ();
  Time pause = Seconds (m_parameters->m_pause->GetValue ());
  double dx = (destination.x - m_current.x) * speed;
  double dy = (destination.y - m_current.y) * speed;
  double dz = (destination.z - m_current.z) * speed;

  m_helper.Reset (Speed (dx,dy,dz), pause);
  Time travelDelay = Seconds (CalculateDistance (destination, m_current) / speed);
  m_event = Simulator::Schedule (travelDelay + pause,
				 &RandomWaypointMobilityModel::Start, this);
}

Position 
RandomWaypointMobilityModel::DoGet (void) const
{
  return m_helper.GetCurrentPosition ();
}
void 
RandomWaypointMobilityModel::DoSet (const Position &position)
{
  m_helper.InitializePosition (position);
  Simulator::Remove (m_event);
  Start ();
}


} // namespace ns3
