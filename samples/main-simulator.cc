/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:nil -*- */
#include "ns3/simulator.h"
#include "ns3/nstime.h"
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
    Simulator::Schedule (Time::RelS (10.0), 
                         &MyModel::DealWithEvent, 
                         this, Simulator::Now ().S ());
}
void
MyModel::DealWithEvent (double value)
{
    std::cout << "Member method received event at " << Simulator::Now ().S () << 
        "s started at " << value << "s" << std::endl;
}

static void 
random_function (MyModel *model)
{
    std::cout << "random function received event at " << 
        Simulator::Now ().S () << "s" << std::endl;
    model->Start ();
}


int main (int argc, char *argv[])
{
    MyModel model;

    Simulator::Schedule (Time::AbsS (10.0), &random_function, &model);

    Simulator::Run ();

    Simulator::Destroy ();
}
