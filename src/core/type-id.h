#ifndef TYPE_ID_H
#define TYPE_ID_H

#include "attribute.h"
#include "attribute-accessor-helper.h"
#include "attribute-helper.h"
#include "callback.h"
#include <string>
#include <stdint.h>

namespace ns3 {

class ObjectBase;
class TraceSourceAccessor;

/**
 * \brief a unique identifier for an interface.
 *
 * This class records a lot of meta-information about a 
 * subclass of the Object base class:
 *  - the base class of the subclass
 *  - the set of accessible constructors in the subclass
 *  - the set of 'attributes' accessible in the subclass
 */
class TypeId
{
public:
  enum {
    /**
     * The attribute can be read
     */
    ATTR_GET = 1<<0,
    /**
     * The attribute can be written
     */
    ATTR_SET = 1<<1,
    /**
     * The attribute can be written at construction-time.
     */
    ATTR_CONSTRUCT = 1<<2,
    /**
     * The attribute can be read, and written at any time.
     */
    ATTR_SGC = ATTR_GET | ATTR_SET | ATTR_CONSTRUCT,
  };

  /**
   * \param name the name of the requested TypeId
   * \returns the unique id associated with the requested
   *          name. 
   *
   * This method cannot fail: it will crash if the input 
   * name is not a valid TypeId name.
   */
  static TypeId LookupByName (std::string name);
  /**
   * \param name the name of the requested TypeId
   * \param tid a pointer to the TypeId instance where the 
   *        result of this function should be stored.
   * \returns true if the requested name was found, false otherwise.
   */
  static bool LookupByNameFailSafe (std::string name, TypeId *tid);

  /**
   * \returns the number of TypeId instances registered.
   */
  static uint32_t GetRegisteredN (void);
  /**
   * \param i index
   * \returns the TypeId instance whose index is i.
   */
  static TypeId GetRegistered (uint32_t i);

  /**
   * \param name the name of the interface to construct.
   *
   * No two instances can share the same name. The name is expected to be
   * the full c++ typename of associated c++ object.
   */
  TypeId (const char * name);

  /**
   * \returns the parent of this TypeId
   *
   * This method cannot fail. It will return itself
   * if this TypeId has no parent. i.e., it is at the top
   * of the TypeId hierarchy. Currently, this is the
   * case for the TypeId associated to the Object class 
   * only.
   */
  TypeId GetParent (void) const;

  /**
   * \param other a parent TypeId
   * \returns true if the input TypeId is really a parent
   *          of this TypeId, false otherwise.
   *
   * Calling this method is roughly similar to calling dynamic_cast
   * except that you do not need object instances: you can do the check
   * with TypeId instances instead.
   */
  bool IsChildOf (TypeId other) const;

  /**
   * \returns the name of the group associated to this TypeId.
   */
  std::string GetGroupName (void) const;

  /**
   * \returns the name of this interface.
   */
  std::string GetName (void) const;

  /**
   * \returns true if this TypeId has a constructor
   */
  bool HasConstructor (void) const;

  /**
   * \returns the number of attributes associated to this TypeId
   */
  uint32_t GetAttributeListN (void) const;
  /**
   * \param i index into attribute array
   * \returns the name associated to the attribute whose
   *          index is i.
   */
  std::string GetAttributeName (uint32_t i) const;
  /**
   * \param i index into attribute array.
   * \returns the help text associated to the attribute whose
   *          index is i.
   */
  std::string GetAttributeHelp (uint32_t i) const;
  /**
   * \param i index into attribute array
   * \returns the full name associated to the attribute whose
   *          index is i.
   */
  std::string GetAttributeFullName (uint32_t i) const;

  /**
   * \param i index into attribute array.
   * \returns the value with which the associated attribute 
   *          is initialized.
   */
  Attribute GetAttributeInitialValue (uint32_t i) const;
  /**
   * \param i index into attribute array.
   * \returns the flags associated to the requested attribute.
   */
  uint32_t GetAttributeFlags (uint32_t i) const;
  /**
   * \param i index into attribute array.
   * \returns the checker associated to the requested attribute.
   */
  Ptr<const AttributeChecker> GetAttributeChecker (uint32_t i) const;
  /**
   * \param i index into attribute array.
   * \returns the accessor associated to the requested attribute.
   */
  Ptr<const AttributeAccessor> GetAttributeAccessor (uint32_t i) const;

  /**
   * \returns a callback which can be used to instanciate an object
   *          of this type.
   */
  Callback<ObjectBase *> GetConstructor (void) const;

  /**
   * \returns true if this TypeId should be hidden from the user, 
   *          false otherwise.
   */
  bool MustHideFromDocumentation (void) const;


  /**
   * \returns the number of trace sources defined in this TypeId.
   */
  uint32_t GetTraceSourceN (void) const;
  /**
   * \param i index into trace source array.
   * \returns the name of the requested trace source.
   */
  std::string GetTraceSourceName (uint32_t i) const;
  /**
   * \param i index into trace source array.
   * \returns the help text of the requested trace source.
   */
  std::string GetTraceSourceHelp (uint32_t i) const;
  /**
   * \param i index into trace source array.
   * \returns the accessor used to get access to the requested
   *          trace source.
   */
  Ptr<const TraceSourceAccessor> GetTraceSourceAccessor (uint32_t i) const;


  /**
   * \param tid the TypeId of the base class.
   * \return this TypeId instance.
   *
   * Record in this TypeId which TypeId is the TypeId
   * of the base class of the subclass.
   */
  TypeId SetParent (TypeId tid);
  /**
   * \return this TypeId instance.
   *
   * Record in this TypeId which TypeId is the TypeId
   * of the base class of the subclass.
   */
  template <typename T>
  TypeId SetParent (void);

  /**
   * \param groupName the name of the group this TypeId belongs to.
   * \returns this TypeId instance.
   *
   * The group name is purely an advisory information used to 
   * group together types according to a user-specific grouping
   * scheme.
   */
  TypeId SetGroupName (std::string groupName);

  /**
   * \returns this TypeId instance
   *
   * Record in this TypeId the fact that the default constructor
   * is accessible.
   */
  template <typename T>
  TypeId AddConstructor (void);

  /**
   * \param name the name of the new attribute
   * \param help some help text which describes the purpose of this
   *        attribute.
   * \param initialValue the initial value for this attribute.
   * \param accessor an instance of the associated AttributeAccessor subclass.
   * \param checker an instance of the associated AttributeChecker subclass.
   * \returns this TypeId instance
   *
   * Record in this TypeId the fact that a new attribute exists.
   */
  TypeId AddAttribute (std::string name,
                       std::string help, 
                       Attribute initialValue,
                       Ptr<const AttributeAccessor> accessor,
                       Ptr<const AttributeChecker> checker);

  /**
   * \param name the name of the new attribute
   * \param help some help text which describes the purpose of this
   *        attribute
   * \param flags flags which describe how this attribute can be read and/or written.
   * \param initialValue the initial value for this attribute.
   * \param accessor an instance of the associated AttributeAccessor subclass.
   * \param checker an instance of the associated AttributeChecker subclass.
   * \returns this TypeId instance
   *
   * Record in this TypeId the fact that a new attribute exists.
   */
  TypeId AddAttribute (std::string name,
                       std::string help, 
                       uint32_t flags,
                       Attribute initialValue,
                       Ptr<const AttributeAccessor> accessor,
                       Ptr<const AttributeChecker> checker);

  /**
   * \param name the name of the new trace source
   * \param help some help text which describes the purpose of this
   *        trace source.
   * \param accessor a pointer to a TraceSourceAccessor which can be
   *        used to connect/disconnect sinks to this trace source.
   * \returns this TypeId instance.
   */
  TypeId AddTraceSource (std::string name,
                         std::string help,
                         Ptr<const TraceSourceAccessor> accessor);

  TypeId HideFromDocumentation (void);

  /**
   * \brief store together a set of attribute properties.
   */
  struct AttributeInfo {
    // The accessor associated to the attribute.
    Ptr<const AttributeAccessor> accessor;
    // The initial value associated to the attribute.
    Attribute initialValue;
    // The set of access control flags associated to the attribute.
    uint32_t flags;
    // The checker associated to the attribute.
    Ptr<const AttributeChecker> checker;
  };
  /**
   * \param name the name of the requested attribute
   * \param info a pointer to the TypeId::AttributeInfo data structure
   *        where the result value of this method will be stored.
   * \returns true if the requested attribute could be found, false otherwise.
   */
  bool LookupAttributeByName (std::string name, struct AttributeInfo *info) const;
  Ptr<const TraceSourceAccessor> LookupTraceSourceByName (std::string name) const;

  /**
   * \returns the internal integer which uniquely identifies this
   *          TypeId.
   *
   * This is really an internal method which users are not expected
   * to use.
   */
  uint16_t GetUid (void) const;
  /**
   * \param tid the internal integer which uniquely identifies 
   *        this TypeId.
   *
   * This method is even more internal than TypeId::GetUid. Use
   * at your own risk and don't be surprised that it eats raw 
   * babies on full-moon nights.
   */
  void SetUid (uint16_t tid);

  // construct an invalid TypeId.
  TypeId ();
  ~TypeId ();

  ATTRIBUTE_HELPER_HEADER_1 (TypeId);
private:
  friend class AttributeList;
  friend bool operator == (TypeId a, TypeId b);
  friend bool operator != (TypeId a, TypeId b);


  /**
   * \param fullName the full name of the requested attribute
   * \param info a pointer to the TypeId::AttributeInfo data structure
   *        where the result value of this method will be stored.
   * \returns the Accessor associated to the requested attribute
   */
  static bool LookupAttributeByFullName (std::string fullName, struct AttributeInfo *info);

  explicit TypeId (uint16_t tid);
  void DoAddConstructor (Callback<ObjectBase *> callback);
  
  uint16_t m_tid;
};

std::ostream & operator << (std::ostream &os, TypeId tid);
std::istream & operator >> (std::istream &is, TypeId &tid);

ATTRIBUTE_HELPER_HEADER_2 (TypeId);

} // namespace ns3 

namespace ns3 {

/*************************************************************************
 *   The TypeId implementation which depends on templates
 *************************************************************************/

template <typename T>
TypeId 
TypeId::SetParent (void)
{
  return SetParent (T::GetTypeId ());
}

template <typename T>
TypeId 
TypeId::AddConstructor (void)
{
  struct Maker {
    static ObjectBase * Create () {
      ObjectBase * base = new T ();
      return base;
    }
  };
  Callback<ObjectBase *> cb = MakeCallback (&Maker::Create);
  DoAddConstructor (cb);
  return *this;
}

} // namespace ns3

#endif /* TYPE_ID_H */
