#include "param-spec.h"

namespace ns3 {

ParamSpec::ParamSpec ()
  : m_count (1)
{}
void 
ParamSpec::Ref (void) const
{
  m_count++;
}
void 
ParamSpec::Unref (void) const
{
  m_count--;
  if (m_count == 0)
    {
      delete this;
    }
}
ParamSpec::~ParamSpec ()
{}

} // namespace ns3
