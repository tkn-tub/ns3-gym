#ifndef POSITION_H
#define POSITION_H

namespace ns3 {

class Position
{
public:
  Position (double x, double y, double z);
  double x;
  double y;
  double z;
};

} // namespace ns3

#endif /* POSITION_H */
