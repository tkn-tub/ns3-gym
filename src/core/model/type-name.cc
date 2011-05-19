/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "type-name.h"

namespace ns3 {

#define DEF_TYPE(x)                             \
  template <>                                   \
  std::string TypeNameGet<x> (void)             \
  {                                             \
    return # x;                                  \
  }

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


} //namespace ns3
