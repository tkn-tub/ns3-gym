#include "chunk.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (Chunk);

TypeId 
Chunk::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Chunk")
    .SetParent<ObjectBase> ()
  ;
  return tid;
}

} // namespace ns3
