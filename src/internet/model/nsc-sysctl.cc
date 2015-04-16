/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "ns3/string.h"
#include "nsc-sysctl.h"

#include "sim_interface.h"

namespace ns3 {

/**
 * \ingroup nsctcp
 *
 * This object represent the underlying nsc stack attributes and
 * provide a ns-3-like system to access them though sysctls
 */
class NscStackStringAccessor : public AttributeAccessor
{
public:
  /**
   * \brief Constructor
   * \param name name of the attribute
   */
  NscStackStringAccessor (std::string name) : m_name (name) {}

  virtual bool Set (ObjectBase * object, const AttributeValue &val) const;
  virtual bool Get (const ObjectBase * object, AttributeValue &val) const;
  virtual bool HasGetter (void) const;
  virtual bool HasSetter (void) const;
private:
  std::string m_name; //!< name of the attribute
};

bool NscStackStringAccessor::HasGetter (void) const
{
  return true;
}

bool NscStackStringAccessor::HasSetter (void) const
{
  return true;
}


bool NscStackStringAccessor::Set (ObjectBase * object, const AttributeValue & val) const
{
  const StringValue *value = dynamic_cast<const StringValue *> (&val);
  if (value == 0)
    {
      return false;
    }
  Ns3NscStack *obj = dynamic_cast<Ns3NscStack *> (object);
  if (obj == 0)
    {
      return false;
    }
  obj->Set (m_name, value->Get ());
  return true;
}

bool NscStackStringAccessor::Get (const ObjectBase * object, AttributeValue &val) const
{
  StringValue *value = dynamic_cast<StringValue *> (&val);
  if (value == 0)
    {
      return false;
    }
  const Ns3NscStack *obj = dynamic_cast<const Ns3NscStack *> (object);
  if (obj == 0)
    {
      return false;
    }
  value->Set (obj->Get (m_name));
  return true;
}


TypeId
Ns3NscStack::GetInstanceTypeId (void) const
{
  if (m_stack == 0)
    {
      // if we have no stack, we are a normal NscStack without any attributes.
      return GetTypeId ();
    }
  std::string name = "ns3::Ns3NscStack<";
  name += m_stack->get_name ();
  name += ">";
  TypeId tid;
  if (TypeId::LookupByNameFailSafe (name, &tid))
    {
      // if the relevant TypeId has already been registered, no need to do it again.
      return tid;
    }
  else
    {
      // Now, we register a new TypeId for this stack which will look
      // like a subclass of the Ns3NscStack. The class Ns3NscStack is effectively
      // mutating into a subclass of itself from the point of view of the TypeId
      // system _here_
      tid = TypeId (name.c_str ());
      tid.SetParent<Ns3NscStack> ();
      char buf[256];
      for (int i=0; m_stack->sysctl_getnum (i, buf, sizeof(buf)) > 0; i++)
        {
          char value[256];
          if (m_stack->sysctl_get (buf, value, sizeof(value)) > 0)
            {
              tid.AddAttribute (buf, "Help text",
                                StringValue (value),
                                Create<NscStackStringAccessor> (buf),
                                MakeStringChecker ());
            }
        }
      return tid;
    }
}

std::string
Ns3NscStack::Get (std::string name) const
{
  char buf[512];
  if (m_stack->sysctl_get (name.c_str (), buf, sizeof(buf)) <= 0)
    { // name.c_str () is not a valid sysctl name, or internal NSC error (eg. error converting value)
      return NULL;
    }
  return std::string (buf);
}

void
Ns3NscStack::Set (std::string name, std::string value)
{
  int ret = m_stack->sysctl_set (name.c_str (), value.c_str ());
  if (ret < 0)
    {
      NS_FATAL_ERROR ("setting " << name << " to " << value << "failed (retval " << ret << ")");
    }
}

NS_OBJECT_ENSURE_REGISTERED (Ns3NscStack);

TypeId
Ns3NscStack::Ns3NscStack::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ns3NscStack")
    .SetParent<Object> ()
  ;
  return tid;
}

} // namespace ns3
