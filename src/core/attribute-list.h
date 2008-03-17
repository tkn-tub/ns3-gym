#ifndef ATTRIBUTE_LIST_H
#define ATTRIBUTE_LIST_H

#include <string>
#include <vector>
#include "attribute.h"
#include "type-id.h"

namespace ns3 {

/**
 * \brief a container of attributes to be used during object's construction
 *        and in ns3::Object::Set.
 *
 */
class AttributeList
{
public:
  AttributeList ();
  AttributeList (const AttributeList &o);
  AttributeList &operator = (const AttributeList &o);
  ~AttributeList ();
  /**
   * \param name the full name of the attribute to set
   * \param value the value to set
   *
   * This method checks that a attribute with the requested
   * name exists and that the value specified is an acceptable
   * value of that attribute. If any of these checks fails,
   * the program terminates with a message.
   */
  void Set (std::string name, Attribute value);
  /**
   * \param name the full name of the attribute to set
   * \param value the value to set
   * \returns true if the requested attribute exists and could be
   *          stored, false otherwise.
   */
  bool SetFailSafe (std::string name, Attribute value);
  /**
   * \param tid the TypeId associated to this attribute
   * \param name the name (not full!) of the attribute
   * \param value the value to set
   *
   * This method checks that a attribute with the requested
   * name exists and that the value specified is an acceptable
   * value of that attribute. If any of these checks fails,
   * the program terminates with a message.
   */
  void SetWithTid (TypeId tid, std::string name, Attribute value);

  /**
   * Clear the content of this instance.
   */
  void Reset (void);

  /**
   * \returns the global attribute container
   *
   * The global attribute container can be used to specify
   * a set of attribute values without having to re-specify
   * them for each object when it is created. This container
   * is checked only during object construction and 
   * it is always checked last, after any per-object
   * container is checked.
   */
  static AttributeList *GetGlobal (void);

  // XXX: untested.
  std::string SerializeToString (void) const;
  bool DeserializeFromString (std::string value);
private:
  friend class ObjectBase;
  struct Attr {
    Ptr<const AttributeChecker> checker;
    Attribute value;
  };
  typedef std::vector<struct Attr> Attrs;
  typedef Attrs::iterator Iterator;
  typedef Attrs::const_iterator CIterator;



  bool DoSet (struct TypeId::AttributeInfo *info, Attribute param);
  void DoSetOne (Ptr<const AttributeChecker> checker, Attribute param);
  std::string LookupAttributeFullNameByChecker (Ptr<const AttributeChecker> checker) const;

  Attrs m_attributes;
};

} // namespace ns3

#endif /* ATTRIBUTE_LIST_H */
