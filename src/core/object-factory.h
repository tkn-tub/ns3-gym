#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include "attribute.h"
#include "object.h"

namespace ns3 {

/**
 * \brief instantiate subclasses of ns3::Object.
 *
 * This class can also hold a set of attributes to set
 * automatically during the object construction.
 */
class ObjectFactory
{
public:
  ObjectFactory ();

  /**
   * \param tid the TypeId of the object to instantiate.
   */
  void SetTypeId (TypeId tid);
  /**
   * \param tid the TypeId of the object to instantiate.
   */  
  void SetTypeId (const char *tid);
  /**
   * \param tid the TypeId of the object to instantiate.
   */  
  void SetTypeId (std::string tid);
  /**
   * \param name the name of the attribute to set during object construction
   * \param value the value of the attribute to set during object construction
   */
  void Set (std::string name, Attribute value);

  /**
   * \returns the currently-selected TypeId to use to create an object
   *          instance.
   */
  TypeId GetTypeId (void) const;

  /**
   * \returns a new object instance.
   */
  Ptr<Object> Create (void) const;
  /**
   * \returns a new object instance.
   *
   * This method performs an extra call to ns3::Object::GetObject before
   * returning a pointer of the requested type to the user. This method
   * is really syntactical sugar.
   */
  template <typename T>
  Ptr<T> Create (void) const;

  ATTRIBUTE_HELPER_HEADER_1 (ObjectFactory);
private:
  TypeId m_tid;
  AttributeList m_parameters;
};

std::ostream & operator << (std::ostream &os, const ObjectFactory &factory);
std::istream & operator >> (std::istream &is, ObjectFactory &factory);

ATTRIBUTE_HELPER_HEADER_2 (ObjectFactory);

} // namespace ns3

namespace ns3 {

template <typename T>
Ptr<T> 
ObjectFactory::Create (void) const
{
  Ptr<Object> object = Create ();
  return object->GetObject<T> ();
}

} // namespace ns3

#endif /* OBJECT_FACTORY_H */
