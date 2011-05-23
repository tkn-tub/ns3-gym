#include "callback.h"

namespace ns3 {

CallbackValue::CallbackValue ()
  : m_value ()
{
}
CallbackValue::CallbackValue (const CallbackBase &base)
  : m_value (base)
{
}
CallbackValue::~CallbackValue ()
{
}
void
CallbackValue::Set (CallbackBase base)
{
  m_value = base;
}
Ptr<AttributeValue>
CallbackValue::Copy (void) const
{
  return Create<CallbackValue> (m_value);
}
std::string
CallbackValue::SerializeToString (Ptr<const AttributeChecker> checker) const
{
  std::ostringstream oss;
  oss << PeekPointer (m_value.GetImpl ());
  return oss.str ();
}
bool
CallbackValue::DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker)
{
  return false;
}

ATTRIBUTE_CHECKER_IMPLEMENT (Callback);

} // namespace ns3

#if (__GNUC__ >= 3)

#include <stdlib.h>
#include <cxxabi.h>
#include "log.h"

namespace ns3 {

std::string
CallbackBase::Demangle (const std::string& mangled)
{
  int status;
  char* demangled = abi::__cxa_demangle (mangled.c_str (),
                                         NULL, NULL, &status);

  std::string ret;
  if (status == 0) {
      NS_ASSERT (demangled);
      ret = demangled;
    }
  else if (status == -1) {
      NS_LOG_UNCOND ("Callback demangling failed: Memory allocation failure occured.");
      ret = mangled;
    }
  else if (status == -2) {
      NS_LOG_UNCOND ("Callback demangling failed: Mangled name is not a valid under the C++ ABI mangling rules.");
      ret = mangled;
    }
  else if (status == -3) {
      NS_LOG_UNCOND ("Callback demangling failed: One of the arguments is invalid.");
      ret = mangled;
    }
  else {
      NS_LOG_UNCOND ("Callback demangling failed: status " << status);
      ret = mangled;
    }

  if (demangled) {
      free (demangled);
    }
  return ret;
}

} // namespace ns3

#else

std::string
ns3::CallbackBase::Demangle (const std::string& mangled)
{
  return mangled;
}

#endif

