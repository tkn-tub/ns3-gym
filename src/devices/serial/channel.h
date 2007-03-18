/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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
 * Author: Craig Dowell <craigdo@ee.washingon.edu>
 *
 *      Wed Feb 14 16:05:46 PST 2007 craigdo:  Created
 */

#include <list>
#include "ns3/packet.h"
#include "layer-connector.h"

#ifndef CHANNEL_H
#define CHANNEL_H

namespace ns3 {

/**
 * \brief Abstract Channel Base Class.
 *
 * A channel is a logical path over which information flows.  The path can
 * be as simple as a short piece of wire, or as complicated as space-time.
 */
class Channel
{
public:
  Channel ();
  Channel (std::string name);
  virtual ~Channel ();

  virtual void SetName(std::string);
  virtual std::string GetName(void);

protected:
  std::string m_name;

private:
};

}; // namespace ns3

#endif /* CHANNEL_H */
