#include "time-base.h"
#include "ns3/assert.h"

namespace ns3 {

struct TimeBase::Resolution
TimeBase::GetNsResolution (void)
{  
  struct Resolution resolution;
  SetResolution (TimeBase::NS, &resolution);
  return resolution;
}
void 
TimeBase::SetResolution (enum Unit resolution)
{
  SetResolution (resolution, PeekResolution ());
}
void 
TimeBase::SetResolution (enum Unit unit, struct Resolution *resolution)
{
  int8_t power [LAST] = {15, 12, 9, 6, 3, 0};
  for (int i = 0; i < TimeBase::LAST; i++)
    {
      int shift = power[i] - power[(int)unit];
      uint64_t factor = (uint64_t) pow (10, fabs (shift));
      struct Information *info = &resolution->info[i];
      info->factor = factor;
      if (shift == 0)
	{
	  info->timeFrom = HighPrecision (1, false);
	  info->timeTo = HighPrecision (1, false);
	  info->toMul = true;
	  info->fromMul = true;
	}
      else if (shift > 0)
	{
	  info->timeFrom = HighPrecision (factor, false);
	  info->timeTo = HighPrecision::Invert (factor);
	  info->toMul = false;
	  info->fromMul = true;
	}
      else
	{
	  NS_ASSERT (shift < 0);
	  info->timeFrom = HighPrecision::Invert (factor);
	  info->timeTo = HighPrecision (factor, false);
	  info->toMul = true;
	  info->fromMul = false;
	}
    }
  resolution->unit = unit;
}
enum TimeBase::Unit
TimeBase::GetResolution (void)
{
  return PeekResolution ()->unit;
}

} // namespace ns3
