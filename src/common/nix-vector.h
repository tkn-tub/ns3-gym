/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 The Georgia Institute of Technology 
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
 * Authors: Josh Pelkey <jpelkey@gatech.edu>
 */

#ifndef __NIX_VECTOR_H__
#define __NIX_VECTOR_H__

#include "ns3/object.h"
#include "ns3/buffer.h"

namespace ns3 {

/**
 * \ingroup packet
 *
 * \brief Neighbor-index data structure for nix-vector routing
 *
 * This data structure holds a vector of "neighbor-indexes" for 
 * a simulation specific routing protocol, nix-vector routing.  
 * Theses neighbor-indexes correspond to the net-device which a 
 * node should use to route a packet.  A nix-vector is built
 * (or fetched from a cache) on-demand. The nix-vector is 
 * transmitted with the packet, and along each hop of the 
 * route, the current node extracts the appropriate 
 * neighbor-index and routes the packet.
 *
 * \internal
 * The implementation of NixVector uses a vector to store 
 * the neighbor-indexes.  Each entry in the vector is 32 
 * bits long and can store multiple neighbor-indexes.  A 
 * fair amount of bit manipulation is used to store these 
 * neighbor-indexes efficiently.  A vector is used so that 
 * the nix-vector can grow arbitraily if the topoplogy and 
 * route requires a large number of neighbor-indexes.
 *
 * As the nix-vector travels along the route, an internal 
 * private member variable keeps track of how many bits 
 * have been used.  At a particular node, the nix-vector 
 * is used to return the next neighbor-index. This 
 * neighbor-index is used to determine which net-device 
 * to use.  The number of bits used would then be 
 * incremented accordingly, and the packet would be 
 * routed.
 */

class NixVector : public Object
{
  public:
    NixVector ();
    NixVector (const NixVector &o);
    ~NixVector ();
    Ptr<NixVector> Copy (void) const;
    NixVector &operator = (const NixVector &o);
    static TypeId GetTypeId (void);
    /**
     * \param newBits the neighbor-index to be added to the vector
     * \param numberOfBits the number of bits that newBits contains
     *
     * Adds the neighbor index to the vector using a fair amount of 
     * bit manipulation to pack everything in efficiently.
     *
     * Note: This function assumes that the number of bits to be added
     * is always less than or equal to 32, ie., you can only span one 
     * entry of a nix-vector at a time.  This is reasonable, since 32 
     * bits gives you 2^32 possible neighbors.
     */
    void AddNeighborIndex (uint32_t newBits, uint32_t numberOfBits);
    /**
     * \return the neighbor index
     *
     * \param numberOfBits the number of bits to extract from the vector
     *
     * Extracts the number of bits specified from 
     * the vector and returns the value extracted
     *
     * Note: This function assumes that the number of bits to be extracted 
     * is always less than or equal to 32, ie., you can only span one 
     * entry of a nix-vector at a time.  This is reasonable, since 32 
     * bits gives you 2^32 possible neighbors.
     */
    uint32_t ExtractNeighborIndex (uint32_t numberOfBits);
    /**
     * \return number of bits remaining in the 
     *         nix-vector (ie m_total - m_used)
     */
    uint32_t GetRemainingBits (void);
    /**
     * \return the number of bytes required for serialization 
     */
    uint32_t GetSerializedSize (void) const;
    /**
     * \param i Buffer iterator for writing
     *
     * \param size number of bytes to write
     */
    void Serialize (Buffer::Iterator i, uint32_t size) const;
    /**
     * \return the number of bytes deserialized
     *
     * \param i Buffer iterator for reading
     */
    uint32_t Deserialize (Buffer::Iterator i);
    /**
     * \return number of bits of numberOfNeighbors
     *
     * \param numberOfNeighbors the total number of neighbors
     *
     * This function is used to determine the number of bits of 
     * numberOfNeighbors so that this value can be passed in to 
     * AddNeighborIndex or ExtractNeighborIndex.
     */
    uint32_t BitCount (uint32_t numberOfNeighbors) const;  
    /* for printing of nix-vector */
    void DumpNixVector (std::ostream &os) const;
    /* for printing of nix-vector */
    friend std::ostream & operator <<( std::ostream &outs, const NixVector &nix); 


  private:
    typedef std::vector<uint32_t> NixBits_t;

    /* the actual nix-vector */
    NixBits_t m_nixVector;

    /* for tracking where we are
     * in the nix-vector
     */
    uint32_t m_used;
                                  
    /* for tracking how many bits we
     * have used in the current vector
     * entry. need this in order to 
     * expand the vector passed 32bits
     */
    uint32_t m_currentVectorBitSize;

    /* a counter of how total bits are in 
     * the nix-vector
     */
    uint32_t m_totalBitSize;

    /* internal for pretty printing of nix-vector */
    void PrintDec2BinNixFill (uint32_t, uint32_t, std::ostream &os) const;

    /* internal for pretty printing of nix-vector */
    void PrintDec2BinNix (uint32_t, uint32_t, std::ostream &os) const;
};
} // namespace ns3

#endif
