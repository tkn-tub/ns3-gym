#include "application-container.h"

namespace ns3 {

ApplicationContainer::Iterator 
ApplicationContainer::Begin (void) const
{
  return m_applications.begin ();
}
ApplicationContainer::Iterator 
ApplicationContainer::End (void) const
{
  return m_applications.end ();
}

uint32_t 
ApplicationContainer::GetN (void) const
{
  return m_applications.size ();
}
Ptr<Application> 
ApplicationContainer::Get (uint32_t i) const
{
  return m_applications[i];
}
void 
ApplicationContainer::Add (ApplicationContainer other)
{
  for (Iterator i = other.Begin (); i != other.End (); i++)
    {
      m_applications.push_back (*i);
    }
}
void 
ApplicationContainer::Add (Ptr<Application> application)
{
  m_applications.push_back (application);
}

void 
ApplicationContainer::Start (Time start)
{
  for (Iterator i = Begin (); i != End (); ++i)
    {
      Ptr<Application> app = *i;
      app->Start (start);
    }
}
void 
ApplicationContainer::Stop (Time stop)
{
  for (Iterator i = Begin (); i != End (); ++i)
    {
      Ptr<Application> app = *i;
      app->Stop (stop);
    }
}


} // namespace ns3
