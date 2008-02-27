#include "trace-source-accessor.h"

namespace ns3 {

TraceSourceAccessor::TraceSourceAccessor ()
  : m_count (1)
{}
TraceSourceAccessor::~TraceSourceAccessor ()
{}
void 
TraceSourceAccessor::Ref (void) const
{
  m_count++;
}
void 
TraceSourceAccessor::Unref (void) const
{
  m_count--;
  if (m_count == 0)
    {
      delete this;
    }
}

} // namespace ns3
