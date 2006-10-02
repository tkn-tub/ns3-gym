/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2005 INRIA
 * All rights reserved.
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#ifndef HEADER_H
#define HEADER_H

#include <stdint.h>
#include <ostream>
#include "buffer.h"

namespace ns3 {

/**
 * \brief Protocol header serialization and deserialization.
 *
 * Every Protocol header which needs to be inserted and removed
 * from a Packet instance must derive from this abstract base class
 * and implement the private pure virtual methods listed below:
 *   - ns3::Header::serializeTo
 *   - ns3::Header::deserializeFrom
 *   - ns3::Header::getSerializedSize
 *   - ns3::Header::printTo
 */
class Header {
public:
    Header ();
    /**
     * Derived classes must provide an explicit virtual destructor
     */
    virtual ~Header () = 0;

    void print (std::ostream &os) const;
    uint32_t getSize (void) const;
    void serialize (Buffer::Iterator start) const;
    void deserialize (Buffer::Iterator start);
    bool isDeserialized (void) const;
private:
    bool m_isDeserialized;
    /**
     * \param os the std output stream in which this 
     *       protocol header must print itself.
     */
    virtual void printTo (std::ostream &os) const = 0;

    /**
     * \returns the size of the serialized Header.
     */
    virtual uint32_t getSerializedSize (void) const = 0;

    /**
     * \param start the buffer iterator in which the protocol header
     *    must serialize itself.
     */
    virtual void serializeTo (Buffer::Iterator start) const = 0;
    /**
     * \param start the buffer iterator from which the protocol header must
     *    deserialize itself.
     */
    virtual void deserializeFrom (Buffer::Iterator start) = 0;
};

std::ostream& operator<< (std::ostream& os, Header const& header);

}; // namespace ns3

#endif /* HEADER_H */
