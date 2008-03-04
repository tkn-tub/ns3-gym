#include "mobility-helper.h"
#include "mobility-model.h"
#include "mobility-model-notifier.h"
#include "position-allocator.h"
#include "hierarchical-mobility-model.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("MobilityHelper");

MobilityHelper::MobilityHelper ()
  : m_notifierEnabled (false)
{
  m_position = CreateObjectWith<RandomRectanglePositionAllocator> ("X", ConstantVariable (0.0),
								   "Y", ConstantVariable (0.0));
  m_mobility.SetTypeId ("StaticMobilityModel");
}
void 
MobilityHelper::EnableNotifier (void)
{
  m_notifierEnabled = true;
}
void 
MobilityHelper::DisableNotifier (void)
{
  m_notifierEnabled = false;
}
void 
MobilityHelper::SetPositionAllocator (Ptr<PositionAllocator> allocator)
{
  m_position = allocator;
}
void 
MobilityHelper::SetPositionAllocator (std::string type,
				      std::string n1, Attribute v1,
				      std::string n2, Attribute v2,
				      std::string n3, Attribute v3,
				      std::string n4, Attribute v4,
				      std::string n5, Attribute v5,
				      std::string n6, Attribute v6,
				      std::string n7, Attribute v7,
				      std::string n8, Attribute v8,
				      std::string n9, Attribute v9)
{
  ObjectFactory pos;
  pos.SetTypeId (type);
  pos.Set (n1, v1);
  pos.Set (n2, v2);
  pos.Set (n3, v3);
  pos.Set (n4, v4);
  pos.Set (n5, v5);
  pos.Set (n6, v6);
  pos.Set (n7, v7);
  pos.Set (n8, v8);
  pos.Set (n9, v9);
  m_position = pos.Create ()->GetObject<PositionAllocator> ();
}

void 
MobilityHelper::SetMobilityModel (std::string type,
				  std::string n1, Attribute v1,
				  std::string n2, Attribute v2,
				  std::string n3, Attribute v3,
				  std::string n4, Attribute v4,
				  std::string n5, Attribute v5,
				  std::string n6, Attribute v6,
				  std::string n7, Attribute v7,
				  std::string n8, Attribute v8,
				  std::string n9, Attribute v9)
{
  m_mobility.SetTypeId (type);
  m_mobility.Set (n1, v1);
  m_mobility.Set (n2, v2);
  m_mobility.Set (n3, v3);
  m_mobility.Set (n4, v4);
  m_mobility.Set (n5, v5);
  m_mobility.Set (n6, v6);
  m_mobility.Set (n7, v7);
  m_mobility.Set (n8, v8);
  m_mobility.Set (n9, v9);
}

void 
MobilityHelper::PushReferenceMobilityModel (Ptr<Object> reference)
{
  Ptr<MobilityModel> mobility = reference->GetObject<MobilityModel> ();
  m_mobilityStack.push_back (mobility);
}
void 
MobilityHelper::PopReferenceMobilityModel (void)
{
  m_mobilityStack.pop_back ();
}


std::string 
MobilityHelper::GetMobilityModelType (void) const
{
  return m_mobility.GetTypeId ().GetName ();
}

void 
MobilityHelper::Layout (const std::vector<Ptr<Object> > &objects)
{
  for (std::vector<Ptr<Object> >::const_iterator i = objects.begin (); i != objects.end (); i++)
    {
      Ptr<Object> object = *i;
      Ptr<MobilityModel> model = object->GetObject<MobilityModel> ();
      if (model == 0)
	{
	  model = m_mobility.Create ()->GetObject<MobilityModel> ();
	  if (model == 0)
	    {
	      NS_FATAL_ERROR ("The requested mobility model is not a mobility model: \""<< 
			      m_mobility.GetTypeId ().GetName ()<<"\"");
	    }
	  if (m_mobilityStack.empty ())
	    {
	      NS_LOG_DEBUG ("node="<<object<<", mob="<<model);
	      object->AggregateObject (model);
	    }
	  else
	    {
	      // we need to setup a hierarchical mobility model
	      Ptr<MobilityModel> parent = m_mobilityStack.back ();
	      Ptr<MobilityModel> hierarchical = 
		CreateObjectWith<HierarchicalMobilityModel> ("Child", model,
							     "Parent", parent);
	      object->AggregateObject (hierarchical);
	      NS_LOG_DEBUG ("node="<<object<<", mob="<<hierarchical);
	    }
	}
      Vector position = m_position->GetNext ();
      model->SetPosition (position);
      if (m_notifierEnabled)
	{
	  Ptr<MobilityModelNotifier> notifier = 
	    object->GetObject<MobilityModelNotifier> ();
	  if (notifier == 0)
	    {
	      object->AggregateObject (CreateObject<MobilityModelNotifier> ());
	    }
	}
    }
}

} // namespace ns3
