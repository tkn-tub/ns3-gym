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

class TraceContextElement
{
protected:
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
  static void Destroy (uint16_t uid, uint8_t *instance);
private:
  typedef void (*PrintCb) (uint8_t *instance, std::ostream &os);
  typedef void (*DestroyCb) (uint8_t *instance);
  struct Info {
    uint32_t size;
    std::string uidString;
    PrintCb print;
    DestroyCb destroy;
  };
  typedef std::vector<struct Info> InfoVector;
  static InfoVector *GetInfoVector (void);
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
