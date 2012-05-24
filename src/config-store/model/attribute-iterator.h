/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
 
#ifndef ATTRIBUTE_ITERATOR_H
#define ATTRIBUTE_ITERATOR_H

#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/object-ptr-container.h"
#include <vector>

namespace ns3 {

/**
 * \ingroup configstore
 *
 * This class is used internally by ConfigStore and GtkConfigStore.
 */
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
  virtual void DoStartVisitArrayAttribute (Ptr<Object> object, std::string name, const ObjectPtrContainerValue &vector);
  virtual void DoEndVisitArrayAttribute (void);
  virtual void DoStartVisitArrayItem (const ObjectPtrContainerValue &vector, uint32_t index, Ptr<Object> item);
  virtual void DoEndVisitArrayItem (void);

  void DoIterate (Ptr<Object> object);
  bool IsExamined (Ptr<const Object> object);
  std::string GetCurrentPath (std::string attr) const;

  void VisitAttribute (Ptr<Object> object, std::string name);
  void StartVisitObject (Ptr<Object> object);
  void EndVisitObject (void);
  void StartVisitPointerAttribute (Ptr<Object> object, std::string name, Ptr<Object> value);
  void EndVisitPointerAttribute (void);
  void StartVisitArrayAttribute (Ptr<Object> object, std::string name, const ObjectPtrContainerValue &vector);
  void EndVisitArrayAttribute (void);
  void StartVisitArrayItem (const ObjectPtrContainerValue &vector, uint32_t index, Ptr<Object> item);
  void EndVisitArrayItem (void);


  std::vector<Ptr<Object> > m_examined;
  std::vector<std::string> m_currentPath;
};

} // namespace ns3

#endif /* ATTRIBUTE_ITERATOR_H */
