#include "value.h"

namespace ns3 {

Value::Value ()
  : m_count (1)
{}
void 
Value::Ref (void) const
{
  m_count++;
}
void 
Value::Unref (void) const
{
  m_count--;
  if (m_count == 0)
    {
      delete this;
    }
}
Value::~Value ()
{}

} // namespace ns3
