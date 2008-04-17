#ifndef OBJECT_VECTOR_H
#define OBJECT_VECTOR_H

#include <vector>
#include "object.h"
#include "ptr.h"
#include "attribute.h"

namespace ns3 {

/**
 * \brief contain a vector of ns3::Object pointers.
 *
 * This class it used to get attribute access to an array of
 * ns3::Object pointers.
 */
class ObjectVectorValue : public AttributeValue
{
public:
  typedef std::vector<Ptr<Object> >::const_iterator Iterator;

  ObjectVectorValue ();

  /**
   * \returns an iterator to the first object contained in this vector
   */
  Iterator Begin (void) const;
  /**
   * \returns an iterator to the last object contained in this vector
   */
  Iterator End (void) const;
  /**
   * \returns the number of objects contained in this vector.
   */
  uint32_t GetN (void) const;
  /**
   * \param i the index of the requested object.
   * \returns the requested object
   */
  Ptr<Object> Get (uint32_t i) const;

  virtual Ptr<AttributeValue> Copy (void) const;
  virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const;
  virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker);

private:
  friend class ObjectVectorAccessor;
  std::vector<Ptr<Object> > m_objects;
};

template <typename T, typename U>
Ptr<const AttributeAccessor>
MakeObjectVectorAccessor (U T::*memberVector);

template <typename T, typename U, typename INDEX>
Ptr<const AttributeAccessor>
MakeObjectVectorAccessor (Ptr<U> (T::*get) (INDEX) const,
			  INDEX (T::*getN) (void) const);

template <typename T, typename U, typename INDEX>
Ptr<const AttributeAccessor>
MakeObjectVectorAccessor (INDEX (T::*getN) (void) const,
			  Ptr<U> (T::*get) (INDEX) const);

class ObjectVectorChecker : public AttributeChecker
{
public:
  virtual TypeId GetItemTypeId (void) const = 0;
};

template <typename T>
Ptr<const AttributeChecker> MakeObjectVectorChecker (void);

} // namespace ns3

namespace ns3 {

namespace internal {

template <typename T>
class AnObjectVectorChecker : public ObjectVectorChecker
{
public:
  virtual TypeId GetItemTypeId (void) const {
    return T::GetTypeId ();
  }
  virtual bool Check (const AttributeValue &value) const {
    return dynamic_cast<const ObjectVectorValue *> (&value) != 0;
  }
  virtual std::string GetType (void) const {
    return "ns3::ObjectVectorValue";
  }
  virtual bool HasTypeConstraints (void) const {
    return true;
  }
  virtual std::string GetTypeConstraints (void) const {
    return T::GetTypeId ().GetName ();
  }
  virtual Ptr<AttributeValue> Create (void) const {
    return ns3::Create<ObjectVectorValue> ();
  }
  virtual bool Copy (const AttributeValue &source, AttributeValue &destination) const {
    const ObjectVectorValue *src = dynamic_cast<const ObjectVectorValue *> (&source);
    ObjectVectorValue *dst = dynamic_cast<ObjectVectorValue *> (&destination);
    if (src == 0 || dst == 0)
      {
	return false;
      }
    *dst = *src;
    return true;    
  }
};

} // namespace internal


class ObjectVectorAccessor : public AttributeAccessor
{
public:
  virtual bool Set (ObjectBase * object, const AttributeValue &value) const;
  virtual bool Get (const ObjectBase * object, AttributeValue &value) const;
  virtual bool HasGetter (void) const;
  virtual bool HasSetter (void) const;
private:
  virtual bool DoGetN (const ObjectBase *object, uint32_t *n) const = 0;
  virtual Ptr<Object> DoGet (const ObjectBase *object, uint32_t i) const = 0;
};

template <typename T, typename U>
Ptr<const AttributeAccessor>
MakeObjectVectorAccessor (U T::*memberVector)
{
  struct MemberStdContainer : public ObjectVectorAccessor
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
      typename U::const_iterator begin = (obj->*m_memberVector).begin ();
      typename U::const_iterator end = (obj->*m_memberVector).end ();
      uint32_t k = 0;
      for (typename U::const_iterator j = begin; j != end; j++, k++)
	{
	  if (k == i)
	    {
	      return *j;
	      break;
	    }
	}
      NS_ASSERT (false);
      // quiet compiler.
      return 0;
    }
    U T::*m_memberVector;
  } *spec = new MemberStdContainer ();
  spec->m_memberVector = memberVector;
  return Ptr<const AttributeAccessor> (spec, false);
}

template <typename T, typename U, typename INDEX>
Ptr<const AttributeAccessor>
MakeObjectVectorAccessor (Ptr<U> (T::*get) (INDEX) const,
			   INDEX (T::*getN) (void) const)
{
  struct MemberGetters : public ObjectVectorAccessor
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
  return Ptr<const AttributeAccessor> (spec, false);
}

template <typename T, typename U, typename INDEX>
Ptr<const AttributeAccessor>
MakeObjectVectorAccessor (INDEX (T::*getN) (void) const,
			   Ptr<U> (T::*get) (INDEX) const)
{
  return MakeObjectVectorAccessor (get, getN);
}

template <typename T>
Ptr<const AttributeChecker> MakeObjectVectorChecker (void)
{
  return Create<internal::AnObjectVectorChecker<T> > ();
}


} // namespace ns3

#endif /* OBJECT_VECTOR_H */
