/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
/*
 * Copyright (c) 2006 INRIA
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
#include "mac-address-factory.h"

namespace yans {

MacAddressFactory::MacAddressFactory ()
{
	m_addr[0] = 0;
	m_addr[1] = 0;
	m_addr[2] = 0;
	m_addr[3] = 0;
	m_addr[4] = 0;
	m_addr[5] = 0;
	m_index = 0;
}

MacAddress 
MacAddressFactory::get_next (void)
{
	m_addr[m_index]++;
	if (m_addr[m_index] == 0) {
		m_index++;
	}
	return MacAddress (m_addr);
}

}; // namespace yans
