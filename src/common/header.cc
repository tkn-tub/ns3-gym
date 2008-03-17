#include "header.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (Header);

Header::~Header ()
{}

TypeId 
Header::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Header")
    .SetParent<ObjectBase> ()
    ;
  return tid;
}

} // namespace ns3
