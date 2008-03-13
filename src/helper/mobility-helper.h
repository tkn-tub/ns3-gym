#ifndef MOBILITY_HELPER_H
#define MOBILITY_HELPER_H

#include <vector>
#include "ns3/object-factory.h"
#include "ns3/attribute.h"
#include "position-allocator.h"

namespace ns3 {

class PositionAllocator;
class MobilityModel;

class MobilityHelper
{
public:
  MobilityHelper ();

  void EnableNotifier (void);
  void DisableNotifier (void);

  void SetPositionAllocator (Ptr<PositionAllocator> allocator);

  void SetPositionAllocator (std::string type,
			     std::string n1 = "", Attribute v1 = Attribute (),
			     std::string n2 = "", Attribute v2 = Attribute (),
			     std::string n3 = "", Attribute v3 = Attribute (),
			     std::string n4 = "", Attribute v4 = Attribute (),
			     std::string n5 = "", Attribute v5 = Attribute (),
			     std::string n6 = "", Attribute v6 = Attribute (),
			     std::string n7 = "", Attribute v7 = Attribute (),
			     std::string n8 = "", Attribute v8 = Attribute (),
			     std::string n9 = "", Attribute v9 = Attribute ());

  void SetMobilityModel (std::string type,
			 std::string n1 = "", Attribute v1 = Attribute (),
			 std::string n2 = "", Attribute v2 = Attribute (),
			 std::string n3 = "", Attribute v3 = Attribute (),
			 std::string n4 = "", Attribute v4 = Attribute (),
			 std::string n5 = "", Attribute v5 = Attribute (),
			 std::string n6 = "", Attribute v6 = Attribute (),
			 std::string n7 = "", Attribute v7 = Attribute (),
			 std::string n8 = "", Attribute v8 = Attribute (),
			 std::string n9 = "", Attribute v9 = Attribute ());

  void PushReferenceMobilityModel (Ptr<Object> reference);
  void PopReferenceMobilityModel (void);

  std::string GetMobilityModelType (void) const;

  template <typename T>
  void Layout (T begin, T end);
private:
  void Layout (const std::vector<Ptr<Object> > &objects);

  std::vector<Ptr<MobilityModel> > m_mobilityStack;
  bool m_notifierEnabled;
  ObjectFactory m_mobility;
  Ptr<PositionAllocator> m_position;
};

} // namespace ns3

namespace ns3 {

template <typename T>
void
MobilityHelper::Layout (T begin, T end)
{
  std::vector<Ptr<Object> > objects;
  for (T i = begin; i != end; i++)
    {
      Ptr<Object> object = *i;
      objects.push_back (object);
    }
  Layout (objects);
}

} // namespace ns3

#endif /* MOBILITY_HELPER_H */
