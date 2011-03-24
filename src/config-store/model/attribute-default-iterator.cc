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
 
#include "attribute-default-iterator.h"
#include "ns3/attribute.h"
#include "ns3/object-vector.h"
#include "ns3/pointer.h"
#include "ns3/global-value.h"
#include "ns3/string.h"

namespace ns3
{

AttributeDefaultIterator::~AttributeDefaultIterator ()
{
}
void 
AttributeDefaultIterator::Iterate (void)
{
  for (uint32_t i = 0; i < TypeId::GetRegisteredN (); i++)
    {
      TypeId tid = TypeId::GetRegistered (i);
      if (tid.MustHideFromDocumentation ())
        {
          continue;
        }
      bool calledStart = false;
      for (uint32_t j = 0; j < tid.GetAttributeN (); j++)
        {
          uint32_t flags = tid.GetAttributeFlags (j);
          if (!(flags & TypeId::ATTR_CONSTRUCT))
            {
              // we can't construct the attribute, so, there is no
              // initial value for the attribute
              continue;
            }
          Ptr<const AttributeAccessor> accessor = tid.GetAttributeAccessor (j);
          //No accessor, go to next attribute
          if (accessor == 0)
            {
              continue;
            }
          if (!accessor->HasSetter ())
            {
              //skip this attribute it doesn't have an setter
              continue;
            }
          Ptr<const AttributeChecker> checker = tid.GetAttributeChecker (j);
          if (checker == 0)
            {
              //skip, it doesn't have a checker
              continue;
            }
          Ptr<const AttributeValue> value = tid.GetAttributeInitialValue (j);
          if (value == 0)
            {
              //No value, check next attribute
              continue;
            }
          Ptr<const ObjectVectorValue> vector = DynamicCast<const ObjectVectorValue> (value);
          if (vector != 0)
            {
              //a vector value, won't take it
              continue;
            }
          Ptr<const PointerValue> pointer = DynamicCast<const PointerValue> (value);
          if (pointer != 0)
            {
              //pointer value, won't take it
              continue;
            }
          //We take only values, no pointers or vectors
          if (!calledStart)
            {
              StartVisitTypeId (tid.GetName ());
            }
          VisitAttribute (tid, tid.GetAttributeName (j), value->SerializeToString (checker), j);
          calledStart = true;
        }
      if (calledStart)
        {
          EndVisitTypeId ();
        }
    }
}

void 
AttributeDefaultIterator::StartVisitTypeId (std::string name)
{
}
void 
AttributeDefaultIterator::EndVisitTypeId (void)
{
}

void 
AttributeDefaultIterator::DoVisitAttribute (std::string name, std::string defaultValue)
{
}

void 
AttributeDefaultIterator::VisitAttribute (TypeId tid, std::string name, std::string defaultValue, uint32_t index)
{
  DoVisitAttribute (name, defaultValue);
}

} // namespace ns3
