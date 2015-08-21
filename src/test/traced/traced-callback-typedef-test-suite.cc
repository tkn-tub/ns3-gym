/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Lawrence Livermore National Laboratory
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author:  Peter D. Barnes, Jr. <pdbarnes@llnl.gov>
 */

#include "ns3/test.h"
#include "ns3/core-module.h"

#include "ns3/dsr-module.h"       // DsrOPtionSRHeader
#include "ns3/internet-module.h"  // Ipv4, Ipv4L3Protocol, Ipv4PacketProbe
                                  // Ipv6L3Protocol, Ipv6PacketProbe
#include "ns3/lr-wpan-mac.h"      // LrWpanMac
#include "ns3/lte-module.h"       // PhyReceptionStatParameters,
                                  // PhyTransmissionStatParameters,
                                  // LteUePowerControl
#include "ns3/mesh-module.h"      // PeerManagementProtocol
#include "ns3/mobility-module.h"  // MobilityModel
#include "ns3/network-module.h"   // Packet, PacketBurst
#include "ns3/olsr-module.h"      // olsr::RoutingProtocol
#include "ns3/sixlowpan-module.h" // SixLowPanNetDevice
#include "ns3/spectrum-module.h"  // SpectrumValue
#include "ns3/stats-module.h"     // TimeSeriesAdapter
#include "ns3/uan-module.h"       // UanPhy
#include "ns3/wifi-module.h"      // WifiMacHeader, WifiPhyStateHelper


#include <iostream>
#include <sstream>
#include <set>
#include <string>

using namespace ns3;

namespace {

/**
 * Stringify the known TracedCallback type names.
 *
 * \tparam T \explicit The typedef name.
 * \param [in] N The number of arguments expected.
 * \returns The \c TracedCallback type name.
 */
template <typename T> inline
std::string TypeName (int N) { return "unknown"; }

#define TYPENAME(T)		  \
  template <> inline std::string  \
  TypeName < T >     (int N)	  \
  {				  \
    std::stringstream ss;	  \
    ss << # T << "(" << N << ")"; \
    return ss.str ();		  \
  }

/**
 * \name Stringify known typename.
 * @{
 */
TYPENAME (dsr::DsrOptionSRHeader::TracedCallback);
TYPENAME (EpcUeNas::StateTracedCallback);
TYPENAME (Ipv4L3Protocol::DropTracedCallback);
TYPENAME (Ipv4L3Protocol::SentTracedCallback);
TYPENAME (Ipv4L3Protocol::TxRxTracedCallback);
TYPENAME (Ipv6L3Protocol::DropTracedCallback);
TYPENAME (Ipv6L3Protocol::SentTracedCallback);
TYPENAME (Ipv6L3Protocol::TxRxTracedCallback);
TYPENAME (LrWpanMac::SentTracedCallback);
TYPENAME (LrWpanMac::StateTracedCallback);
TYPENAME (LrWpanPhy::StateTracedCallback);
TYPENAME (LteEnbMac::DlSchedulingTracedCallback);
TYPENAME (LteEnbMac::UlSchedulingTracedCallback);
TYPENAME (LteEnbPhy::ReportInterferenceTracedCallback);
TYPENAME (LteEnbPhy::ReportUeSinrTracedCallback);
TYPENAME (LteEnbRrc::ConnectionHandoverTracedCallback);
TYPENAME (LteEnbRrc::HandoverStartTracedCallback);
TYPENAME (LteEnbRrc::NewUeContextTracedCallback);
TYPENAME (LteEnbRrc::ReceiveReportTracedCallback);
TYPENAME (LtePdcp::PduRxTracedCallback);  
TYPENAME (LtePdcp::PduTxTracedCallback);
TYPENAME (LteUePhy::StateTracedCallback);
TYPENAME (LteUePhy::RsrpSinrTracedCallback);  
TYPENAME (LteUePhy::RsrpRsrqTracedCallback);
TYPENAME (LteUeRrc::CellSelectionTracedCallback);
TYPENAME (LteUeRrc::StateTracedCallback);  
TYPENAME (Mac48Address::TracedCallback);
TYPENAME (MobilityModel::TracedCallback);
TYPENAME (olsr::RoutingProtocol::PacketTxRxTracedCallback);
TYPENAME (olsr::RoutingProtocol::TableChangeTracedCallback);
TYPENAME (Packet::AddressTracedCallback);
TYPENAME (Packet::Mac48AddressTracedCallback);
TYPENAME (Packet::SinrTracedCallback);
TYPENAME (Packet::SizeTracedCallback);
TYPENAME (Packet::TracedCallback);
TYPENAME (PacketBurst::TracedCallback);
TYPENAME (dot11s::PeerManagementProtocol::LinkOpenCloseTracedCallback);
TYPENAME (PhyReceptionStatParameters::TracedCallback);
TYPENAME (PhyTransmissionStatParameters::TracedCallback);
TYPENAME (SixLowPanNetDevice::DropTracedCallback);
TYPENAME (SixLowPanNetDevice::RxTxTracedCallback);
TYPENAME (SpectrumChannel::LossTracedCallback);
TYPENAME (SpectrumValue::TracedCallback);
TYPENAME (TimeSeriesAdaptor::OutputTracedCallback);
TYPENAME (UanMac::PacketModeTracedCallback);
TYPENAME (UanMacCw::QueueTracedCallback);
TYPENAME (UanMacRc::QueueTracedCallback);
TYPENAME (UanNetDevice::RxTxTracedCallback);
TYPENAME (UanPhy::TracedCallback);
TYPENAME (UeManager::StateTracedCallback);
TYPENAME (WifiMacHeader::TracedCallback);
TYPENAME (WifiPhyStateHelper::RxOkTracedCallback);
TYPENAME (WifiPhyStateHelper::StateTracedCallback);
TYPENAME (WifiPhyStateHelper::TxTracedCallback);
TYPENAME (WifiRemoteStationManager::PowerChangeTracedCallback);
TYPENAME (WifiRemoteStationManager::RateChangeTracedCallback);
/** @} */
#undef TYPENAME


/** Record typedefs which are identical to previously declared. */
std::set<std::string>
Duplicates (void)
{
  std::set<std::string> dupes;

#define dupename(T)    dupes.insert (# T)
  
  dupename (LteRlc::NotifyTxTracedCallback);
  dupename (LteRlc::ReceiveTracedCallback);
  dupename (LteUeRrc::ImsiCidRntiTracedCallback);
  dupename (LteUeRrc::MibSibHandoverTracedCallback);
  dupename (WifiPhyStateHelper::RxEndErrorTracedCallback);

#undef dupename
  
  return dupes;
}
  
/**
 * Container for duplicate types.
 */
std::set<std::string> g_dupes = Duplicates ();

  
/**
 * Number of arguments passed to callback.
 *
 * Since the sink function is outside the invoking class,
 * which in this case is TracedCallbackTestCase, we can't use
 * the test macros directly.  Instead, we cache success
 * in the \c g_NArgs global value, then inspect it
 * in the TracedValueCallbackTestCase::CheckType method.
 */
int g_NArgs = 0;


/**
 * Log that a callback was invoked.
 *
 * We can't actually do anything with any of the arguments,
 * but the fact we got called is what's important.
 *
 * \param [in] N The number of arguments passed to the callback.
 */
void SinkIt (unsigned int N)
{
  std::cout << "with " << N << " args." << std::endl;
  g_NArgs = N;
}

/**
 * Sink functions.
 * @{
 */
template <typename T1, typename T2, typename T3, typename T4, typename T5>
class TracedCbSink
{
public:
  static void Sink (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) { SinkIt (5); }
};

template <typename T1, typename T2, typename T3, typename T4>
class TracedCbSink<T1, T2, T3, T4, empty>
{
public:
  static void Sink (T1 a1, T2 a2, T3 a3, T4 a4) { SinkIt (4); }
};

template <typename T1, typename T2, typename T3>
class TracedCbSink<T1, T2, T3, empty, empty>
{
public:
  static void Sink (T1 a1, T2 a2, T3 a3) { SinkIt (3); }
};

template <typename T1, typename T2>
class TracedCbSink<T1, T2, empty, empty, empty>
{
public:
  static void Sink (T1 a1, T2 a2) { SinkIt (2); }
};

template <typename T1>
class TracedCbSink< T1, empty, empty, empty, empty>
{
public:
  static void Sink (T1 a1) { SinkIt (1); }
};
/** @} */

/** Non-const non-reference type. */
template <typename T>
struct NonConstReferenced
{
  typedef typename TypeTraits< typename TypeTraits<T>::ReferencedType >::NonConstType Type;
};

}  // anonymous namespace


class TracedCallbackTypedefTestCase : public TestCase
{
public:
  TracedCallbackTypedefTestCase ();
  virtual ~TracedCallbackTypedefTestCase () {}
  
private:
  
  /** Invoker boilerplate. */
  template <typename T1, typename T2, typename T3, typename T4, typename T5>
  class CheckerBase : public Object
  {
  public:
    typename TypeTraits<T1>::BaseType m1;
    typename TypeTraits<T2>::BaseType m2;
    typename TypeTraits<T3>::BaseType m3;
    typename TypeTraits<T4>::BaseType m4;
    typename TypeTraits<T5>::BaseType m5;
    
    void Cleanup (int N)
    {
      if (g_NArgs == 0) std::cout << std::endl;
      NS_ASSERT_MSG (g_NArgs && g_NArgs == N, "failed.");
      g_NArgs = 0;
    }
  };  // CheckerBase
  
  /** Callback checkers. */
  template <typename T1, typename T2, typename T3, typename T4, typename T5>
  class Checker : public CheckerBase<T1, T2, T3, T4, T5>
  {
    TracedCallback<T1, T2, T3, T4, T5> m_cb;
    
  public:
    template <typename U>
    void Invoke (void)
    {
      const int N = 5;
      U sink = TracedCbSink<T1, T2, T3, T4, T5>::Sink;
      Callback<void, T1, T2, T3, T4, T5> cb = MakeCallback (sink);
      
      std::cout << TypeName<U> (N) << " invoked ";
      m_cb.ConnectWithoutContext (cb);
      m_cb (this->m1, this->m2, this->m3, this->m4, this->m5);
      this->Cleanup (N);
    }
  };  // Checker<5>
    
  template <typename T1, typename T2, typename T3, typename T4>
  class Checker<T1, T2, T3, T4, empty>
    : public CheckerBase<T1, T2, T3, T4, empty>
  {
    TracedCallback<T1, T2, T3, T4> m_cb;
    
  public:
    template <typename U>
    void Invoke (void)
    {
      const int N = 4;
      U sink = TracedCbSink<T1, T2, T3, T4, empty>::Sink;
      Callback<void, T1, T2, T3, T4> cb = MakeCallback (sink);
      
      std::cout << TypeName<U> (N) << " invoked ";
      m_cb.ConnectWithoutContext (cb);
      m_cb (this->m1, this->m2, this->m3, this->m4);
      this->Cleanup (N);
    }
  };  // Checker <4>
    

  template <typename T1, typename T2, typename T3>
  class Checker<T1, T2, T3, empty, empty>
    : public CheckerBase<T1, T2, T3, empty, empty>
  {
    TracedCallback<T1, T2, T3> m_cb;
    
  public:
    template <typename U>
    void Invoke (void)
    {
      const int N = 3;
      U sink = TracedCbSink<T1, T2, T3, empty, empty>::Sink;
      Callback<void, T1, T2, T3> cb = MakeCallback (sink);
      
      std::cout << TypeName<U> (N) << " invoked ";
      m_cb.ConnectWithoutContext (cb);
      m_cb (this->m1, this->m2, this->m3);
      this->Cleanup (N);
    }
  };  // Checker<3>
    
  template <typename T1, typename T2>
  class Checker<T1, T2, empty, empty, empty>
    : public CheckerBase<T1, T2, empty, empty, empty>
  {
    TracedCallback<T1, T2> m_cb;
    
  public:
    template <typename U>
    void Invoke (void)
    {
      const int N = 2;
      U sink = TracedCbSink<T1, T2, empty, empty, empty>::Sink;
      Callback<void, T1, T2> cb = MakeCallback (sink);
      
      std::cout << TypeName<U> (N) << " invoked ";
      m_cb.ConnectWithoutContext (cb);
      m_cb (this->m1, this->m2);
      this->Cleanup (N);
    }
  };  // Checker<2>

  template <typename T1>
  class Checker<T1, empty, empty, empty, empty>
    : public CheckerBase<T1, empty, empty, empty, empty>
  {
    TracedCallback<T1> m_cb;
    
  public:
    template <typename U>
    void Invoke (void)
    {
      const int N = 1;
      U sink = TracedCbSink<T1, empty, empty, empty, empty>::Sink;
      Callback<void, T1> cb = MakeCallback (sink);
      
      std::cout << TypeName<U> (N) << " invoked ";
      m_cb.ConnectWithoutContext (cb);
      m_cb (this->m1);
      this->Cleanup (N);
    }
  };  // Checker<1>

  virtual void DoRun (void);

};  // TracedCallbackTypedefTestCase

TracedCallbackTypedefTestCase::TracedCallbackTypedefTestCase ()
  : TestCase ("Check basic TracedCallback operation")
{
}

void
TracedCallbackTypedefTestCase::DoRun (void)
{

#define DUPE(U, T1)                                                     \
  if (g_dupes.find ( # U ) == g_dupes.end ())                           \
    NS_TEST_ASSERT_MSG_NE (0, 1,                                        \
                         "expected to find " <<  # U << " in dupes.");  \
  if (TypeName<U> (0) == TypeName<T1> (0))                              \
    std::cout << # U << " matches " << # T1  << std::endl;              \
  else                                                                  \
    NS_TEST_ASSERT_MSG_EQ                                               \
      (TypeName<U> (0), TypeName<T1> (0),                               \
       "the typedef " << # U <<                                         \
       " used to match the typedef " << # T1 <<                         \
       " but no longer does.  Please add a new CHECK call.")

#define CHECK(U, T1, T2, T3, T4, T5)                                    \
    CreateObject< Checker<T1, T2, T3, T4, T5> > () -> Invoke<U> ()

  CHECK (dsr::DsrOptionSRHeader::TracedCallback,
         const dsr::DsrOptionSRHeader &,
         empty, empty, empty, empty);

  CHECK (EpcUeNas::StateTracedCallback,
         EpcUeNas::State, EpcUeNas::State,
         empty, empty, empty);

  CHECK (Ipv4L3Protocol::DropTracedCallback,
         const Ipv4Header &, Ptr<const Packet>,
         Ipv4L3Protocol::DropReason, Ptr<Ipv4>, uint32_t );

  CHECK (Ipv4L3Protocol::SentTracedCallback,
         const Ipv4Header &, Ptr<const Packet>, uint32_t,
         empty, empty);

  CHECK (Ipv4L3Protocol::TxRxTracedCallback,
         Ptr<const Packet>, Ptr<Ipv4>, uint32_t,
         empty, empty);
  
  CHECK (Ipv6L3Protocol::DropTracedCallback,
         const Ipv6Header &, Ptr<const Packet>,
         Ipv6L3Protocol::DropReason, Ptr<Ipv6>, uint32_t
        );

  CHECK (Ipv6L3Protocol::SentTracedCallback,
         const Ipv6Header &, Ptr<const Packet>, uint32_t,
         empty, empty);

  CHECK (Ipv6L3Protocol::TxRxTracedCallback,
         Ptr<const Packet>, Ptr<Ipv6>, uint32_t,
         empty, empty);

  CHECK (LrWpanMac::SentTracedCallback,
         Ptr<const Packet>, uint8_t, uint8_t,
         empty, empty);

  CHECK (LrWpanMac::StateTracedCallback,
         LrWpanMacState, LrWpanMacState,
         empty, empty, empty);

  CHECK (LrWpanPhy::StateTracedCallback,
         Time, LrWpanPhyEnumeration, LrWpanPhyEnumeration,
         empty, empty);

  
  /*  Too many args :(
  CHECK (LteEnbMac::DlSchedulingTracedCallback,
         uint32_t, uint32_t, uint16_t,
         uint8_t, uint16_t, uint8_t, uint16_t);
  */

  CHECK (LteEnbMac::UlSchedulingTracedCallback,
         uint32_t, uint32_t, uint16_t, uint8_t, uint16_t);

  CHECK (LteEnbPhy::ReportUeSinrTracedCallback,
         uint16_t, uint16_t, double,
         empty, empty);

  CHECK (LteEnbPhy::ReportInterferenceTracedCallback,
         uint16_t, Ptr<SpectrumValue>,
         empty, empty, empty);

  CHECK (LteEnbRrc::ConnectionHandoverTracedCallback,
         uint64_t, uint16_t, uint16_t,
         empty, empty);

  CHECK (LteEnbRrc::HandoverStartTracedCallback,
         uint64_t, uint16_t, uint16_t, uint16_t,
         empty);
  
  CHECK (LteEnbRrc::NewUeContextTracedCallback,
         uint16_t, uint16_t,
         empty, empty, empty);

  CHECK (LteEnbRrc::ReceiveReportTracedCallback,
         uint64_t, uint16_t, uint16_t, LteRrcSap::MeasurementReport,
         empty);

  CHECK (LtePdcp::PduRxTracedCallback,
         uint16_t, uint8_t, uint32_t, uint64_t,
         empty);

  CHECK (LtePdcp::PduTxTracedCallback,
         uint16_t, uint8_t, uint32_t,
         empty, empty);

  DUPE  (LteRlc::NotifyTxTracedCallback, LtePdcp::PduTxTracedCallback);

  DUPE  (LteRlc::ReceiveTracedCallback, LtePdcp::PduRxTracedCallback);

  CHECK (LteUePhy::RsrpSinrTracedCallback,
         uint16_t, uint16_t, double, double,
         empty);
         
  CHECK (LteUePhy::RsrpRsrqTracedCallback,
         uint16_t, uint16_t, double, double, bool);
         
  CHECK (LteUePhy::StateTracedCallback,
         uint16_t, uint16_t, LteUePhy::State, LteUePhy::State,
         empty);

  DUPE   (LteUePowerControl::TxPowerTracedCallback, LteEnbPhy::ReportUeSinrTracedCallback);

  CHECK (LteUeRrc::CellSelectionTracedCallback,
         uint64_t, uint16_t,
         empty, empty, empty);

  DUPE  (LteUeRrc::ImsiCidRntiTracedCallback, LteEnbRrc::ConnectionHandoverTracedCallback);

  DUPE   (LteUeRrc::MibSibHandoverTracedCallback, LteEnbRrc::HandoverStartTracedCallback);

  CHECK (LteUeRrc::StateTracedCallback,
         uint64_t, uint16_t, uint16_t, LteUeRrc::State, LteUeRrc::State);
         
  CHECK (Mac48Address::TracedCallback,
         Mac48Address,
         empty, empty, empty, empty);

  CHECK (MobilityModel::TracedCallback,
         Ptr<const MobilityModel>,
         empty, empty, empty, empty);

  CHECK (olsr::RoutingProtocol::PacketTxRxTracedCallback,
         const olsr::PacketHeader &, const olsr::MessageList &,
         empty, empty, empty);

  CHECK (olsr::RoutingProtocol::TableChangeTracedCallback,
         uint32_t,
         empty, empty, empty, empty);

  CHECK (Packet::AddressTracedCallback,
         Ptr<const Packet>, const Address &,
         empty, empty, empty);

  CHECK (Packet::Mac48AddressTracedCallback,
         Ptr<const Packet>, Mac48Address,
         empty, empty, empty);

  CHECK (Packet::SinrTracedCallback,
         Ptr<const Packet>, double,
         empty, empty, empty);

  CHECK (Packet::SizeTracedCallback,
         uint32_t, uint32_t,
         empty, empty, empty);

  CHECK (Packet::TracedCallback,
         Ptr<const Packet>,
         empty, empty, empty, empty);

  CHECK (PacketBurst::TracedCallback,
         Ptr<const PacketBurst>,
         empty, empty, empty, empty);

  CHECK (dot11s::PeerManagementProtocol::LinkOpenCloseTracedCallback,
         Mac48Address, Mac48Address,
         empty, empty, empty);
         
  CHECK (PhyReceptionStatParameters::TracedCallback,
         PhyReceptionStatParameters,
         empty, empty, empty, empty);

  CHECK (PhyTransmissionStatParameters::TracedCallback,
         PhyTransmissionStatParameters,
         empty, empty, empty, empty);

  CHECK (SixLowPanNetDevice::DropTracedCallback,
         SixLowPanNetDevice::DropReason, Ptr<const Packet>,
         Ptr<SixLowPanNetDevice>, uint32_t,
         empty);

  CHECK (SixLowPanNetDevice::RxTxTracedCallback,
         Ptr<const Packet>, Ptr<SixLowPanNetDevice>, uint32_t,
         empty, empty);

  CHECK (SpectrumChannel::LossTracedCallback,
         Ptr<SpectrumPhy>, Ptr<SpectrumPhy>, double,
         empty, empty);
  
  CHECK (SpectrumValue::TracedCallback,
         Ptr<SpectrumValue>,
         empty, empty, empty, empty);

  CHECK (TimeSeriesAdaptor::OutputTracedCallback,
         double, double,
         empty, empty, empty);

  CHECK (UanMac::PacketModeTracedCallback,
         Ptr<const Packet>, UanTxMode,
         empty, empty, empty);
         
  CHECK (UanMacCw::QueueTracedCallback,
         Ptr<const Packet>, uint16_t,
         empty, empty, empty);
         
  CHECK (UanMacRc::QueueTracedCallback,
         Ptr<const Packet>, uint32_t,
         empty, empty, empty);

  CHECK (UanNetDevice::RxTxTracedCallback,
         Ptr<const Packet>, UanAddress,
         empty, empty, empty);
         
  CHECK (UanPhy::TracedCallback,
         Ptr<const Packet>, double, UanTxMode,
         empty, empty);

  CHECK (UeManager::StateTracedCallback,
         uint64_t, uint16_t, uint16_t, UeManager::State, UeManager::State);
  
  CHECK (WifiMacHeader::TracedCallback,
         const WifiMacHeader &,
         empty, empty, empty, empty);

  DUPE   (WifiPhyStateHelper::RxEndErrorTracedCallback, Packet::SinrTracedCallback);

  CHECK (WifiPhyStateHelper::RxOkTracedCallback,
         Ptr<const Packet>, double, WifiMode, WifiPreamble,
         empty);

  CHECK (WifiPhyStateHelper::StateTracedCallback,
         Time, Time, WifiPhy::State,
         empty, empty);

  CHECK (WifiPhyStateHelper::TxTracedCallback,
         Ptr<const Packet>, WifiMode, WifiPreamble, uint8_t,
         empty);

  CHECK (WifiRemoteStationManager::PowerChangeTracedCallback,
         uint8_t, Mac48Address,
         empty, empty, empty);

  CHECK (WifiRemoteStationManager::RateChangeTracedCallback,
         uint32_t, Mac48Address,
         empty, empty, empty);
}

class TracedCallbackTestSuite : public TestSuite
{
public:
  TracedCallbackTestSuite ();
};

TracedCallbackTestSuite::TracedCallbackTestSuite ()
  : TestSuite ("traced-callback", UNIT)
{
  AddTestCase (new TracedCallbackTypedefTestCase, TestCase::QUICK);
}

static TracedCallbackTestSuite tracedCallbackTestSuite;
