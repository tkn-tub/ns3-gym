#ifndef POSITION_H
#define POSITION_H

namespace ns3 {

class Position
{
public:
  Position (double x, double y, double z);
  Position ();
  double x;
  double y;
  double z;
};

double CalculateDistance (const Position &a, const Position &b);

} // namespace ns3

#endif /* POSITION_H */
