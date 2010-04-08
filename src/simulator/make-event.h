#ifndef MAKE_EVENT_H
#define MAKE_EVENT_H

namespace ns3 {

class EventImpl;

template <typename MEM, typename OBJ>
EventImpl * MakeEvent (MEM mem_ptr, OBJ obj);

template <typename MEM, typename OBJ,
          typename T1>
EventImpl * MakeEvent (MEM mem_ptr, OBJ obj, T1 a1);

template <typename MEM, typename OBJ,
          typename T1, typename T2>
EventImpl * MakeEvent (MEM mem_ptr, OBJ obj, T1 a1, T2 a2);

template <typename MEM, typename OBJ,
          typename T1, typename T2, typename T3>
EventImpl * MakeEvent (MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3);

template <typename MEM, typename OBJ,
          typename T1, typename T2, typename T3, typename T4>
EventImpl * MakeEvent (MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3, T4 a4);

template <typename MEM, typename OBJ,
          typename T1, typename T2, typename T3, typename T4, typename T5>
EventImpl * MakeEvent (MEM mem_ptr, OBJ obj,
                       T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

EventImpl * MakeEvent (void (*f)(void));
template <typename U1,
          typename T1>
EventImpl * MakeEvent (void (*f)(U1), T1 a1);

template <typename U1, typename U2,
          typename T1, typename T2>
EventImpl * MakeEvent (void (*f)(U1,U2), T1 a1, T2 a2);

template <typename U1, typename U2, typename U3,
          typename T1, typename T2, typename T3>
EventImpl * MakeEvent (void (*f)(U1,U2,U3), T1 a1, T2 a2, T3 a3);

template <typename U1, typename U2, typename U3, typename U4,
          typename T1, typename T2, typename T3, typename T4>
EventImpl * MakeEvent (void (*f)(U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4);

template <typename U1, typename U2, typename U3, typename U4, typename U5,
          typename T1, typename T2, typename T3, typename T4, typename T5>
EventImpl * MakeEvent (void (*f)(U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

} // namespace ns3

/********************************************************************
   Implementation of templates defined above
 ********************************************************************/

#include "event-impl.h"
#include "ns3/type-traits.h"

namespace ns3 {

template <typename T>
struct EventMemberImplObjTraits;

template <typename T>
struct EventMemberImplObjTraits<T *>
{
  static T &GetReference (T *p)
  {
    return *p;
  }
};

template <typename MEM, typename OBJ>
EventImpl * MakeEvent (MEM mem_ptr, OBJ obj)
{
  // zero argument version
  class EventMemberImpl0 : public EventImpl
  {
public:
    EventMemberImpl0 (OBJ obj, MEM function)
      : m_obj (obj),
        m_function (function)
    {
    }
    virtual ~EventMemberImpl0 ()
    {
    }
private:
    virtual void Notify (void)
    {
      (EventMemberImplObjTraits<OBJ>::GetReference (m_obj).*m_function)();
    }
    OBJ m_obj;
    MEM m_function;
  } *ev = new EventMemberImpl0 (obj, mem_ptr);
  return ev;
}


template <typename MEM, typename OBJ,
          typename T1>
EventImpl * MakeEvent (MEM mem_ptr, OBJ obj, T1 a1)
{
  // one argument version
  class EventMemberImpl1 : public EventImpl
  {
public:
    EventMemberImpl1 (OBJ obj, MEM function, T1 a1)
      : m_obj (obj),
        m_function (function),
        m_a1 (a1)
    {
    }
protected:
    virtual ~EventMemberImpl1 ()
    {
    }
private:
    virtual void Notify (void)
    {
      (EventMemberImplObjTraits<OBJ>::GetReference (m_obj).*m_function)(m_a1);
    }
    OBJ m_obj;
    MEM m_function;
    typename TypeTraits<T1>::ReferencedType m_a1;
  } *ev = new EventMemberImpl1 (obj, mem_ptr, a1);
  return ev;
}

template <typename MEM, typename OBJ,
          typename T1, typename T2>
EventImpl * MakeEvent (MEM mem_ptr, OBJ obj, T1 a1, T2 a2)
{
  // two argument version
  class EventMemberImpl2 : public EventImpl
  {
public:
    EventMemberImpl2 (OBJ obj, MEM function, T1 a1, T2 a2)
      : m_obj (obj),
        m_function (function),
        m_a1 (a1),
        m_a2 (a2)
    {
    }
protected:
    virtual ~EventMemberImpl2 ()
    {
    }
private:
    virtual void Notify (void)
    {
      (EventMemberImplObjTraits<OBJ>::GetReference (m_obj).*m_function)(m_a1, m_a2);
    }
    OBJ m_obj;
    MEM m_function;
    typename TypeTraits<T1>::ReferencedType m_a1;
    typename TypeTraits<T2>::ReferencedType m_a2;
  } *ev = new EventMemberImpl2 (obj, mem_ptr, a1, a2);
  return ev;
}

template <typename MEM, typename OBJ,
          typename T1, typename T2, typename T3>
EventImpl * MakeEvent (MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3)
{
  // three argument version
  class EventMemberImpl3 : public EventImpl
  {
public:
    EventMemberImpl3 (OBJ obj, MEM function, T1 a1, T2 a2, T3 a3)
      : m_obj (obj),
        m_function (function),
        m_a1 (a1),
        m_a2 (a2),
        m_a3 (a3)
    {
    }
protected:
    virtual ~EventMemberImpl3 ()
    {
    }
private:
    virtual void Notify (void)
    {
      (EventMemberImplObjTraits<OBJ>::GetReference (m_obj).*m_function)(m_a1, m_a2, m_a3);
    }
    OBJ m_obj;
    MEM m_function;
    typename TypeTraits<T1>::ReferencedType m_a1;
    typename TypeTraits<T2>::ReferencedType m_a2;
    typename TypeTraits<T3>::ReferencedType m_a3;
  } *ev = new EventMemberImpl3 (obj, mem_ptr, a1, a2, a3);
  return ev;
}

template <typename MEM, typename OBJ,
          typename T1, typename T2, typename T3, typename T4>
EventImpl * MakeEvent (MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3, T4 a4)
{
  // four argument version
  class EventMemberImpl4 : public EventImpl
  {
public:
    EventMemberImpl4 (OBJ obj, MEM function, T1 a1, T2 a2, T3 a3, T4 a4)
      : m_obj (obj),
        m_function (function),
        m_a1 (a1),
        m_a2 (a2),
        m_a3 (a3),
        m_a4 (a4)
    {
    }
protected:
    virtual ~EventMemberImpl4 ()
    {
    }
private:
    virtual void Notify (void)
    {
      (EventMemberImplObjTraits<OBJ>::GetReference (m_obj).*m_function)(m_a1, m_a2, m_a3, m_a4);
    }
    OBJ m_obj;
    MEM m_function;
    typename TypeTraits<T1>::ReferencedType m_a1;
    typename TypeTraits<T2>::ReferencedType m_a2;
    typename TypeTraits<T3>::ReferencedType m_a3;
    typename TypeTraits<T4>::ReferencedType m_a4;
  } *ev = new EventMemberImpl4 (obj, mem_ptr, a1, a2, a3, a4);
  return ev;
}

template <typename MEM, typename OBJ,
          typename T1, typename T2, typename T3, typename T4, typename T5>
EventImpl * MakeEvent (MEM mem_ptr, OBJ obj,
                       T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
  // five argument version
  class EventMemberImpl5 : public EventImpl
  {
public:
    EventMemberImpl5 (OBJ obj, MEM function, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
      : m_obj (obj),
        m_function (function),
        m_a1 (a1),
        m_a2 (a2),
        m_a3 (a3),
        m_a4 (a4),
        m_a5 (a5)
    {
    }
protected:
    virtual ~EventMemberImpl5 ()
    {
    }
private:
    virtual void Notify (void)
    {
      (EventMemberImplObjTraits<OBJ>::GetReference (m_obj).*m_function)(m_a1, m_a2, m_a3, m_a4, m_a5);
    }
    OBJ m_obj;
    MEM m_function;
    typename TypeTraits<T1>::ReferencedType m_a1;
    typename TypeTraits<T2>::ReferencedType m_a2;
    typename TypeTraits<T3>::ReferencedType m_a3;
    typename TypeTraits<T4>::ReferencedType m_a4;
    typename TypeTraits<T5>::ReferencedType m_a5;
  } *ev = new EventMemberImpl5 (obj, mem_ptr, a1, a2, a3, a4, a5);
  return ev;
}

template <typename U1, typename T1>
EventImpl * MakeEvent (void (*f)(U1), T1 a1)
{
  // one arg version
  class EventFunctionImpl1 : public EventImpl
  {
public:
    typedef void (*F)(U1);

    EventFunctionImpl1 (F function, T1 a1)
      : m_function (function),
        m_a1 (a1)
    {
    }
protected:
    virtual ~EventFunctionImpl1 ()
    {
    }
private:
    virtual void Notify (void)
    {
      (*m_function)(m_a1);
    }
    F m_function;
    typename TypeTraits<T1>::ReferencedType m_a1;
  } *ev = new EventFunctionImpl1 (f, a1);
  return ev;
}

template <typename U1, typename U2, typename T1, typename T2>
EventImpl * MakeEvent (void (*f)(U1,U2), T1 a1, T2 a2)
{
  // two arg version
  class EventFunctionImpl2 : public EventImpl
  {
public:
    typedef void (*F)(U1, U2);

    EventFunctionImpl2 (F function, T1 a1, T2 a2)
      : m_function (function),
        m_a1 (a1),
        m_a2 (a2)
    {
    }
protected:
    virtual ~EventFunctionImpl2 ()
    {
    }
private:
    virtual void Notify (void)
    {
      (*m_function)(m_a1, m_a2);
    }
    F m_function;
    typename TypeTraits<T1>::ReferencedType m_a1;
    typename TypeTraits<T2>::ReferencedType m_a2;
  } *ev = new EventFunctionImpl2 (f, a1, a2);
  return ev;
}

template <typename U1, typename U2, typename U3,
          typename T1, typename T2, typename T3>
EventImpl * MakeEvent (void (*f)(U1,U2,U3), T1 a1, T2 a2, T3 a3)
{
  // three arg version
  class EventFunctionImpl3 : public EventImpl
  {
public:
    typedef void (*F)(U1, U2, U3);

    EventFunctionImpl3 (F function, T1 a1, T2 a2, T3 a3)
      : m_function (function),
        m_a1 (a1),
        m_a2 (a2),
        m_a3 (a3)
    {
    }
protected:
    virtual ~EventFunctionImpl3 ()
    {
    }
private:
    virtual void Notify (void)
    {
      (*m_function)(m_a1, m_a2, m_a3);
    }
    F m_function;
    typename TypeTraits<T1>::ReferencedType m_a1;
    typename TypeTraits<T2>::ReferencedType m_a2;
    typename TypeTraits<T3>::ReferencedType m_a3;
  } *ev = new EventFunctionImpl3 (f, a1, a2, a3);
  return ev;
}

template <typename U1, typename U2, typename U3, typename U4,
          typename T1, typename T2, typename T3, typename T4>
EventImpl * MakeEvent (void (*f)(U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4)
{
  // four arg version
  class EventFunctionImpl4 : public EventImpl
  {
public:
    typedef void (*F)(U1, U2, U3, U4);

    EventFunctionImpl4 (F function, T1 a1, T2 a2, T3 a3, T4 a4)
      : m_function (function),
        m_a1 (a1),
        m_a2 (a2),
        m_a3 (a3),
        m_a4 (a4)
    {
    }
protected:
    virtual ~EventFunctionImpl4 ()
    {
    }
private:
    virtual void Notify (void)
    {
      (*m_function)(m_a1, m_a2, m_a3, m_a4);
    }
    F m_function;
    typename TypeTraits<T1>::ReferencedType m_a1;
    typename TypeTraits<T2>::ReferencedType m_a2;
    typename TypeTraits<T3>::ReferencedType m_a3;
    typename TypeTraits<T4>::ReferencedType m_a4;
  } *ev = new EventFunctionImpl4 (f, a1, a2, a3, a4);
  return ev;
}

template <typename U1, typename U2, typename U3, typename U4, typename U5,
          typename T1, typename T2, typename T3, typename T4, typename T5>
EventImpl * MakeEvent (void (*f)(U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
  // five arg version
  class EventFunctionImpl5 : public EventImpl
  {
public:
    typedef void (*F)(U1,U2,U3,U4,U5);

    EventFunctionImpl5 (F function, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
      : m_function (function),
        m_a1 (a1),
        m_a2 (a2),
        m_a3 (a3),
        m_a4 (a4),
        m_a5 (a5)
    {
    }
protected:
    virtual ~EventFunctionImpl5 ()
    {
    }
private:
    virtual void Notify (void)
    {
      (*m_function)(m_a1, m_a2, m_a3, m_a4, m_a5);
    }
    F m_function;
    typename TypeTraits<T1>::ReferencedType m_a1;
    typename TypeTraits<T2>::ReferencedType m_a2;
    typename TypeTraits<T3>::ReferencedType m_a3;
    typename TypeTraits<T4>::ReferencedType m_a4;
    typename TypeTraits<T5>::ReferencedType m_a5;
  } *ev = new EventFunctionImpl5 (f, a1, a2, a3, a4, a5);
  return ev;
}

} // namespace ns3

#endif /* MAKE_EVENT_H */
