#ifndef ATTRIBUTE_DEFAULT_ITERATOR_H
#define ATTRIBUTE_DEFAULT_ITERATOR_H

#include <string>

namespace ns3 {

class AttributeDefaultIterator
{
public:
  virtual ~AttributeDefaultIterator () = 0;
  void Iterate (void);
private:
  virtual void StartVisitTypeId (std::string name);
  virtual void EndVisitTypeId (void);
  virtual void VisitAttribute (std::string name, std::string defaultValue);
};

} // namespace ns3

#endif /* ATTRIBUTE_DEFAULT_ITERATOR_H */
