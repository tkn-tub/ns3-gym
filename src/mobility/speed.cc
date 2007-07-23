#include "speed.h"

namespace ns3 {

Speed::Speed (double _dx, double _dy, double _dz)
  : dx (_dx),
    dy (_dy),
    dz (_dz)
{}

Speed::Speed ()
  : dx (0.0),
    dy (0.0),
    dz (0.0)
{}

} // namespace ns3
