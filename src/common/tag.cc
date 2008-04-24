#include "tag.h"

namespace ns3 {

TypeId 
Tag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Tag")
    .SetParent<ObjectBase> ()
    ;
  return tid;
}


} // namespace ns3
