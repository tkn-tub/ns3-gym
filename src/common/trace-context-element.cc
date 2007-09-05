#include "trace-context-element.h"

namespace ns3 {

uint32_t 
ElementRegistry::GetSize (uint16_t uid)
{
  InfoVector *vec = GetInfoVector ();
  struct Info info = (*vec)[uid - 1];
  return info.size;
}
void 
ElementRegistry::Print (uint16_t uid, uint8_t *instance, std::ostream &os)
{
  InfoVector *vec = GetInfoVector ();
  struct Info info = (*vec)[uid - 1];
  info.print (instance, os);
}
void 
ElementRegistry::Destroy (uint16_t uid, uint8_t *instance)
{
  InfoVector *vec = GetInfoVector ();
  struct Info info = (*vec)[uid - 1];
  info.destroy (instance);
}
ElementRegistry::InfoVector *
ElementRegistry::GetInfoVector (void)
{
  static InfoVector vector;
  return &vector;
}


} // namespace ns3
