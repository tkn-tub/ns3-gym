/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 INRIA
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
 * Author: Alina Quereilhac <alina.quereilhac@inria.fr>
 *         Claudio Freire <klaussfreire@sourceforge.net>
 */

#ifndef PLANETLAB_FD_NET_DEVICE_HELPER_H
#define PLANETLAB_FD_NET_DEVICE_HELPER_H

#include <string>

#include "ns3/attribute.h"
#include "ns3/fd-net-device.h"
#include "ns3/object-factory.h"
#include "ns3/mac48-address.h"
#include "ns3/net-device-container.h"
#include "ns3/node-container.h"

#include "emu-fd-net-device-helper.h"

namespace ns3 {

/**
 * \brief build a set of FdNetDevice objects attached to a virtual TAP network
 * interface
 *
 */
class PlanetLabFdNetDeviceHelper : public EmuFdNetDeviceHelper
{
public:
  /**
   * Construct a PlanetLabFdNetDeviceHelper.
   */
  PlanetLabFdNetDeviceHelper ();
  virtual ~PlanetLabFdNetDeviceHelper ()
  {
  }

  /**
   * Set the device IPv4 address.
   *
   * \param address The IPv4 address for the TAP device.
   */
  void SetTapIpAddress (Ipv4Address address);

  /**
   * Set the network mask for the TAP device.
   *
   * \param mask The network mask for the TAP device.
   */
  void SetTapMask (Ipv4Mask mask);

protected:
  /*
   * \internal
   */
  Ptr<NetDevice> InstallPriv (Ptr<Node> node) const;

  /**
   * Sets a file descriptor on the FileDescriptorNetDevice.
   */
  virtual void SetFileDescriptor (Ptr<FdNetDevice> device) const;

  /**
   * Call out to a separate process running as suid root in order to create a
   * TAP device and obtain the file descriptor associated to it.
   */
  virtual int CreateFileDescriptor (void) const;

  /**
  * \internal
  *
  * The IP address for the TAP device.
  */
  Ipv4Address m_tapIp;

  /**
  * \internal
  *
  * The network mask for the TAP device.
  */
  Ipv4Mask m_tapMask;

};

} // namespace ns3

#endif /* PLANETLAB_FD_NET_DEVICE_HELPER_H */
