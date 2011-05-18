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
 * Authors: Faker Moatamri <faker.moatamri@sophia.inria.fr>
 *          Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "attribute-iterator.h"
#include <gtk/gtk.h>

namespace ns3
{

enum
{
  COL_NODE = 0, COL_LAST
};

struct ModelNode
{
  enum
  {
    // store object + attribute name
    NODE_ATTRIBUTE,
    // store object + attribute name
    NODE_POINTER,
    // store object + attribute name
    NODE_VECTOR,
    // store index + value (object)
    NODE_VECTOR_ITEM,
    // store object
    NODE_OBJECT
  } type;
  std::string name;
  Ptr<Object> object;
  uint32_t index;
};
/**
 * \ingroup configstore
 *
 */
class ModelCreator : public AttributeIterator
{
public:
  ModelCreator ();

  void Build (GtkTreeStore *treestore);
private:
  virtual void DoVisitAttribute (Ptr<Object> object, std::string name);
  virtual void DoStartVisitObject (Ptr<Object> object);
  virtual void DoEndVisitObject (void);
  virtual void DoStartVisitPointerAttribute (Ptr<Object> object, std::string name, Ptr<Object> value);
  virtual void DoEndVisitPointerAttribute (void);
  virtual void DoStartVisitArrayAttribute (Ptr<Object> object, std::string name, const ObjectVectorValue &vector);
  virtual void DoEndVisitArrayAttribute (void);
  virtual void DoStartVisitArrayItem (const ObjectVectorValue &vector,
                                      uint32_t index, Ptr<Object> item);
  virtual void DoEndVisitArrayItem (void);
  void Add (ModelNode *node);
  void Remove (void);

  GtkTreeStore *m_treestore;
  std::vector<GtkTreeIter *> m_iters;
};
}
