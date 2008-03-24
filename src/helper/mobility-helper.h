#ifndef MOBILITY_HELPER_H
#define MOBILITY_HELPER_H

#include <vector>
#include "ns3/object-factory.h"
#include "ns3/attribute.h"
#include "ns3/position-allocator.h"
#include "node-container.h"

namespace ns3 {

class PositionAllocator;
class MobilityModel;

/**
 * \brief assign positions and mobility models to nodes.
 */
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

  void Layout (NodeContainer container);
  void LayoutAll (void);
private:

  std::vector<Ptr<MobilityModel> > m_mobilityStack;
  bool m_notifierEnabled;
  ObjectFactory m_mobility;
  Ptr<PositionAllocator> m_position;
};

} // namespace ns3

#endif /* MOBILITY_HELPER_H */
