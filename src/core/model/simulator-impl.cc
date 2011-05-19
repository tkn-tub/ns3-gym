#include "simulator-impl.h"

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
