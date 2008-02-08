#ifndef MOBILITY_HELPER_H
#define MOBILITY_HELPER_H

#include <vector>
#include "ns3/object-factory.h"
#include "ns3/value.h"
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
			     std::string n1 = "", PValue v1 = PValue (),
			     std::string n2 = "", PValue v2 = PValue (),
			     std::string n3 = "", PValue v3 = PValue (),
			     std::string n4 = "", PValue v4 = PValue (),
			     std::string n5 = "", PValue v5 = PValue (),
			     std::string n6 = "", PValue v6 = PValue (),
			     std::string n7 = "", PValue v7 = PValue (),
			     std::string n8 = "", PValue v8 = PValue (),
			     std::string n9 = "", PValue v9 = PValue ());

  void SetMobilityModel (std::string type,
			 std::string n1 = "", PValue v1 = PValue (),
			 std::string n2 = "", PValue v2 = PValue (),
			 std::string n3 = "", PValue v3 = PValue (),
			 std::string n4 = "", PValue v4 = PValue (),
			 std::string n5 = "", PValue v5 = PValue (),
			 std::string n6 = "", PValue v6 = PValue (),
			 std::string n7 = "", PValue v7 = PValue (),
			 std::string n8 = "", PValue v8 = PValue (),
			 std::string n9 = "", PValue v9 = PValue ());

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
