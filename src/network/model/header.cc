#include "header.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (Header);

Header::~Header ()
{}

TypeId 
Header::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Header")
    .SetParent<Chunk> ()
    ;
  return tid;
}

std::ostream & operator << (std::ostream &os, const Header &header)
{
  header.Print (os);
  return os;
}

} // namespace ns3
