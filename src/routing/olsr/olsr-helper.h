#ifndef OLSR_HELPER_H
#define OLSR_HELPER_H

#include "ns3/object-factory.h"
#include "ns3/node-container.h"
#include "ns3/node.h"

namespace ns3 {

class OlsrHelper
{
public:
  OlsrHelper ();

  void SetAgent (std::string tid,
		 std::string n0 = "", Attribute v0 = Attribute (),
		 std::string n1 = "", Attribute v2 = Attribute (),
		 std::string n2 = "", Attribute v2 = Attribute (),
		 std::string n3 = "", Attribute v3 = Attribute (),
		 std::string n4 = "", Attribute v4 = Attribute (),
		 std::string n5 = "", Attribute v5 = Attribute (),
		 std::string n6 = "", Attribute v6 = Attribute (),
		 std::string n7 = "", Attribute v7 = Attribute ());

  template <typename InputIterator>
  void Enable (InputIterator begin, InputIterator end);
  void Enable (NodeContainer container);
  void Enable (Ptr<Node> node);
  void EnableAll (void);
private:
  ObjectFactory m_agentFactory;
};

} // namespace ns3

namespace ns3 {

template <typename InputIterator>
void 
OlsrHelper::Enable (InputIterator begin, InputIterator end)
{
  for (InputIterator i = begin; i != end; i++)
    {
      //Ptr<Node> node = (*i)->GetObject<Node> ();
      //Enable (node);
    }
}

} // namespace ns3

#endif /* OLSR_HELPER_H */
