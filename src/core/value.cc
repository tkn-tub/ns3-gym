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

/***************************************************************
 *   Big interesting warning.
 *   ------------------------
 *
 * One might wonder why we re-implement a smart pointer below 
 * in the Attribute class. This is a very good question and the answer
 * is unfortunately pretty complicated.
 *
 * 1) We could have requested the user to use Ptr<Value> and save us
 *    a lot of pain. This, however, does not work because our smart 
 *    pointer needs a special constructor which can be used to convert
 *    objects of type Ptr<T> into a PtrValue<T> to hold the pointer.
 *
 * 2) We could have made the m_value member variable below a Ptr<Value>
 *    rather than store a raw pointer. This, however, does not work
 *    because this would mean that the constructor Attribute (Value *)
 *    should be morphed into Attribute (Ptr<Value>) which, unfortunately,
 *    would conflict with the template constructor Attribute (Ptr<T>)...
 *
 * This is definitely not fun.   
 */
Attribute::Attribute ()
  : m_value (0)
{}
Attribute::Attribute (const Attribute &o)
  : m_value (o.m_value)
{
  if (m_value != 0)
    {
      m_value->m_count++;
      NS_LOG_DEBUG ("this="<<m_value<<" ++count="<<m_value->m_count);
    }
}
Attribute &
Attribute::operator = (const Attribute &o)
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
Attribute::~Attribute ()
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
Attribute::Attribute (Value *value)
  : m_value (value)
{
  NS_LOG_DEBUG ("this="<<m_value<<" count="<<((m_value!=0)?m_value->m_count:666));
}

Attribute 
Attribute::Copy (void) const
{
  return m_value->Copy ();
}
std::string 
Attribute::SerializeToString (Ptr<const AttributeChecker> checker) const
{
  return m_value->SerializeToString (checker);
}
bool 
Attribute::DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker)
{
  return m_value->DeserializeFromString (value, checker);
}

Attribute::Attribute (const char *value)
  : m_value (new StringValue (value))
{}
Attribute::Attribute (std::string value)
  : m_value (new StringValue (value))
{}


AttributeAccessor::AttributeAccessor ()
  : m_count (1)
{}
void 
AttributeAccessor::Ref (void) const
{
  m_count++;
}
void 
AttributeAccessor::Unref (void) const
{
  m_count--;
  if (m_count == 0)
    {
      delete this;
    }
}
AttributeAccessor::~AttributeAccessor ()
{}

AttributeChecker::AttributeChecker ()
  : m_count (1)
{}
void 
AttributeChecker::Ref (void) const
{
  m_count++;
}
void 
AttributeChecker::Unref (void) const
{
  m_count--;
  if (m_count == 0)
    {
      delete this;
    }
}
AttributeChecker::~AttributeChecker ()
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
Attribute 
StringValue::Copy (void) const
{
  return Attribute::Create<StringValue> (*this);
}
std::string 
StringValue::SerializeToString (Ptr<const AttributeChecker> checker) const
{
  return m_value;
}
bool 
StringValue::DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker)
{
  m_value = value;
  return true;
}
StringValue::StringValue (Attribute value)
{
  const StringValue *v = value.DynCast<const StringValue *> ();
  if (v == 0)
    {
      NS_FATAL_ERROR ("Expected value of type String.");
    }
  m_value = v->Get ();
}
StringValue::operator Attribute () const
{
  return Attribute::Create<StringValue> (*this);
}


std::string 
PtrValueBase::SerializeToString (Ptr<const AttributeChecker> checker) const
{
  std::ostringstream oss;
  oss << PeekObjectBase ();
  return oss.str ();
}

bool 
PtrValueBase::DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker)
{
  // XXX
  return false;
}

} // namespace ns3
