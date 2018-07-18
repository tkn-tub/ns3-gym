/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017
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
 * Author: Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#include "ns3/log.h"
#include "frame-capture-model.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("FrameCaptureModel");

NS_OBJECT_ENSURE_REGISTERED (FrameCaptureModel);
NS_OBJECT_ENSURE_REGISTERED (NoOpFrameCaptureModel);

TypeId
FrameCaptureModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FrameCaptureModel")
    .SetParent<Object> ()
    .SetGroupName ("Wifi")
  ;
  return tid;
}

TypeId
NoOpFrameCaptureModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NoOpFrameCaptureModel")
    .SetParent<FrameCaptureModel> ()
    .SetGroupName ("Wifi")
    .AddConstructor<NoOpFrameCaptureModel> ()
  ;
  return tid;
}

NoOpFrameCaptureModel::NoOpFrameCaptureModel ()
{
  NS_LOG_FUNCTION (this);
}

NoOpFrameCaptureModel::~NoOpFrameCaptureModel ()
{
  NS_LOG_FUNCTION (this);
}

bool
NoOpFrameCaptureModel::CaptureNewFrame (Ptr<Event> currentEvent, Ptr<Event> newEvent) const
{
  return false;
}

} //namespace ns3
