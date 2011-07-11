#include "int64x64.h"
#include <stdint.h>
#include <iostream>
#include <sstream>
#include "ns3/assert.h"

namespace ns3 {

static uint8_t MostSignificantDigit (uint64_t value)
{
  uint8_t n = 0;
  do
    {
      n++;
      value /= 10;
    } while (value != 0);
  return n;
}

static uint64_t PowerOfTen (uint8_t n)
{
  uint64_t retval = 1;
  while (n > 0)
    {
      retval *= 10;
      n--;
    }
  return retval;
}

std::ostream &operator << (std::ostream &os, const int64x64_t &value)
{
  int64_t hi = value.GetHigh ();
  os << ((hi<0) ? "-" : "+") << ((hi<0) ? -hi : hi) << ".";
  uint64_t low = value.GetLow ();
  uint8_t msd = MostSignificantDigit (~((uint64_t)0));
  do
    {
      msd--;
      uint64_t pow = PowerOfTen (msd);
      uint8_t digit = low / pow;
      NS_ASSERT (digit < 10);
      os << (uint16_t) digit;
      low -= digit * pow;
    } while (msd > 0 && low > 0);
  return os;
}

static uint64_t ReadDigits (std::string str)
{
  const char *buf = str.c_str ();
  uint64_t retval = 0;
  while (*buf != 0)
    {
      retval *= 10;
      retval += *buf - 0x30;
      buf++;
    }
  return retval;
}

std::istream &operator >> (std::istream &is, int64x64_t &value)
{
  std::string str;

  is >> str;
  bool negative;
  // skip heading spaces
  std::string::size_type cur;
  cur = str.find_first_not_of (" ");
  std::string::size_type next;
  // first, remove the sign.
  next = str.find ("-", cur);
  if (next != std::string::npos)
    {
      negative = true;
      next++;
    }
  else
    {
      next = str.find ("+", cur);
      if (next != std::string::npos)
        {
          next++;
        }
      else
        {
          next = cur;
        }
      negative = false;
    }
  cur = next;
  int64_t hi;
  uint64_t lo;
  next = str.find (".", cur);
  if (next != std::string::npos)
    {
      hi = ReadDigits (str.substr (cur, next-cur));
      lo = ReadDigits (str.substr (next+1, str.size ()-(next+1)));
    }
  else
    {
      hi = ReadDigits (str.substr (cur, str.size ()-cur));
      lo = 0;
    }
  hi = negative ? -hi : hi;
  value = int64x64_t (hi, lo);
  return is;
}

} // namespace ns3
