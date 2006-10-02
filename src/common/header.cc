/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:f -*- */
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

#include "header.h"
#include <cassert>

namespace ns3 {

Header::Header ()
    : m_isDeserialized (false) {}

void 
Header::print (std::ostream &os) const
{
    printTo (os);
}
uint32_t
Header::getSize (void) const
{
	return getSerializedSize ();
}
void 
Header::serialize (Buffer::Iterator start) const
{
    serializeTo (start);
}
void 
Header::deserialize (Buffer::Iterator start)
{
    deserializeFrom (start);
    m_isDeserialized = true;
}
bool 
Header::isDeserialized (void) const
{
	return m_isDeserialized;
}



Header::~Header ()
{}

std::ostream& operator<< (std::ostream& os, Header const& header)
{
    header.print (os);
    return os;
}

}; // namespace ns3
