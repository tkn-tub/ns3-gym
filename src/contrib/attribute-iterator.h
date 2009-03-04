#ifndef ATTRIBUTE_ITERATOR_H
#define ATTRIBUTE_ITERATOR_H

#include "ns3/ptr.h"
#include "ns3/object.h"
#include <vector>

namespace ns3 {

class ObjectVectorValue;

// This class is used internally by ConfigStore and GtkConfigStore.
class AttributeIterator
{
public:
  AttributeIterator ();
  virtual ~AttributeIterator ();

  void Iterate (void);
protected:
  std::string GetCurrentPath (void) const;
private:
  virtual void DoVisitAttribute (Ptr<Object> object, std::string name) = 0;
  virtual void DoStartVisitObject (Ptr<Object> object);
  virtual void DoEndVisitObject (void);
  virtual void DoStartVisitPointerAttribute (Ptr<Object> object, std::string name, Ptr<Object> value);
  virtual void DoEndVisitPointerAttribute (void);
  virtual void DoStartVisitArrayAttribute (Ptr<Object> object, std::string name, const ObjectVectorValue &vector);
  virtual void DoEndVisitArrayAttribute (void);
  virtual void DoStartVisitArrayItem (const ObjectVectorValue &vector, uint32_t index, Ptr<Object> item);
  virtual void DoEndVisitArrayItem (void);

  void DoIterate (Ptr<Object> object);
  bool IsExamined (Ptr<const Object> object);
  std::string GetCurrentPath (std::string attr) const;

  void VisitAttribute (Ptr<Object> object, std::string name);
  void StartVisitObject (Ptr<Object> object);
  void EndVisitObject (void);
  void StartVisitPointerAttribute (Ptr<Object> object, std::string name, Ptr<Object> value);
  void EndVisitPointerAttribute (void);
  void StartVisitArrayAttribute (Ptr<Object> object, std::string name, const ObjectVectorValue &vector);
  void EndVisitArrayAttribute (void);
  void StartVisitArrayItem (const ObjectVectorValue &vector, uint32_t index, Ptr<Object> item);
  void EndVisitArrayItem (void);


  std::vector<Ptr<Object> > m_examined;
  std::vector<std::string> m_currentPath;
};

} // namespace ns3

#endif /* ATTRIBUTE_ITERATOR_H */
