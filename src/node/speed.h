#ifndef SPEED_H
#define SPEED_H

namespace ns3 {

class Speed 
{
public:
  Speed (double _dx, double _dy, double _dz);
  Speed ();
  double dx;
  double dy;
  double dz;
};

} // namespace ns3

#endif /* SPEED_H */
