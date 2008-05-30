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
 */

#ifndef CHANNEL_H
#define CHANNEL_H

#include <string>
#include <stdint.h>
#include "ns3/object.h"
#include "ns3/ptr.h"

namespace ns3 {

class NetDevice;

/**
 * \ingroup node
 * \defgroup channel Channel
 *
 * \brief Abstract Channel Base Class.
 *
 * A channel is a logical path over which information flows.  The path can
 * be as simple as a short piece of wire, or as complicated as space-time.
 */
class Channel : public Object
{
public:
  static TypeId GetTypeId (void);

  Channel ();
  Channel (std::string name);

  void SetName(std::string);
  std::string GetName(void);

  /**
   * \returns the number of NetDevices connected to this Channel.
   *
   * This method must be implemented by subclasses.
   */
  virtual uint32_t GetNDevices (void) const = 0;
  /**
   * \param i index of NetDevice to retrieve
   * \returns one of the NetDevices connected to this channel.
   *
   * This method must be implemented by subclasses.
   */
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const = 0;

protected:
  virtual      ~Channel ();
  std::string   m_name;
};

}; // namespace ns3

#endif /* CHANNEL_H */
