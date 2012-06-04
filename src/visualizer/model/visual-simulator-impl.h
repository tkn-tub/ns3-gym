/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Gustavo Carneiro
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
 * Author: Gustavo Carneiro  <gjcarneiro@gmail.com>
 */

#ifndef VISUAL_SIMULATOR_IMPL_H
#define VISUAL_SIMULATOR_IMPL_H

#include "ns3/simulator-impl.h"

namespace ns3 {


/**
 * \defgroup  visualizer Visualizer
 *
 */

/**
 * \ingroup visualizer
 *
 * \brief A replacement simulator that starts the visualizer
 * \internal
 *
 * To use this class, run any ns-3 simulation with the command-line
 * argument --SimulatorImplementationType=ns3::VisualSimulatorImpl.
 * This causes the visualizer (PyViz) to start automatically.
 **/
class VisualSimulatorImpl : public SimulatorImpl
{
public:
  static TypeId GetTypeId (void);

  VisualSimulatorImpl ();
  ~VisualSimulatorImpl ();

  virtual void Destroy ();
  virtual bool IsFinished (void) const;
  virtual void Stop (void);
  virtual void Stop (Time const &time);
  virtual EventId Schedule (Time const &time, EventImpl *event);
  virtual void ScheduleWithContext (uint32_t context, Time const &time, EventImpl *event);
  virtual EventId ScheduleNow (EventImpl *event);
  virtual EventId ScheduleDestroy (EventImpl *event);
  virtual void Remove (const EventId &ev);
  virtual void Cancel (const EventId &ev);
  virtual bool IsExpired (const EventId &ev) const;
  virtual void Run (void);
  virtual Time Now (void) const;
  virtual Time GetDelayLeft (const EventId &id) const;
  virtual Time GetMaximumSimulationTime (void) const;
  virtual void SetScheduler (ObjectFactory schedulerFactory);
  virtual uint32_t GetSystemId (void) const; 
  virtual uint32_t GetContext (void) const;

  /// calls Run() in the wrapped simulator
  void RunRealSimulator (void);

protected:
  void DoDispose ();
  void NotifyConstructionCompleted (void);

private:
  Ptr<SimulatorImpl> GetSim ();
  Ptr<SimulatorImpl> m_simulator;
  ObjectFactory m_simulatorImplFactory;

};

} // namespace ns3

#endif /* DEFAULT_SIMULATOR_IMPL_H */
