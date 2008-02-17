#ifndef OBJECT_VECTOR_H
#define OBJECT_VECTOR_H

#include <vector>
#include "object.h"
#include "ptr.h"
#include "value.h"

namespace ns3 {

class ObjectVector
{
public:
  typedef std::vector<Ptr<Object> >::const_iterator Iterator;

  ObjectVector ();

  Iterator Begin (void) const;
  Iterator End (void) const;
  uint32_t GetN (void) const;
  Ptr<Object> Get (uint32_t i) const;

  ObjectVector (PValue value);
  operator PValue () const;
private:
  friend class ObjectVectorParamSpec;
  std::vector<Ptr<Object> > m_objects;
};

template <typename T, typename U>
Ptr<ParamSpec>
MakeObjectVectorParamSpec (U T::*memberVector);

template <typename T, typename U, typename INDEX>
Ptr<ParamSpec>
MakeObjectVectorParamSpec (Ptr<U> (T::*get) (INDEX) const,
			   INDEX (T::*getN) (void) const);

template <typename T, typename U, typename INDEX>
Ptr<ParamSpec>
MakeObjectVectorParamSpec (INDEX (T::*getN) (void) const,
			   Ptr<U> (T::*get) (INDEX) const);


Ptr<AttributeChecker> MakeObjectVectorChecker (void);

} // namespace ns3

namespace ns3 {

class ObjectVectorValue : public Value
{
public:
  ObjectVectorValue ();
  ObjectVectorValue (const ObjectVector &vector);

  ObjectVector Get (void) const;

  virtual PValue Copy (void) const;
  virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const;
  virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker);

private:
  friend class ObjectVectorParamSpec;
  ObjectVector m_vector;
};

class ObjectVectorParamSpec : public ParamSpec
{
public:
  virtual bool Set (ObjectBase * object, PValue value) const;
  virtual bool Get (const ObjectBase * object, PValue value) const;
private:
  virtual bool DoGetN (const ObjectBase *object, uint32_t *n) const = 0;
  virtual Ptr<Object> DoGet (const ObjectBase *object, uint32_t i) const = 0;
};

template <typename T, typename U>
Ptr<ParamSpec>
MakeObjectVectorParamSpec (U T::*memberVector)
{
  struct MemberStdContainer : public ObjectVectorParamSpec
  {
    virtual bool DoGetN (const ObjectBase *object, uint32_t *n) const {
      const T *obj = dynamic_cast<const T *> (object);
      if (obj == 0)
	{
	  return false;
	}
      *n = (obj->*m_memberVector).size ();
      return true;
    }
    virtual Ptr<Object> DoGet (const ObjectBase *object, uint32_t i) const {
      const T *obj = static_cast<const T *> (object);
      return (obj->*m_memberVector)[i];
    }
    U T::*m_memberVector;
  } *spec = new MemberStdContainer ();
  spec->m_memberVector = memberVector;
  return Ptr<ParamSpec> (spec, false);
}

template <typename T, typename U, typename INDEX>
Ptr<ParamSpec>
MakeObjectVectorParamSpec (Ptr<U> (T::*get) (INDEX) const,
			   INDEX (T::*getN) (void) const)
{
  struct MemberGetters : public ObjectVectorParamSpec
  {
    virtual bool DoGetN (const ObjectBase *object, uint32_t *n) const {
      const T *obj = dynamic_cast<const T *> (object);
      if (obj == 0)
	{
	  return false;
	}
      *n = (obj->*m_getN) ();
      return true;
    }
    virtual Ptr<Object> DoGet (const ObjectBase *object, uint32_t i) const {
      const T *obj = static_cast<const T *> (object);
      return (obj->*m_get) (i);
    }
    Ptr<U> (T::*m_get) (INDEX) const;
    INDEX (T::*m_getN) (void) const;
  } *spec = new MemberGetters ();
  spec->m_get = get;
  spec->m_getN = getN;
  return Ptr<ParamSpec> (spec, false);
}

template <typename T, typename U, typename INDEX>
Ptr<ParamSpec>
MakeObjectVectorParamSpec (INDEX (T::*getN) (void) const,
			   Ptr<U> (T::*get) (INDEX) const)
{
  return MakeObjectVectorParamSpec (get, getN);
}

} // namespace ns3

#endif /* OBJECT_VECTOR_H */
