#include "header.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Header");

NS_OBJECT_ENSURE_REGISTERED (Header);

Header::~Header ()
{
  NS_LOG_FUNCTION (this);
}

TypeId
Header::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Header")
    .SetParent<Chunk> ()
    .SetGroupName("Network")
  ;
  return tid;
}

std::ostream & operator << (std::ostream &os, const Header &header)
{
  header.Print (os);
  return os;
}

} // namespace ns3
