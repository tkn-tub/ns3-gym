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
#ifndef IPV4_H
#define IPV4_H

#include <stdint.h>
#include "ns3/object.h"
#include "ns3/socket.h"
#include "ns3/callback.h"
#include "ipv4-address.h"
#include "ipv4-interface-address.h"

namespace ns3 {

class Node;
class NetDevice;
class Packet;
class Ipv4RoutingProtocol;

/**
 * \ingroup node
 * \defgroup ipv4 Ipv4
 * 
 * \brief Access to the Ipv4 forwarding table, interfaces, and configuration
 *
 * This class defines the API to manipulate the following aspects of
 * the Ipv4 implementation:  
 * -# set/get an Ipv4RoutingProtocol 
 * -# register a NetDevice for use by the Ipv4 layer (basically, to
 * create Ipv4-related state such as addressing and neighbor cache that 
 * is associated with a NetDevice)
 * -# manipulate the status of the NetDevice from the Ipv4 perspective, 
 * such as marking it as Up or Down, 
 * -# adding, deleting, and getting addresses associated to the Ipv4 
 * interfaces.
 * -# exporting Ipv4 configuration attributes
 * 
 * Each NetDevice has conceptually a single Ipv4 interface associated
 * with it (the corresponding structure in the Linux Ipv4 implementation
 * is struct in_device).  Each interface may have one or more Ipv4
 * addresses associated with it.  Each Ipv4 address may have different
 * subnet mask, scope, etc., so all of this per-address information 
 * is stored in an Ipv4InterfaceAddress class (the corresponding 
 * structure in Linux is struct in_ifaddr)
 *
 * Ipv4 attributes such as whether IP forwarding is enabled and disabled
 * are also stored in this class
 *
 * TO DO:  Add API to allow access to the Ipv4 neighbor table
 *
 * \see Ipv4RoutingProtocol
 * \see Ipv4InterfaceAddress
 */
class Ipv4 : public Object
{
public:
  static TypeId GetTypeId (void);
  Ipv4 ();
  virtual ~Ipv4 ();

  /**
   * \brief Register a new routing protocol to be used by this Ipv4 stack
   *   
   * This call will replace any routing protocol that has been previously 
   * registered.  If you want to add multiple routing protocols, you must
   * add them to a Ipv4ListRoutingProtocol directly.
   * 
   * \param routingProtocol smart pointer to Ipv4RoutingProtocol object
   */
  virtual void SetRoutingProtocol (Ptr<Ipv4RoutingProtocol> routingProtocol) = 0;

  /**
   * \brief Get the routing protocol to be used by this Ipv4 stack
   * 
   * \returns smart pointer to Ipv4RoutingProtocol object, or null pointer if none
   */
  virtual Ptr<Ipv4RoutingProtocol> GetRoutingProtocol (void) const = 0;

  /**
   * \param device device to add to the list of Ipv4 interfaces
   *        which can be used as output interfaces during packet forwarding.
   * \returns the index of the Ipv4 interface added.
   *
   * Once a device has been added, it can never be removed: if you want
   * to disable it, you can invoke Ipv4::SetDown which will
   * make sure that it is never used during packet forwarding.
   */
  virtual uint32_t AddInterface (Ptr<NetDevice> device) = 0;

  /**
   * \returns the number of interfaces added by the user.
   */
  virtual uint32_t GetNInterfaces (void) const = 0;  

  /**
   * \brief Return the interface number of the interface that has been
   *        assigned the specified IP address.
   *
   * \param address The IP address being searched for
   * \returns The interface number of the Ipv4 interface with the given 
   *          address or -1 if not found.
   *
   * Each IP interface has one or more IP addresses associated with it.  
   * This method searches the list of interfaces for one that holds a
   * particular address.  This call takes an IP address as a parameter and
   * returns the interface number of the first interface that has been assigned
   * that address, or -1 if not found.  There must be an exact match.
   */
  virtual int32_t GetInterfaceForAddress (Ipv4Address address) const = 0;

  /**
   * \brief Return the interface number of first interface found that 
   *  has an Ipv4 address within the prefix specified by the input
   *  address and mask parameters
   *
   * \param address The IP address assigned to the interface of interest.
   * \param mask The IP prefix to use in the mask
   * \returns The interface number of the Ipv4 interface with the given 
   *          address or -1 if not found.
   *
   * Each IP interface has one or more IP addresses associated with it.  
   * This method searches the list of interfaces for the first one found
   * that holds an address that is included within the prefix 
   * formed by the input address and mask parameters.  The value -1 is
   * returned if no match is found.
   */
  virtual int32_t GetInterfaceForPrefix (Ipv4Address address,
    Ipv4Mask mask) const = 0;

  /**
   * \param interface The interface number of an Ipv4 interface.
   * \returns The NetDevice associated with the Ipv4 interface number.
   */
  virtual Ptr<NetDevice> GetNetDevice (uint32_t interface) = 0;

  /**
   * \param device The NetDevice for an Ipv4Interface
   * \returns The interface number of an Ipv4 interface or -1 if not found.
   */
  virtual int32_t GetInterfaceForDevice (Ptr<const NetDevice> device) const = 0;

  /**
   * \param interface Interface number of an Ipv4 interface
   * \param address Ipv4InterfaceAddress address to associate with the underlying Ipv4 interface
   * \returns true if the operation succeeded
   */
  virtual bool AddAddress (uint32_t interface, Ipv4InterfaceAddress address) = 0;

  /**
   * \param interface Interface number of an Ipv4 interface
   * \returns the number of Ipv4InterfaceAddress entries for the interface.
   */
  virtual uint32_t GetNAddresses (uint32_t interface) const = 0;

  /**
   * Because addresses can be removed, the addressIndex is not guaranteed
   * to be static across calls to this method.
   * 
   * \param interface Interface number of an Ipv4 interface
   * \param addressIndex index of Ipv4InterfaceAddress 
   * \returns the Ipv4InterfaceAddress associated to the interface and addresIndex
   */
  virtual Ipv4InterfaceAddress GetAddress (uint32_t interface, uint32_t addressIndex) const = 0;

  /**
   * Remove the address at addressIndex on named interface.  The addressIndex
   * for all higher indices will decrement by one after this method is called;
   * so, for example, to remove 5 addresses from an interface i, one could
   * call RemoveAddress (i, 0); 5 times.  
   * 
   * \param interface Interface number of an Ipv4 interface
   * \param addressIndex index of Ipv4InterfaceAddress to remove 
   * \returns true if the operation succeeded
   */
  virtual bool RemoveAddress (uint32_t interface, uint32_t addressIndex) = 0;

  /**
   * \param interface The interface number of an Ipv4 interface
   * \param metric routing metric (cost) associated to the underlying 
   *          Ipv4 interface
   */
  virtual void SetMetric (uint32_t interface, uint16_t metric) = 0;

  /**
   * \param interface The interface number of an Ipv4 interface
   * \returns routing metric (cost) associated to the underlying 
   *          Ipv4 interface
   */
  virtual uint16_t GetMetric (uint32_t interface) const = 0;

  /**
   * \param interface Interface number of Ipv4 interface
   * \returns the Maximum Transmission Unit (in bytes) associated
   *          to the underlying Ipv4 interface
   */
  virtual uint16_t GetMtu (uint32_t interface) const = 0;

  /**
   * \param interface Interface number of Ipv4 interface
   * \returns true if the underlying interface is in the "up" state,
   *          false otherwise.
   */
  virtual bool IsUp (uint32_t interface) const = 0;

  /**
   * \param interface Interface number of Ipv4 interface
   * 
   * Set the interface into the "up" state. In this state, it is
   * considered valid during Ipv4 forwarding.
   */
  virtual void SetUp (uint32_t interface) = 0;

  /**
   * \param interface Interface number of Ipv4 interface
   *
   * Set the interface into the "down" state. In this state, it is
   * ignored during Ipv4 forwarding.
   */
  virtual void SetDown (uint32_t interface) = 0;

  /**
   * \param interface Interface number of Ipv4 interface
   * \returns true if IP forwarding enabled for input datagrams on this device
   */
  virtual bool IsForwarding (uint32_t interface) const = 0;

  /**
   * \param interface Interface number of Ipv4 interface
   * \param val Value to set the forwarding flag
   * 
   * If set to true, IP forwarding is enabled for input datagrams on this device
   */
  virtual void SetForwarding (uint32_t interface, bool val) = 0;

  static const uint32_t IF_ANY = 0xffffffff;

private:
  // Indirect the Ipv4 attributes through private pure virtual methods
  virtual void SetIpForward (bool forward) = 0;
  virtual bool GetIpForward (void) const = 0;
};

} // namespace ns3 

#endif /* IPV4_H */
