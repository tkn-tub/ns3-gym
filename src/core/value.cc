#include "value.h"
#include "log.h"
#include "fatal-error.h"
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
bool 
Value::ConvertFrom (PValue value, Ptr<const ParamSpec> spec)
{
  return false;
}

/***************************************************************
 *   Big interesting warning.
 *   ------------------------
 *
 * One might wonder why we re-implement a smart pointer below 
 * in the PValue class. This is a very good question and the answer
 * is unfortunately pretty complicated.
 *
 * 1) We could have requested the user to use Ptr<Value> and save us
 *    a lot of pain. This, however, does not work because our smart 
 *    pointer needs a special constructor which can be used to convert
 *    objects of type Ptr<T> into a PtrValue<T> to hold the pointer.
 *
 * 2) We could have made the m_value member variable below a Ptr<Value>
 *    rather than store a raw pointer. This, however, does not work
 *    because this would mean that the constructor PValue (Value *)
 *    should be morphed into PValue (Ptr<Value>) which, unfortunately,
 *    would conflict with the template constructor PValue (Ptr<T>)...
 *
 * This is definitely not fun.   
 */
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
bool 
PValue::ConvertFrom (PValue value, Ptr<const ParamSpec> spec)
{
  const StringValue *str = value.DynCast<const StringValue *> ();
  if (str == 0)
    {
      return false;
    }
  return DeserializeFromString (str->Get (), spec);
}

PValue::PValue (const char *value)
  : m_value (new StringValue (value))
{}
PValue::PValue (std::string value)
  : m_value (new StringValue (value))
{}


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


StringValue::StringValue (const char *value)
  : m_value (value)
{}
StringValue::StringValue (std::string value)
  : m_value (value)
{}
void 
StringValue::Set (std::string value)
{
  m_value = value;
}
std::string 
StringValue::Get (void) const
{
  return m_value;
}
PValue 
StringValue::Copy (void) const
{
  return PValue::Create<StringValue> (*this);
}
std::string 
StringValue::SerializeToString (Ptr<const ParamSpec> spec) const
{
  return m_value;
}
bool 
StringValue::DeserializeFromString (std::string value, Ptr<const ParamSpec> spec)
{
  m_value = value;
  return true;
}
StringValue::StringValue (PValue value)
{
  const StringValue *v = value.DynCast<const StringValue *> ();
  if (v == 0)
    {
      NS_FATAL_ERROR ("Expected value of type String.");
    }
  m_value = v->Get ();
}
StringValue::operator PValue () const
{
  return PValue::Create<StringValue> (*this);
}


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
