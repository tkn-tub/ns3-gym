#include "trailer.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (Trailer);

Trailer::~Trailer ()
{}

TypeId 
Trailer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Trailer")
    .SetParent<ObjectBase> ()
    ;
  return tid;
}

std::ostream & operator << (std::ostream &os, const Trailer &trailer)
{
  trailer.Print (os);
  return os;
}

} // namespace ns3
