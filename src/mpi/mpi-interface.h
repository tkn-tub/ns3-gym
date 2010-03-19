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

const uint32_t MAX_MPI_MSG_SIZE = 2000;
// Define a class for tracking the non-block sends
class SentBuffer
{
public:
  SentBuffer ();
  ~SentBuffer ();

  uint8_t* GetBuffer ();
  void SetBuffer (uint8_t*);
  MPI_Request* GetRequest ();

private:
  uint8_t* m_buffer;
  MPI_Request m_request;
};

class Packet;

class MpiInterface
{
public:
  static void     Destroy ();
  static uint32_t GetSystemId ();        // Get the MPI Rank (system id)
  static uint32_t GetSize ();        // Get the MPI Size (number of systems)
  static bool     IsEnabled ();   // True if using MPI
  static void     Enable (int*, char***); // Called by ns3 main program

  // Serialize and send a packet to the specified node and net device
  static void     SendPacket (Ptr<Packet>, const Time &, uint32_t, uint32_t);
  static void     ReceiveMessages ();  // Check for received messages complete
  static void     TestSendComplete (); // Check for completed sends
  static uint32_t GetRxCount ();
  static uint32_t GetTxCount ();

private:
  static uint32_t m_sid;
  static uint32_t m_size;
  static uint32_t m_rxCount;   // Total packets received
  static uint32_t m_txCount;   // Total packets sent
  static bool     m_initialized;
  static bool     m_enabled;
  static MPI_Request* m_requests; // Pending non-blocking receives
  static char**   m_pRxBuffers;   // Data buffers for non-blocking reads
  static std::list<SentBuffer> m_pendingTx;  // List of pending non-blocking sends
};

} // namespace ns3

#endif /* NS3_MPI_INTERFACE_H */
