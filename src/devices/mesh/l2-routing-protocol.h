/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */


#ifndef L2_ROUTING_PROTOCOL_H
#define L2_ROUTING_PROTOCOL_H

#include <list>
#include <vector>
#include <ostream>
#include "ns3/nstime.h"
#include "ns3/mac48-address.h"
#include "ns3/net-device.h"
#include "ns3/ptr.h"

namespace ns3 {

class Packet;
class MeshPointDevice;

/**
 * \ingroup mesh
 *
 * \brief Interface for L2 mesh routing protocol and mesh point communication. 
 *
 * Every mesh routing protocol must implement this interface. Each mesh point (MeshPointDevice) is supposed
 * to know single L2RoutingProtocol to work with, see MeshPointDevice::SetRoutingProtocol().  
 * 
 * This interface is similar to ipv4 routiong protocol base class.
 */
class L2RoutingProtocol : public Object
{
public:
  /// Support NS3 object model
  static TypeId GetTypeId();
  /// virtual D-tor
  virtual ~L2RoutingProtocol();
  
  /**
   * Callback to be invoked when route discovery  procedure is completed.
   * 
   * \param flag        indicating whether a route was actually found and all needed information is 
   *                    added to the packet succesfully
   *                    
   * \param packet      for which the route was resolved. All routing information for MAC layer
   *                    must be stored in proper tags (like in case of HWMP, when WifiMacHeader 
   *                    needs address of next hop), or must be added as a packet header (if MAC
   *                    does not need any additional information). So, the packet is returned back 
   *                    to MeshPointDevice looks like a pure packet with ethernet header 
   *                    (i.e data + src +dst + protocol). The only special information addressed
   *                    to MeshPointDevice is an outcoming interface ID.
   *                    
   * \param src         source address of the packet
   * 
   * \param dst         destiation address of the packet
   * 
   * \param protocol    ethernet 'Protocol' field, needed to form a proper MAC-layer header
   * 
   * \param uint32_t    outcoming interface to use or 0xffffffff if packet should be sent by ALL interfaces
   */
  typedef Callback<void,                /* return type */
                   bool,                /* flag */
                   Ptr<Packet>,         /* packet */
                   Mac48Address,        /* src */
                   Mac48Address,        /* dst */
                   uint16_t,            /* protocol */
                   uint32_t             /* out interface ID */ 
          > RouteReplyCallback;
  /**
   * Request routing information, all packets must go through this request.
   * 
   * Note that route discobery works async. -- RequestRoute returns immediately, while
   * reply callback will be called when routing information will be avaliable.
   *  
   * \return true if valid route is already known
   * 
   * \param sourceIface         the incoming interface of the packet
   * 
   * \param source              source address
   * 
   * \param destination         destination address
   * 
   * \param packet              the packet to be resolved (needed the whole packet, because routing information 
   *                            is added as tags or headers). The packet will be retutned to reply callback. 
   *                            
   * \param protocolType        protocol ID, needed to form a proper MAC-layer header
   * 
   * \param routeReply          callback to be invoked after route discovery procedure, supposed to really send packet using routing information.
   */
  virtual bool RequestRoute(uint32_t sourceIface, const Mac48Address source, const Mac48Address destination, 
                            Ptr<Packet> packet, uint16_t  protocolType, RouteReplyCallback routeReply ) = 0;
  /**
   * Set host mesh point, analog of SetNode(...) methods for upper layer protocols
   */
  virtual void SetMeshPoint(MeshPointDevice * mp) = 0;
  
protected:
  struct QueuedPacket {
    Ptr<Packet> pkt;
    Mac48Address dst;
    Mac48Address src;
    uint16_t protocol;
    uint32_t inPort;
    RouteReplyCallback reply;
  };
  /**
   * \brief Set Max queue size per destinztion
   * \details Routing Queue is implemented inside the
   * routing protocol and keeps one queue per
   * destination (to make it easier to find resolved and
   * timed out packets).
   * \param maxPacketsPerDestination Packets per
   * destination that can be stored inside protocol.
   */
  virtual void SetMaxQueueSize(int maxPacketsPerDestination) = 0;
  /**
   * \brief Queue packet with 'Ethernet header'
   * \returns false if the queue is full.
   */
  virtual bool QueuePacket(struct QueuedPacket packet) = 0;
  /**
   * \brief Deques packet with 'Ethernet header'
   * \returns Ptr<packet> (NULL if queue is empty), src,
   * dst, protocol ID, incoming port ID, and reply
   * callback
   * \param destination The destination address, which
   * identifyes queue.
   */
  virtual struct QueuedPacket DequeuePacket(Mac48Address destination) = 0;
};
}//namespace ns3
#endif
