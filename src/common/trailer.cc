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

} // namespace ns3
