/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#ifndef SIMULATION_SINGLETON_H
#define SIMULATION_SINGLETON_H

namespace ns3 {

/**
 * This singleton class template ensures that the type
 * for which we want a singleton has a lifetime bounded
 * by the simulation lifetime. That it, the underlying
 * type will be automatically deleted upon a users' call
 * to Simulator::Destroy.
 */
template <typename T>
class SimulationSingleton
{
public:
  /**
   * \returns the instance underlying this singleton.
   *
   * This instance will be automatically deleted when the
   * user calls ns3::Simulator::Destroy.
   */
  static T *Get (void);
private:
  static T **GetObject (void);
  static void DeleteObject (void);
};

}//namespace ns3


#include "simulator.h"

namespace ns3 {

template <typename T>
T *
SimulationSingleton<T>::Get (void)
{
  T ** ppobject = GetObject ();
  return *ppobject;
}

template <typename T>
T **
SimulationSingleton<T>::GetObject (void)
{
  static T *pobject = 0;
  if (pobject == 0)
    {
      pobject = new T ();
      Simulator::ScheduleDestroy (&SimulationSingleton<T>::DeleteObject);
    }
  return &pobject;
}

template <typename T>
void
SimulationSingleton<T>::DeleteObject (void)
{
  T **ppobject = GetObject ();
  delete (*ppobject);
  *ppobject = 0;
}

} // namespace ns3


#endif /* SIMULATION_SINGLETON_H */
