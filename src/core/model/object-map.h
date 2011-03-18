#ifndef OBJECT_MAP_H
#define OBJECT_MAP_H

#include "object.h"
#include "ptr.h"
#include "attribute.h"

#include <map>

namespace ns3 {

/**
 * \ingroup object
 * 
 * \brief contain a map of ns3::Object pointers.
 *
 * This class it used to get attribute access to an array of
 * ns3::Object pointers.
 */
class ObjectMapValue : public AttributeValue
{


public:
  typedef std::map<uint32_t, Ptr<Object> >::const_iterator Iterator;

  ObjectMapValue ();

  /**
   * \returns an iterator to the first object contained in this map
   */
  Iterator Begin (void) const;
  /**
   * \returns an iterator to the last object contained in this map
   */
  Iterator End (void) const;
  /**
   * \returns the number of objects contained in this map.
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
  friend class ObjectMapAccessor;
  std::map<uint32_t, Ptr<Object> > m_objects;
};

template <typename T, typename U>
Ptr<const AttributeAccessor>
MakeObjectMapAccessor (U T::*memberMap);

template <typename T, typename U, typename INDEX>
Ptr<const AttributeAccessor>
MakeObjectMapAccessor (Ptr<U> (T::*get) (INDEX) const,
			  INDEX (T::*getN) (void) const);

template <typename T, typename U, typename INDEX>
Ptr<const AttributeAccessor>
MakeObjectMapAccessor (INDEX (T::*getN) (void) const,
			  Ptr<U> (T::*get) (INDEX) const);

class ObjectMapChecker : public AttributeChecker
{
public:
  virtual TypeId GetItemTypeId (void) const = 0;
};

template <typename T>
Ptr<const AttributeChecker> MakeObjectMapChecker (void);

} // namespace ns3

namespace ns3 {

namespace internal {

template <typename T>
class AnObjectMapChecker : public ObjectMapChecker
{
public:
  virtual TypeId GetItemTypeId (void) const {
    return T::GetTypeId ();
  }
  virtual bool Check (const AttributeValue &value) const {
    return dynamic_cast<const ObjectMapValue *> (&value) != 0;
  }
  virtual std::string GetValueTypeName (void) const {
    return "ns3::ObjectMapValue";
  }
  virtual bool HasUnderlyingTypeInformation (void) const {
    return true;
  }
  virtual std::string GetUnderlyingTypeInformation (void) const {
    return "ns3::Ptr< " + T::GetTypeId ().GetName () + " >";
  }
  virtual Ptr<AttributeValue> Create (void) const {
    return ns3::Create<ObjectMapValue> ();
  }
  virtual bool Copy (const AttributeValue &source, AttributeValue &destination) const {
    const ObjectMapValue *src = dynamic_cast<const ObjectMapValue *> (&source);
    ObjectMapValue *dst = dynamic_cast<ObjectMapValue *> (&destination);
    if (src == 0 || dst == 0)
      {
	return false;
      }
    *dst = *src;
    return true;    
  }
};

} // namespace internal

class ObjectMapAccessor : public AttributeAccessor
{
public:
  virtual bool Set (ObjectBase * object, const AttributeValue &value) const;
  virtual bool Get (const ObjectBase * object, AttributeValue &value) const;
  virtual bool HasGetter (void) const;
  virtual bool HasSetter (void) const;
private:
  virtual bool DoGetN (const ObjectBase *object, uint32_t *n) const = 0;
  virtual std::pair<uint32_t, Ptr<Object> > DoGet (const ObjectBase *object, uint32_t i) const = 0;
};

template <typename T, typename U>
Ptr<const AttributeAccessor>
MakeObjectMapAccessor (U T::*memberMap)
{
  struct MemberStdContainer : public ObjectMapAccessor
  {
    virtual bool DoGetN (const ObjectBase *object, uint32_t *n) const {
      const T *obj = dynamic_cast<const T *> (object);
      if (obj == 0)
	{
	  return false;
	}
      *n = (obj->*m_memberMap).size ();
      return true;
    }
    virtual std::pair<uint32_t, Ptr<Object> > DoGet (const ObjectBase *object, uint32_t i) const {
      const T *obj = static_cast<const T *> (object);
      typename U::const_iterator begin = (obj->*m_memberMap).begin ();
      typename U::const_iterator end = (obj->*m_memberMap).end ();
      uint32_t k = 0;
      for (typename U::const_iterator j = begin; j != end; j++, k++)
	{
	  if (k == i)
	    {
	      return std::pair<uint32_t, Ptr<Object> > ( k, (*j).second);
	      break;
	    }
	}
      NS_ASSERT (false);
      // quiet compiler.
      std::pair<uint32_t, Ptr<Object> > empty;
      return empty;
    }
    U T::*m_memberMap;
  } *spec = new MemberStdContainer ();
  spec->m_memberMap = memberMap;
  return Ptr<const AttributeAccessor> (spec, false);
}

template <typename T, typename U, typename INDEX>
Ptr<const AttributeAccessor>
MakeObjectMapAccessor (Ptr<U> (T::*get) (INDEX) const,
			   INDEX (T::*getN) (void) const)
{
  struct MemberGetters : public ObjectMapAccessor
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
MakeObjectMapAccessor (INDEX (T::*getN) (void) const,
			   Ptr<U> (T::*get) (INDEX) const)
{
  return MakeObjectMapAccessor (get, getN);
}

template <typename T>
Ptr<const AttributeChecker> MakeObjectMapChecker (void)
{
  return Create<internal::AnObjectMapChecker<T> > ();
}


} // namespace ns3

#endif /* OBJECT_MAP_H */
