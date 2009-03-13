/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */


#ifndef MAC48ADDRESS_COMPARATOR
#define MAC48ADDRESS_COMPARATOR
#include "ns3/mac48-address.h"
namespace ns3 {
struct mac48addrComparator
			{
				bool operator()(const Mac48Address addr1, Mac48Address addr2) const
				{
					uint8_t s1[6], s2[6];
					addr1.CopyTo(s1);
					addr2.CopyTo(s2);
					for(int i = 0; i < 6; i ++)
						if(s1[i] > s2[i])
							return true;
					return false;
				}
			};
}//namespace ns3
#endif
