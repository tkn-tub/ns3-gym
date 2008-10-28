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

#ifndef __datapdu_h__
#define __datapdu_h__

#include "ns3/packet.h"
#include "pending-data.h"
#include "sequence-number.h"

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
  uint32_t Size () const { return size;}
  // Serialization
  uint32_t SSize ();                   // Size needed for serialization
  uint8_t*  Serialize (uint8_t*, uint32_t&); // Serialize to a buffer
  uint8_t*  Construct (uint8_t*, uint32_t&); // Construct from buffer
  virtual void Clear ();// Remove all associated data
  virtual void Add (uint32_t s, const uint8_t* d = 0);// Add some data to end
  virtual void Add (Ptr<Packet> p);
  // Inquire available data from (f,o) sequence pair
  virtual uint32_t SizeFromSeq (const SequenceNumber&, const SequenceNumber&);
  // Inquire available data from offset
  virtual uint32_t SizeFromOffset (uint32_t);
  // Available size from sequence difference 
  virtual uint32_t OffsetFromSeq (const SequenceNumber&, const SequenceNumber&);
  virtual Ptr<Packet> CopyFromOffset (uint32_t, uint32_t);  // Size, offset, ret packet
  // Copy data, size, offset specified by sequence difference
  virtual Ptr<Packet> CopyFromSeq (uint32_t, const SequenceNumber&, const SequenceNumber&);
  PendingData*   Copy () const;          // Create a copy of this header
  PendingData*   CopyS (uint32_t);         // Copy with new size
  PendingData*   CopySD (uint32_t, uint8_t*); // Copy with new size, new data
public:
  uint32_t size;        // Number of data bytes
  std::vector<Ptr<Packet> > data;         // Corresponding data (may be null)
  // The next two fields allow simulated applications to exchange some info
  uint32_t msgSize;     // Total size of message
  uint32_t responseSize;// Size of response requested
};

}//namepsace ns3
#endif

