#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <stdint.h>
#include "object-base.h"
#include "ptr.h"

namespace ns3 {

class ParamSpec;

class Value : public ObjectBase
{
public:
  Value ();
  void Ref (void) const;
  void Unref (void) const;
  virtual ~Value ();

  virtual Ptr<Value> Copy (void) const = 0;
  virtual std::string SerializeToString (Ptr<const ParamSpec> spec) const = 0;
  virtual bool DeserializeFromString (std::string value, Ptr<const ParamSpec> spec) = 0;
private:
  mutable uint32_t m_count;
};

} // namespace ns3

#endif /* VALUE_H */
