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

#ifndef NS3_MPI_INTERFACE_H
#define NS3_MPI_INTERFACE_H

#include <ns3/nstime.h>
#include <ns3/packet.h>

namespace ns3 {
/**
 * \defgroup mpi MPI Distributed Simulation
 *
 */

class ParallelCommunicationInterface;

/**
 * \ingroup mpi
 *
 * \brief Singleton used to interface to the communications infrastructure
 * when running NS3 in parallel.  
 *
 * Delegates the implementation to the specific parallel
 * infrastructure being used.  Implementation is defined in the
 * ParallelCommunicationInterface virtual base class; this API mirrors
 * that interface.  This singleton is responsible for instantiating an
 * instance of the communication interface based on
 * SimulatorImplementationType attribute in ns3::GlobalValues.  The
 * attribute must be set before Enable is invoked.
 */
class MpiInterface
{
public:
  /**
   * Deletes storage used by the parallel environment.
   */
  static void Destroy ();
  /**
   * \return system identification
   *
   * When running a sequential simulation this will return a systemID of 0.
   */
  static uint32_t GetSystemId ();
  /**
   * \return number of parallel tasks
   *
   * When running a sequential simulation this will return a size of 1.
   */
  static uint32_t GetSize ();
  /**
   * \return true if parallel communication is enabled
   */
  static bool IsEnabled ();
  /**
   * \param pargc number of command line arguments
   * \param pargv command line arguments
   *
   * \brief Sets up parallel communication interface.
   *
   * SimulatorImplementationType attribute in ns3::GlobalValues must be set before
   * Enable is invoked.
   */
  static void Enable (int* pargc, char*** pargv);
  /**
   * Terminates the parallel environment.
   * This function must be called after Destroy ()
   */
  static void Disable ();
  /**
   * \param p packet to send
   * \param rxTime received time at destination node
   * \param node destination node
   * \param dev destination device
   *
   * Serialize and send a packet to the specified node and net device
   */
  static void SendPacket (Ptr<Packet> p, const Time &rxTime, uint32_t node, uint32_t dev);
private:

  /**
   * Static instance of the instantiated parallel controller.
   */
  static ParallelCommunicationInterface* g_parallelCommunicationInterface;
};

} // namespace ns3

#endif /* NS3_MPI_INTERFACE_H */
