#ifndef TRACE_CONTEXT_ELEMENT_H
#define TRACE_CONTEXT_ELEMENT_H

#include <string>
#include <vector>

#define NS_TRACE_CONTEXT_ELEMENT_ENSURE_REGISTERED(x)          \
namespace {						       \
static class thisisaveryverylongclassname ##x		       \
  {							       \
  public:						       \
    thisisaveryverylongclassname ##x ()			       \
      { uint32_t uid; uid = x::GetUid ();}		       \
  } g_thisisanotherveryveryverylongname ##x ;		       \
}

namespace ns3 {

/**
 * \brief an item stored in a TraceContext
 *
 * To store trace context information in a TraceContext instance,
 * users must subclass this base class and store subclass instances
 * in a TraceContext with TraceContext::Add.
 *
 * Each subclass should define and implement:
 *   - a public default constructor: it is used by the internals
 *     of the implementation of TraceContext.
 *   - a public destructor: it is also used by the internals of
 *     the implementation of TraceContext.
 *   - a public static method named GetUid which returns a 16 bit 
 *     integer. The integer returned from this method should be
 *     allocated with the protected AllocatedUid method.
 *   - a public Print method: this method is used by the 
 *     TraceContext::Print method to print the content of each
 *     of the trace context element stored in the trace context.
 *     This method takes a c++ output stream and argument and is
 *     expected to write an ascii string describing its content
 *     in this output stream.
 *
 * A typical subclass should look like this:
 * \code
 * class MyContext : public TraceContextElement
 * {
 * public:
 *   // the _required_ public API
 *   static uint16_t GetUid (void);
 *   MyContext ();
 *   ~MyContext ();
 *   void Print (std::ostream &os) const;
 *   std::string GetName (void) const;
 *
 *   // the user-specific API to manipulate the context.
 *   void SetData (uint8_t data);
 *   uint8_t GetData (void) const;
 * private:
 *   uint8_t m_myContextData;
 * };
 *
 * uint16_t 
 * MyContext::GetUid (void)
 * {
 *   static uint16_t uid = AllocateUid<MyContext> ("MyContext");
 *   return uid;
 * }
 * MyContext::MyContext ()
 * {}
 * MyContext::~MyContext ()
 * {}
 * void 
 * MyContext::Print (std::ostream &os) const
 * {
 *   os << "mycontext=" << (uint32_t) m_myContextData;
 * }
 * std::string 
 * MyContext::GetName (void) const
 * {
 *   return "MyContext";
 * }
 * void 
 * MyContext::SetData (uint8_t data)
 * {
 *   m_myContextData = data;
 * }
 * uint8_t 
 * MyContext::GetData (void) const
 * {
 *   return m_myContextData;
 * }
 * \endcode
 */
class TraceContextElement
{
protected:
  /**
   * \param name a string which uniquely identifies the type
   *        of the subclass which is calling this method.
   * \returns a unique 32 bit integer associated to the
   *          input string.
   *
   * Subclasses are expected to call this method from their
   * public static GetUid method.
   */
  template <typename T>
  static uint16_t AllocateUid (std::string name);
};

} // namespace ns3

namespace ns3 {

/**
 * \brief a registry of TraceContextElement subclasses
 * \internal
 */
class ElementRegistry
{
public:
  template <typename T>
  static uint16_t AllocateUid (std::string name);

  static uint32_t GetSize (uint16_t uid);
  static void Print (uint16_t uid, uint8_t *instance, std::ostream &os);
  static std::string GetName (uint16_t uid);
  static void Destroy (uint16_t uid, uint8_t *instance);
private:
  typedef std::string (*GetNameCb) (void);
  typedef void (*PrintCb) (uint8_t *instance, std::ostream &os);
  typedef void (*DestroyCb) (uint8_t *instance);
  struct Info {
    uint32_t size;
    std::string uidString;
    GetNameCb getName;
    PrintCb print;
    DestroyCb destroy;
  };
  typedef std::vector<struct Info> InfoVector;
  static InfoVector *GetInfoVector (void);
  template <typename T>
  static std::string DoGetName (void);
  template <typename T>
  static void DoPrint (uint8_t *instance, std::ostream &os);
  template <typename T>
  static void DoDestroy (uint8_t *instance);  
};

template <typename T>
void 
ElementRegistry::DoPrint (uint8_t *instance, std::ostream &os)
{
  static T obj;
  // make sure we are aligned.
  memcpy ((void*)&obj, instance, sizeof (T));
  obj.Print (os);
}
template <typename T>
std::string
ElementRegistry::DoGetName (void)
{
  static T obj;
  return obj.GetName ();
}
template <typename T>
void 
ElementRegistry::DoDestroy (uint8_t *instance)
{
  static T obj;
  // make sure we are aligned.
  memcpy ((void*)&obj, instance, sizeof (T));
  obj.~T ();
}

template <typename T>
uint16_t 
ElementRegistry::AllocateUid (std::string name)
{
  InfoVector *vec = GetInfoVector ();
  uint16_t uid = 1;
  for (InfoVector::iterator i = vec->begin (); i != vec->end (); i++)
    {
      if (i->uidString == name)
	{
	  return uid;
	}
      uid++;
    }
  struct Info info;
  info.size = sizeof (T);
  info.uidString = name;
  info.getName = &ElementRegistry::DoGetName<T>;
  info.print = &ElementRegistry::DoPrint<T>;
  info.destroy = &ElementRegistry::DoDestroy<T>;
  vec->push_back (info);
  return vec->size ();
}



template <typename T>
uint16_t 
TraceContextElement::AllocateUid (std::string name)
{
  return ElementRegistry::AllocateUid<T> (name);
}

} // namespace ns3

#endif /* TRACE_CONTEXT_ELEMENT_H */
