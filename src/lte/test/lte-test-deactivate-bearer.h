/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef LENA_TEST_DEACTIVATE_BEARER_H
#define LENA_TEST_DEACTIVATE_BEARER_H

#include "ns3/simulator.h"
#include "ns3/test.h"


namespace ns3 {

class LenaDeactivateBearerTestCase : public TestCase
{
public:
  LenaDeactivateBearerTestCase (std::vector<uint16_t> dist, std::vector<uint32_t> estThrPssDl, std::vector<uint16_t> packetSize, uint16_t interval, bool  errorModelEnabled, bool useIdealRrc);
  virtual ~LenaDeactivateBearerTestCase ();

private:
  static std::string BuildNameString (uint16_t nUser, std::vector<uint16_t> dist);
  virtual void DoRun (void);
  uint16_t m_nUser;
  std::vector<uint16_t> m_dist;
  std::vector<uint16_t> m_packetSize;  // byte
  uint16_t m_interval;    // ms
  std::vector<uint32_t> m_estThrPssDl;
  bool m_errorModelEnabled;
};



class LenaTestBearerDeactivateSuite : public TestSuite
{
public:
  LenaTestBearerDeactivateSuite ();
};

} // namespace ns3

#endif
