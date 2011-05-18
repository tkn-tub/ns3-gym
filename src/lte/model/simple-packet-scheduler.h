/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 */

#ifndef SIMPLE_PACKET_SCHEDULER_H
#define SIMPLE_PACKET_SCHEDULER_H


#include <ns3/nstime.h>
#include <ns3/object.h>
#include <list>
#include <ns3/ptr.h>
#include "packet-scheduler.h"


namespace ns3 {


/**
 * \ingroup lte
 *
 * This class implements a simple packet scheduler
 */
class SimplePacketScheduler : public PacketScheduler
{

public:
  SimplePacketScheduler ();

  /**
   * \brief Create a simple packet scheduler
   * \param enb the enb where the packet scheduler works
   */
  SimplePacketScheduler (Ptr<EnbNetDevice> enb);
  virtual ~SimplePacketScheduler ();

  static TypeId GetTypeId (void);

  virtual void DoRunPacketScheduler (void);


private:
};


}

#endif /* SIMPLE_PACKET_SCHEDULER_H */
