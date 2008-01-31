#include "value.h"
#include "log.h"
#include <sstream>

NS_LOG_COMPONENT_DEFINE ("Value");

namespace ns3 {

Value::Value ()
  : m_count (1)
{}
Value::Value (const Value &o)
  : m_count (1)
{}
Value &
Value::operator = (const Value &o)
{
  return *this;
}
Value::~Value ()
{}

PValue::PValue ()
  : m_value (0)
{}
PValue::PValue (const PValue &o)
  : m_value (o.m_value)
{
  if (m_value != 0)
    {
      m_value->m_count++;
      NS_LOG_DEBUG ("this="<<m_value<<" ++count="<<m_value->m_count);
    }
}
PValue &
PValue::operator = (const PValue &o)
{
  if (&o != this)
    {
      if (m_value != 0)
	{
	  m_value->m_count--;
	  NS_LOG_DEBUG ("this="<<m_value<<" --count="<<m_value->m_count);
	  if (m_value->m_count == 0)
	    {
	      delete m_value;
	      m_value = 0;
	    }
	}
      m_value = o.m_value;
      if (m_value != 0)
	{
	  m_value->m_count++;
	  NS_LOG_DEBUG ("this="<<m_value<<" ++count="<<m_value->m_count);
	}
    }
  return *this;
}
PValue::~PValue ()
{
  if (m_value != 0) 
    {
      m_value->m_count--;
      NS_LOG_DEBUG ("this="<<m_value<<" --count="<<m_value->m_count);
      if (m_value->m_count == 0)
	{
	  delete m_value;
	  m_value = 0;
	}
    }
}
PValue::PValue (Value *value)
  : m_value (value)
{
  NS_LOG_DEBUG ("this="<<m_value<<" count="<<((m_value!=0)?m_value->m_count:666));
}

PValue 
PValue::Copy (void) const
{
  return m_value->Copy ();
}
std::string 
PValue::SerializeToString (Ptr<const ParamSpec> spec) const
{
  return m_value->SerializeToString (spec);
}
bool 
PValue::DeserializeFromString (std::string value, Ptr<const ParamSpec> spec)
{
  return m_value->DeserializeFromString (value, spec);
}

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

std::string 
PtrValueBase::SerializeToString (Ptr<const ParamSpec> spec) const
{
  std::ostringstream oss;
  oss << PeekObjectBase ();
  return oss.str ();
}

bool 
PtrValueBase::DeserializeFromString (std::string value, Ptr<const ParamSpec> spec)
{
  // XXX
  return false;
}

} // namespace ns3
