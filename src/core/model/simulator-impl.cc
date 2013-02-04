#include "simulator-impl.h"
#include "log.h"

NS_LOG_COMPONENT_DEFINE ("SimulatorImpl");

namespace ns3 {

TypeId 
SimulatorImpl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SimulatorImpl")
    .SetParent<Object> ()
  ;
  return tid;
}

} // namespace ns3
