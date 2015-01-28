/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef TYPE_ID_H
#define TYPE_ID_H

#include "attribute.h"
#include "attribute-accessor-helper.h"
#include "trace-source-accessor.h"
#include "attribute-helper.h"
#include "callback.h"
#include "deprecated.h"
#include "hash.h"
#include <string>
#include <stdint.h>

namespace ns3 {

class ObjectBase;

/**
 * \brief a unique identifier for an interface.
 *
 * This class records a lot of meta-information about a 
 * subclass of the Object base class:
 *  - the base class of the subclass
 *  - the set of accessible constructors in the subclass
 *  - the set of 'attributes' accessible in the subclass
 *
 * \see attribute_TypeId
 *
 * \internal
 *  See the discussion in IidManager about hash chaining of TypeId's.
 */
class TypeId
{
public:
  /**
   * Flags describing when a given attribute can be read or written
   */
  enum AttributeFlag {
    ATTR_GET = 1<<0, /**< The attribute can be read */
    ATTR_SET = 1<<1, /**< The attribute can be written */
    ATTR_CONSTRUCT = 1<<2, /**< The attribute can be written at construction-time */
    ATTR_SGC = ATTR_GET | ATTR_SET | ATTR_CONSTRUCT, /**< The attribute can be read, and written at any time */
  };
  struct AttributeInformation {
    std::string name;
    std::string help;
    uint32_t flags;
    Ptr<const AttributeValue> originalInitialValue;
    Ptr<const AttributeValue> initialValue;
    Ptr<const AttributeAccessor> accessor;
    Ptr<const AttributeChecker> checker;
  };
  struct TraceSourceInformation {
    std::string name;
    std::string help;
    std::string callback;
    Ptr<const TraceSourceAccessor> accessor;
  };

  /**
   * Type of hash values
   */
  typedef uint32_t hash_t;

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
   * \param hash the hash to lookup
   * \returns the unique id associated with the requested hash.
   *
   * This method cannot fail: it will crash if the input 
   * hash does not match an existing TypeId.
   */
  static TypeId LookupByHash (hash_t hash);
  /**
   * \param hash the hash of the requested TypeId
   * \param tid a pointer to the TypeId instance where the 
   *        result of this function should be stored.
   * \returns true if the requested hash was found, false otherwise.
   */
  static bool LookupByHashFailSafe (hash_t hash, TypeId *tid);

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
  explicit TypeId (const char * name);

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

  bool HasParent (void) const;

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
   * \returns the hash of this interface.
   */
  hash_t GetHash (void) const;

  /**
   * \returns the size of this interface.
   */
  std::size_t GetSize (void) const;

  /**
   * \returns true if this TypeId has a constructor
   */
  bool HasConstructor (void) const;

  /**
   * \returns the number of attributes associated to this TypeId
   */
  uint32_t GetAttributeN (void) const;
  /**
   * \param i index into attribute array
   * \returns the information associated to attribute whose 
   *          index is i.
   */
  struct TypeId::AttributeInformation GetAttribute(uint32_t i) const;
  /**
   * \param i index into attribute array
   * \returns the full name associated to the attribute whose
   *          index is i.
   */
  std::string GetAttributeFullName (uint32_t i) const;

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
   * \returns detailed information about the requested trace source.
   */
  struct TypeId::TraceSourceInformation GetTraceSource(uint32_t i) const;

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
   * Set the size of this type, based on the \p sizeof operator.
   *
   * Call this way:
   * \code
   *   SetSize (sizeof (<typename>));
   * \endcode
   * This is done automatically by NS_LOG_ENSURE_REGISTERED()
   * A ridiculously large reported size is a symptom that the
   * type hasn't been registered.
   *
   * \param size The size of the object, in bytes.
   * \returns this TypeId instance.
   */
  TypeId SetSize (std::size_t size);
  
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
                       const AttributeValue &initialValue,
                       Ptr<const AttributeAccessor> accessor,
                       Ptr<const AttributeChecker> checker);

  /**
   * \param i the attribute to manipulate
   * \param initialValue the new initial value to use for this attribute.
   * \returns true if the call was successfuly, false otherwise.
   */
  bool SetAttributeInitialValue(uint32_t i, 
                                Ptr<const AttributeValue> initialValue);

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
                       const AttributeValue &initialValue,
                       Ptr<const AttributeAccessor> accessor,
                       Ptr<const AttributeChecker> checker);

  /**
   * \param name the name of the new trace source
   * \param help some help text which describes the purpose of this
   *        trace source.
   * \param accessor a pointer to a TraceSourceAccessor which can be
   *        used to connect/disconnect sinks to this trace source.
   * \param callback fully qualified typedef name for the callback signature.
   *        Generally this should begin with the "ns3::" namespace qualifier.
   * \returns this TypeId instance.
   */
  TypeId AddTraceSource (std::string name,
                         std::string help,
                         Ptr<const TraceSourceAccessor> accessor)
    NS_DEPRECATED;
  
  /**
   * \param name the name of the new trace source
   * \param help some help text which describes the purpose of this
   *        trace source.
   * \param accessor a pointer to a TraceSourceAccessor which can be
   *        used to connect/disconnect sinks to this trace source.
   * \param callback fully qualified typedef name for the callback signature.
   *        Generally this should begin with the "ns3::" namespace qualifier.
   * \returns this TypeId instance.
   */
  TypeId AddTraceSource (std::string name,
                         std::string help,
                         Ptr<const TraceSourceAccessor> accessor,
                         std::string callback);

  TypeId HideFromDocumentation (void);

  /**
   * \param name the name of the requested attribute
   * \param info a pointer to the TypeId::AttributeInformation data structure
   *        where the result value of this method will be stored.
   * \returns true if the requested attribute could be found, false otherwise.
   */
  bool LookupAttributeByName (std::string name, struct AttributeInformation *info) const;
  /**
   * \param name the name of the requested trace source
   * \returns the trace source accessor which can be used to connect and disconnect
   *          trace sinks with the requested trace source on an object instance.
   *
   * If no matching trace source is found, this method returns zero.
   */
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
  inline TypeId ();
  inline TypeId (const TypeId &o);
  inline TypeId &operator = (const TypeId &o);
  inline ~TypeId ();

private:
  friend class AttributeList;
  friend bool operator == (TypeId a, TypeId b);
  friend bool operator != (TypeId a, TypeId b);
  friend bool operator <  (TypeId a, TypeId b);


  explicit TypeId (uint16_t tid);
  void DoAddConstructor (Callback<ObjectBase *> callback);

  uint16_t m_tid;
};
  
std::ostream & operator << (std::ostream &os, TypeId tid);
std::istream & operator >> (std::istream &is, TypeId &tid);
inline bool operator == (TypeId a, TypeId b);
inline bool operator != (TypeId a, TypeId b);
bool operator <  (TypeId a, TypeId b);

ATTRIBUTE_HELPER_HEADER (TypeId);

} // namespace ns3 

namespace ns3 {

TypeId::TypeId ()
  : m_tid (0) {
}
TypeId::TypeId (const TypeId &o)
  : m_tid (o.m_tid) {
}
TypeId &TypeId::operator = (const TypeId &o)
{
  m_tid = o.m_tid;
  return *this;
}
TypeId::~TypeId ()
{
}
inline bool operator == (TypeId a, TypeId b)
{
  return a.m_tid == b.m_tid;
}

inline bool operator != (TypeId a, TypeId b)
{
  return a.m_tid != b.m_tid;
}


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
