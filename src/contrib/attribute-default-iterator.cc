#include "attribute-default-iterator.h"
#include "ns3/type-id.h"
#include "ns3/attribute.h"
#include "ns3/object-vector.h"
#include "ns3/pointer.h"
#include "ns3/global-value.h"
#include "ns3/string.h"

namespace ns3 {

AttributeDefaultIterator::~AttributeDefaultIterator ()
{}
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
	  if (accessor == 0)
	    {
	      continue;
	    }
	  if (!accessor->HasSetter ())
	    {
	      continue;
	    }
	  Ptr<const AttributeChecker> checker = tid.GetAttributeChecker (j);
	  if (checker == 0)
	    {
	      continue;
	    }
	  Ptr<const AttributeValue> value = tid.GetAttributeInitialValue (j);
	  if (value == 0)
	    {
	      continue;
	    }
	  Ptr<const ObjectVectorValue> vector = DynamicCast<const ObjectVectorValue> (value);
	  if (vector != 0)
	    {
	      continue;
	    }
	  Ptr<const PointerValue> pointer = DynamicCast<const PointerValue> (value);
	  if (pointer != 0)
	    {
	      continue;
	    }
	  if (!calledStart)
	    {
	      StartVisitTypeId (tid.GetName ());
	    }
	  VisitAttribute (tid.GetAttributeName (j),
			  value->SerializeToString (checker));
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
{}
void 
AttributeDefaultIterator::EndVisitTypeId (void)
{}
void 
AttributeDefaultIterator::VisitAttribute (std::string name, std::string defaultValue)
{}


} // namespace ns3
