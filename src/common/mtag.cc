#include "mtag.h"

namespace ns3 {

TypeId 
Mtag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Mtag")
    .SetParent<ObjectBase> ()
    ;
  return tid;
}


} // namespace ns3
