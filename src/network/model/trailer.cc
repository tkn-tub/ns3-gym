#include "trailer.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Trailer");

NS_OBJECT_ENSURE_REGISTERED (Trailer);

Trailer::~Trailer ()
{
  NS_LOG_FUNCTION (this);
}

TypeId
Trailer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Trailer")
    .SetParent<Chunk> ()
    .SetGroupName("Network")
  ;
  return tid;
}

std::ostream & operator << (std::ostream &os, const Trailer &trailer)
{
  trailer.Print (os);
  return os;
}

} // namespace ns3
