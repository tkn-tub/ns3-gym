#include "ref-count-base.h"
#include "log.h"

NS_LOG_COMPONENT_DEFINE ("RefCountBase");

namespace ns3 {

RefCountBase::~RefCountBase ()
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3
