#include "simulator-impl.h"
#include "log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("SimulatorImpl");

TypeId 
SimulatorImpl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SimulatorImpl")
    .SetParent<Object> ()
  ;
  return tid;
}

} // namespace ns3
