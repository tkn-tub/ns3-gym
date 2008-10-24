#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/simulator-module.h"
#include "ns3/helper-module.h"
#include "ns3/mobility-module.h"
#include <iostream>

using namespace ns3;

static void 
CourseChange (std::ostream *os, std::string foo, Ptr<const MobilityModel> mobility)
{
  Vector pos = mobility->GetPosition ();
  Vector vel = mobility->GetVelocity ();
  *os << Simulator::Now () << " POS: x=" << pos.x << ", y=" << pos.y
      << ", z=" << pos.z << "; VEL:" << vel.x << ", y=" << vel.y
      << ", z=" << vel.z << std::endl;
}

int main (int argc, char *argv[])
{
  Ns2MobilityHelper mobility(argv[1]);
  std::ofstream os;
  os.open (argv[2]);
  NodeContainer stas;
  stas.Create (1);

  mobility.Install ();

  Config::Connect ("/NodeList/*/$ns3::MobilityModel/CourseChange",
                   MakeBoundCallback (&CourseChange, &os));

  Simulator::Stop (Seconds (10.0));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
