#ifndef ATTRIBUTE_ITERATOR_H
#define ATTRIBUTE_ITERATOR_H

#include "ns3/ptr.h"
#include "ns3/object.h"
#include <vector>

namespace ns3 {

class AttributeIterator
{
public:
  AttributeIterator ();
  virtual ~AttributeIterator ();

  void Iterate (void);
private:
  virtual void DoVisit (Ptr<Object> object, std::string name, std::string path) = 0;
  virtual void DoPush (std::string name, std::string path) = 0;
  virtual void DoPop (void) = 0;

  void DoIterate (Ptr<Object> object);
  bool IsExamined (Ptr<const Object> object);
  std::string GetCurrentPath (std::string attr) const;
  void Push (std::string name);
  void Pop (void);
  void Visit (Ptr<Object> object, std::string name);


  std::vector<Ptr<Object> > m_examined;
  std::vector<std::string> m_currentPath;
};

class TextFileAttributeIterator : public AttributeIterator
{
public:
  TextFileAttributeIterator (std::ostream &os);
  void Save (void);
private:
  virtual void DoVisit (Ptr<Object> object, std::string name, std::string path);
  virtual void DoPush (std::string name, std::string path);
  virtual void DoPop (void);
  std::ostream &m_os;
};


} // namespace ns3

#endif /* ATTRIBUTE_ITERATOR_H */
