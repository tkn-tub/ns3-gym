#include "int64x64.h"
#include <stdint.h>
#include <iostream>
#include <iomanip>  // showpos
#include <sstream>
#include "assert.h"
#include "log.h"

// Note:  Logging in this file is largely avoided due to the
// number of calls that are made to these functions and the possibility
// of causing recursions leading to stack overflow

NS_LOG_COMPONENT_DEFINE ("int64x64");

namespace ns3 {

std::ostream &operator << (std::ostream &os, const int64x64_t &value)
{
  int64_t hi = value.GetHigh ();

  // Save stream format flags
  std::ios_base::fmtflags ff = os.flags ();
  os << std::setw (1);

  { /// \internal
    /// See \bugid{1737}:  gcc libstc++ 4.2 bug
    if (hi == 0)
      { 
	os << '+';
      }
    else
      {
	os << std::showpos;
      }
  }
  
  os << std::right << hi << ".";

  os << std::noshowpos;

  int64x64_t low(0, value.GetLow ());
  int places = 0;    // Number of decimal places printed so far
  const bool floatfield = os.flags () & std::ios_base::floatfield;
  bool more = true;  // Should we print more digits?

  do 
    {
      low = 10 * low;
      int64_t digit = low.GetHigh ();
      low -= digit;

      os << std::setw (1) << digit;

      ++places;
      if (floatfield)
	{
	  more = places < os.precision ();
	}
      else  // default
	{
	  // Full resolution is 20 decimal digits
	  more = low.GetLow () && (places < 20);
	}

    } while (more);

  os.flags (ff);  // Restore stream flags
  return os;
}

static uint64_t ReadHiDigits (std::string str)
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

static uint64_t ReadLoDigits (std::string str)
{
  int64x64_t low (0, 0);
  const int64x64_t round (0, 5);

  for (std::string::const_reverse_iterator rchar = str.rbegin ();
       rchar != str.rend ();
       ++rchar)
    {
      int digit = *rchar - '0';
      low = (low + digit + round) / 10; 
    }
  
  return low.GetLow ();
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
      hi = ReadHiDigits (str.substr (cur, next-cur));
      lo = ReadLoDigits (str.substr (next+1, str.size ()-(next+1)));
    }
  else
    {
      hi = ReadHiDigits (str.substr (cur, str.size ()-cur));
      lo = 0;
    }
  hi = negative ? -hi : hi;
  value = int64x64_t (hi, lo);
  return is;
}

} // namespace ns3
