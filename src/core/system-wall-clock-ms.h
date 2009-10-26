/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage.inria.fr>
 */

#ifndef SYSTEM_WALL_CLOCK_MS_H
#define SYSTEM_WALL_CLOCK_MS_H

namespace ns3 {

/**
 * \brief measure wall-clock time in milliseconds

 * \todo This class exists also in non-unix systems but is not
 * implemented and always return zero as measurd time.
 */
class SystemWallClockMs {
public:
  SystemWallClockMs ();
  ~SystemWallClockMs ();

  /**
   * Start a measure.
   */
  void Start (void);
  /**
   * \brief Stop measuring the time since Start() was called.
   * \returns the measured elapsed wall clock time (in milliseconds) since 
   *          ns3::SystemWallClockMs::Start was invoked.
   *
   * It is possible to start a new measurement with ns3::SystemWallClockMs::Start
   * after this method returns.
   */
  unsigned long long End (void);

  /**
   * \returns the measured elapsed wall clock time (in milliseconds) since 
   *          ns3::SystemWallClockMs::Start was invoked.
   */
  double GetElapsedReal (void) const;
  /**
   * \returns the measured elapsed 'user' wall clock time (in milliseconds) since 
   *          ns3::SystemWallClockMs::Start was invoked.
   */
  double GetElapsedUser (void) const;
  /**
   * \returns the measured elapsed 'system' wall clock time (in milliseconds) since 
   *          ns3::SystemWallClockMs::Start was invoked.
   */
  double GetElapsedSystem (void) const;

private:
  class SystemWallClockMsPrivate *m_priv;
};

}; // namespace ns3

#endif /* SYSTEM_WALL_CLOCK_MS_H */
