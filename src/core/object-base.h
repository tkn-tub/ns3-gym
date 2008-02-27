#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

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
  virtual ~ObjectBase ();
};

} // namespace ns3

#endif /* OBJECT_BASE_H */
