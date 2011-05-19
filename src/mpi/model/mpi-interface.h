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
 * Author: George Riley <riley@ece.gatech.edu>
 */

// This object contains static methods that provide an easy interface
// to the necessary MPI information.

#ifndef NS3_MPI_INTERFACE_H
#define NS3_MPI_INTERFACE_H

#include <stdint.h>
#include <list>

#include "ns3/nstime.h"
#include "ns3/buffer.h"

struct ompi_request_t;
typedef struct ompi_request_t* MPI_Request;

namespace ns3 {

/**
 * \defgroup mpi MPI Distributed Simulation
 *
 */

/**
 * maximum MPI message size for easy
 * buffer creation
 */
const uint32_t MAX_MPI_MSG_SIZE = 2000;

/**
 * \ingroup mpi
 *
 * Define a class for tracking the non-block sends
 */
class SentBuffer
{
public:
  SentBuffer ();
  ~SentBuffer ();

  /**
   * \return pointer to sent buffer
   */
  uint8_t* GetBuffer ();
  /**
   * \param buffer pointer to sent buffer
   */
  void SetBuffer (uint8_t* buffer);
  /**
   * \return MPI request
   */
  MPI_Request* GetRequest ();

private:
  uint8_t* m_buffer;
  MPI_Request m_request;
};

class Packet;

/**
 * \ingroup mpi
 *
 * Interface between ns-3 and MPI
 */
class MpiInterface
{
public:
  /**
   * Delete all buffers
   */
  static void Destroy ();
  /**
   * \return MPI rank
   */
  static uint32_t GetSystemId ();
  /**
   * \return MPI size (number of systems)
   */
  static uint32_t GetSize ();
  /**
   * \return true if using MPI
   */
  static bool IsEnabled ();
  /**
   * \param pargc number of command line arguments
   * \param pargv command line arguments
   *
   * Sets up MPI interface
   */
  static void Enable (int* pargc, char*** pargv);
  /**
   * Terminates the MPI environment by calling MPI_Finalize
   * This function must be called after Destroy ()
   * It also resets m_initialized, m_enabled
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
  /**
   * Check for received messages complete
   */
  static void ReceiveMessages ();
  /**
   * Check for completed sends
   */
  static void TestSendComplete ();
  /**
   * \return received count in packets
   */
  static uint32_t GetRxCount ();
  /**
   * \return transmitted count in packets
   */
  static uint32_t GetTxCount ();

private:
  static uint32_t m_sid;
  static uint32_t m_size;

  // Total packets received
  static uint32_t m_rxCount;

  // Total packets sent
  static uint32_t m_txCount;
  static bool     m_initialized;
  static bool     m_enabled;

  // Pending non-blocking receives
  static MPI_Request* m_requests;

  // Data buffers for non-blocking reads
  static char**   m_pRxBuffers;

  // List of pending non-blocking sends
  static std::list<SentBuffer> m_pendingTx;
};

} // namespace ns3

#endif /* NS3_MPI_INTERFACE_H */
