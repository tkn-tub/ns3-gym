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
