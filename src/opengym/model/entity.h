/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018 Piotr Gawlowicz
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
 * Author: Piotr Gawlowicz <gawlowicz.p@gmail.com>
 *
 */

#ifndef OPENGYM_ENTITY_H
#define OPENGYM_ENTITY_H

#include "ns3/object.h"
#include "ns3/simulator.h"

namespace ns3 {

class OpenGymSpace;
class OpenGymDataContainer;
class OpenGymEnv;

class OpenGymEntity : public Object
{
public:
  OpenGymEntity ();
  virtual ~OpenGymEntity ();

  static TypeId GetTypeId ();

  virtual Ptr<OpenGymSpace> GetActionSpace() = 0;
  virtual Ptr<OpenGymSpace> GetObservationSpace() = 0;
  // TODO:  get all in one function like below, do we need it?
  //virtual void GetEnvState(Ptr<OpenGymDataContainer>  &obs, float &reward, bool &done, std::string &info) = 0;
  virtual bool GetGameOver() = 0;
  virtual Ptr<OpenGymDataContainer> GetObservation() = 0;
  virtual float GetReward() = 0;
  virtual std::string GetExtraInfo() = 0;
  virtual bool ExecuteActions(Ptr<OpenGymDataContainer> action) = 0;

  void SetOpenGymEnv(Ptr<OpenGymEnv> openGymEnv);
  void Notify();

protected:
  // Inherited
  virtual void DoInitialize (void);
  virtual void DoDispose (void);

  Ptr<OpenGymEnv> m_openGymEnv;
private:

};

} // end of namespace ns3

#endif /* OPENGYM_ENTITY_H */