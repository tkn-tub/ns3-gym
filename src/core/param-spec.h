#ifndef PARAM_SPEC_H
#define PARAM_SPEC_H

#include <string>
#include <stdint.h>
#include "object-base.h"
#include "ptr.h"

namespace ns3 {

class Value;

class ParamSpec : public ObjectBase
{
public:
  ParamSpec ();
  void Ref (void) const;
  void Unref (void) const;
  virtual ~ParamSpec ();

  /**
   * \param object the object instance to set the value in
   * \param value the value to set
   * \returns true if the value is valid and it could be set
   *          successfully, false otherwise.
   */
  virtual bool Set (ObjectBase * object, Ptr<const Value> parameter) const = 0;
  virtual bool Get (const ObjectBase * object, Ptr<Value> parameter) const = 0;
  virtual bool Check (Ptr<const Value> parameter) const = 0;
  virtual Ptr<Value> CreateInitialValue (void) const = 0;
private:
  mutable uint32_t m_count;
};

} // namespace ns3

#endif /* PARAM_SPEC_H */
