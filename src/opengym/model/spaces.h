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

#ifndef OPENGYM_SPACES_H
#define OPENGYM_SPACES_H

#include "ns3/object.h"
#include "ns3/simulator.h"
#include <map>

#include <zmq.hpp>
#include <string>
#include <iostream>

#include "messages.pb.h"

namespace ns3 {

enum Dtype { INT, UINT, FLOAT, DOUBLE };

class OpenGymSpace : public Object
{
public:
  OpenGymSpace ();
  virtual ~OpenGymSpace ();

  static TypeId GetTypeId ();

  virtual ns3opengym::SpaceDescription GetSpaceDescription() = 0;
  virtual void Print(std::ostream& where) const = 0;
protected:
  // Inherited
  virtual void DoInitialize (void);
  virtual void DoDispose (void);
};


class OpenGymDiscreteSpace : public OpenGymSpace
{
public:
  OpenGymDiscreteSpace ();
  OpenGymDiscreteSpace (int n);
  virtual ~OpenGymDiscreteSpace ();

  static TypeId GetTypeId ();

  virtual ns3opengym::SpaceDescription GetSpaceDescription();

  int GetN(void);
  virtual void Print(std::ostream& where) const;
protected:
  // Inherited
  virtual void DoInitialize (void);
  virtual void DoDispose (void);

private:
	int m_n;

	friend std::ostream& operator<< ( std::ostream& os, const OpenGymDiscreteSpace& a);
};

class OpenGymBoxSpace : public OpenGymSpace
{
public:
  OpenGymBoxSpace ();
  OpenGymBoxSpace (float low, float high, std::vector<uint32_t> shape, std::string dtype);
  OpenGymBoxSpace (std::vector<float> low, std::vector<float> high, std::vector<uint32_t> shape, std::string dtype);
  virtual ~OpenGymBoxSpace ();

  static TypeId GetTypeId ();

  virtual ns3opengym::SpaceDescription GetSpaceDescription();

  float GetLow();
  float GetHigh();
  std::vector<uint32_t> GetShape();
  Dtype GetDtype();
  virtual void Print(std::ostream& where) const;
protected:
  // Inherited
  virtual void DoInitialize (void);
  virtual void DoDispose (void);

private:
  void SetDtype ();

	float m_low;
	float m_high;
	std::vector<uint32_t> m_shape;
  std::string m_dtypeName;
  std::vector<float> m_lowVec;
  std::vector<float> m_highVec;

  Dtype m_dtype;

	friend std::ostream& operator<< ( std::ostream& os, const OpenGymBoxSpace& a);
};

} // end of namespace ns3

#endif /* OPENGYM_SPACES_H */

