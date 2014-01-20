/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright 2013. Lawrence Livermore National Security, LLC.
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
 * Author: Steven Smith <smith84@llnl.gov>
 *
 */

#ifndef NS3_PARALLEL_COMMUNICATION_INTERFACE_H
#define NS3_PARALLEL_COMMUNICATION_INTERFACE_H

#include <stdint.h>
#include <list>

#include <ns3/object.h>
#include <ns3/nstime.h>
#include <ns3/buffer.h>
#include <ns3/packet.h>

#if defined(NS3_MPI)
#include "mpi.h"
#endif

namespace ns3 {

/**
 * \ingroup mpi
 *
 * \brief Pure virtual base class for the interface between ns-3 and
 * the parallel communication layer being used.
 *
 * Each type of parallel communication layer is required to implement
 * this interface.  This interface is called through the
 * MpiInterface.
 */
  class ParallelCommunicationInterface
{
public:
  /**
    * Destructor
    */
  virtual ~ParallelCommunicationInterface() {}
  /**
   * Deletes storage used by the parallel environment.
   */
  virtual void Destroy () = 0;
  /**
   * \return system identification
   */
  virtual uint32_t GetSystemId () = 0;
  /**
   * \return number of parallel tasks
   */
  virtual uint32_t GetSize () = 0;
  /**
   * \return true if parallel communication is enabled
   */
  virtual bool IsEnabled () = 0;
  /**
   * \param pargc number of command line arguments
   * \param pargv command line arguments
   *
   * Sets up parallel communication interface
   */
  virtual void Enable (int* pargc, char*** pargv) = 0;
  /**
   * Terminates the parallel environment.
   * This function must be called after Destroy ()
   */
  virtual void Disable () = 0;
  /**
   * \param p packet to send
   * \param rxTime received time at destination node
   * \param node destination node
   * \param dev destination device
   *
   * Serialize and send a packet to the specified node and net device
   */
  virtual void SendPacket (Ptr<Packet> p, const Time &rxTime, uint32_t node, uint32_t dev) = 0;

private:
};

} // namespace ns3

#endif /* NS3_PARALLEL_COMMUNICATION_INTERFACE_H */
