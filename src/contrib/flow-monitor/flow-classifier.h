/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2009 INESC Porto
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: Gustavo J. A. M. Carneiro  <gjc@inescporto.pt> <gjcarneiro@gmail.com>
//

#ifndef __FLOW_CLASSIFIER_H__
#define __FLOW_CLASSIFIER_H__

#include "ns3/ref-count-base.h"
#include <ostream>

namespace ns3 {

typedef uint32_t FlowId;
typedef uint32_t FlowPacketId;


class FlowClassifier : public RefCountBase
{
  FlowId m_lastNewFlowId;

public:

  FlowClassifier ();

  virtual void SerializeToXmlStream (std::ostream &os, int indent) const = 0;

protected:
  FlowId GetNewFlowId ();

};


} // namespace ns3

#endif

