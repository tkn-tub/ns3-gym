/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
#ifndef STATIC_SPEED_HELPER_H
#define STATIC_SPEED_HELPER_H

#include "ns3/nstime.h"
#include "position.h"
#include "speed.h"

namespace ns3 {

class Rectangle;

class StaticSpeedHelper
{
 public:
  StaticSpeedHelper ();
  StaticSpeedHelper (const Position &position);
  StaticSpeedHelper (const Position &position,
		     const Speed &speed);
  void InitializePosition (const Position &position);

  void Reset (const Speed &speed, const Time &pauseDelay);
  void Reset (const Speed &speed);
  Position GetCurrentPosition (const Rectangle &bounds) const;
  Position GetCurrentPosition (void) const;
  Speed GetSpeed (void) const;
  void SetSpeed (const Speed &speed);

 private:
  void Update (void) const;
  void UpdateFull (const Rectangle &rectangle) const;
  mutable Time m_lastUpdate;
  mutable Position m_position;
  Speed m_speed;
  Time m_pauseEnd;
};

} // namespace ns3

#endif /* STATIC_SPEED_HELPER_H */
