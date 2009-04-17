/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/command-line.h"
#include <iostream>

using namespace ns3;

class MyModel {
public:
  void Start (void);
private:
  void DealWithEvent (double eventValue);
};

void 
MyModel::Start (void)
{
  Simulator::Schedule (Seconds (10.0), 
                       &MyModel::DealWithEvent, 
                       this, Simulator::Now ().GetSeconds ());
}
void
MyModel::DealWithEvent (double value)
{
  std::cout << "Member method received event at " << Simulator::Now ().GetSeconds () 
            << "s started at " << value << "s" << std::endl;
}

static void 
random_function (MyModel *model)
{
  std::cout << "random function received event at " << 
      Simulator::Now ().GetSeconds () << "s" << std::endl;
  model->Start ();
}


int main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);

  MyModel model;

  Simulator::Schedule (Seconds (10.0), &random_function, &model);

  Simulator::Run ();

  Simulator::Destroy ();
}
