#ifndef TRACE_SOURCE_ACCESSOR_H
#define TRACE_SOURCE_ACCESSOR_H

#include <stdint.h>
#include "object-base.h"
#include "callback.h"
#include "ptr.h"

namespace ns3 {

class TraceSourceAccessor : public ObjectBase
{
public:
  TraceSourceAccessor ();
  virtual ~TraceSourceAccessor ();
  void Ref (void) const;
  void Unref (void) const;

  virtual bool Connect (ObjectBase *obj, const CallbackBase &cb) const = 0;
  virtual bool ConnectWithContext (ObjectBase *obj, std::string context, const CallbackBase &cb) const = 0;
  virtual bool Disconnect (ObjectBase *obj, const CallbackBase &cb) const = 0;
private:
  mutable uint32_t m_count;
};

template <typename T>
Ptr<const TraceSourceAccessor> MakeTraceSourceAccessor (T a);

} // namespace ns3

namespace ns3 {

template <typename T, typename SOURCE>
Ptr<const TraceSourceAccessor> 
DoMakeTraceSourceAccessor (SOURCE T::*a)
{
  struct Accessor : public TraceSourceAccessor
  {
    virtual bool Connect (ObjectBase *obj, const CallbackBase &cb) const {
      T *p = dynamic_cast<T*> (obj);
      if (p == 0)
	{
	  return false;
	}
      (p->*m_source).Connect (cb);
      return true;
    }
    virtual bool ConnectWithContext (ObjectBase *obj, std::string context, const CallbackBase &cb) const {
      T *p = dynamic_cast<T*> (obj);
      if (p == 0)
	{
	  return false;
	}
      (p->*m_source).ConnectWithContext (cb, context);
      return true;
    }
    virtual bool Disconnect (ObjectBase *obj, const CallbackBase &cb) const {
      T *p = dynamic_cast<T*> (obj);
      if (p == 0)
	{
	  return false;
	}
      (p->*m_source).Disconnect (cb);
      return true;      
    }
    SOURCE T::*m_source;
  } *accessor = new Accessor ();
  accessor->m_source = a;
  return Ptr<const TraceSourceAccessor> (accessor, false);
}

template <typename T>
Ptr<const TraceSourceAccessor> MakeTraceSourceAccessor (T a)
{
  return DoMakeTraceSourceAccessor (a);
}

} // namespace ns3


#endif /* TRACE_SOURCE_ACCESSOR_H */
