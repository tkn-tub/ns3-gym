#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#include "type-id.h"
#include "callback.h"
#include "attribute-list.h"
#include <string>

/**
 * This macro should be invoked once for every class which
 * defines a new GetTypeId method.
 */
#define NS_OBJECT_ENSURE_REGISTERED(type)       \
  static struct X##type##RegistrationClass      \
  {                                             \
    X##type##RegistrationClass () {             \
      ns3::TypeId tid = type::GetTypeId ();     \
      tid.GetParent ();                         \
    }                                           \
} x_##type##RegistrationVariable

namespace ns3 {

/**
 * This base class is really used only to make sure that 
 * every subclass has RTTI information and that they all
 * share a single base class to allow us to make type 
 * checks across all these types.
 */
class ObjectBase
{
public:
  static TypeId GetTypeId (void);

  virtual ~ObjectBase ();

  /**
   * \returns the TypeId associated to the most-derived type
   *          of this instance.
   *
   * This method is typically implemented by ns3::Object::GetInstanceTypeId
   * but some classes which derive from ns3::ObjectBase directly
   * have to implement it themselves.
   */
  virtual TypeId GetInstanceTypeId (void) const = 0;

  /**
   * \param name the name of the attribute to set
   * \param value the name of the attribute to set
   *
   * Set a single attribute. This cannot fail: if the input is invalid,
   * it will crash immediately.
   */
  void SetAttribute (std::string name, Attribute value);
  /**
   * \param name the name of the attribute to set
   * \param value the name of the attribute to set
   * \returns true if the requested attribute exists and could be set, 
   * false otherwise.
   */
  bool SetAttributeFailSafe (std::string name, Attribute value);
  /**
   * \param name the name of the attribute to read
   * \returns true if the requested attribute was found, false otherwise.
   *
   * If the input attribute name does not exist, this method crashes.
   */
  std::string GetAttributeAsString (std::string name) const;
  /**
   * \param name the name of the attribute to read
   * \returns the attribute read.
   *
   * If the input attribute name does not exist, this method crashes.
   */
  Attribute GetAttribute (std::string name) const;

  /**
   * \param name the name of the attribute to read
   * \param value the string where the result value should be stored
   * \returns true if the requested attribute was found, false otherwise.
   */
  bool GetAttributeAsStringFailSafe (std::string name, std::string &value) const;
  /**
   * \param name the name of the attribute to read
   * \param attribute the attribute where the result value should be stored
   * \returns true if the requested attribute was found, false otherwise.
   *
   * If the input attribute name does not exist, this method crashes.
   */
  bool GetAttributeFailSafe (std::string name, Attribute &attribute) const;

  bool TraceConnectWithoutContext (std::string name, const CallbackBase &cb);
  bool TraceConnectWithoutContext (std::string name, std::string context, const CallbackBase &cb);
  bool TraceDisconnectWithoutContext (std::string name, const CallbackBase &cb);
  bool TraceDisconnectWithoutContext (std::string name, std::string context, const CallbackBase &cb);

protected:
  virtual void NotifyConstructionCompleted (void);
  /**
   * \param attributes the attribute values used to initialize 
   *        the member variables of this object's instance.
   *
   * Invoked from subclasses to initialize all of their 
   * attribute members.
   */
  void ConstructSelf (const AttributeList &attributes);

private:
  bool DoSet (Ptr<const AttributeAccessor> spec, Attribute intialValue, 
              Ptr<const AttributeChecker> checker, Attribute value);

};

} // namespace ns3

#endif /* OBJECT_BASE_H */
