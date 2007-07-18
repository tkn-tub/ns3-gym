#ifndef STATIC_SPEED_HELPER_H
#define STATIC_SPEED_HELPER_H

#include "ns3/nstime.h"
#include "position.h"
#include "speed.h"

namespace ns3 {

class StaticSpeedHelper
{
 public:
  StaticSpeedHelper ();
  void InitializePosition (const Position &position);

  void Reset (const Speed &speed, const Time &pauseDelay);

  Position GetCurrentPosition (void) const;
  Speed GetCurrentSpeed (void) const;

 private:
  void Update (void) const;
  mutable Time m_lastUpdate;
  mutable Position m_position;
  Speed m_speed;
  Time m_pauseEnd;
};

} // namespace ns3

#endif /* STATIC_SPEED_HELPER_H */
