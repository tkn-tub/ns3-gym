/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * This code should be moved to src/common/error-model.h during ns-3.9 
 * release cycle with some minor modifications, such as adding GetTypeId 
 * method and logging to all methods. This can be done by uncommenting 
 * relevant code below.
 */
#ifndef RECEIVE_LIST_ERROR_MODEL_H
#define RECEIVE_LIST_ERROR_MODEL_H

#include <list>
#include "ns3/error-model.h"
#include "ns3/object.h"
#include "ns3/random-variable.h"

namespace ns3 {

class Packet;

/**
 * \brief Provide a list of Packets to corrupt
 *
 * This model also processes a user-generated list of packets to
 * corrupt, except that the list corresponds to the sequence of
 * received packets as observed by this error model, and not the
 * Packet UID.
 * 
 * Reset() on this model will clear the list
 *
 * IsCorrupt() will not modify the packet data buffer
 */
class ReceiveListErrorModel : public ErrorModel
{
public:
  /* uncomment GetTypeId when moving to src/common/error-model.h */
  //static TypeId GetTypeId (void);
  ReceiveListErrorModel ();
  virtual ~ReceiveListErrorModel ();

  /**
   * \return a copy of the underlying list
   */
  std::list<uint32_t> GetList (void) const;
  /**
   * \param packetlist The list of packets to error.
   *
   * This method overwrites any previously provided list.
   */
  void SetList (const std::list<uint32_t> &packetlist);

private:
  virtual bool DoCorrupt (Ptr<Packet> p);
  virtual void DoReset (void);

  typedef std::list<uint32_t> PacketList;
  typedef std::list<uint32_t>::const_iterator PacketListCI;

  PacketList m_packetList;
  uint32_t m_timesInvoked;
  
};


} //namespace ns3
#endif
