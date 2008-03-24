#include "internet-stack-helper.h"
#include "ns3/internet-stack.h"

namespace ns3 {

void 
InternetStackHelper::Build (NodeContainer c)
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      AddInternetStack (node);
    }
}


} // namespace ns3
