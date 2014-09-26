#include "ref-count-base.h"
#include "log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RefCountBase");

RefCountBase::~RefCountBase ()
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3
