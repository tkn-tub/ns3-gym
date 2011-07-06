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
 * Author: George F. Riley<riley@ece.gatech.edu>
 */

// Interface between ns3 and the network animator

#ifndef ANIMATION_INTERFACE__H
#define ANIMATION_INTERFACE__H

#include <string>

#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/nstime.h"
#include "ns3/log.h"
#include "ns3/node-list.h"


namespace ns3 {

class NetModel;

/**
 * \defgroup netanim Netanim
 *
 * This section documents the API of the ns-3 netanim module. For a generic functional description, please refer to the ns-3 manual.
 */

/**
 * \ingroup netanim
 *
 * \brief Interface to network animator
 *
 * Provides functions that facilitate communications with an
 * external or internal network animator.
 */
class AnimationInterface
{
public:
/**
 * @brief Construct the animator interface. No arguments needed.
 */
  AnimationInterface ();
/**
 * @brief Destructor for the animator interface.
 */
  ~AnimationInterface ();
/**
 * @brief Specify that animation commands are to be written
 * to the specified output file.
 *
 * This call is used to write the animation information to a text
 * file that can later be used as input to the network animator tool.
 *
 * @param fn The name of the output file.
 * @returns true if successful open.
 */
  bool SetOutputFile (const std::string& fn);

/**
 * @brief Specify that animation commands are to be written to
 * a socket.
 *
 * This call is used to set the ns3 process in server mode, waiting
 * for a TCP connection from the animator.  This call will not
 * return until the animator connects in, or if the bind to the
 * specified port fails.
 *
 * @param port Port number to bind to.
 * @returns true if connection created, false if bind failed.
 */
  bool SetServerPort (uint16_t port);

/**
 * @brief Writes the topology information and sets up the appropriate
 *  animation packet tx callback
 *
 * Writes the topology information to the appropriate output, depending
 * on prior calls to SetOutputFile, SetServerPort, or SetInternalAnimation.
 * Then creates the callbacks needed for the animator to start processing
 * packets.
 *
 */
  void StartAnimation ();

/**
 * @brief Closes the interface to the animator.
 *
 */
  void StopAnimation ();


private:
  // Packet tx animation callback
  void DevTxTrace (std::string context, Ptr<const Packet> p,
                   Ptr<NetDevice> tx, Ptr<NetDevice> rx,
                   Time txTime, Time rxTime);
  // Write specified amount of data to the specified handle
  int  WriteN (int, const char*, uint32_t);
private:
  int       m_fHandle;  // File handle for output (-1 if none)
  NetModel* m_model;    // If non nil, points to the internal network model
                        // for the interlan animator
};
}
#endif

