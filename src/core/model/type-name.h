/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef TYPE_NAME_H
#define TYPE_NAME_H

#include <stdint.h>
#include <string>

namespace ns3 {

template <typename T>
std::string TypeNameGet (void)
{
  return "unknown";
}

#define DEF_TYPE(x)                             \
  template <>                                   \
  std::string TypeNameGet<x> (void)

DEF_TYPE (uint8_t);
DEF_TYPE (uint16_t);
DEF_TYPE (uint32_t);
DEF_TYPE (uint64_t);
DEF_TYPE (int8_t);
DEF_TYPE (int16_t);
DEF_TYPE (int32_t);
DEF_TYPE (int64_t);
DEF_TYPE (float);
DEF_TYPE (double);

#undef DEF_TYPE

} //namespace ns3

#endif /* TYPE_NAME_H */
