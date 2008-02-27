#ifndef NODE_CONTAINER_H
#define NODE_CONTAINER_H

#include <stdint.h>
#include <vector>
#include "node.h"

namespace ns3 {

class NodeContainer
{
public:
  typedef std::vector<Ptr<Node> >::const_iterator Iterator;

  Iterator Begin (void) const;
  Iterator End (void) const;

  uint32_t GetN (void) const;
  Ptr<Node> Get (uint32_t i) const;

  void Create (uint32_t n);
  void Add (NodeContainer other);
  void Add (Ptr<Node> node);

 private:
  std::vector<Ptr<Node> > m_nodes;
};

} // namespace ns3

#endif /* NODE_CONTAINER_H */
