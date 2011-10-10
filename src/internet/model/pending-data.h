/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2006 Georgia Tech Research Corporation
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: Rajib Bhattacharjea<raj.b@gatech.edu>
//

// Georgia Tech Network Simulator - Data Descriptors
// George F. Riley.  Georgia Tech, Spring 2002

#ifndef PENDING_DATA_H
#define PENDING_DATA_H

#include "ns3/packet.h"
#include "pending-data.h"
#include "ns3/sequence-number.h"

#include "ns3/ptr.h"
namespace ns3
{
class Packet;

/**
 * \ingroup tcp
 *
 * \brief class for managing I/O between applications and TCP
 */
class PendingData {
public:
  PendingData ();
  PendingData (uint32_t s, uint8_t* d = NULL, uint32_t msg = 0, uint32_t resp = 0);
  PendingData (const std::string&); // Construct from string
  PendingData (uint8_t*, uint32_t&, Packet*); // Construct from serialized buffer
  PendingData (const PendingData&);   // Copy constructor
  virtual ~PendingData ();     // Destructor
  uint32_t Size () const { return size; }
  // Serialization
  uint8_t*  Serialize (uint8_t*, uint32_t&); // Serialize to a buffer
  uint8_t*  Construct (uint8_t*, uint32_t&); // Construct from buffer
  virtual void Clear (); // Remove all associated data
  virtual void Add (uint32_t s, const uint8_t* d = 0); // Add some data to end
  virtual void Add (Ptr<Packet> p);
  /**
   * This method returns the number of bytes in the PendingData buffer
   * beyond the sequence number specified by seqOffset.
   *
   * The variables seqFront and seqOffset correspond to a sequence number
   * space in use by the user.  What is significant in this method is the
   * difference between them; i.e. the quantity (seqOffset - seqFront). 
   * This difference is subtracted from Size(), yielding the number of
   * bytes beyond seqOffset, from the user perspective, in the PendingData
   * buffer.
   *
   * If the first number specified is not a sequence number that corresponds
   * to the first data byte in the PendingData buffer, the computation
   * returned will be in error.
   * 
   * \return number of bytes 
   * \param seqFront sequence number of assumed first byte in the PendingData
   * \param seqOffset sequence number of offset 
   */
  virtual uint32_t SizeFromSeq (const SequenceNumber32& seqFront, const SequenceNumber32& seqOffset);
  // Inquire available data from offset
  /**
   * \return number of bytes in the data buffer beyond the offset specified
   * \param offset offset (from zero) 
   */
  virtual uint32_t SizeFromOffset (uint32_t offset);
  // Available size from sequence difference 
  /**
   * Subtracts seqFront from seqOffset after enforcing seqFront is less
   * than seqOffset
   *
   * \param seqFront sequence number to be subtracted from seqOffset
   * \param seqOffset higher sequence number
   * \return seqOffset-seqFront
   */
  virtual uint32_t OffsetFromSeq (const SequenceNumber32& seqFront, const SequenceNumber32& seqOffset);
  virtual Ptr<Packet> CopyFromOffset (uint32_t, uint32_t);  // Size, offset, ret packet
  // Copy data, size, offset specified by sequence difference
  virtual Ptr<Packet> CopyFromSeq (uint32_t, const SequenceNumber32&, const SequenceNumber32&);
  /**
   * Permits object to clear any pending data between seqFront and 
   * seqOffset - 1).  Callers should check the return value to determine
   * whether any data was removed from the front.
   *
   * \param seqFront sequence number to start to try to remove from 
   * \param seqOffset first sequence number in buffer that should be retained
   * \return number of bytes from the front that were removed from the buffer
   */
  virtual uint32_t RemoveToSeq (const SequenceNumber32& seqFront, const SequenceNumber32& seqOffset);
  PendingData*   Copy () const;          // Create a copy of this header
  PendingData*   CopyS (uint32_t);         // Copy with new size
  PendingData*   CopySD (uint32_t, uint8_t*); // Copy with new size, new data
public:
  uint32_t size;        // Number of data bytes
  std::vector<Ptr<Packet> > data;         // Corresponding data (may be null)
  // The next two fields allow simulated applications to exchange some info
  uint32_t msgSize;     // Total size of message
  uint32_t responseSize; // Size of response requested
};

} //namepsace ns3

#endif /* PENDING_DATA_H */
